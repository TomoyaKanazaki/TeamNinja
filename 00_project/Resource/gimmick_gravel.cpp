//==========================================
//
//  砂利道ギミック(gimmick_gravel.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_gravel.h"
#include "player_clone.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const char GRAVEL_FRAG = 'g'; // 砂利道のフラグ
}

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

//===========================================
//  文字列(フラグ)の追加
//===========================================
void CGimmickGravel::SetChar(CPlayerClone* pClone)
{
	// 分身に文字列を渡す
	pClone->AddFrag(GRAVEL_FRAG);
}

//=========================================
//  文字列(フラグ)の削除
//===========================================
void CGimmickGravel::DeleteChar(CPlayerClone* pClone)
{
	// 分身からフラグを削除する
	pClone->SabFrag(GRAVEL_FRAG);
}
