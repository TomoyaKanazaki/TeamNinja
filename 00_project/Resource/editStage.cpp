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

//************************************************************
//	親クラス [CEditStage] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditStage::CEditStage()
{
#if _DEBUG

	// メンバ変数をクリア
	m_pEditManager = nullptr;	// エディットマネージャーの情報

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
//	生成処理
//============================================================
CEditStage *CEditStage::Create(CEditManager *pEditManager, EType type)
{
#if _DEBUG

	// ポインタを宣言
	CEditStage *pEditStage = nullptr;	// エディットステージ情報

	// エディットステージの生成
	switch (type)
	{ // 種類ごとの処理
	case TYPE_FIELD:
		//pEditStage = new CEditFiled;	// エディットフィールド
		break;

	case TYPE_WALL:
		//pEditStage = new CEditWall;	// エディットウォール
		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (pEditStage == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// エディットステージの初期化
		if (FAILED(pEditStage->Init()))
		{ // 初期化に失敗した場合

			// エディットステージの破棄
			SAFE_DELETE(pEditStage);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pEditStage;
	}

#else	// NDEBUG

	// nullptrを返す
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	破棄処理
//============================================================
void CEditStage::Release(CEditStage *&prEditStage)
{
#if _DEBUG

	// エディットステージの終了
	assert(prEditStage != nullptr);
	prEditStage->Uninit();

	// メモリ開放
	SAFE_DELETE(prEditStage);

#else	// NDEBUG

	// 成功を返す
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	エディットマネージャー取得処理
//============================================================
const CEditManager *CEditStage::GetPtrEditManager(void) const
{
#if _DEBUG

	// エディットマネージャーを返す
	return m_pEditManager;

#else	// NDEBUG

	// nullptrを返す
	return nullptr;

#endif	// _DEBUG
}
