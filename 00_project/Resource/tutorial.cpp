//==========================================
//
//  チュートリアルクラス(tutorial.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "tutorial.h"
#include "manager.h"
#include "player.h"
#include "collision.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const char* TEXTURE_PASS[] = // テクスチャファイルパス
	{
		"data\\TEXTURE\\tutorial000.png",
		"data\\TEXTURE\\tutorial001.png",
		"data\\TEXTURE\\tutorial002.png"
	};

	const float WIDTH = 500.0f; // ポリゴンの横幅
	const D3DXVECTOR3 HIT_BOX = D3DXVECTOR3(250.0f, 250.0f, 250.0f); // 判定距離
}

//==========================================
//  静的警告処理
//==========================================
static_assert(NUM_ARRAY(TEXTURE_PASS) == CTutorial::TYPE_MAX, "ERROR : Type Count Missmatch");

//==========================================
//  静的変数宣言
//==========================================
CListManager<CTutorial>* CTutorial::m_pList = nullptr; // オブジェクトリスト

//==========================================
//  コンストラクタ
//==========================================
CTutorial::CTutorial() : CObjectBillboard(CObject::LABEL_UI, CObject::SCENE_MAIN, CObject::DIM_3D, 7),
	m_sizeDefault(VEC3_ZERO)
{
}

//==========================================
//  デストラクタ
//==========================================
CTutorial::~CTutorial()
{
}

//==========================================
//  初期化処理
//==========================================
HRESULT CTutorial::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CObjectBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CTutorial>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// ラベルを設定
	SetLabel(LABEL_UI);

	// 成功を返す
	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void CTutorial::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 親クラスの終了
	CObjectBillboard::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CTutorial::Update(const float fDeltaTime)
{
	// 遠距離判定
	bool bFar = useful::IsNearPosR(GetVec3Position());
	SetEnableDraw(bFar);
	if (!bFar) { return; }

	// プレイヤー距離の判定
	MeasureDistance();

	// 親クラスの更新
	CObjectBillboard::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CTutorial::Draw(CShader* pShader)
{
	// オブジェクト3Dの描画
	CObjectBillboard::Draw(pShader);
}

//==========================================
//  テクスチャ割り当て
//==========================================
void CTutorial::BindTexture(const char* pTexturePass)
{
	// テクスチャを割り当て
	CObjectBillboard::BindTexture(pTexturePass);

	// アスペクト比から初期サイズを設定
	m_sizeDefault = D3DXVECTOR3
	(
		WIDTH,
		useful::GetTexHeightFromAspect(WIDTH, GET_MANAGER->GetTexture()->Regist(pTexturePass)),
		0.0f
	);
}

//==========================================
//  プレイヤーとの距離を測る処理
//==========================================
void CTutorial::MeasureDistance()
{
	// プレイヤー情報を取得
	CPlayer* pPlayer = GET_PLAYER;
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();
	D3DXVECTOR3 sizePlayer = D3DXVECTOR3(pPlayer->GetRadius(), pPlayer->GetHeight(), pPlayer->GetRadius());

	// 当たり判定
	if (collision::Box3D
	(
		GetVec3Position(), posPlayer,
		HIT_BOX, HIT_BOX,
		sizePlayer, sizePlayer
	))
	{
		m_bMove = true;
	}
}

//==========================================
//  生成処理
//==========================================
CTutorial* CTutorial::Create(const D3DXVECTOR3& rPos, const EType type)
{
	// メモリを確保
	CTutorial* pTutorial = new CTutorial;

	// メモリの確保に失敗した場合関数を抜ける
	if (pTutorial == nullptr) { assert(false); return nullptr; }

	// 初期化処理
	if (FAILED(pTutorial->Init()))
	{ // 初期化に失敗した場合

		// 分身のUIの破棄
		SAFE_DELETE(pTutorial);
		return nullptr;
	}

	// 座標を設定
	D3DXVECTOR3 pos = rPos;
	pos.y += 100.0f;
	pTutorial->SetVec3Position(pos);

	// 種類に合わせたテクスチャを割り当て
	pTutorial->BindTexture(TEXTURE_PASS[type]);

	// 確保したアドレスを返す
	return pTutorial;
}

//==========================================
//  リストの取得
//==========================================
CListManager<CTutorial>* CTutorial::GetList(void)
{
	return m_pList;
}

//==========================================
//  拡縮処理
//==========================================
void CTutorial::Scaling(const float fDeltaTime)
{
	// 移動フラグがoffの場合関数を抜ける
	if (!m_bMove) { return; }
}
