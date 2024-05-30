////============================================================
////
////	エディットコリジョン処理 [editCollision.cpp]
////	Author：小原立暉
////
////============================================================
////************************************************************
////	インクルードファイル
////************************************************************
//#include "manager.h"
//#include "editCollision.h"
//#include "editManager.h"
//
//#include "editorObject.h"
//
////************************************************************
////	マクロ定義
////************************************************************
//#define KEY_CHANGE_OBJECT	(DIK_2)	// オブジェクトタイプ変更キー
//#define NAME_CHANGE_OBJECT	("2")	// オブジェクトタイプ変更表示
//
////************************************************************
////	定数宣言
////************************************************************
//namespace
//{
//	const char* TYPE_NAME[] =	// オブジェクトタイプ名
//	{
//		"キューブ",
//		"円柱",
//		"球",
//	};
//}
//
////************************************************************
////	スタティックアサート
////************************************************************
//static_assert(NUM_ARRAY(TYPE_NAME) == CEditCollision::TYPE_MAX, "ERROR : Type Count Mismatch");
//
////************************************************************
////	親クラス [CEditCollision] のメンバ関数
////************************************************************
////============================================================
////	コンストラクタ
////============================================================
//CEditCollision::CEditCollision(CEditManager* pEditManager) : CEditor(pEditManager)
//{
//#if _DEBUG
//
//	// メンバ変数をクリア
//	m_pEditor = nullptr;	// エディター情報
//	m_type = TYPE_CUBE;		// オブジェクトタイプ
//
//#endif	// _DEBUG
//}
//
////============================================================
////	デストラクタ
////============================================================
//CEditCollision::~CEditCollision()
//{
//#if _DEBUG
//#endif	// _DEBUG
//}
//
////============================================================
////	初期化処理
////============================================================
//HRESULT CEditCollision::Init(void)
//{
//#if _DEBUG
//
//	// メンバ変数を初期化
//	m_pEditor = nullptr;	// エディター情報
//	m_type = TYPE_CUBE;		// オブジェクトタイプ
//
//	// エディター情報の生成
//	//m_pEditor = CEditorObject::Create(m_type);
//	if (m_pEditor == nullptr)
//	{ // 生成に失敗した場合
//
//		// 失敗を返す
//		assert(false);
//		return E_FAIL;
//	}
//
//	// 成功を返す
//	return S_OK;
//
//#else	// NDEBUG
//
//	// 成功を返す
//	return S_OK;
//
//#endif	// _DEBUG
//}
//
////============================================================
////	終了処理
////============================================================
//void CEditCollision::Uninit(void)
//{
//#if _DEBUG
//
//	// エディター情報の破棄
//	SAFE_REF_RELEASE(m_pEditor);
//
//#endif	// _DEBUG
//}
//
////============================================================
////	更新処理
////============================================================
//void CEditCollision::Update(void)
//{
//#if _DEBUG
//
//	// オブジェクトタイプの変更
//	ChangeObjectType();
//
//	// エディター情報の更新
//	assert(m_pEditor != nullptr);
//	m_pEditor->Update();
//
//#endif	// _DEBUG
//}
//
////============================================================
////	保存処理
////============================================================
//HRESULT CEditCollision::Save(void)
//{
//#if _DEBUG
//
//	// エディター情報の保存
//	assert(m_pEditor != nullptr);
//	if (FAILED(m_pEditor->Save()))
//	{ // セーブに失敗した場合
//
//		// 失敗を返す
//		assert(false);
//		return E_FAIL;
//	}
//
//	// 成功を返す
//	return S_OK;
//
//#else	// NDEBUG
//
//	// 成功を返す
//	return S_OK;
//
//#endif	// _DEBUG
//}
//
////============================================================
////	保存状況取得処理
////============================================================
//bool CEditCollision::IsSave(void)
//{
//#if _DEBUG
//
//	// エディター情報の保存
//	assert(m_pEditor != nullptr);
//	return m_pEditor->IsSave();
//
//#else	// NDEBUG
//
//	// falseを返す
//	return false;
//
//#endif	// _DEBUG
//}
//
////============================================================
////	情報保存処理
////============================================================
//void CEditCollision::SaveInfo(void)
//{
//#if _DEBUG
//
//	// エディター情報の情報保存
//	assert(m_pEditor != nullptr);
//	m_pEditor->SaveInfo();
//
//#endif	// _DEBUG
//}
//
////============================================================
////	情報読込処理
////============================================================
//void CEditCollision::LoadInfo(void)
//{
//#if _DEBUG
//
//	// エディター情報の情報読込
//	assert(m_pEditor != nullptr);
//	m_pEditor->LoadInfo();
//
//#endif	// _DEBUG
//}
//
////============================================================
////	操作表示の描画処理
////============================================================
//void CEditCollision::DrawDebugControl(void)
//{
//#if _DEBUG
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "エディットステージタイプ変更：[%s]\n", NAME_CHANGE_OBJECT);
//
//	// エディター情報の操作表示
//	assert(m_pEditor != nullptr);
//	m_pEditor->DrawDebugControl();
//
//#endif	// _DEBUG
//}
//
////============================================================
////	情報表示の描画処理
////============================================================
//void CEditCollision::DrawDebugInfo(void)
//{
//#if _DEBUG
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "%s：[エディットステージタイプ]\n", TYPE_NAME[m_type]);
//
//	// エディター情報の情報表示
//	assert(m_pEditor != nullptr);
//	m_pEditor->DrawDebugInfo();
//
//#endif	// _DEBUG
//}
//
////============================================================
////	オブジェクトタイプの変更処理
////============================================================
//void CEditCollision::ChangeObjectType(void)
//{
//	// オブジェクトタイプの変更
//	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// キーボード情報
//	if (pKeyboard->IsTrigger(KEY_CHANGE_OBJECT))
//	{
//		// エディター情報の破棄
//		SAFE_REF_RELEASE(m_pEditor);
//
//		// オブジェクトタイプの変更
//		m_type = (CEditCollision::EType)((m_type + 1) % CEditCollision::TYPE_MAX);
//
//		if (m_pEditor == nullptr)
//		{
//			// エディター情報の生成
//			//m_pEditor = CEditorObject::Create(m_type);
//			assert(m_pEditor != nullptr);	// 生成失敗
//		}
//	}
//}
