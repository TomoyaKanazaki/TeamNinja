#if 1
//============================================================
//
//	エディットコリジョンキューブ処理 [editCollCube.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editCollCube.h"
#include "editManager.h"
#include "manager.h"
#include "useful.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_UP_WIDTH		(DIK_T)	// 幅拡大キー
#define NAME_UP_WIDTH		("T")	// 幅拡大表示
#define KEY_DOWN_WIDTH		(DIK_G)	// 幅縮小キー
#define NAME_DOWN_WIDTH		("G")	// 幅縮小表示
#define KEY_UP_HEIGHT		(DIK_Y)	// 高さ拡大キー
#define NAME_UP_HEIGHT		("Y")	// 高さ拡大表示
#define KEY_DOWN_HEIGHT		(DIK_H)	// 高さ縮小キー
#define NAME_DOWN_HEIGHT	("H")	// 高さ縮小表示
#define KEY_UP_DEPTH		(DIK_U)	// 奥行拡大キー
#define NAME_UP_DEPTH		("U")	// 奥行拡大表示
#define KEY_DOWN_DEPTH		(DIK_J)	// 奥行縮小キー
#define NAME_DOWN_DEPTH		("J")	// 奥行縮小表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float INIT_WIDTH = 20.0f;		// 初期の幅
	const float INIT_HEIGHT = 20.0f;	// 初期の高さ
	const float INIT_DEPTH = 20.0f;		// 初期の奥行
	const float	INIT_ALPHA = 0.5f;		// 配置前のα値
	const float SCALING = 1.0f;			// 当たり判定の拡縮率
}

//************************************************************
//	親クラス [CEditCollCube] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditCollCube::CEditCollCube(const int nIdx) : CEditorCollShape(nIdx)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pCube = nullptr;	// キューブ情報
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// コリジョンキューブ配置情報
	m_bSave = false;	// 保存状況

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditCollCube::~CEditCollCube()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditCollCube::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pCube = nullptr;						// キューブ情報
	m_infoCreate.fWidth = INIT_WIDTH;		// 幅
	m_infoCreate.fHeight = INIT_HEIGHT;		// 高さ
	m_infoCreate.fDepth = INIT_DEPTH;		// 奥行
	m_bSave = false;						// 保存状況

	// 親クラスの初期化
	if (FAILED(CEditorCollShape::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// コリジョンキューブの生成
	m_pCube = CCollisionCube::Create
	( // 引数
		VEC3_ZERO,				// 位置
		GetVec3OffSet(),		// オフセット座標
		m_infoCreate.fWidth,	// 幅
		m_infoCreate.fHeight,	// 高さ
		m_infoCreate.fDepth		// 奥行
	);
	if (m_pCube == nullptr)
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
void CEditCollCube::Uninit(void)
{
#if _DEBUG

	// 親クラスの終了
	CEditorCollShape::Uninit();

	// コリジョンキューブの終了
	SAFE_UNINIT(m_pCube);

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditCollCube::Update(void)
{
#if _DEBUG

	// 親クラスの更新
	CEditorCollShape::Update();

	// 大きさの更新
	UpdateSizing();

	// 位置を反映
	m_pCube->SetOffset(m_pCube->GetPos());

#endif	// _DEBUG
}

//============================================================
//	保存状況取得処理
//============================================================
bool CEditCollCube::IsSave(void)
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
void CEditCollCube::SaveInfo(void)
{
#if _DEBUG

	// 現在の情報を保存
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	情報読込処理
//============================================================
void CEditCollCube::LoadInfo(void)
{
#if _DEBUG

	// 保存情報を設定
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditCollCube::DrawDebugControl(void)
{
#if _DEBUG

	// 操作表示の描画
	CEditorCollShape::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "幅：[%s/%s+%s]\n", NAME_UP_WIDTH, NAME_DOWN_WIDTH, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "高さ：[%s/%s+%s]\n", NAME_UP_HEIGHT, NAME_DOWN_HEIGHT, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "奥行：[%s/%s+%s]\n", NAME_UP_DEPTH, NAME_DOWN_DEPTH, NAME_TRIGGER);

#endif	// _DEBUG
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditCollCube::DrawDebugInfo(void)
{
#if _DEBUG

	// 情報表示の描画
	CEditorCollShape::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f：[幅]\n", m_infoCreate.fWidth);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f：[高さ]\n", m_infoCreate.fHeight);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f：[奥行]\n", m_infoCreate.fDepth);

#endif	// _DEBUG
}

//============================================================
// 生成処理
//============================================================
void CEditCollCube::Create(void)
{
#ifdef _DEBUG

	//----------------------------------------------------
	//	コリジョンキューブの情報を配置用に変更
	//----------------------------------------------------
	// 自動更新・自動描画をONにしてLABELを変更する
	m_pCube->GetCube()->SetEnableUpdate(true);
	m_pCube->GetCube()->SetEnableDraw(true);
	m_pCube->GetCube()->SetLabel(CObject::LABEL_DEBUG);

#endif // _DEBUG

	// 未保存を設定
	m_bSave = false;

	//----------------------------------------------------
	//	新しいコリジョンキューブの生成
	//----------------------------------------------------
	// コリジョンキューブの生成
	m_pCube = CCollisionCube::Create
	( // 引数
		VEC3_ZERO,				// 位置
		GetVec3OffSet(),		// オフセット座標
		m_infoCreate.fWidth,	// 半径
		m_infoCreate.fHeight,	// 高さ
		m_infoCreate.fDepth		// 奥行
	);
	assert(m_pCube != nullptr);
}

//============================================================
//	位置更新処理
//============================================================
void CEditCollCube::UpdateOffset(void)
{
	// 向きの更新
	CEditorCollShape::UpdateOffset();

#if _DEBUG

	// 向きを反映
	m_pCube->GetCube()->SetVec3Position(GetVec3OffSet());

#endif	// _DEBUG
}

//============================================================
//	大きさの更新処理
//============================================================
void CEditCollCube::UpdateSizing(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 大きさを変更
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_WIDTH))
		{
			m_infoCreate.fWidth += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_WIDTH))
		{
			m_infoCreate.fWidth -= SCALING;
		}
		if (pKeyboard->IsPress(KEY_UP_HEIGHT))
		{
			m_infoCreate.fHeight += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_HEIGHT))
		{
			m_infoCreate.fHeight -= SCALING;
		}
		if (pKeyboard->IsPress(KEY_UP_DEPTH))
		{
			m_infoCreate.fDepth += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_DEPTH))
		{
			m_infoCreate.fDepth -= SCALING;
		}
	}
	else
	{
		if (pKeyboard->IsTrigger(KEY_UP_WIDTH))
		{
			m_infoCreate.fWidth += SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_WIDTH))
		{
			m_infoCreate.fWidth -= SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_UP_HEIGHT))
		{
			m_infoCreate.fHeight += SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_HEIGHT))
		{
			m_infoCreate.fHeight -= SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_UP_DEPTH))
		{
			m_infoCreate.fDepth += SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_DEPTH))
		{
			m_infoCreate.fDepth -= SCALING;
		}
	}

	// 大きさを補正
	useful::LimitMinNum(m_infoCreate.fWidth, SCALING);
	useful::LimitMinNum(m_infoCreate.fHeight, SCALING);
	useful::LimitMinNum(m_infoCreate.fDepth, SCALING);

#ifdef _DEBUG

	// 大きさを反映
	m_pCube->GetCube()->SetVec3Sizing(D3DXVECTOR3
	(
		m_infoCreate.fWidth,
		m_infoCreate.fHeight,
		m_infoCreate.fDepth
	));

#endif // _DEBUG
}

#endif
