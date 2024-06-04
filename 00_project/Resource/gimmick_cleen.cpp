//==========================================
//
//  掃除したての床ギミック(gimmick_cleen.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_cleen.h"
#include "player_clone.h"

//==========================================
//  コンストラクタ
//==========================================
CGimmickCleen::CGimmickCleen() : CGimmickState()
{

}

//==========================================
//  デストラクタ
//==========================================
CGimmickCleen::~CGimmickCleen()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CGimmickCleen::Init(void)
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
void CGimmickCleen::Uninit(void)
{
	// 親クラスの終了
	CGimmickState::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CGimmickCleen::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CGimmickState::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CGimmickCleen::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmickState::Draw(pShader);
}

//===========================================
//  
//===========================================
void CGimmickCleen::HitClone(CPlayerClone* pClone)
{
	// 分身に文字列を渡す
	pClone->AddFrags(CLEEN);
}

//=========================================
//  文字列(フラグ)の削除
//===========================================
void CGimmickCleen::MissClone(CPlayerClone* pClone)
{
	// 分身からフラグを削除する
	pClone->SabFrags(CLEEN);
}
