//============================================================
//
//	エディターオブジェクト処理 [editorObject.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "editorObject.h"
#include "manager.h"
#include "editManager.h"

#include "editField.h"
#include "editWall.h"
#include "editActor.h"
#include "editCheckPoint.h"
#include "editGoalPoint.h"
#include "editChanger.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{

}

//************************************************************
//	親クラス [CEditorObject] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEditorObject::CEditorObject(CEditStage* pEditor)
{
#if _DEBUG

	// メンバ変数をクリア
	m_pEditor = pEditor;	// エディター情報

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CEditorObject::~CEditorObject()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	初期化処理
//============================================================
HRESULT CEditorObject::Init(void)
{
#if _DEBUG

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
void CEditorObject::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	更新処理
//============================================================
void CEditorObject::Update(void)
{
#if _DEBUG

	// 位置の更新
	UpdatePosition();

	// 向きの更新
	UpdateRotation();

#endif	// _DEBUG
}

//============================================================
//	操作表示の描画処理
//============================================================
void CEditorObject::DrawDebugControl(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	情報表示の描画処理
//============================================================
void CEditorObject::DrawDebugInfo(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	生成処理
//============================================================
CEditorObject *CEditorObject::Create(CEditStage* pEditor, CEditStage::EType type)
{
#if _DEBUG

	// ポインタを宣言
	CEditorObject *pEditorObject = nullptr;	// エディターオブジェクト情報

	// エディターオブジェクトの生成
	switch (type)
	{ // 種類ごとの処理
	case CEditStage::TYPE_FIELD:
		pEditorObject = new CEditField(pEditor);		// エディットフィールド
		break;

	case CEditStage::TYPE_WALL:
		pEditorObject = new CEditWall(pEditor);			// エディットウォール
		break;

	case CEditStage::TYPE_ACTOR:
		pEditorObject = new CEditActor(pEditor);		// エディットアクター
		break;

	case CEditStage::TYPE_CHECKPOINT:
		pEditorObject = new CEditCheckPoint(pEditor);	// エディットチェックポイント
		break;

	case CEditStage::TYPE_GOALPOINT:
		pEditorObject = new CEditGoalPoint(pEditor);	// エディットゴールポイント
		break;

	case CEditStage::TYPE_GIMMICK:
		pEditorObject = new CEditActor(pEditor);		// エディットギミック TODO：あとでやる
		break;

	case CEditStage::TYPE_CHANGER:
		pEditorObject = new CEditChanger(pEditor);		// エディットカメラ変更地点
		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (pEditorObject == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// エディターオブジェクトの初期化
		if (FAILED(pEditorObject->Init()))
		{ // 初期化に失敗した場合

			// エディターオブジェクトの破棄
			SAFE_DELETE(pEditorObject);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pEditorObject;
	}

#else	// NDEBUG

	// nullptrを返す
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	破棄処理
//============================================================
void CEditorObject::Release(CEditorObject *&prEditorObject)
{
#if _DEBUG

	// エディターオブジェクトの終了
	assert(prEditorObject != nullptr);
	prEditorObject->Uninit();

	// メモリ開放
	SAFE_DELETE(prEditorObject);

#endif	// _DEBUG
}

//============================================================
//	位置の更新処理
//============================================================
void CEditorObject::UpdatePosition(void)
{
	// 位置の更新
	m_pEditor->UpdatePosition();
}

//============================================================
//	向き更新処理
//============================================================
void CEditorObject::UpdateRotation(void)
{
	// 向きの更新
	m_pEditor->UpdateRotation();
}
