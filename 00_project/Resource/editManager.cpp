//============================================================
//
//	エディットマネージャー処理 [editManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editManager.h"
#include "manager.h"

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_SAVE	(DIK_F9)	// 保存キー
#define NAME_SAVE	("F9")		// 保存表示
#define KEY_CHANGE_EDITOR	(DIK_1)	// エディットタイプ変更キー
#define NAME_CHANGE_EDITOR	("1")	// エディットタイプ変更表示

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SAVE_TXT = "data\\TXT\\save_stage.txt";	// ステージセーブテキスト

	const char *TYPE_NAME[] =	// エディットタイプ名
	{
		"ステージ",
		"当たり判定",
	};
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TYPE_NAME) == CEditor::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	親クラス [CEditManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditManager::CEditManager()
{
#if _DEBUG

	// メンバ変数をクリア
	m_pEditor	= nullptr;	// エディター情報
	m_bSave		= false;	// 保存状況
	m_bEdit		= false;	// エディット状況
	m_type		= CEditor::TYPE_STAGE;	// エディタータイプ


#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditManager::~CEditManager()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditManager::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_pEditor	= nullptr;	// エディター情報
	m_bSave		= false;	// 保存状況
	m_bEdit		= false;	// エディット状況
	m_type		= CEditor::TYPE_STAGE;	// エディタータイプ

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
void CEditManager::Uninit(void)
{
#if _DEBUG

	// エディター情報の破棄
	SAFE_REF_RELEASE(m_pEditor);

#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditManager::Update(void)
{
#if _DEBUG

	// エディットモードじゃない場合抜ける
	if (!m_bEdit) { return; }

	// エディタータイプ変更
	ChangeEditorType();

	// エディター情報の更新
	assert(m_pEditor != nullptr);
	m_pEditor->Update();

	// 保存操作
	Save();

	// 操作表示の描画
	DrawDebugControl();

	// 情報表示の描画
	DrawDebugInfo();

#endif	// _DEBUG
}

//============================================================
//	未保存の設定処理
//============================================================
void CEditManager::UnSave(void)
{
	// 保存していない状況にする
	m_bSave = false;
}

//============================================================
//	エディット状況の切替処理
//============================================================
void CEditManager::SwitchEnableEdit(void)
{
	// エディット状況を反転
	SetEnableEdit(!m_bEdit);
}

//============================================================
//	エディット状況の設定処理
//============================================================
void CEditManager::SetEnableEdit(const bool bEdit)
{
	// 引数のエディット状況にする
	m_bEdit = bEdit;

	if (m_bEdit)
	{ // エディットONの場合

		// エディットステージの生成
		if (m_pEditor == nullptr)
		{ // エディットステージが使用されていない場合

			m_pEditor = CEditor::Create(this, m_type);
			assert(m_pEditor != nullptr);	// 生成失敗
		}

		// 情報読込
		m_pEditor->LoadInfo();
	}
	else
	{ // エディットOFFの場合

		// 情報保存
		m_pEditor->SaveInfo();

		// エディットステージの破棄
		SAFE_REF_RELEASE(m_pEditor);
	}
}

//============================================================
//	生成処理
//============================================================
CEditManager *CEditManager::Create(void)
{
#if _DEBUG

	// エディターマネージャーの生成
	CEditManager *pEditManager = new CEditManager;
	if (pEditManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// エディターマネージャーの初期化
		if (FAILED(pEditManager->Init()))
		{ // 初期化に失敗した場合

			// エディターマネージャーの破棄
			SAFE_DELETE(pEditManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pEditManager;
	}

#else	// NDEBUG

	// nullptrを返す
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	破棄処理
//============================================================
void CEditManager::Release(CEditManager *&prEditManager)
{
#if _DEBUG

	// エディットマネージャーの終了
	assert(prEditManager != nullptr);
	prEditManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prEditManager);

#endif	// _DEBUG
}

//============================================================
//	エディタータイプの変更処理
//============================================================
void CEditManager::ChangeEditorType(void)
{
	// エディタータイプの変更
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報
	if (pKeyboard->IsTrigger(KEY_CHANGE_EDITOR))
	{
		// エディター情報の破棄
		SAFE_REF_RELEASE(m_pEditor);

		// エディタータイプの変更
		m_type = (CEditor::EType)((m_type + 1) % CEditor::TYPE_MAX);

		if (m_pEditor == nullptr)
		{
			// エディター情報の生成
			m_pEditor = CEditor::Create(this, m_type);
			assert(m_pEditor != nullptr);	// 生成失敗
		}
	}
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditManager::DrawDebugControl(void)
{
	DebugProc::Print(DebugProc::POINT_RIGHT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "[エディット操作]　\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "ステージ保存：[%s+%s]\n", NAME_DOUBLE, NAME_SAVE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "エディットタイプ変更：[%s]\n", NAME_CHANGE_EDITOR);

	// エディター情報の操作表示
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugControl();
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditManager::DrawDebugInfo(void)
{
	DebugProc::Print(DebugProc::POINT_RIGHT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "[エディット情報]　\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, (m_bSave) ? "保存済：[保存状況]\n" : "未保存：[保存状況]\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "%s：[エディタータイプ]\n", TYPE_NAME[m_type]);

	// エディター情報の情報表示
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugInfo();
}

//============================================================
//	保存処理
//============================================================
void CEditManager::Save(void)
{
	// ステージを保存
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報
	if (pKeyboard->IsPress(KEY_DOUBLE))
	{
		if (pKeyboard->IsTrigger(KEY_SAVE))
		{
			// 現在のエディタータイプ情報を保存
			assert(m_pEditor != nullptr);
			m_pEditor->Save();

			// 保存した状態にする
			m_bSave = true;
		}
	}
}
