//=========================================
//
//  デバッグ専用オブジェクト(絶対消す) [debug_object.cpp]
//  Author：Tomoya Kanazaki
//
//=========================================
#include "manager.h"
#include "debug_object.h"

//=========================================
//  定数定義
//=========================================
namespace
{
	const int PRIORITY = 2;				// ギミック範囲ポリゴンの優先順位
	const int INIT_NUM_ACTIVE = -1;		// 発動可能人数の初期値
}

//=========================================
//	静的メンバ変数宣言
//=========================================
CListManager<CDebugObject>* CDebugObject::m_pList = nullptr;	// オブジェクトリスト

//===========================================
//  コンストラクタ
//===========================================
CDebugObject::CDebugObject()
{
}

//=========================================
//	デストラクタ
//=========================================
CDebugObject::~CDebugObject()
{

}

//=========================================
//	初期化処理
//=========================================
HRESULT CDebugObject::Init(void)
{
	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CDebugObject>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//=========================================
//	終了処理
//=========================================
void CDebugObject::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//=========================================
//	更新処理
//=========================================
void CDebugObject::Update(const float fDeltaTime)
{
	// オブジェクト3Dの更新
	CObject3D::Update(fDeltaTime);
}

//=========================================
//	描画処理
//=========================================
void CDebugObject::Draw(CShader* pShader)
{
	// オブジェクト3Dの描画
	CObject3D::Draw(pShader);
}

//=========================================
//	生成処理
//=========================================
CDebugObject* CDebugObject::Create
(
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rSize		// サイズ
)
{
	// インスタンス生成
	CDebugObject* pDebug = new CDebugObject;

	// 生成に失敗した場合
	if (pDebug == nullptr) { return nullptr; }

	// 分身のUIの初期化
	if (FAILED(pDebug->Init()))
	{ // 初期化に失敗した場合

		// 分身のUIの破棄
		SAFE_DELETE(pDebug);
		return nullptr;
	}

	// 位置を設定
	pDebug->SetVec3Position(rPos);

	// サイズを設定
	pDebug->SetVec3Sizing(rSize);

	// 確保したアドレスを返す
	return pDebug;
}

//=========================================
// リスト取得
//=========================================
CListManager<CDebugObject>* CDebugObject::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}
