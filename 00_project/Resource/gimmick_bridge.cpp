//===========================================
//
//  橋のギミック(gimmick_bridge.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "gimmick_bridge.h"

//===========================================
//  コンストラクタ
//===========================================
CGimmickBridge::CGimmickBridge()
{
}

//===========================================
//  デストラクタ
//===========================================
CGimmickBridge::~CGimmickBridge()
{
}

//===========================================
//  初期化処理
//===========================================
HRESULT CGimmickBridge::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CGimmickAction::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
//  終了処理
//===========================================
void CGimmickBridge::Uninit(void)
{
	// 親クラスの終了
	CGimmickAction::Uninit();
}

//===========================================
//  更新処理
//===========================================
void CGimmickBridge::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CGimmickAction::Update(fDeltaTime);
}

//===========================================
//  描画処理
//===========================================
void CGimmickBridge::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmickAction::Draw(pShader);
}

//===========================================
//  各分身毎の待機位置を算出
//===========================================
D3DXVECTOR3 CGimmickBridge::CalcWaitPoint(const int Idx) const
{
	return D3DXVECTOR3();
}
