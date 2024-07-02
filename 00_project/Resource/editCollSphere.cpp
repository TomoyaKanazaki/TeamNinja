#if 1
//============================================================
//
//	エディットコリジョンスフィア処理 [editCollSphere.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editCollSphere.h"
#include "editManager.h"
#include "manager.h"
#include "useful.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_UP_RADIUS		(DIK_T)	// 半径拡大キー
#define NAME_UP_RADIUS		("T")	// 半径拡大表示
#define KEY_DOWN_RADIUS		(DIK_G)	// 半径縮小キー
#define NAME_DOWN_RADIUS	("G")	// 半径縮小表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float INIT_RADIUS = 20.0f;	// 初期の半径
	const float	INIT_ALPHA = 0.5f;		// 配置前のα値
	const float SCALING = 1.0f;			// 当たり判定の拡縮率
}

//************************************************************
//	親クラス [CEditCollSphere] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditCollSphere::CEditCollSphere(const int nIdx) : CEditorCollShape(nIdx)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pSphere = nullptr;	// スフィア情報
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// コリジョンスフィア配置情報
	m_bSave = false;	// 保存状況

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditCollSphere::~CEditCollSphere()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditCollSphere::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pSphere = nullptr;					// スフィア情報
	m_infoCreate.fRadius = INIT_RADIUS;		// 半径
	m_bSave = false;						// 保存状況

	// 親クラスの初期化
	if (FAILED(CEditorCollShape::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// コリジョンスフィアの生成
	m_pSphere = CCollisionSphere::Create
	( // 引数
		VEC3_ZERO,				// 位置
		GetVec3OffSet(),		// オフセット座標
		m_infoCreate.fRadius	// 半径
	);
	if (m_pSphere == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	終了処理
//============================================================
void CEditCollSphere::Uninit(void)
{
#if _DEBUG

	// 親クラスの終了
	CEditorCollShape::Uninit();

	// コリジョンスフィアの終了
	SAFE_UNINIT(m_pSphere);

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditCollSphere::Update(void)
{
#if _DEBUG

	// 親クラスの更新
	CEditorCollShape::Update();

	// 大きさの更新
	UpdateSize();

	// 位置を反映
	m_pSphere->SetOffset(m_pSphere->GetPos());

#endif	// _DEBUG
}

//============================================================
//	保存状況取得処理
//============================================================
bool CEditCollSphere::IsSave(void)
{
#if _DEBUG

	// 保存状況を返す
	return m_bSave;

#else	// NDEBUG

	// falseを返す
	return false;

#endif	// _DEBUG
}

//============================================================
//	情報保存処理
//============================================================
void CEditCollSphere::SaveInfo(void)
{
#if _DEBUG

	// 現在の情報を保存
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	情報読込処理
//============================================================
void CEditCollSphere::LoadInfo(void)
{
#if _DEBUG

	// 保存情報を設定
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditCollSphere::DrawDebugControl(void)
{
#if _DEBUG

	// 操作表示の描画
	CEditorCollShape::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "半径：[%s/%s+%s]\n", NAME_UP_RADIUS, NAME_DOWN_RADIUS, NAME_TRIGGER);

#endif	// _DEBUG
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditCollSphere::DrawDebugInfo(void)
{
#if _DEBUG

	// 情報表示の描画
	CEditorCollShape::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f：[半径]\n", m_infoCreate.fRadius);

#endif	// _DEBUG
}

//============================================================
//	位置更新処理
//============================================================
void CEditCollSphere::UpdateOffset(void)
{
	// 向きの更新
	CEditorCollShape::UpdateOffset();

#if _DEBUG

	// 向きを反映
	m_pSphere->GetSphere()->SetVec3Position(GetVec3OffSet());

#endif	// _DEBUG
}

//============================================================
//	大きさの更新処理
//============================================================
void CEditCollSphere::UpdateSize(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 大きさを変更
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_RADIUS))
		{
			m_infoCreate.fRadius += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_RADIUS))
		{
			m_infoCreate.fRadius -= SCALING;
		}
	}
	else
	{
		if (pKeyboard->IsTrigger(KEY_UP_RADIUS))
		{
			m_infoCreate.fRadius += SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_RADIUS))
		{
			m_infoCreate.fRadius -= SCALING;
		}
	}

	// 大きさを補正
	useful::LimitMinNum(m_infoCreate.fRadius, SCALING);

#ifdef _DEBUG

	// 大きさを反映
	m_pSphere->GetSphere()->SetRadius(m_infoCreate.fRadius);

#endif // _DEBUG
}

//============================================================
//	生成処理
//============================================================
void CEditCollSphere::Create(void)
{
#ifdef _DEBUG

	//----------------------------------------------------
	//	コリジョンスフィアの情報を配置用に変更
	//----------------------------------------------------
	// 自動更新・自動描画をONにしてLABELを変更する
	m_pSphere->GetSphere()->SetEnableUpdate(true);
	m_pSphere->GetSphere()->SetEnableDraw(true);
	m_pSphere->GetSphere()->SetLabel(CObject::LABEL_DEBUG);

#endif // _DEBUG

	// 未保存を設定
	m_bSave = false;

	//----------------------------------------------------
	//	新しいコリジョンスフィアの生成
	//----------------------------------------------------
	// コリジョンスフィアの生成
	m_pSphere = CCollisionSphere::Create
	( // 引数
		VEC3_ZERO,				// 位置
		GetVec3OffSet(),		// オフセット座標
		m_infoCreate.fRadius	// 半径
	);
	assert(m_pSphere != nullptr);
}

#endif
