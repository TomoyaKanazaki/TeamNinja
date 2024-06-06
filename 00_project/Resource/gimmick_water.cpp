//==========================================
//
//  水場ギミック(gimmick_water.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_water.h"
#include "player_clone.h"

//==========================================
//  コンストラクタ
//==========================================
CGimmickWater::CGimmickWater() : CGimmickState()
{

}

//==========================================
//  デストラクタ
//==========================================
CGimmickWater::~CGimmickWater()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CGimmickWater::Init(void)
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
void CGimmickWater::Uninit(void)
{
	// 親クラスの終了
	CGimmickState::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CGimmickWater::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CGimmickState::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CGimmickWater::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmickState::Draw(pShader);
}

//===========================================
//  文字列(フラグ)の追加
//===========================================
void CGimmickWater::HitClone(CPlayerClone* pClone)
{
	// 分身にフラグを追加する
	pClone->AddFrags(WATER);

	// ドブフラグを削除する
	pClone->SabFrags(BOOB);
}

//=========================================
//  文字列(フラグ)の削除
//===========================================
void CGimmickWater::MissClone(CPlayerClone* pClone)
{
	// 分身からフラグを削除する
	pClone->SabFrags(WATER);
}
