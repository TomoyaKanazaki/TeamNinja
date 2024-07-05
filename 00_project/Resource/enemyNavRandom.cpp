//============================================================
//
//	敵のナビゲーション(ランダム座標)処理 [enemyNavRandom.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "enemyNavRandom.h"

#include "objectMeshCube.h"

#include "effect3D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int STOP_COUNT = 100;				// 停止カウント
	const float ROT_CORRECT_DIFF = 0.01f;	// 向きを補正する差分
	const int DEST_ROT_RAND = 101;			// 目的の向きのランダム数
	const int DEST_ROT_MIN = 50;			// 目的の向きの最低保障
	const int MOVE_COUNT_RAND = 30;			// 移動カウントのランダム数
	const int MOVE_COUNT_MIN = 70;			// 移動カウントの最低保障
}

//************************************************************
//	子クラス [CEnemyNavRandom] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyNavRandom::CEnemyNavRandom() : CEnemyNav(),
m_pRangeCube(nullptr)	// 範囲
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyNavRandom::~CEnemyNavRandom()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyNavRandom::Init(void)
{
	if (FAILED(CEnemyNav::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyNavRandom::Uninit(void)
{
	if (m_pRangeCube != nullptr)
	{ // 範囲が NULL じゃない場合

		// 範囲の終了処理
		m_pRangeCube->Uninit();
		m_pRangeCube = nullptr;
	}

	// ナビの終了処理
	CEnemyNav::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyNavRandom::Update
(
	D3DXVECTOR3* pPos,		// 位置
	D3DXVECTOR3* pRot,		// 向き
	D3DXVECTOR3* pMove,		// 移動量
	D3DXVECTOR3* pRotDest,	// 目的の向き
	const float fSpeed,		// 速度
	const float fDeltaTime	// デルタタイム
)
{
	// ナビの更新処理
	CEnemyNav::Update
	(
		pPos,		// 位置
		pRot,		// 向き
		pMove,		// 移動量
		pRotDest,	// 目的の向き
		fSpeed,		// 速度
		fDeltaTime	// デルタタイム
	);
}

//============================================================
// 生成処理
//============================================================
CEnemyNavRandom* CEnemyNavRandom::Create(const D3DXVECTOR3& rPosInit, const float fWidth, const float fDepth)
{
	// ナビゲーションの生成
	CEnemyNavRandom* pNav = new CEnemyNavRandom;

	if (pNav == nullptr)
	{ // 生成に失敗した場合

		// NULL を返す
		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// アクターの初期化
		if (FAILED(pNav->Init()))
		{ // 初期化に失敗した場合

			// アクターの破棄
			SAFE_DELETE(pNav);
			return nullptr;
		}

		// 初期位置を設定
		pNav->SetPosInit(rPosInit);

		// 移動範囲を設定
		pNav->SetMoveRange(D3DXVECTOR3(fWidth, 0.0f, fDepth));

		// 円柱を生成
		pNav->m_pRangeCube = CObjectMeshCube::Create
		(
			rPosInit,
			VEC3_ZERO,
			D3DXVECTOR3(fWidth, 10.0f, fDepth),
			XCOL_WHITE
		);

		// 確保したアドレスを返す
		return pNav;
	}
}

//============================================================
// 停止状態処理
//============================================================
void CEnemyNavRandom::StopFunc
(
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rRot,		// 向き
	D3DXVECTOR3* pRotDest			// 目的の向き
)
{
	// 状態カウントを取得する
	int nStateCount = GetStateCount();

	// 状態カウントを加算する
	nStateCount++;

	if (nStateCount % STOP_COUNT == 0)
	{ // 状態カウントが一定数になった場合

		// 状態カウントを0にする
		nStateCount = 0;

		// ターン状態にする
		SetState(STATE_TURN);

		// 目的位置のランダム処理
		DestPosRandom();

		// 向きを設定する
		pRotDest->y = atan2f(rPos.x - GetPosDest().x, rPos.z - GetPosDest().z);

		// 向きの正規化
		useful::NormalizeRot(pRotDest->y);
	}

	// 状態カウントを適用する
	SetStateCount(nStateCount);
}

//============================================================
// ターン状態処理
//============================================================
void CEnemyNavRandom::TurnFunc
(
	D3DXVECTOR3* pRot,				// 向き
	D3DXVECTOR3* pMove,				// 移動量
	const D3DXVECTOR3& rRotDest,	// 目的の向き
	const float fSpeed,				// 速度
	const float fDeltaTime			// デルタタイム
)
{
	if (pRot->y <= rRotDest.y + ROT_CORRECT_DIFF &&
		pRot->y >= rRotDest.y - ROT_CORRECT_DIFF)
	{ // 向きが目的に近づいた場合

		// 向きを補正する
		pRot->y = rRotDest.y;

		// 移動状態にする
		SetState(STATE_MOVE);

		// 移動量を設定する
		pMove->x = sinf(pRot->y) * fSpeed * fDeltaTime;
		pMove->z = cosf(pRot->y) * fSpeed * fDeltaTime;
	}

	CEffect3D* p = CEffect3D::Create(GetPosDest(), 40.0f, CEffect3D::TYPE_NORMAL, 10, D3DXVECTOR3(0.0f, 5.0f, 0.0f), VEC3_ZERO, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	p->SetPriority(6);
}

//============================================================
// 移動状態処理
//============================================================
void CEnemyNavRandom::MoveFunc
(
	D3DXVECTOR3* pPos, 
	const D3DXVECTOR3& rMove
)
{
	D3DXVECTOR3 posDest = GetPosDest();		// 目的の位置

	if (PosCorrect(posDest.x,&pPos->x,rMove.x) ||
		PosCorrect(posDest.z, &pPos->z, rMove.z) ||
		CollisionRange(pPos))
	{ // 歩き終えるか、範囲を超えた場合

		// 状態カウントを0にする
		SetStateCount(0);

		// 停止状態にする
		SetState(STATE_STOP);
	}
}

//============================================================
// 位置のランダム処理
//============================================================
void CEnemyNavRandom::DestPosRandom(void)
{
	D3DXVECTOR3 posDest = VEC3_ZERO;	// 目的の位置

	// 目的の位置を設定する
	posDest.x = GetPosInit().x + rand() % ((int)GetMoveRange().x + 1) - ((int)GetMoveRange().x * 0.5f);
	posDest.z = GetPosInit().z + rand() % ((int)GetMoveRange().z + 1) - ((int)GetMoveRange().z * 0.5f);

	// 目的位置を適用する
	SetPosDest(posDest);
}

//============================================================
// 位置の補正処理
//============================================================
bool CEnemyNavRandom::PosCorrect(const float fDest, float* fTarget, const float fMove)
{
	if (*fTarget > fDest)
	{ // サイズが目標超過だった場合

		// サイズを減算する
		*fTarget += fMove;

		if (*fTarget <= fDest)
		{ // 目標のサイズ以下になった場合

			// サイズを補正する
			*fTarget = fDest;

			// true を返す
			return true;
		}
	}

	if (*fTarget < fDest)
	{ // サイズが目標未満だった場合

		// サイズを加算する
		*fTarget += fMove;

		if (*fTarget >= fDest)
		{ // 目標のサイズ以上になった場合

			// サイズを補正する
			*fTarget = fDest;

			// true を返す
			return true;
		}
	}

	// false を返す
	return false;
}
