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
CGimmickWater::CGimmickWater() : CField()
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
void CGimmickWater::Uninit(void)
{
	// 親クラスの終了
	CField::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CGimmickWater::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CField::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CGimmickWater::Draw(CShader* pShader)
{
	// 親クラスの描画
	CField::Draw(pShader);
}

//===========================================
//  文字列(フラグ)の追加
//===========================================
void CGimmickWater::Hit(CPlayerClone* pClone)
{
	// 分身にフラグを追加する
	pClone->AddFrags(GetFlag());

	// 泥フラグを削除する
	pClone->SabFrags(GetFlag(TYPE_BOOB));
}

//=========================================
//  文字列(フラグ)の削除
//===========================================
void CGimmickWater::Miss(CPlayerClone* pClone)
{
	// 分身からフラグを削除する
	pClone->SabFrags(GetFlag());
}
