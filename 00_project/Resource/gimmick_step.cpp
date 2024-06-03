//=========================================
//
//  踏み台ギミック (gimmick_step.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_step.h"
#include "manager.h"
#include "player.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const float CLIMB_MAX = 300.0f; // 登れる限界の高さ
	const float CLIMB_SPEED = 150.0f; // 1秒間に登る速度
}

//=========================================
//  コンストラクタ
//=========================================
CGimmickStep::CGimmickStep() : CGimmickAction()
{

}

//=========================================
//  デストラクタ
//=========================================
CGimmickStep::~CGimmickStep()
{

}

//=========================================
//  初期化処理
//=========================================
HRESULT CGimmickStep::Init(void)
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

//=========================================
//  終了処理
//=========================================
void CGimmickStep::Uninit(void)
{
	// 親クラスの終了
	CGimmickAction::Uninit();
}

//=========================================
//  更新処理
//=========================================
void CGimmickStep::Update(const float fDeltaTime)
{
	// プレイヤーの座標を取得
	CPlayer* player = GET_PLAYER;

	// プレイヤーの座標が上昇限界じゃない場合判定を取る
	if (player->GetVec3Position().y >= GetVec3Position().y + CLIMB_MAX)
	{
		// プレイヤーとの当たり判定
		if (CollisionPlayer())
		{
			// 登る
			Climb(player);
		}
	}

	// 親クラスの更新
	CGimmickAction::Update(fDeltaTime);
}

//=========================================
//  描画処理
//=========================================
void CGimmickStep::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmickAction::Draw(pShader);
}

//===========================================
//  登る
//===========================================
void CGimmickStep::Climb(CPlayer* player)
{
	// プレイヤーの移動量を取得
	D3DXVECTOR3 movePlasyer = player->GetMove();

	// yの移動量を加算
	movePlasyer.y = CLIMB_SPEED;

	// 移動量を適用する
	player->SetMove(movePlasyer);
}
