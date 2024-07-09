//============================================================
//
//	敵のナビゲーション(往来)処理 [enemyNavStreet.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "enemyNavStreet.h"

#include "enemy.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int STOP_COUNT = 100;				// 停止カウント
	const float ROT_CORRECT_DIFF = 0.01f;	// 向きを補正する差分
}

//************************************************************
//	子クラス [CEnemyNavStreet] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyNavStreet::CEnemyNavStreet() : CEnemyNav(),
m_nNumRoute(0)	// 現在向かうルートの番号
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyNavStreet::~CEnemyNavStreet()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyNavStreet::Init(void)
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
void CEnemyNavStreet::Uninit(void)
{
	// 経路を全消去する
	m_Route.clear();

	// ナビの終了処理
	CEnemyNav::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyNavStreet::Update
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
CEnemyNavStreet* CEnemyNavStreet::Create(const std::vector<D3DXVECTOR3>& rRoute)
{
	// ナビゲーションの生成
	CEnemyNavStreet* pNav = new CEnemyNavStreet;

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

		// 経路を設定
		pNav->m_Route = rRoute;

		// 確保したアドレスを返す
		return pNav;
	}
}

//============================================================
// 停止状態処理
//============================================================
void CEnemyNavStreet::StopFunc
(
	const D3DXVECTOR3& rPos,	// 位置
	CEnemy* pEnemy				// 敵の情報
)
{
	int nStateCount = GetStateCount();					// 状態カウントを取得する
	D3DXVECTOR3 posDest = GetPosDest();					// 目的の位置を取得する
	D3DXVECTOR3 rotDest = pEnemy->GetDestRotation();	// 目的の向きを取得する

	// 状態カウントを加算する
	nStateCount++;

	if (nStateCount % STOP_COUNT == 0)
	{ // 状態カウントが一定数になった場合

		// 状態カウントを0にする
		nStateCount = 0;

		// ターン状態にする
		SetState(STATE_TURN);

		// 目的の位置を設定
		posDest = m_Route.at(m_nNumRoute);

		// 向きを設定する
		rotDest.y = atan2f(rPos.x - posDest.x, rPos.z - posDest.z);

		// 向きの正規化
		useful::NormalizeRot(rotDest.y);
	}

	SetStateCount(nStateCount);			// 状態カウントを適用
	SetPosDest(posDest);				// 目的の位置を適用
	pEnemy->SetDestRotation(rotDest);	// 目的の向きを適用
}

//============================================================
// ターン状態処理
//============================================================
void CEnemyNavStreet::TurnFunc
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
void CEnemyNavStreet::MoveFunc
(
	D3DXVECTOR3* pPos,			// 位置
	CEnemy* pEnemy				// 敵の情報
)
{
	D3DXVECTOR3 posDest = GetPosDest();				// 目的の位置
	D3DXVECTOR3 move = pEnemy->GetMovePosition();	// 移動量を取得

	if (PosCorrect(posDest.x, &pPos->x, move.x) ||
		PosCorrect(posDest.z, &pPos->z, move.z))
	{ // 歩き終えるか、範囲を超えた場合

		int nNumAll = m_Route.size();	// 場所の総数

		// 向かう場所を次に進める
		m_nNumRoute = (m_nNumRoute + 1) % nNumAll;

		// 状態カウントを0にする
		SetStateCount(0);

		// 停止状態にする
		SetState(STATE_STOP);
	}
}

//============================================================
// 位置の補正処理
//============================================================
bool CEnemyNavStreet::PosCorrect(const float fDest, float* fTarget, const float fMove)
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