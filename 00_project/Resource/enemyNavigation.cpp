//============================================================
//
//	敵のナビゲーション処理 [enemyNavigation.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "enemyNavigation.h"
#include "deltaTime.h"

#include "actor.h"
#include "stage.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float MOVE_DISTANCE = -700.0f;	// 移動する範囲
	const int STOP_COUNT = 300;				// 停止カウント
}

//************************************************************
//	子クラス [CEnemyNav] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyNav::CEnemyNav() :
	m_posInit(VEC3_ZERO),	// 初期位置
	m_posDest(VEC3_ZERO),	// 目標地点
	m_state(STATE_STOP),	// 状態
	m_nStopCount(0),		// 停止カウント
	m_fRotMove(0.0f)		// 向きの移動量
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyNav::~CEnemyNav()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyNav::Init(void)
{
	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyNav::Uninit(void)
{
	// 自身を破棄
	delete this;
}

//============================================================
//	更新処理
//============================================================
void CEnemyNav::Update(D3DXVECTOR3* pPos, const D3DXVECTOR3& pPosOld, D3DXVECTOR3* pRot, D3DXVECTOR3* pRotDest, D3DXVECTOR3* pMove)
{
	switch (m_state)
	{
	case CEnemyNav::STATE_STOP:

		// 停止状態処理
		Stop(pPos, pRotDest);

		break;

	case CEnemyNav::STATE_TURN:

		// ターン状態処理
		Turn(pRot, pRotDest, pMove);

		break;

	case CEnemyNav::STATE_WALK:

		// 歩行状態処理
		Walk(pPos, pPosOld, pMove);

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//============================================================
// 生成処理
//============================================================
CEnemyNav* CEnemyNav::Create(const D3DXVECTOR3& rPosInit)
{
	// ナビゲーションの生成
	CEnemyNav* pNav = new CEnemyNav;

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
		pNav->m_posInit = rPosInit;

		// 確保したアドレスを返す
		return pNav;
	}
}

//============================================================
// 停止状態処理
//============================================================
void CEnemyNav::Stop(D3DXVECTOR3* pPos, D3DXVECTOR3* pRotDest)
{
	// 停止させる
	m_nStopCount++;

	if (m_nStopCount % STOP_COUNT == 0)
	{ // 停止カウントが一定数の場合

		// 向きを設定する
		pRotDest->y = useful::RandomRot();

		// 目的位置を設定
		m_posDest.x = pPos->x + sinf(pRotDest->y) * MOVE_DISTANCE;
		m_posDest.z = pPos->z + cosf(pRotDest->y) * MOVE_DISTANCE;

		// アクターの当たり判定処理
		CollisionActor(pPos);

		// 壁の当たり判定処理
		CollisionWall(pPos);

		// 状態をクリアする
		m_state = STATE_TURN;
	}
}

//============================================================
// ターン状態処理
//============================================================
void CEnemyNav::Turn(D3DXVECTOR3* pRot, D3DXVECTOR3* pRotDest, D3DXVECTOR3* pMove)
{
	if (pRot->y <= pRotDest->y + 0.01f &&
		pRot->y >= pRotDest->y - 0.01f)
	{ // 向きが目的に近づいた場合

		// 向きを補正する
		pRot->y = pRotDest->y;

		// 歩行状態にする
		m_state = STATE_WALK;

		// 移動量を設定する
		pMove->x = sinf(pRot->y) * -290.0f * GET_MANAGER->GetDeltaTime()->GetTime();
		pMove->z = cosf(pRot->y) * -290.0f * GET_MANAGER->GetDeltaTime()->GetTime();
	}
}

//============================================================
// 歩行状態処理
//============================================================
void CEnemyNav::Walk(D3DXVECTOR3* pPos, const D3DXVECTOR3& pPosOld, D3DXVECTOR3* pMove)
{
	// 移動する
	pPos->x += pMove->x;
	pPos->z += pMove->z;

	if (pPosOld.x > m_posDest.x &&
		pPos->x <= m_posDest.x)
	{ // 位置を超過した場合

		// サイズを設定する
		*pPos = m_posDest;

		// 停止状態にする
		m_state = STATE_STOP;
	}

	if (pPosOld.x < m_posDest.x &&
		pPos->x >= m_posDest.x)
	{ // 位置を超過した場合

		// サイズを設定する
		*pPos = m_posDest;

		// 停止状態にする
		m_state = STATE_STOP;
	}
}

//============================================================
// アクターの当たり判定処理
//============================================================
void CEnemyNav::CollisionActor(D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 move = VEC3_ZERO;
	bool bJump = false;

	// アクターのリスト構造が無ければ抜ける
	if (CActor::GetList() == nullptr) { return; }

	// リストを取得
	std::list<CActor*> list = CActor::GetList()->GetList();

	for (auto actor : list)
	{
		// 当たり判定処理
		actor->Collision
		(
			m_posDest,	// 位置
			*pPos,		// 前回の位置
			50.0f,		// 半径
			50.0f,		// 高さ
			move,		// 移動量
			bJump		// ジャンプ状況
		);
	}
}

//============================================================
// 壁の当たり判定処理
//============================================================
void CEnemyNav::CollisionWall(D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 move = VEC3_ZERO;

	// 壁の当たり判定
	CScene::GetStage()->CollisionWall(m_posDest, *pPos, 50.0f, 50.0f, move);
}
