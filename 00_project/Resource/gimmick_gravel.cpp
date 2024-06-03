//==========================================
//
//  砂利道ギミック(gimmick_gravel.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_gravel.h"

//==========================================
//  コンストラクタ
//==========================================
CGimmickGravel::CGimmickGravel() : CGimmickState()
{

}

//==========================================
//  デストラクタ
//==========================================
CGimmickGravel::~CGimmickGravel()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CGimmickGravel::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CGimmickState::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void CGimmickGravel::Uninit(void)
{
	// 親クラスの終了
	CGimmickState::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CGimmickGravel::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CGimmickState::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CGimmickGravel::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmickState::Draw(pShader);
}