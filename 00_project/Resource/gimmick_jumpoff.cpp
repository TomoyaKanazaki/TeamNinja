//============================================================
//
// 飛び降り [gimmick_jumpoff.cpp]
// Author：佐藤根詩音
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "gimmick_jumpoff.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"

//============================================================
//	コンストラクタ
//============================================================
CGimmickJumpOff::CGimmickJumpOff() : CGimmickAction()
{

}

//============================================================
//	デストラクタ
//============================================================
CGimmickJumpOff::~CGimmickJumpOff()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGimmickJumpOff::Init(void)
{
	// ギミックアクションの初期化
	if (FAILED(CGimmickAction::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGimmickJumpOff::Uninit(void)
{
	// ギミックアクションの終了
	CGimmickAction::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGimmickJumpOff::Update(const float fDeltaTime)
{
	if (GET_PLAYER->IsJump())
	{ // プレイヤーがジャンプしている場合

		// プレイヤーとの当たり判定
		if (CollisionPlayer())
		{
			if (GetMoment() == false)
			{ // ギミック発動してない場合

				D3DXVECTOR3 pos = GetVec3Position();		// 発動位置

				if ((GET_PLAYER->GetVec3Position().y - (pos.y + CPlayerClone::GetHeight())) <= 10.0f)
				{ // 特定の高さまで下がった場合

					// 発動中
					SetMoment(GET_PLAYER->GimmickLand());
				}
			}
		}
		else
		{ // 範囲内じゃない場合

			// 発動しない
			SetMoment(false);
		}
	}
	
	// ギミックアクションの更新
	CGimmickAction::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CGimmickJumpOff::Draw(CShader* pShader)
{
	// ギミックアクションの描画
	CGimmickAction::Draw(pShader);
}

//===========================================
//  各分身毎の待機位置を算出
//===========================================
D3DXVECTOR3 CGimmickJumpOff::CalcWaitPoint(const int Idx) const
{
	// 受け取ったインデックスが最大値を超えている場合警告
	if (Idx > GetNumActive()) { assert(false); }

	// TODO : 飛び降りの待機位置
	return GetVec3Position();
}
