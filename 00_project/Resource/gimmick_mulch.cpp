//=========================================
//
//  複数管理ギミック (gimmick_mulch.cpp)
//  Author : Tomoya kanazaki
//  Adder  : Yuichi Fujita
//
//=========================================
#include "gimmick_mulch.h"
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
CGimmickMulch::CGimmickMulch() : CGimmick(),
m_bActive (true) // アクティブフラグ
{
	// ボタン動的配列をクリア
	m_vecButton.clear();
}

//=========================================
//  デストラクタ
//=========================================
CGimmickMulch::~CGimmickMulch()
{

}

//=========================================
//  初期化処理
//=========================================
HRESULT CGimmickMulch::Init(void)
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
void CGimmickMulch::Uninit(void)
{
	// ボタン動的配列をクリア
	m_vecButton.clear();

	// 親クラスの終了
	CGimmick::Uninit();
}

//=========================================
//  更新処理
//=========================================
void CGimmickMulch::Update(const float fDeltaTime)
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

	// 親クラスの更新
	CGimmick::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CGimmickMulch::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmick::Draw(pShader);
}

//===========================================
//  設置ギミックの生成
//===========================================
CGimmickMulch* CGimmickMulch::Create(std::vector<SButton> vecButton)
{
	// 複数管理ギミックの生成
	CGimmickMulch *pGimmick = new CGimmickMulch;
	if (pGimmick == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 複数管理ギミックの初期化
		if (FAILED(pGimmick->Init()))
		{ // 初期化に失敗した場合

			// 複数管理ギミックの破棄
			SAFE_DELETE(pGimmick);
			return nullptr;
		}

		// ボタン情報の生成
		if (FAILED(pGimmick->CreateButton(vecButton)))
		{ // 生成に失敗した場合

			// 複数管理ギミックの破棄
			SAFE_DELETE(pGimmick);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pGimmick;
	}
}

//===========================================
//  ボタン情報の生成
//===========================================
HRESULT CGimmickMulch::CreateButton(std::vector<SButton> vecButton)
{
	for (const auto& rButton : vecButton)
	{ // ボタン配列分繰り返す

		// ボタンの生成
		CGimmick* pButton = CGimmick::Create
		( // 引数
			rButton.pos,			// 生成位置
			EAngle::ANGLE_0,		// 生成角度
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
