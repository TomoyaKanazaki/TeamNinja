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
#include "enemy.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int STOP_COUNT = 100;				// 停止カウント
	const float ROT_CORRECT_DIFF = 0.01f;	// 向きを補正する差分
	const float MIN_DISTANCE = 250.0f;		// 最低限の距離
}

//************************************************************
//	子クラス [CEnemyNavRandom] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyNavRandom::CEnemyNavRandom() : CEnemyNav(),
#ifdef _DEBUG

m_pRangeCube(nullptr),	// 範囲

#endif
m_MoveRange(VEC3_ZERO)	// 移動範囲
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
#ifdef _DEBUG

	// 範囲の終了処理
	SAFE_UNINIT(m_pRangeCube);

#endif

	// ナビの終了処理
	CEnemyNav::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyNavRandom::Update
(
	D3DXVECTOR3* pPos,			// 位置
	D3DXVECTOR3* pRot,			// 向き
	CEnemy* pEnemy,				// 敵の情報
	const float fSpeed,			// 速度
	const float fDeltaTime		// デルタタイム
)
{
	// ナビの更新処理
	CEnemyNav::Update
	(
		pPos,		// 位置
		pRot,		// 向き
		pEnemy,		// 敵の情報
		fSpeed,		// 速度
		fDeltaTime	// デルタタイム
	);
}

//============================================================
// 生成処理
//============================================================
CEnemyNavRandom* CEnemyNavRandom::Create(const D3DXVECTOR3& pos, const float fWidth, const float fDepth)
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

		// 移動範囲を設定
		pNav->m_MoveRange = D3DXVECTOR3(fWidth, 0.0f, fDepth);

		// 種類を設定
		pNav->SetType(TYPE_RANDOM);

#if 0
#ifdef _DEBUG

		// キューブを生成
		pNav->m_pRangeCube = CObjectMeshCube::Create
		(
			pos,
			VEC3_ZERO,
			D3DXVECTOR3(fWidth, 10.0f, fDepth),
			XCOL_WHITE
		);

#endif // _DEBUG
#endif // 0


		// 確保したアドレスを返す
		return pNav;
	}
}

//============================================================
// 停止状態処理
//============================================================
void CEnemyNavRandom::StopFunc
(
	const D3DXVECTOR3& rPos,	// 位置
	CEnemy* pEnemy				// 敵の情報
)
{
	int nStateCount = GetStateCount();					// 状態カウントを取得する
	D3DXVECTOR3 posDest = GetPosDest();					// 目的の位置を取得する
	D3DXVECTOR3 posInit = pEnemy->GetPosInit();			// 初期位置を取得する
	D3DXVECTOR3 rotDest = pEnemy->GetDestRotation();	// 目的の向きを取得する

	// 状態カウントを加算する
	nStateCount++;

	if (nStateCount % STOP_COUNT == 0)
	{ // 状態カウントが一定数になった場合

		// 状態カウントを0にする
		nStateCount = 0;

		// ターン状態にする
		SetState(STATE_TURN);

		// 目的の位置を設定する
		posDest.x = posInit.x + rand() % ((int)m_MoveRange.x + 1) - ((int)m_MoveRange.x * 0.5f);
		posDest.z = posInit.z + rand() % ((int)m_MoveRange.z + 1) - ((int)m_MoveRange.z * 0.5f);

		// 向きを設定する
		rotDest.y = atan2f(rPos.x - posDest.x, rPos.z - posDest.z);

		if (sqrtf((rPos.x - posDest.x) * (rPos.x - posDest.x) + (rPos.z - posDest.z) * (rPos.z - posDest.z)) <= MIN_DISTANCE)
		{ // 最低限の距離以下の場合

			// 目的の位置を少し先に延ばす
			posDest.x -= sinf(rotDest.y) * (m_MoveRange.x * 0.5f);
			posDest.z -= cosf(rotDest.y) * (m_MoveRange.z * 0.5f);
		}

		// 向きの正規化
		useful::NormalizeRot(rotDest.y);
	}

	SetStateCount(nStateCount);			// 状態カウントを適用
	pEnemy->SetDestRotation(rotDest);	// 目的の向きを適用
	SetPosDest(posDest);				// 目的の位置を適用
}

//============================================================
// ターン状態処理
//============================================================
void CEnemyNavRandom::TurnFunc
(
	D3DXVECTOR3* pRot,			// 向き
	CEnemy* pEnemy,				// 敵の情報
	const float fSpeed,			// 速度
	const float fDeltaTime		// デルタタイム
)
{
	D3DXVECTOR3 rotDest = pEnemy->GetDestRotation();	// 目的の向きを取得
	D3DXVECTOR3 move = pEnemy->GetMovePosition();		// 移動量を取得

	if (pRot->y <= rotDest.y + ROT_CORRECT_DIFF &&
		pRot->y >= rotDest.y - ROT_CORRECT_DIFF)
	{ // 向きが目的に近づいた場合

		// 向きを補正する
		pRot->y = rotDest.y;

		// 移動状態にする
		SetState(STATE_MOVE);

		// 移動量を設定する
		move.x = sinf(pRot->y) * fSpeed * fDeltaTime;
		move.z = cosf(pRot->y) * fSpeed * fDeltaTime;
	}

	pEnemy->SetMovePosition(move);		// 移動量を適用
}

//============================================================
// 移動状態処理
//============================================================
void CEnemyNavRandom::MoveFunc
(
	D3DXVECTOR3* pPos,			// 位置
	CEnemy* pEnemy				// 敵の情報
)
{
	D3DXVECTOR3 posDest = GetPosDest();				// 目的の位置を取得
	D3DXVECTOR3 move = pEnemy->GetMovePosition();	// 移動量を取得
	D3DXVECTOR3 posInit = pEnemy->GetPosInit();		// 初期位置を取得

	if (PosCorrect(posDest.x, &pPos->x, move.x) ||
		PosCorrect(posDest.z, &pPos->z, move.z) ||
		CollisionRange(pPos, posInit))
	{ // 歩き終えるか、範囲を超えた場合

		// 状態カウントを0にする
		SetStateCount(0);

		// 停止状態にする
		SetState(STATE_STOP);
	}
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

//============================================================
// 範囲との衝突
//============================================================
bool CEnemyNavRandom::CollisionRange(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosInit)
{
	// 範囲を超えたかどうか
	bool bOver = false;

	if (pPos->x >= rPosInit.x + m_MoveRange.x)
	{ // 右端を超えた場合

		// 位置を補正する
		pPos->x = rPosInit.x + m_MoveRange.x;

		// 範囲超えた
		bOver = true;
	}

	if (pPos->x <= rPosInit.x - m_MoveRange.x)
	{ // 左端を超えた場合

		// 位置を補正する
		pPos->x = rPosInit.x - m_MoveRange.x;

		// 範囲超えた
		bOver = true;
	}

	if (pPos->z >= rPosInit.z + m_MoveRange.z)
	{ // 奥端を超えた場合

		// 位置を補正する
		pPos->z = rPosInit.z + m_MoveRange.z;

		// 範囲超えた
		bOver = true;
	}

	if (pPos->z <= rPosInit.z - m_MoveRange.z)
	{ // 手前端を超えた場合

		// 位置を補正する
		pPos->z = rPosInit.z - m_MoveRange.z;

		// 範囲超えた
		bOver = true;
	}

	// 範囲状況を返す
	return bOver;
}