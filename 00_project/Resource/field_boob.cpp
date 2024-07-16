//==========================================
//
//  泥ギミック(field_boob.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field_boob.h"
#include "player_clone.h"
#include "player.h"
#include "liquid.h"

//==========================================
//  コンストラクタ
//==========================================
CGimmickBoob::CGimmickBoob() : CField(),
m_bLiquid(false)
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
void CGimmickBoob::Uninit(void)
{
	// 親クラスの終了
	CField::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CGimmickBoob::Update(const float fDeltaTime)
{
	// リキッドの生成
	if (!m_bLiquid)
	{
		m_bLiquid = true;
		CLiquid::Create
		(
			CLiquid::TYPE_BOOB,
			GetVec3Position(),
			GetVec3Rotation(),
			GetVec2Sizing(),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),
			GetTexPattern(),
			CLiquid::STexMove(D3DXVECTOR2(0.0005f, 0.0005f), D3DXVECTOR2(-0.0005f, -0.0005f))
		);
	}

	// 親クラスの更新
	CField::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CGimmickBoob::Draw(CShader* pShader)
{
	// 親クラスの描画
	CField::Draw(pShader);
}

//===========================================
//  文字列(フラグ)の追加
//===========================================
void CGimmickBoob::Hit(CPlayerClone* pClone)
{
	// 分身にフラグを追加する
	pClone->AddFrags(GetFlag());
}

//=========================================
//  文字列(フラグ)の削除
//===========================================
void CGimmickBoob::Miss(CPlayerClone* pClone)
{

}

// ========================================
//  文字列(フラグ)の追加
//===========================================
void CGimmickBoob::Hit(CPlayer* pPlayer)
{
	// 分身にフラグを追加する
	pPlayer->AddFrags(GetFlag());
}

//=========================================
//  文字列(フラグ)の削除
//===========================================
void CGimmickBoob::Miss(CPlayer* pPlayer)
{

}
