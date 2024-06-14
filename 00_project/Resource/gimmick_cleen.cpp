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
CGimmickCleen::CGimmickCleen() : CField()
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
void CGimmickCleen::Uninit(void)
{
	// 親クラスの終了
	CField::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CGimmickCleen::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CField::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CGimmickCleen::Draw(CShader* pShader)
{
	// 親クラスの描画
	CField::Draw(pShader);
}

//===========================================
//  当たっていた場合の処理
//===========================================
void CGimmickCleen::Hit(CPlayerClone* pClone)
{
	// 分身に文字列を渡す
	pClone->AddFrags(GetFlag());
}

//=========================================
//  当たっていない場合の処理
//===========================================
void CGimmickCleen::Miss(CPlayerClone* pClone)
{
	// 分身からフラグを削除する
	pClone->SabFrags(GetFlag());
}
