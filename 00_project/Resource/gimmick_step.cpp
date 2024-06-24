//=========================================
//
//  踏み台ギミック (gimmick_step.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_step.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const float CLIMB_SPEED = 600.0f; // 1秒間に登る速度
}

//=========================================
//  コンストラクタ
//=========================================
CGimmickStep::CGimmickStep() : CGimmickAction(),
m_bClimb(false) // 登っているフラグ
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
	// 登る
	if (IsActive())
	{
		// プレイヤー情報を取得
		CPlayer* player = GET_PLAYER;

		// フラグを一時保存
		bool bTemp = m_bClimb;

		// 上昇判定
		if (!Climb(player) && bTemp)
		{
			// プレイヤー移動量を設定
			D3DXVECTOR3 move = player->GetMove();
			move.y = 0.0f;
			player->SetMove(move);

			// ジャンプ
			player->GimmickLowJump();

			// フラグを更新
			m_bClimb = false;
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
//  各分身毎の待機位置を算出
//===========================================
D3DXVECTOR3 CGimmickStep::CalcWaitPoint(const int Idx) const
{
	// 受け取ったインデックスが最大値を超えている場合警告
	if (Idx > GetNumActive()) { assert(false); }

	// 待機位置を取得
	D3DXVECTOR3 pos = GetActionPoint();

	// 自分の番号に合わせて高さを加算する
	pos.y += Idx * CPlayerClone::GetHeight();

	return pos;
}

//===========================================
//  登る
//===========================================
bool CGimmickStep::Climb(CPlayer* player)
{
	// xz平面で接触していない場合関数を抜ける
	if (!DistancePlayer()) { return false; }

	// プレイヤーのy座標を取得
	float fPlayer = player->GetVec3Position().y;

	// 頂上の座標を算出
	float fHeight = GetVec3Position().y + (CPlayerClone::GetHeight() * GetNumActive());

	// プレイヤーが頂上より上にいる場合関数を抜ける
	if (fPlayer > fHeight) { return false; }

	// プレイヤーの移動量を加算する
	D3DXVECTOR3 move = player->GetMove();
	move.y = CLIMB_SPEED;
	player->SetMove(move);

	// フラグを立てる
	m_bClimb = true;
	return m_bClimb;
}
