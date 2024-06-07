//=========================================
//
//  朽ちた床ギミック (gimmick_decayed.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_decayed.h"
#include "manager.h"
#include "player.h"

//=========================================
//  コンストラクタ
//=========================================
CGimmickDecaed::CGimmickDecaed() : CGimmickAction()
{

}

//=========================================
//  デストラクタ
//=========================================
CGimmickDecaed::~CGimmickDecaed()
{

}

//=========================================
//  初期化処理
//=========================================
HRESULT CGimmickDecaed::Init(void)
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
void CGimmickDecaed::Uninit(void)
{
	// 親クラスの終了
	CGimmickAction::Uninit();
}

//=========================================
//  更新処理
//=========================================
void CGimmickDecaed::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CGimmickAction::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CGimmickDecaed::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmickAction::Draw(pShader);
}
