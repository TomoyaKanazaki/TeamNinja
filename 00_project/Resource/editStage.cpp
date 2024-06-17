//============================================================
//
//	エディットステージ処理 [editStage.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editStage.h"
#include "manager.h"
#include "editManager.h"
#include "editorObject.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_CHANGE_OBJECT	(DIK_2)	// オブジェクトタイプ変更キー
#define NAME_CHANGE_OBJECT	("2")	// オブジェクトタイプ変更表示

#define KEY_FAR		(DIK_W)	// 奥移動キー
#define NAME_FAR	("W")	// 奥移動表示
#define KEY_NEAR	(DIK_S)	// 手前移動キー
#define NAME_NEAR	("S")	// 手前移動表示
#define KEY_RIGHT	(DIK_D)	// 右移動キー
#define NAME_RIGHT	("D")	// 右移動表示
#define KEY_LEFT	(DIK_A)	// 左移動キー
#define NAME_LEFT	("A")	// 左移動表示
#define KEY_UP		(DIK_E)	// 上移動キー
#define NAME_UP		("E")	// 上移動表示
#define KEY_DOWN	(DIK_Q)	// 下移動キー
#define NAME_DOWN	("Q")	// 下移動表示

#define KEY_ROTA_RIGHT	(DIK_Z)	// 右回転キー
#define NAME_ROTA_RIGHT	("Z")	// 右回転表示
#define KEY_ROTA_LEFT	(DIK_C)	// 左回転キー
#define NAME_ROTA_LEFT	("C")	// 左回転表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *TYPE_NAME[] =	// オブジェクトタイプ名
	{
		"地面",
		"壁",
		"アクター",
		"チェックポイント",
		"ゴールポイント",
		"ギミック",
	};
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TYPE_NAME) == CEditStage::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	親クラス [CEditStage] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditStage::CEditStage(CEditManager *pEditManager) : CEditor(pEditManager)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pEditor	= nullptr;		// エディター情報
	m_type		= TYPE_FIELD;	// オブジェクトタイプ
	m_pos		= VEC3_ZERO;	// 位置
	m_rot		= VEC3_ZERO;	// 向き
	m_angle		= ANGLE_0;		// 角度

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditStage::~CEditStage()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditStage::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pEditor	= nullptr;		// エディター情報
	m_type		= TYPE_FIELD;	// オブジェクトタイプ
	m_pos		= VEC3_ZERO;	// 位置
	m_rot		= VEC3_ZERO;	// 向き
	m_angle		= ANGLE_0;		// 角度

	// エディター情報の生成
	m_pEditor = CEditorObject::Create(this, m_type);
	if (m_pEditor == nullptr)
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
void CEditStage::Uninit(void)
{
#if _DEBUG

	// エディター情報の破棄
	SAFE_REF_RELEASE(m_pEditor);

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditStage::Update(void)
{
#if _DEBUG

	// オブジェクトタイプの変更
	ChangeObjectType();

	// エディター情報の更新
	assert(m_pEditor != nullptr);
	m_pEditor->Update();

#endif	// _DEBUG
}

//============================================================
//	保存処理
//============================================================
HRESULT CEditStage::Save(void)
{
#if _DEBUG

	// エディター情報の保存
	assert(m_pEditor != nullptr);
	if (FAILED(m_pEditor->Save()))
	{ // セーブに失敗した場合

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
//	保存状況取得処理
//============================================================
bool CEditStage::IsSave(void)
{
#if _DEBUG

	// エディター情報の保存
	assert(m_pEditor != nullptr);
	return m_pEditor->IsSave();

#else	// NDEBUG

	// falseを返す
	return false;

#endif	// _DEBUG
}

//============================================================
//	情報保存処理
//============================================================
void CEditStage::SaveInfo(void)
{
#if _DEBUG

	// エディター情報の情報保存
	assert(m_pEditor != nullptr);
	m_pEditor->SaveInfo();

#endif	// _DEBUG
}

//============================================================
//	情報読込処理
//============================================================
void CEditStage::LoadInfo(void)
{
#if _DEBUG

	// エディター情報の情報読込
	assert(m_pEditor != nullptr);
	m_pEditor->LoadInfo();

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditStage::DrawDebugControl(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "エディットステージタイプ変更：[%s]\n", NAME_CHANGE_OBJECT);
	DebugProc::Print(DebugProc::POINT_RIGHT, "移動：[%s/%s/%s/%s/%s/%s+%s]\n", NAME_FAR, NAME_LEFT, NAME_NEAR, NAME_RIGHT, NAME_UP, NAME_DOWN, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "回転：[%s/%s]\n", NAME_ROTA_RIGHT, NAME_ROTA_LEFT);

	// エディター情報の操作表示
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugControl();

#endif	// _DEBUG
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditStage::DrawDebugInfo(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "%s：[エディットステージタイプ]\n", TYPE_NAME[m_type]);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f：[位置]\n", m_pos.x, m_pos.y, m_pos.z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f：[向き]\n", m_rot.x, m_rot.y, m_rot.z);

	// エディター情報の情報表示
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugInfo();

#endif	// _DEBUG
}

//============================================================
//	位置の更新処理
//============================================================
void CEditStage::UpdatePosition(void)
{
	// 位置を変更
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_FAR))
		{
			m_pos.z += editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_NEAR))
		{
			m_pos.z -= editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_RIGHT))
		{
			m_pos.x += editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_LEFT))
		{
			m_pos.x -= editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_UP))
		{
			m_pos.y += editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN))
		{
			m_pos.y -= editstage::SIZE;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_FAR))
		{
			m_pos.z += editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_NEAR))
		{
			m_pos.z -= editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_RIGHT))
		{
			m_pos.x += editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_LEFT))
		{
			m_pos.x -= editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP))
		{
			m_pos.y += editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN))
		{
			m_pos.y -= editstage::SIZE;
		}
	}
}

//============================================================
//	向き更新処理
//============================================================
void CEditStage::UpdateRotation(void)
{
	// 向きを変更
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// キーボード情報
	if (m_pKeyboard->IsTrigger(KEY_ROTA_RIGHT))
	{
		// 角度を左回転
		m_angle = (EAngle)((m_angle + (ANGLE_MAX - 1)) % ANGLE_MAX);

		// 現在の角度から向きを計算
		int nTemp = ((m_angle - 1) + (ANGLE_MAX - 1)) % ANGLE_MAX;
		m_rot.y = ((float)nTemp * HALF_PI) - D3DX_PI;
	}
	if (m_pKeyboard->IsTrigger(KEY_ROTA_LEFT))
	{
		// 角度を右回転
		m_angle = (EAngle)((m_angle + 1) % ANGLE_MAX);

		// 現在の角度から向きを計算
		int nTemp = ((m_angle - 1) + (ANGLE_MAX - 1)) % ANGLE_MAX;
		m_rot.y = ((float)nTemp * HALF_PI) - D3DX_PI;
	}

	// 向きを正規化
	useful::NormalizeRot(m_rot);
}

//============================================================
//	オブジェクトタイプの変更処理
//============================================================
void CEditStage::ChangeObjectType(void)
{
	// オブジェクトタイプの変更
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報
	if (pKeyboard->IsTrigger(KEY_CHANGE_OBJECT))
	{
		// エディター情報の破棄
		SAFE_REF_RELEASE(m_pEditor);

		// オブジェクトタイプの変更
		m_type = (CEditStage::EType)((m_type + 1) % CEditStage::TYPE_MAX);

		if (m_pEditor == nullptr)
		{
			// エディター情報の生成
			m_pEditor = CEditorObject::Create(this, m_type);
			assert(m_pEditor != nullptr);	// 生成失敗
		}
	}
}
