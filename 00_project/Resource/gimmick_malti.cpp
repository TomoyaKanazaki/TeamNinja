//=========================================
//
//  複数管理ギミック (gimmick_malti.cpp)
//  Author : Tomoya kanazaki
//  Adder  : Yuichi Fujita
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
	const int NUM_CLONE = 1;	// ボタン押し込みに必要な人数
}

//=========================================
//  コンストラクタ
//=========================================
CGimmickMalti::CGimmickMalti() : CGimmick(),
m_bActive (true) // アクティブフラグ
{
	// ボタン動的配列をクリア
	m_vecButton.clear();
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

	// 成功を返す
	return S_OK;
}

//=========================================
//  終了処理
//=========================================
void CGimmickMalti::Uninit(void)
{
	// ボタン動的配列をクリア
	m_vecButton.clear();

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
	for (const auto& rGimmick : m_vecButton)
	{ // ボタン配列分繰り返す

		// ギミックのフラグが立っていたら次に進む
		if (rGimmick->IsSet()) { continue; }

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
CGimmickMalti* CGimmickMalti::Create(std::vector<SButton> vecButton)
{
	// 複数管理ギミックの生成
	CGimmickMalti *pGimmickMalti = new CGimmickMalti;
	if (pGimmickMalti == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 複数管理ギミックの初期化
		if (FAILED(pGimmickMalti->Init()))
		{ // 初期化に失敗した場合

			// 複数管理ギミックの破棄
			SAFE_DELETE(pGimmickMalti);
			return nullptr;
		}

		// ボタン情報の生成
		if (FAILED(pGimmickMalti->CreateButton(vecButton)))
		{ // 生成に失敗した場合

			// 複数管理ギミックの破棄
			SAFE_DELETE(pGimmickMalti);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pGimmickMalti;
	}
}

//===========================================
//  ボタン情報の生成
//===========================================
HRESULT CGimmickMalti::CreateButton(std::vector<SButton> vecButton)
{
	for (const auto& rButton : vecButton)
	{ // ボタン配列分繰り返す

		// ボタンの生成
		CGimmick* pButton = CGimmick::Create
		( // 引数
			rButton.pos,			// 生成位置
			rButton.size,			// 生成大きさ
			CGimmick::TYPE_POST,	// ギミックラベル (ボタン)
			NUM_CLONE				// ボタン押込に必要な人数
		);
		if (pButton == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// 最後尾に生成したボタンを保存
		m_vecButton.push_back(pButton);
	}

	return S_OK;
}
