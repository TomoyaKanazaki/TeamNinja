//===========================================
//
//  カメラ方向変更判定クラス(camera_change.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "camera_change.h"
#include "manager.h"
#include "player.h"
#include "collision.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const float CAMERA_DIRECTION[] = // カメラの方向
	{
		D3DX_PI * 0.5f, // 正面
		D3DX_PI * -0.5f, // 後方
		D3DX_PI * 1.0f, // 左
		D3DX_PI * 0.0f, // 右
	};

	const float CAMERA_ROTATION[] = // カメラの角度
	{
		1.3f, // デフォルト
		1.05f, // 上
		1.5f // 下
	};
}

//===========================================
//  静的警告処理
//===========================================
static_assert(NUM_ARRAY(CAMERA_DIRECTION) == CCameraChanger::DIRECTION_MAX, "ERROR : Type Count Mismatch");
static_assert(NUM_ARRAY(CAMERA_ROTATION) == CCameraChanger::ROTATION_MAX, "ERROR : Type Count Mismatch");

//===========================================
//  静的メンバ変数宣言
//===========================================
CListManager<CCameraChanger>* CCameraChanger::m_pList = nullptr; // オブジェクトリスト

//===========================================
//  コンストラクタ
//===========================================
CCameraChanger::CCameraChanger() : CObjectMeshCube(CObject::LABEL_COLLISION),
m_bChange(false),
m_eDir(DIRECTION_FRONT),
m_eRot(ROTATION_DEFAULT)
{
}

//===========================================
//  デストラクタ
//===========================================
CCameraChanger::~CCameraChanger()
{
}

//===========================================
//  初期化処理
//===========================================
HRESULT CCameraChanger::Init()
{
	// 親クラスの初期化
	if (FAILED(CObjectMeshCube::Init())) { assert(false); return E_FAIL; }

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CCameraChanger>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// 色を変更
	D3DXCOLOR col = XCOL_ACYAN;
	col.a = 0.5f;
	SetCubeColor(col);

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//===========================================
//  終了処理
//===========================================
void CCameraChanger::Uninit()
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 親クラスの終了
	CObjectMeshCube::Uninit();
}

//===========================================
//  更新処理
//===========================================
void CCameraChanger::Update(const float fDeltaTime)
{
	// 各種判定用情報の取得
	CPlayer* player = GET_PLAYER; // プレイヤーポインタ
	D3DXVECTOR3 posPlayer = player->GetVec3Position(); // プレイヤー座標
	D3DXVECTOR3 sizePlayer = player->GetVec3Sizing(); // プレイヤーサイズ
	D3DXVECTOR3 posThis = GetVec3Position(); // 自身の座標
	D3DXVECTOR3 sizeThis = GetVec3Sizing(); // 自身のサイズ

	// フラグの更新
	m_bChange = collision::Box3D(posThis, posPlayer, sizeThis, sizeThis, sizePlayer, sizePlayer);

	// 親クラスの更新処理
	CObjectMeshCube::Update(fDeltaTime);
}

//===========================================
//  描画処理
//===========================================
void CCameraChanger::Draw(CShader* pShader)
{
#ifndef _DEBUG
	// デバッグ中以外は描画したくない
	return;
#endif

	//親クラスの描画処理
	CObjectMeshCube::Draw(pShader);
}

//===========================================
//  生成処理
//===========================================
CCameraChanger* CCameraChanger::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const EDirection Dir, const ERotation Rot)
{
	// メモリの確保
	CCameraChanger* pChanger = new CCameraChanger;

	// 確保に失敗した場合nullを返す
	if (pChanger == nullptr) { assert(false); return nullptr; }

	// 初期化処理
	if (FAILED(pChanger->Init())) { assert(false); return nullptr; }

	// 位置の設定
	pChanger->SetVec3Position(rPos);

	// サイズの設定
	pChanger->SetVec3Sizing(rSize);

	// カメラ情報の設定
	pChanger->m_eDir = Dir;
	pChanger->m_eRot = Rot;

	return pChanger;
}

//===========================================
//  リストの取得
//===========================================
CListManager<CCameraChanger>* CCameraChanger::GetList(void)
{
	return m_pList;
}
