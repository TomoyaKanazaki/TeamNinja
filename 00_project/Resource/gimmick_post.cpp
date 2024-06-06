//=========================================
//
//  設置ギミック (gimmick_post.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_post.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"

//=========================================
//  コンストラクタ
//=========================================
CGimmickPost::CGimmickPost() : CGimmickAction()
{

}

//=========================================
//  デストラクタ
//=========================================
CGimmickPost::~CGimmickPost()
{

}

//=========================================
//  初期化処理
//=========================================
HRESULT CGimmickPost::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CGimmickAction::Init()))
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
void CGimmickPost::Uninit(void)
{
	// 親クラスの終了
	CGimmickAction::Uninit();
}

//=========================================
//  更新処理
//=========================================
void CGimmickPost::Update(const float fDeltaTime)
{
	// アクティブ状態になっていたら設置フラグをon
	m_bPost = IsActive();

	// 親クラスの更新
	CGimmickAction::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CGimmickPost::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmickAction::Draw(pShader);
}
