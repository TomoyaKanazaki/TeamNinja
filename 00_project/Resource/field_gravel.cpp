//==========================================
//
//  砂利道ギミック(field_gravel.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field_gravel.h"
#include "player_clone.h"
#include "player.h"

//==========================================
//  コンストラクタ
//==========================================
CGimmickGravel::CGimmickGravel()
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
	if (FAILED(CField::Init()))
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
	CField::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CGimmickGravel::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CField::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CGimmickGravel::Draw(CShader* pShader)
{
	// 親クラスの描画
	CField::Draw(pShader);
}

//===========================================
//  当たっていた場合の処理
//===========================================
void CGimmickGravel::Hit(CPlayerClone* pClone)
{
	// 分身に文字列を渡す
	pClone->AddFrags(GetFlag());
}

//==========================================
//  当たっていない場合の処理
//==========================================
void CGimmickGravel::Miss(CPlayerClone* pClone)
{
	// 分身からフラグを削除する
	pClone->SabFrags(GetFlag());
}

//===========================================
//  当たっていた場合の処理
//===========================================
void CGimmickGravel::Hit(CPlayer* pPlayer)
{
	// 分身に文字列を渡す
	pPlayer->AddFrags(GetFlag());
}

//==========================================
//  当たっていない場合の処理
//==========================================
void CGimmickGravel::Miss(CPlayer* pPlayer)
{
	// 分身からフラグを削除する
	pPlayer->SabFrags(GetFlag());
}
