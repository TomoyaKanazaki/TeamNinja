//=========================================
//
//  落とし穴ギミック (gimmick_fall.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_fall.h"
#include "manager.h"
#include "player_clone.h"

//=========================================
//  コンストラクタ
//=========================================
CGimmickFall::CGimmickFall() : CField(),
m_bFall(false) // 落下フラグ
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
	if (FAILED(CField::Init()))
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
	CField::Uninit();
}

//=========================================
//  更新処理
//=========================================
void CGimmickFall::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CField::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CGimmickFall::Draw(CShader* pShader)
{
	// 親クラスの描画
	CField::Draw(pShader);
}

//===========================================
//  当たっていた場合の処理
//===========================================
void CGimmickFall::Hit(CPlayerClone* pClone)
{
	// 分身に文字列を渡す
	pClone->AddFrags(GetFlag());
}

//==========================================
//  当たっていない場合の処理
//==========================================
void CGimmickFall::Miss(CPlayerClone* pClone)
{
	// 分身からフラグを削除する
	pClone->SabFrags(GetFlag());
}
