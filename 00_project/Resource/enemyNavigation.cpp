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

#include "actor.h"
#include "objectMeshCube.h"
#include "stage.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int STOP_COUNT = 300;				// 停止カウント
	const float ROT_CORRECT_DIFF = 0.01f;	// 向きを補正する差分
}

//************************************************************
//	子クラス [CEnemyNav] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyNav::CEnemyNav() :
	m_pRange(nullptr),		// 範囲
	m_posInit(VEC3_ZERO),	// 初期位置
	m_posDest(VEC3_ZERO),	// 目標位置
	m_state(STATE_STOP),	// 状態
	m_nStopCount(0)			// 停止カウント
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
	if (m_pRange != nullptr)
	{ // 範囲が NULL じゃない場合

		// 範囲の終了処理
		m_pRange->Uninit();
		m_pRange = nullptr;
	}

	// 自身を破棄
	delete this;
}

//============================================================
//	更新処理
//============================================================
void CEnemyNav::Update
(
	D3DXVECTOR3* pPos,		// 位置
	D3DXVECTOR3* pRot,		// 向き
	D3DXVECTOR3* pMove,		// 移動量
	D3DXVECTOR3* pRotDest,	// 目的の向き
	const float fRadius,	// 半径
	const float fHeight,	// 高さ
	const float fSpeed,		// 速度
	const float fDeltaTime	// デルタタイム
)
{
	switch (m_state)
	{
	case CEnemyNav::STATE_STOP:

		// 停止状態処理
		StopFunc(pRotDest);

		break;

	case CEnemyNav::STATE_TURN:

		// ターン状態処理
		TurnFunc(pRot, pMove, *pRotDest, fSpeed, fDeltaTime);

		break;

	case CEnemyNav::STATE_MOVE:

		// 移動状態処理
		MoveFunc(*pPos);

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	//D3DXVECTOR3 posDest = m_posDest;
	//D3DXVECTOR3 move = VEC3_ZERO;
	//bool bJump = false;
	//bool bHit = false;

	//// アクターのリスト構造が無ければ抜ける
	//if (CActor::GetList() == nullptr) { return; }

	//// リストを取得
	//std::list<CActor*> list = CActor::GetList()->GetList();

	//for (auto actor : list)
	//{
	//	// 当たり判定処理
	//	actor->Collision
	//	(
	//		posDest,	// 位置
	//		rPos,		// 前回の位置
	//		fRadius,	// 半径
	//		fHeight,	// 高さ
	//		move,		// 移動量
	//		bJump,		// ジャンプ状況
	//		bHit		// ヒット状況
	//	);
	//}
}

//============================================================
// 状態の設定処理
//============================================================
void CEnemyNav::SetState(const EState state)
{
	// 状態を設定する
	m_state = state;
}

//============================================================
// 生成処理
//============================================================
CEnemyNav* CEnemyNav::Create(const D3DXVECTOR3& rPosInit, const float fWidth, const float fDepth)
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

		// 円柱を生成
		pNav->m_pRange = CObjectMeshCube::Create
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
void CEnemyNav::StopFunc(D3DXVECTOR3* pRotDest)
{
	// 停止カウントを加算する
	m_nStopCount++;

	if (m_nStopCount % STOP_COUNT == 0)
	{ // 停止カウントが一定数になった場合

		// 停止カウントを0にする
		m_nStopCount = 0;

		// ターン状態にする
		m_state = STATE_TURN;

		// 向きを設定する
		pRotDest->y = useful::RandomRot();
	}
}

//============================================================
// ターン状態処理
//============================================================
void CEnemyNav::TurnFunc
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
		m_state = STATE_MOVE;

		// 移動量を設定する
		pMove->x = sinf(pRot->y) * fSpeed * fDeltaTime;
		pMove->z = cosf(pRot->y) * fSpeed * fDeltaTime;
	}
}

//============================================================
// 移動状態処理
//============================================================
void CEnemyNav::MoveFunc(const D3DXVECTOR3& rPos)
{

}
