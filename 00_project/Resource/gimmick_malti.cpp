//=========================================
//
//  複数管理ギミック (gimmick_malti.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_malti.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const int NUM_GIMMICK = 3; // 管理するギミックの数
}

//=========================================
//  コンストラクタ
//=========================================
CGimmickMalti::CGimmickMalti() : CGimmick(),
m_pGimmick (nullptr), // 管理するギミック
m_bActive (true) // アクティブフラグ
{

}

//=========================================
//  デストラクタ
//=========================================
CGimmickMalti::~CGimmickMalti()
{

}

//=========================================
//  初期化処理
//=========================================
HRESULT CGimmickMalti::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CGimmick::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 設置ギミックを生成
	if (FAILED(Create()))
	{
		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//=========================================
//  終了処理
//=========================================
void CGimmickMalti::Uninit(void)
{
	// リストマネージャーの破棄
	m_pGimmick->Release(m_pGimmick);

	// 親クラスの終了
	CGimmick::Uninit();
}

//=========================================
//  更新処理
//=========================================
void CGimmickMalti::Update(const float fDeltaTime)
{
	// フラグをリセット
	m_bActive = true;

	// 管理しているギミックの設置フラグを取得
	for (int i = 0; i < NUM_GIMMICK; ++i)
	{
		// ギミックを取得
		CGimmick* pGimmick = *m_pGimmick->GetIndex(i);

		// ギミックのフラグが立っていたら次に進む
		if (pGimmick->IsSet()) { continue; }

		// 1つでもフラグがoffならフラグをfalseにする
		m_bActive = false;
		break;
	}

	if (m_bActive) { DebugProc::Print(DebugProc::POINT_CENTER, "設置OK\n"); }

	// 親クラスの更新
	CGimmick::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CGimmickMalti::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmick::Draw(pShader);
}

//===========================================
//  設置ギミックの生成
//===========================================
HRESULT CGimmickMalti::Create()
{
	if (m_pGimmick == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pGimmick = CListManager<CGimmick>::Create();
		if (m_pGimmick == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// ギミック管理リストにオブジェクトを追加・イテレーターを取得
	for (int i = 0; i < NUM_GIMMICK; ++i)
	{
		m_iterator = m_pGimmick->AddList(CGimmick::Create(D3DXVECTOR3(0.0f, 0.0f, -500 + -500.0f * i), D3DXVECTOR3(100.0f, 0.0f, 50.0f), CGimmick::TYPE_POST, 1));
	}

	return S_OK;
}
