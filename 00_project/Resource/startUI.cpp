//============================================================
//
// startUI2D処理 [startUI.cpp]
// Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "startUI.h"
#include "manager.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* TEXTURE_FILE = "TODO";		// テクスチャ
	const int PRIORITY = 6;			// スタートUIの優先順位

	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(1600.0f, 400.0f, 0.0f);		// サイズ
	const D3DXVECTOR3 INIT_ZOOM_SUB = D3DXVECTOR3(20.0f, 5.0f, 0.0f);		// 初期のサイズの減算量
	const D3DXVECTOR3 ACCELE_ZOOM_SUB = D3DXVECTOR3(4.0f, 1.0f, 0.0f);		// サイズの減算加速度
	const D3DXVECTOR3 BOUND_SIZE = D3DXVECTOR3(400.0f, 100.0f, 0.0f);		// バウンド状態に移行するサイズ
}

//************************************************************
//	子クラス [CStartUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CStartUI::CStartUI() : CObject2D(CObject::LABEL_STARTUI, CObject::DIM_2D, PRIORITY),
m_sizeMove(VEC3_ZERO),		// サイズの移動量
m_state(STATE_ZOOM)			// ズーム状態
{

}

//============================================================
//	デストラクタ
//============================================================
CStartUI::~CStartUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CStartUI::Init(void)
{
	// オブジェクト2Dの初期化
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CStartUI::Uninit(void)
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CStartUI::Update(const float fDeltaTime)
{
	// 状態処理
	State();

	// オブジェクト2Dの更新
	CObject2D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CStartUI::Draw(CShader* pShader)
{
	// オブジェクト2Dの描画
	CObject2D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CStartUI* CStartUI::Create(void)
{
	// スタートUIの生成
	CStartUI* pStartUI = new CStartUI;

	if (pStartUI == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// スタートUIの初期化
		if (FAILED(pStartUI->Init()))
		{ // 初期化に失敗した場合

			// スタートUIの破棄
			SAFE_DELETE(pStartUI);
			return nullptr;
		}

		// 値を設定
		pStartUI->SetVec3Position(SCREEN_CENT);		// 位置を設定
		pStartUI->SetVec3Rotation(VEC3_ZERO);		// 向きを設定
		pStartUI->SetVec3Sizing(INIT_SIZE);			// 大きさを設定
		pStartUI->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE));// テクスチャを登録・割当

		// 確保したアドレスを返す
		return pStartUI;
	}
}

//============================================================
// 状態処理
//============================================================
void CStartUI::State(void)
{
	switch (m_state)
	{
	case CStartUI::STATE_ZOOM:		// ズーム状態

		// ズーム処理
		Zoom();

		break;

	case CStartUI::STATE_BOUND:		// バウンド状態


		break;

	case CStartUI::STATE_STOP:		// 停止状態


		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//============================================================
// ズーム処理
//============================================================
void CStartUI::Zoom(void)
{
	// サイズを減算する
	D3DXVECTOR3 size = GetVec3Sizing();
	size -= m_sizeMove;

	// サイズの移動量を加算する
	m_sizeMove += ACCELE_ZOOM_SUB;

	if (size.x < BOUND_SIZE.x ||
		size.y < BOUND_SIZE.y)
	{ // 一定のサイズになった場合

		// サイズを補正する
		size = BOUND_SIZE;

		// バウンド状態にする
		m_state = STATE_BOUND;
	}

	// サイズを適用
	SetVec3Sizing(size);
}
