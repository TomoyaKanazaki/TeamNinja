//===========================================
//
//  植物クラス(plant.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "plant.h"
#include "manager.h"
#include "camera.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const float	POLYGON_SCALE = 60.0f;		// ポリゴンのサイズ
	const int	ALPHA_NUMREF = 180;			// αテストの参照値
	const D3DXVECTOR3 GAP_SCALE = D3DXVECTOR3(15.0f, 0.0f, 0.0f); // 揺れ範囲
}

//===========================================
//  静的メンバ変数宣言
//===========================================
CListManager<CPlant>* CPlant::m_pList = nullptr; // オブジェクトリスト

//===========================================
//  コンストラクタ
//===========================================
CPlant::CPlant():
	m_fGapRate(0.0f)
{
}

//===========================================
//  デストラクタ
//===========================================
CPlant::~CPlant()
{
}

//===========================================
//  初期化処理
//===========================================
HRESULT CPlant::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CObject3D::Init())) { assert(false); return E_FAIL; }

	// 原点設定
	SetOrigin(ORIGIN_CENTER);

	// ラベルの変更
	SetLabel(LABEL_GIMMICK);

	// レンダーステートの情報を取得
	CRenderState *pRenderState = GetRenderState();

	// αテストの設定
	pRenderState->SetAlphaTest(true);			// αテストの有効 / 無効の設定
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// αテストの設定
	pRenderState->SetAlphaNumRef(ALPHA_NUMREF);	// αテストの参照値設定

	// ライティングをOFFにする
	pRenderState->SetLighting(false);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CPlant>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 揺れ周期を設定
	m_fGapRate = (float)(rand() % 628 + 1) * 0.01f;

	// 成功を返す
	return S_OK;
}

//===========================================
//  終了処理
//===========================================
void CPlant::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 親クラスの終了
	CObject3D::Uninit();
}

//===========================================
//  更新処理
//===========================================
void CPlant::Update(const float fDeltaTime)
{
	// 方向転換
	Rotation();

	// 揺れを設定
	Shake(fDeltaTime);

	// 親クラスの更新
	CObject3D::Update(fDeltaTime);
}

//===========================================
//  描画処理
//===========================================
void CPlant::Draw(CShader* pShader)
{
	// 親クラスの描画
	CObject3D::Draw(pShader);
}

//===========================================
//  生成処理
//===========================================
CPlant* CPlant::Create(const D3DXVECTOR3& rPos, const char* sPass)
{
	// ギミックの生成
	CPlant* pPlant = new CPlant;

	// メモリの確保に失敗した場合関数を抜ける
	if (pPlant == nullptr) { assert(false); return nullptr; }

	// 初期化
	if (FAILED(pPlant->Init())) { assert(false); SAFE_DELETE(pPlant); return nullptr; }

	// テクスチャ割り当て
	pPlant->BindTexture(GET_MANAGER->GetTexture()->Regist(sPass));

	// サイズの設定
	pPlant->SetVec3Sizing(useful::GetTexAspect(GET_MANAGER->GetTexture()->Regist(sPass)) * POLYGON_SCALE);

	// 位置を設定
	pPlant->SetVec3Position(rPos + D3DXVECTOR3(0.0f, pPlant->GetVec3Sizing().y * 0.5f, 0.0f));

	// 確保したアドレスを返す
	return pPlant;
}

//===========================================
//  リスト取得
//===========================================
CListManager<CPlant>* CPlant::GetList(void)
{
	return m_pList;
}

//==========================================
//  回転処理
//==========================================
void CPlant::Rotation()
{
	// 自身の座標を取得
	D3DXVECTOR3 posThis = GetVec3Position();

	// 視点の座標を取得
	D3DXVECTOR3 posCamera = GET_CAMERA->GetPositionV();

	// 自身の座標と視点の座標を結ぶベクトルを作成
	D3DXVECTOR3 vecToPos = posThis - posCamera;

	// 2点間を結ぶベクトルの角度を算出
	float fRot = atan2f(vecToPos.x, vecToPos.z);

	// 自身の向きを取得
	D3DXVECTOR3 rot = GetVec3Rotation();

	// yの向きをカメラに向ける
	rot.y = fRot;

	// 向きを設定
	SetVec3Rotation(rot);
}

//==========================================
//  揺れる処理
//==========================================
void CPlant::Shake(const float fDeltaTime)
{
	// 揺れ時間の加算
	m_fGapRate += fDeltaTime;

	// 時間の丸め込み
	if (m_fGapRate > D3DX_PI * 2.0f)
	{
		m_fGapRate -= D3DX_PI * 2.0f;
	}

	// 上半分を移動する
	SetGapPosition(0, GAP_SCALE * sinf(m_fGapRate));
	SetGapPosition(1, GAP_SCALE * sinf(m_fGapRate));
}
