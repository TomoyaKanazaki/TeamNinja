//==========================================
//
//  水場ギミック(field_water.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field_water.h"
#include "player_clone.h"
#include "player.h"
#include "liquid.h"

//==========================================
//  コンストラクタ
//==========================================
CGimmickWater::CGimmickWater() : CField(),
m_bLiquid(false)
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

	// 描画を消す
	SetEnableDraw(false);

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
	// リキッドの生成
	if (!m_bLiquid)
	{
		m_bLiquid = true;
		CLiquid::Create
		(
			CLiquid::TYPE_WATER,
			GetVec3Position(),
			GetVec3Rotation(),
			GetVec2Sizing(),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),
			GetTexPattern(),
			CLiquid::STexMove(D3DXVECTOR2(0.0025f, 0.0025f), D3DXVECTOR2(-0.0025f, -0.0025f))
		);
	}

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
//  当たっていた場合の処理
//===========================================
void CGimmickWater::Hit(CPlayerClone* pClone)
{
	// 分身にフラグを追加する
	pClone->AddFrags(GetFlag());

	// 泥フラグを削除する
	pClone->SabFrags(GetFlag(TYPE_BOOB));
}

//==========================================
//  当たっていない場合の処理
//==========================================
void CGimmickWater::Miss(CPlayerClone* pClone)
{
	// 分身からフラグを削除する
	pClone->SabFrags(GetFlag());
}

//===========================================
//  当たっていた場合の処理
//===========================================
void CGimmickWater::Hit(CPlayer* pPlayer)
{
	// 分身にフラグを追加する
	pPlayer->AddFrags(GetFlag());

	// 泥フラグを削除する
	pPlayer->SabFrags(GetFlag(TYPE_BOOB));
}

//==========================================
//  当たっていない場合の処理
//==========================================
void CGimmickWater::Miss(CPlayer* pPlayer)
{
	// 分身からフラグを削除する
	pPlayer->SabFrags(GetFlag());
}
