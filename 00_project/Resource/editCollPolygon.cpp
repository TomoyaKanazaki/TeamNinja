#if 1
//============================================================
//
//	エディットコリジョンポリゴン処理 [editCollPolygon.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editCollPolygon.h"
#include "editManager.h"
#include "manager.h"
#include "useful.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_UP_WIDTH		(DIK_T)	// X軸拡大キー
#define NAME_UP_WIDTH		("T")	// X軸拡大表示
#define KEY_DOWN_WIDTH		(DIK_G)	// X軸縮小キー
#define NAME_DOWN_WIDTH		("G")	// X軸縮小表示
#define KEY_UP_DEPTH		(DIK_Y)	// Y軸拡大キー
#define NAME_UP_DEPTH		("Y")	// Y軸拡大表示
#define KEY_DOWN_DEPTH		(DIK_H)	// Y軸縮小キー
#define NAME_DOWN_DEPTH		("H")	// Y軸縮小表示

#define KEY_UP_ROT_X		(DIK_U)	// X軸拡大キー
#define NAME_UP_ROT_X		("U")	// X軸拡大表示
#define KEY_DOWN_ROT_X		(DIK_J)	// X軸縮小キー
#define NAME_DOWN_ROT_X		("J")	// X軸縮小表示
#define KEY_UP_ROT_Z		(DIK_I)	// Z軸拡大キー
#define NAME_UP_ROT_Z		("I")	// Z軸拡大表示
#define KEY_DOWN_ROT_Z		(DIK_K)	// Z軸縮小キー
#define NAME_DOWN_ROT_Z		("K")	// Z軸縮小表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float INIT_RADIUS = 20.0f;	// 初期の半径
	const float	INIT_ALPHA = 0.5f;		// 配置前のα値
	const float SCALING = 1.0f;			// 当たり判定の拡縮率
	const float ROT = 0.01f;			// 向きの拡縮率
}

//************************************************************
//	親クラス [CEditCollPolygon] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditCollPolygon::CEditCollPolygon(const int nIdx) : CEditorCollShape(nIdx)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pPolygon = nullptr;	// ポリゴン情報
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// コリジョンポリゴン配置情報
	m_bSave = false;	// 保存状況

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditCollPolygon::~CEditCollPolygon()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditCollPolygon::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pPolygon = nullptr;				// ポリゴン情報
	m_infoCreate.rot = VEC3_ZERO;		// 向き
	m_infoCreate.size = VEC3_ZERO;		// サイズ
	m_bSave = false;					// 保存状況

	// 親クラスの初期化
	if (FAILED(CEditorCollShape::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// コリジョンポリゴンの生成
	m_pPolygon = CCollisionPolygon::Create
	( // 引数
		VEC3_ZERO,			// 位置
		GetVec3OffSet(),	// オフセット座標
		m_infoCreate.rot,	// 向き
		m_infoCreate.size	// サイズ
	);
	if (m_pPolygon == nullptr)
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
void CEditCollPolygon::Uninit(void)
{
#if _DEBUG

	// 親クラスの終了
	CEditorCollShape::Uninit();

	// コリジョンポリゴンの終了
	SAFE_UNINIT(m_pPolygon);

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditCollPolygon::Update(void)
{
#if _DEBUG

	// 親クラスの更新
	CEditorCollShape::Update();

	// 向きの更新
	UpdateRot();

	// 大きさの更新
	UpdateSize();

	// 位置を反映
	m_pPolygon->SetOffset(m_pPolygon->GetPos());

#endif	// _DEBUG
}

//============================================================
//	保存状況取得処理
//============================================================
bool CEditCollPolygon::IsSave(void)
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
void CEditCollPolygon::SaveInfo(void)
{
#if _DEBUG

	// 現在の情報を保存
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	情報読込処理
//============================================================
void CEditCollPolygon::LoadInfo(void)
{
#if _DEBUG

	// 保存情報を設定
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditCollPolygon::DrawDebugControl(void)
{
#if _DEBUG

	// 操作表示の描画
	CEditorCollShape::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "X軸回転：[%s/%s+%s]\n", NAME_UP_ROT_X, NAME_DOWN_ROT_X, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "Z軸回転：[%s/%s+%s]\n", NAME_UP_ROT_Z, NAME_DOWN_ROT_Z, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "幅：[%s/%s+%s]\n", NAME_UP_WIDTH, NAME_DOWN_WIDTH, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "奥行：[%s/%s+%s]\n", NAME_UP_DEPTH, NAME_DOWN_DEPTH, NAME_TRIGGER);

#endif	// _DEBUG
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditCollPolygon::DrawDebugInfo(void)
{
#if _DEBUG

	// 情報表示の描画
	CEditorCollShape::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f：[X軸]\n", m_infoCreate.rot.x);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f：[Y軸]\n", m_infoCreate.rot.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f：[Z軸]\n", m_infoCreate.rot.z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f：[幅]\n", m_infoCreate.size.x);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f：[奥行]\n", m_infoCreate.size.z);

#endif	// _DEBUG
}

//============================================================
//	位置更新処理
//============================================================
void CEditCollPolygon::UpdateOffset(void)
{
	// 向きの更新
	CEditorCollShape::UpdateOffset();

#if _DEBUG

	// オフセットを反映
	m_pPolygon->GetPolygon()->SetVec3Position(GetVec3OffSet());

#endif	// _DEBUG
}

//============================================================
// 向き更新
//============================================================
void CEditCollPolygon::UpdateRot(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 向きを変更
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_ROT_X))
		{
			m_infoCreate.rot.x += ROT;
		}
		if (pKeyboard->IsPress(KEY_DOWN_ROT_X))
		{
			m_infoCreate.rot.x -= ROT;
		}

		if (pKeyboard->IsPress(KEY_UP_ROT_Z))
		{
			m_infoCreate.rot.z += ROT;
		}
		if (pKeyboard->IsPress(KEY_DOWN_ROT_Z))
		{
			m_infoCreate.rot.z -= ROT;
		}
	}
	else
	{
		if (pKeyboard->IsPress(KEY_UP_ROT_X))
		{
			m_infoCreate.rot.x += ROT;
		}
		if (pKeyboard->IsPress(KEY_DOWN_ROT_X))
		{
			m_infoCreate.rot.x -= ROT;
		}

		if (pKeyboard->IsPress(KEY_UP_ROT_Z))
		{
			m_infoCreate.rot.z += ROT;
		}
		if (pKeyboard->IsPress(KEY_DOWN_ROT_Z))
		{
			m_infoCreate.rot.z -= ROT;
		}
	}

#ifdef _DEBUG

	// 向きを反映
	m_pPolygon->GetPolygon()->SetVec3Rotation(m_infoCreate.rot);

#endif // _DEBUG
}

//============================================================
//	大きさの更新処理
//============================================================
void CEditCollPolygon::UpdateSize(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報

	// 大きさを変更
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_WIDTH))
		{
			m_infoCreate.size.x += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_WIDTH))
		{
			m_infoCreate.size.x -= SCALING;
		}

		if (pKeyboard->IsPress(KEY_UP_DEPTH))
		{
			m_infoCreate.size.z += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_DEPTH))
		{
			m_infoCreate.size.z -= SCALING;
		}
	}
	else
	{
		if (pKeyboard->IsPress(KEY_UP_WIDTH))
		{
			m_infoCreate.size.x += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_WIDTH))
		{
			m_infoCreate.size.x -= SCALING;
		}

		if (pKeyboard->IsPress(KEY_UP_DEPTH))
		{
			m_infoCreate.size.z += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_DEPTH))
		{
			m_infoCreate.size.z -= SCALING;
		}
	}

	// 大きさを補正
	useful::LimitMinNum(m_infoCreate.size.x, SCALING);
	useful::LimitMinNum(m_infoCreate.size.z, SCALING);

#ifdef _DEBUG

	// 大きさを反映
	m_pPolygon->GetPolygon()->SetVec3Sizing(m_infoCreate.size);

#endif // _DEBUG
}

//============================================================
//	生成処理
//============================================================
void CEditCollPolygon::Create(void)
{
#ifdef _DEBUG

	//----------------------------------------------------
	//	コリジョンポリゴンの情報を配置用に変更
	//----------------------------------------------------
	// 自動更新・自動描画をONにしてLABELを変更する
	m_pPolygon->GetPolygon()->SetEnableUpdate(true);
	m_pPolygon->GetPolygon()->SetEnableDraw(true);
	m_pPolygon->GetPolygon()->SetLabel(CObject::LABEL_DEBUG);

#endif // _DEBUG

	// 未保存を設定
	m_bSave = false;

	//----------------------------------------------------
	//	新しいコリジョンポリゴンの生成
	//----------------------------------------------------
	// コリジョンポリゴンの生成
	m_pPolygon = CCollisionPolygon::Create
	( // 引数
		VEC3_ZERO,				// 位置
		GetVec3OffSet(),		// オフセット座標
		m_infoCreate.rot,		// 向き
		m_infoCreate.size		// サイズ
	);
	assert(m_pPolygon != nullptr);
}

#endif
