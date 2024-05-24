//============================================================
//
//	エディター処理 [editor.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editor.h"
#include "manager.h"
#include "editManager.h"
#include "editStage.h"

//************************************************************
//	親クラス [CEditor] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditor::CEditor(CEditManager *pEditManager)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pEditManager = pEditManager;	// エディットマネージャー

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditor::~CEditor()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	生成処理
//============================================================
CEditor *CEditor::Create(CEditManager *pEditManager, EType type)
{
#if _DEBUG

	// ポインタを宣言
	CEditor *pEditor = nullptr;	// エディター情報

	// エディターの生成
	switch (type)
	{ // 種類ごとの処理
	case TYPE_STAGE:
		pEditor = new CEditStage(pEditManager);	// エディットステージ
		break;

	case TYPE_COLLISION:

		// TODO：エディット当たり判定できたら置き換え
		//pEditor = new CEditCollision(pEditManager);	// エディット当たり判定
		pEditor = new CEditStage(pEditManager);	// エディットステージ

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (pEditor == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// エディターの初期化
		if (FAILED(pEditor->Init()))
		{ // 初期化に失敗した場合

			// エディターの破棄
			SAFE_DELETE(pEditor);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pEditor;
	}

#else	// NDEBUG

	// nullptrを返す
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	破棄処理
//============================================================
void CEditor::Release(CEditor *&prEditor)
{
#if _DEBUG

	// エディターの終了
	assert(prEditor != nullptr);
	prEditor->Uninit();

	// メモリ開放
	SAFE_DELETE(prEditor);

#endif	// _DEBUG
}

//============================================================
//	エディットマネージャー取得処理
//============================================================
CEditManager *CEditor::GetPtrEditManager(void) const
{
#if _DEBUG

	// エディットマネージャーを返す
	return m_pEditManager;

#else	// NDEBUG

	// nullptrを返す
	return nullptr;

#endif	// _DEBUG
}
