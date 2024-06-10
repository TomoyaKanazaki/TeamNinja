//=========================================
//
//  デバッグ専用オブジェクト(絶対消す) [debug_object.cpp]
//  Author：Tomoya Kanazaki
//
//=========================================
#include "manager.h"
#include "debug_object.h"

#include "collision.h"
#include "gimmick.h"

//=========================================
//  定数定義
//=========================================
namespace
{
	const D3DXVECTOR3 DEBUGSIZE = D3DXVECTOR3(300.0f, 0.0f, 300.0f); // 当たり判定取るとこ
	const D3DXVECTOR3 DEBUGPOS = D3DXVECTOR3(1500.0f, 0.5f, 0.0f); // 発動可能人数の初期値
	const D3DXVECTOR3 CLONE_RADIUS = D3DXVECTOR3(20.0f, 0.0f, 20.0f);	// 半径
}

//=========================================
//  静的メンバ変数宣言
//=========================================
CListManager<CDebugObject>* CDebugObject::m_pList = nullptr; // オブジェクトリスト

//===========================================
//  コンストラクタ
//===========================================
CDebugObject::CDebugObject() : CObject3D(CObject::LABEL_GIMMICK, CObject::DIM_3D, 2)
{

}

//=========================================
//  デストラクタ
//=========================================
CDebugObject::~CDebugObject()
{

}

//=========================================
//  初期化処理
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

#ifdef _DEBUG

	// 色の設定
	SetColor(XCOL_RED);

#else

	// 色の設定
	SetColor(XCOL_AWHITE);

#endif // _DEBUG

	// ジャンプ台設置
	CGimmick::Create(D3DXVECTOR3(DEBUGPOS.x, 0.2f, DEBUGPOS.z + DEBUGSIZE.z * 0.5f), D3DXVECTOR3(300.0f, 0.0f, 150.0f), CGimmick::TYPE_JUMPTABLE, 2);
	CGimmick::Create(D3DXVECTOR3(DEBUGPOS.x, 0.2f, DEBUGPOS.z - DEBUGSIZE.z * 0.5f), D3DXVECTOR3(300.0f, 0.0f, 150.0f), CGimmick::TYPE_JUMPTABLE, 2);
	CGimmick::Create(D3DXVECTOR3(DEBUGPOS.x + DEBUGSIZE.x * 0.5f, 0.2f, DEBUGPOS.z), D3DXVECTOR3(150.0f, 0.0f, 300.0f), CGimmick::TYPE_JUMPTABLE, 2);
	CGimmick::Create(D3DXVECTOR3(DEBUGPOS.x - DEBUGSIZE.x * 0.5f, 0.2f, DEBUGPOS.z), D3DXVECTOR3(150.0f, 0.0f, 300.0f), CGimmick::TYPE_JUMPTABLE, 2);

	// 成功を返す
	return S_OK;
}

//=========================================
//  終了処理
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
//  更新処理
//=========================================
void CDebugObject::Update(const float fDeltaTime)
{
	// オブジェクト3Dの更新
	CObject3D::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CDebugObject::Draw(CShader* pShader)
{
	// オブジェクト3Dの描画
	CObject3D::Draw(pShader);
}

//==========================================
//  当たり判定
//==========================================
bool CDebugObject::Hit(const D3DXVECTOR3& rPos)
{
	D3DXVECTOR3 pos = GetVec3Position();	// 位置
	D3DXVECTOR3 size = GetVec3Sizing() * 0.5f;	// サイズ

	if (collision::Box2D
	(
		pos,		// 判定位置
		rPos,	// 判定目標位置
		size,		// 判定サイズ(右・上・後)
		size,		// 判定サイズ(左・下・前)
		CLONE_RADIUS,	// 判定目標サイズ(右・上・後)
		CLONE_RADIUS	// 判定目標サイズ(左・下・前)
	))
	{ // 四角の中に入った場合
		return true;
	}

	return false;
}

//=========================================
//  生成処理
//=========================================
CDebugObject* CDebugObject::Create()
{
	// デバッグ以外で生成するなゴミカス死ね死ね死ね
#ifndef _DEBUG
	return nullptr;
#endif

	// インスタンス生成
	CDebugObject* pDebug = new CDebugObject;

	// 生成に失敗した場合
	if (pDebug == nullptr) { return nullptr; }

	// 初期化
	if (FAILED(pDebug->Init()))
	{ // 初期化に失敗した場合

		// 破棄
		SAFE_DELETE(pDebug);
		return nullptr;
	}

	// 位置を設定
	pDebug->SetVec3Position(DEBUGPOS);

	// サイズを設定
	pDebug->SetVec3Sizing(DEBUGSIZE);

	// 確保したアドレスを返す
	return pDebug;
}

//=========================================
//  リスト取得
//=========================================
CListManager<CDebugObject>* CDebugObject::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}
