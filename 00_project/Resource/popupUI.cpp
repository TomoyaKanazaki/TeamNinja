//============================================================
//
// startUI2D処理 [startUI.cpp]
// Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "popupUI.h"
#include "manager.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;			// スタートUIの優先順位
	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(1600.0f, 400.0f, 0.0f);		// サイズ

	// ズーム状態の定数
	namespace zoom
	{
		const D3DXVECTOR3 INIT_ZOOM_SUB = D3DXVECTOR3(20.0f, 5.0f, 0.0f);	// 初期のサイズの減算量
		const D3DXVECTOR3 ACCELE_ZOOM_SUB = D3DXVECTOR3(4.0f, 1.0f, 0.0f);	// サイズの減算加速度
		const D3DXVECTOR3 DEST_SIZE = D3DXVECTOR3(360.0f, 90.0f, 0.0f);		// バウンド状態に移行するサイズ
	}

	// バウンド状態の定数
	namespace bound
	{
		const D3DXVECTOR3 DEST_SIZE = D3DXVECTOR3(440.0f, 110.0f, 0.0f);	// バウンド状態に移行するサイズ
		const D3DXVECTOR3 BOUND_SUB = D3DXVECTOR3(15.0f, 3.75f, 0.0f);		// 初期のサイズの減算量
	}

	// 描画状態の定数
	namespace disp
	{
		const int DISP_COUNT = 50;					// 表示カウント
	}

	// フェード状態の定数
	namespace fade
	{
		const float INIT_ALPHA_SUB = 0.02f;			// 初期の透明度の減算量
		const float ACCELE_ALPHA_SUB = 0.01f;		// 透明度の減算加速度
	}
}

//************************************************************
//	子クラス [CPopUpUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPopUpUI::CPopUpUI() : CObject2D(CObject::LABEL_STARTUI, CObject::DIM_2D, PRIORITY),
m_sizeMove(VEC3_ZERO),				// サイズの移動量
m_state(STATE_ZOOM),				// 状態
m_nDispCount(0),					// 表示カウント
m_fSubAlpha(fade::INIT_ALPHA_SUB),	// 透明度の減算量
m_fAlpha(1.0f)						// 透明度
{

}

//============================================================
//	デストラクタ
//============================================================
CPopUpUI::~CPopUpUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPopUpUI::Init(void)
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
void CPopUpUI::Uninit(void)
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPopUpUI::Update(const float fDeltaTime)
{
	// 状態処理
	State();
}

//============================================================
//	描画処理
//============================================================
void CPopUpUI::Draw(CShader* pShader)
{
	// オブジェクト2Dの描画
	CObject2D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CPopUpUI* CPopUpUI::Create(const char* texture)
{
	// スタートUIの生成
	CPopUpUI* pStartUI = new CPopUpUI;

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

		if (texture != nullptr)
		{ // テクスチャが NULL じゃない場合
			pStartUI->BindTexture(GET_MANAGER->GetTexture()->Regist(texture));	// テクスチャを登録・割当
		}

		// 確保したアドレスを返す
		return pStartUI;
	}
}

//============================================================
// 状態処理
//============================================================
void CPopUpUI::State(void)
{
	switch (m_state)
	{
	case CPopUpUI::STATE_ZOOM:		// ズーム状態

		// ズーム処理
		Zoom();

		break;

	case CPopUpUI::STATE_BOUND:		// バウンド状態

		// バウンド処理
		Bound();

		break;

	case CPopUpUI::STATE_DISP:		// 表示状態

		// 表示処理
		Disp();

		break;

	case CPopUpUI::STATE_FADE:		// フェーズ状態

		if (Fade())
		{ // フェードし終わった場合

			// この先の処理を行わない
			return;
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 頂点情報の設定
	SetVtx();
}

//============================================================
// ズーム処理
//============================================================
void CPopUpUI::Zoom(void)
{
	// サイズを減算する
	D3DXVECTOR3 size = GetVec3Sizing();
	size -= m_sizeMove;

	// サイズの移動量を加算する
	m_sizeMove += zoom::ACCELE_ZOOM_SUB;

	if (size.x < zoom::DEST_SIZE.x ||
		size.y < zoom::DEST_SIZE.y)
	{ // 一定のサイズになった場合

		// サイズを補正する
		size = zoom::DEST_SIZE;

		// サイズの移動量を設定する
		m_sizeMove = bound::BOUND_SUB;

		// バウンド状態にする
		m_state = STATE_BOUND;
	}

	// サイズを適用
	SetVec3Sizing(size);
}

//============================================================
// バウンド処理
//============================================================
void CPopUpUI::Bound(void)
{
	// サイズを減算する
	D3DXVECTOR3 size = GetVec3Sizing();
	size += m_sizeMove;

	if (size.x > bound::DEST_SIZE.x ||
		size.y > bound::DEST_SIZE.y)
	{ // 一定のサイズになった場合

		// サイズを補正する
		size = bound::DEST_SIZE;

		// 表示状態にする
		m_state = STATE_DISP;
	}

	// サイズを適用
	SetVec3Sizing(size);
}

//============================================================
// 表示処理
//============================================================
void CPopUpUI::Disp(void)
{
	// 表示カウントを加算する
	m_nDispCount++;

	if (m_nDispCount % disp::DISP_COUNT == 0)
	{ // カウントが一定以上になった場合

		// フェード状態にする
		m_state = STATE_FADE;
	}
}

//============================================================
// フェード処理
//============================================================
bool CPopUpUI::Fade(void)
{
	// 透明度を減算する
	m_fAlpha -= m_fSubAlpha;

	// 透明度の減算量を増やす
	m_fSubAlpha += fade::ACCELE_ALPHA_SUB;

	if (m_fAlpha <= 0.0f)
	{ // 完全に透明になった場合

		// 透明度を補正する
		m_fAlpha = 0.0f;

		// 終了処理
		Uninit();

		// true を返す
		return true;
	}

	// 頂点カラーの設定処理
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

	// false を返す
	return false;
}
