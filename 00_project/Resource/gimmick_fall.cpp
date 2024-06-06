//=========================================
//
//  落とし穴ギミック (gimmick_fall.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_fall.h"
#include "manager.h"
#include "player.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const float CLIMB_MAX = 300.0f; // 登れる限界の高さ
	const float CLIMB_SPEED = 200.0f; // 1秒間に登る速度
}

//=========================================
//  コンストラクタ
//=========================================
CGimmickFall::CGimmickFall() : CGimmickAction()
{

}

//=========================================
//  デストラクタ
//=========================================
CGimmickFall::~CGimmickFall()
{

}

//=========================================
//  初期化処理
//=========================================
HRESULT CGimmickFall::Init(void)
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
void CGimmickFall::Uninit(void)
{
	// 親クラスの終了
	CGimmickAction::Uninit();
}

//=========================================
//  更新処理
//=========================================
void CGimmickFall::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CGimmickAction::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CGimmickFall::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmickAction::Draw(pShader);
}
