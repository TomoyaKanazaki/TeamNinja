//============================================================
//
//	エディット処理 [edit.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "edit.h"
#include "editPlay.h"
#include "editColl.h"
#include "manager.h"
#include "sceneGame.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *MODE_NAME[] =	// モード名
	{
		"プレイ",	// プレイモード
		"判定作成",	// 判定設定モード
	};
}

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_MODE_CHANGE		(DIK_F4)	// エディットモード変更キー
#define NAME_MODE_CHANGE	("F4")		// エディットモード変更表示

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(MODE_NAME) == CEdit::MODE_MAX, "ERROR : Mode Count Mismatch");

//************************************************************
//	子クラス [CEdit] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEdit::CEdit() : m_mode(MODE_PLAY)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEdit::~CEdit()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEdit::Init(void)
{
#if _DEBUG

	// メンバ変数を初期化
	m_mode = MODE_PLAY;	// エディットモード

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	終了処理
//============================================================
void CEdit::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEdit::Update(void)
{
#if _DEBUG

	if (GET_INPUTKEY->IsTrigger(KEY_MODE_CHANGE))
	{
		CEdit *pEdit = CSceneGame::GetEdit();	// エディット取得
		CEdit::NextMode(pEdit);					// エディット変更
		CSceneGame::SetEdit(pEdit);				// エディット設定
	}

#endif	// _DEBUG
}

//============================================================
//	描画処理
//============================================================
void CEdit::Draw(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	エディット操作表示
//============================================================
void CEdit::DrawEditControl(void)
{
#if _DEBUG

	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "[エディット操作]　\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "エディットモード変更：[%s]\n", NAME_MODE_CHANGE);

#endif	// _DEBUG
}

//============================================================
//	エディット情報表示
//============================================================
void CEdit::DrawEditData(void)
{
#if _DEBUG

	// ポインタを宣言
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// デバッグの情報

	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "[エディット情報]　\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "%s：[エディットモード]\n", MODE_NAME[m_mode]);

#endif	// _DEBUG
}

//============================================================
//	エディットモード取得処理
//============================================================
CEdit::EMode CEdit::GetMode(void)
{
#if _DEBUG

	// エディットモードを返す
	return m_mode;

#else	// NDEBUG

	return (EMode)0;

#endif	// _DEBUG
}

//============================================================
//	生成処理
//============================================================
#if _DEBUG

CEdit *CEdit::Create(const EMode mode)
{
	// エディットの生成
	CEdit *pEdit = nullptr;
	switch (mode)
	{ // モードごとの処理
	case MODE_PLAY:
		pEdit = new CEditPlay;
		break;

	case MODE_COLL_SET:
		pEdit = new CEditColl;
		break;

	default:
		assert(false);
		break;
	}

	if (pEdit == nullptr)
	{ // 生成していない場合

		return nullptr;
	}
	else
	{ // 生成している場合

		// エディットの初期化
		if (FAILED(pEdit->Init()))
		{ // 初期化に失敗した場合

			// エディットの破棄
			SAFE_DELETE(pEdit);
			return nullptr;
		}

		// 現在のモードを保存
		pEdit->m_mode = mode;

		// 確保したアドレスを返す
		return pEdit;
	}
}

#else	// NDEBUG

CEdit *CEdit::Create(const EMode /*mode*/)
{
	return nullptr;
}

#endif	// _DEBUG

//============================================================
//	破棄処理
//============================================================
#if _DEBUG

void CEdit::Release(CEdit *&prEdit)
{
	// エディットの終了
	assert(prEdit != nullptr);
	prEdit->Uninit();

	// メモリ開放
	SAFE_DELETE(prEdit);
}

#else	// NDEBUG

void CEdit::Release(CEdit *& /*prEdit*/)
{

}

#endif	// _DEBUG

//============================================================
//	次のモード設定処理
//============================================================
#if _DEBUG

void CEdit::NextMode(CEdit *&prEdit)
{
	EMode nextMode = (EMode)((prEdit->m_mode + 1) % MODE_MAX);	// 次のモード

	// エディットの破棄
	assert(prEdit != nullptr);
	SAFE_REF_RELEASE(prEdit);

	// エディットの生成
	assert(prEdit == nullptr);
	prEdit = CEdit::Create(nextMode);
}

#else	// NDEBUG

void CEdit::NextMode(CEdit *& /*prEdit*/)
{

}

#endif	// _DEBUG
