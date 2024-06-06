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

//=========================================
//  コンストラクタ
//=========================================
CGimmickMalti::CGimmickMalti() : CGimmick()
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

	// 成功を返す
	return S_OK;
}

//=========================================
//  終了処理
//=========================================
void CGimmickMalti::Uninit(void)
{
	// 親クラスの終了
	CGimmick::Uninit();
}

//=========================================
//  更新処理
//=========================================
void CGimmickMalti::Update(const float fDeltaTime)
{
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
