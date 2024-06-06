//==========================================
//
//  ドブギミック(gimmick_boob.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_boob.h"
#include "player_clone.h"

//==========================================
//  コンストラクタ
//==========================================
CGimmickBoob::CGimmickBoob() : CGimmickState()
{

}

//==========================================
//  デストラクタ
//==========================================
CGimmickBoob::~CGimmickBoob()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CGimmickBoob::Init(void)
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
void CGimmickBoob::Uninit(void)
{
	// 親クラスの終了
	CGimmickState::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CGimmickBoob::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CGimmickState::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CGimmickBoob::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmickState::Draw(pShader);
}

//===========================================
//  文字列(フラグ)の追加
//===========================================
void CGimmickBoob::HitClone(CPlayerClone* pClone)
{
	// 分身にフラグを追加する
	pClone->AddFrags(BOOB);
}

//=========================================
//  文字列(フラグ)の削除
//===========================================
void CGimmickBoob::MissClone(CPlayerClone* pClone)
{

}
