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
	m_pEditor	 = nullptr;	// エディター情報
	m_bSave		 = false;	// 保存状況
	m_bEdit		 = false;	// エディット状況
	m_typeEditor = CEditor::TYPE_STAGE;	// エディットタイプ

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
	m_pEditor	 = nullptr;	// エディター情報
	m_bSave		 = false;	// 保存状況
	m_bEdit		 = false;	// エディット状況
	m_typeEditor = CEditor::TYPE_STAGE;	// エディットタイプ

	// エディター情報の生成
	m_pEditor = CEditor::Create(this, m_typeEditor);
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
void CEditManager::Uninit(void)
{
#if _DEBUG

	// エディター情報の終了
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

	// エディットタイプ変更
	ChangeEditorType();

	// エディター情報の更新
	assert(m_pEditor != nullptr);
	m_pEditor->Update();

	// ステージ保存
	SaveStage();

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
	m_bEdit = !m_bEdit;

#if 0
	if (m_bEdit)
	{ // エディットONの場合

		// エディットステージの生成
		if (m_pStage == nullptr)
		{ // エディットステージが使用されていない場合

			m_pStage = CEditStage::Create(this, m_thing);
			assert(m_pStage != nullptr);	// 生成失敗
		}

		// 情報読込
		m_pStage->LoadInfo();
	}
	else
	{ // エディットOFFの場合

		// 情報保存
		m_pStage->SaveInfo();

		// エディットステージの破棄
		if (m_pStage != nullptr)
		{ // エディットステージが使用されている場合

			HRESULT hr = CEditStage::Release(m_pStage);
			assert(hr != E_FAIL);	// 破棄失敗
		}
	}
#endif
}

//============================================================
//	エディット状況の設定処理
//============================================================
void CEditManager::SetEnableEdit(const bool bEdit)
{
	// 引数のエディット状況にする
	m_bEdit = bEdit;

#if 0
	if (bEdit)
	{ // エディットモードの場合

		// エディットステージの生成
		if (m_pStage == nullptr)
		{ // エディットステージが使用されていない場合

			m_pStage = CEditStage::Create(this, m_thing);
			assert(m_pStage != nullptr);	// 生成失敗
		}
	}
	else
	{ // エディットモードではない場合

		// エディットステージの破棄
		if (m_pStage != nullptr)
		{ // エディットステージが使用されている場合

			HRESULT hr = CEditStage::Release(m_pStage);
			assert(hr != E_FAIL);	// 破棄失敗
		}
	}
#endif
}

//============================================================
//	エディット状況取得処理
//============================================================
bool CEditManager::IsEdit(void) const
{
	// エディット状況を返す
	return m_bEdit;
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
//	エディットタイプの変更処理
//============================================================
void CEditManager::ChangeEditorType(void)
{
	// エディターのタイプ変更
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報
	if (pKeyboard->IsTrigger(KEY_CHANGE_EDITOR))
	{
		// エディター情報の破棄
		SAFE_REF_RELEASE(m_pEditor);

		// エディットタイプの変更
		m_typeEditor = (CEditor::EType)((m_typeEditor + 1) % CEditor::TYPE_MAX);

		if (m_pEditor == nullptr)
		{
			// エディター情報の生成
			m_pEditor = CEditor::Create(this, m_typeEditor);
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
	DebugProc::Print(DebugProc::POINT_RIGHT, "%s：[エディットタイプ]\n", TYPE_NAME[m_typeEditor]);

	// エディター情報の情報表示
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugInfo();
}

//============================================================
//	ステージ保存処理
//============================================================
void CEditManager::SaveStage(void)
{
	// ステージを保存
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// キーボード情報
	if (pKeyboard->IsPress(KEY_DOUBLE))
	{
		if (pKeyboard->IsTrigger(KEY_SAVE))
		{
			// 保存処理
			Save();

			// 保存した状態にする
			m_bSave = true;
		}
	}
}

//============================================================
//	保存処理
//============================================================
void CEditManager::Save(void)
{
	// ポインタを宣言
	FILE *pFile = nullptr;	// ファイルポインタ

	// ファイルを書き出し形式で開く
	pFile = fopen(SAVE_TXT, "w");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		// 見出しを書き出し
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#	ステージセーブテキスト [save_stage.txt]\n");
		fprintf(pFile, "#	Author : you\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "---------->--<---------- ここから下を コピーし貼り付け ---------->--<----------\n\n");

#if 0
		// 情報保存
		m_pStage->SaveInfo();

		for (int nCntThing = 0; nCntThing < CEditStage::THING_MAX; nCntThing++)
		{ // 配置物の総数分繰り返す

			// エディットステージの破棄
			if (m_pStage != nullptr)
			{ // エディットステージが使用されている場合

				HRESULT hr = CEditStage::Release(m_pStage);
				assert(hr != E_FAIL);	// 破棄失敗
			}

			// エディットステージの生成
			if (m_pStage == nullptr)
			{ // エディットステージが使用されていない場合

				m_pStage = CEditStage::Create(this, (CEditStage::EThing)nCntThing);
				assert(m_pStage != nullptr);	// 生成失敗
			}

			// 配置物の保存
			m_pStage->Save(pFile);
		}

		// エディットステージの破棄
		if (m_pStage != nullptr)
		{ // エディットステージが使用されている場合

			HRESULT hr = CEditStage::Release(m_pStage);
			assert(hr != E_FAIL);	// 破棄失敗
		}

		// エディットステージの生成
		if (m_pStage == nullptr)
		{ // エディットステージが使用されていない場合

			m_pStage = CEditStage::Create(this, m_thing);
			assert(m_pStage != nullptr);	// 生成失敗
		}

		// 情報読込
		m_pStage->LoadInfo();
#endif

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ステ－ジセーブファイルの書き出しに失敗！", "警告！", MB_ICONWARNING);
	}
}
