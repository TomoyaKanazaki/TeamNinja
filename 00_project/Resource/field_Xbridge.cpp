//==========================================
//
//  水場ギミック(field_Xbridge.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field_Xbridge.h"
#include "player_clone.h"
#include "player.h"

//==========================================
//  コンストラクタ
//==========================================
CGimmickXBridge::CGimmickXBridge() : CField()
{

}

//==========================================
//  デストラクタ
//==========================================
CGimmickXBridge::~CGimmickXBridge()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CGimmickXBridge::Init(void)
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
void CGimmickXBridge::Uninit(void)
{
	// 親クラスの終了
	CField::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CGimmickXBridge::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CField::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CGimmickXBridge::Draw(CShader* pShader)
{
	// 親クラスの描画
	CField::Draw(pShader);
}

//===========================================
//  当たっていた場合の処理
//===========================================
void CGimmickXBridge::Hit(CPlayerClone* pClone)
{
	// フラグを追加する
	pClone->AddFrags(GetFlag());
}

//==========================================
//  当たっていない場合の処理
//==========================================
void CGimmickXBridge::Miss(CPlayerClone* pClone)
{
	// フラグを削除する
	pClone->SabFrags(GetFlag());
}

//===========================================
//  当たっていた場合の処理
//===========================================
void CGimmickXBridge::Hit(CPlayer* pPlayer)
{
	// フラグを追加する
	pPlayer->AddFrags(GetFlag());
}

//==========================================
//  当たっていない場合の処理
//==========================================
void CGimmickXBridge::Miss(CPlayer* pPlayer)
{
	// フラグを削除する
	pPlayer->SabFrags(GetFlag());
}
