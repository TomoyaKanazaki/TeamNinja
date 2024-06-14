//=========================================
//
//  朽ちた床ギミック (gimmick_decayed.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_decayed.h"
#include "manager.h"
#include "player.h"

//=========================================
//  コンストラクタ
//=========================================
CGimmickDecaed::CGimmickDecaed() : CField(),
m_bFall(false) // 落ちるフラグ
{

}

//=========================================
//  デストラクタ
//=========================================
CGimmickDecaed::~CGimmickDecaed()
{

}

//=========================================
//  初期化処理
//=========================================
HRESULT CGimmickDecaed::Init(void)
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
void CGimmickDecaed::Uninit(void)
{
	// 親クラスの終了
	CField::Uninit();
}

//=========================================
//  更新処理
//=========================================
void CGimmickDecaed::Update(const float fDeltaTime)
{
	if (m_bFall) { DebugProc::Print(DebugProc::POINT_CENTER, "落ちるon\n"); }

	// 親クラスの更新
	CField::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CGimmickDecaed::Draw(CShader* pShader)
{
	// 親クラスの描画
	CField::Draw(pShader);
}
