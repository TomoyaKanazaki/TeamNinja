//============================================================
//
//	犬敵処理 [enemyDog.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "enemyWolf.h"
#include "renderer.h"
#include "deltaTime.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\wolf.txt";	// セットアップテキスト相対パス
	const int	BLEND_FRAME_OTHER	= 5;	// モーションの基本的なブレンドフレーム
	const int	BLEND_FRAME_TURN	= 20;	// モーション動揺のブレンドフレーム
	const int	BLEND_FRAME_LAND	= 15;	// モーション着地のブレンドフレーム

	const float MOVE = 500.0f;	// 移動量
	const float	REV_ROTA		= 4.5f;		// 向き変更の補正係数
	const float	REV_ROTA_LOOK	= 9.0f;		// ガウガウしてる時の向き変更の補正係数
	const float ATTACK_DISTANCE	= 50.0f;	// 攻撃判定に入る距離

	const float	JUMP_REV = 0.16f;	// 通常状態時の空中の移動量の減衰係数
	const float	LAND_REV = 0.16f;	// 通常状態時の地上の移動量の減衰係数

}

//************************************************************
//	子クラス [CEnemyWolf] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyWolf::CEnemyWolf() : CEnemy(),
m_posTarget(VEC3_ZERO),	// 目標の位置
m_target(TARGET_NONE),	// 標的
m_state(STATE_CRAWL)	// 状態
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyWolf::~CEnemyWolf()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyWolf::Init(void)
{
	// 敵の初期化
	if (FAILED(CEnemy::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キャラクター情報の割当
	BindCharaData(SETUP_TXT);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyWolf::Uninit(void)
{
	// 敵の終了
	CEnemy::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyWolf::Update(const float fDeltaTime)
{
	// 敵の更新
	CEnemy::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CEnemyWolf::Draw(CShader* pShader)
{
	// 敵の描画
	CEnemy::Draw(pShader);
}

//============================================================
//	情報の設定処理
//============================================================
void CEnemyWolf::SetData(void)
{

}

//============================================================
//	状態の更新処理
//============================================================
int CEnemyWolf::UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// 現在のモーション
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_CRAWL:	// 巡回状態

		// 巡回状態時の更新
		nCurMotion = UpdateCrawl(pPos, pRot, fDeltaTime);
		break;

	case STATE_CAVEAT:	// 警告状態

		// 警告状態時の更新
		nCurMotion = UpdateCaveat(pPos, pRot, fDeltaTime);
		break;

	case STATE_FOUND:	// 追跡状態

		// 追跡状態時の更新
		nCurMotion = UpdateFound(pPos, pRot, fDeltaTime);
		break;

	case STATE_ATTACK:	// 攻撃状態

		// 攻撃状態時の更新
		nCurMotion = UpdateAttack(pPos, pRot, fDeltaTime);
		break;

	case STATE_UPSET:	// 動揺状態

		// 動揺状態時の更新
		nCurMotion = UpdateUpset(pPos, pRot, fDeltaTime);
		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 現在のモーションを返す
	return nCurMotion;
}

//============================================================
//	モーション・キャラクターの更新処理
//============================================================
void CEnemyWolf::UpdateMotion(int nMotion, const float fDeltaTime)
{
	// 死んでたら抜ける
	if (IsDeath()) { return; }

	int nAnimMotion = GetMotionType();	// 現在再生中のモーション
	if (nMotion != NONE_IDX)
	{ // モーションが設定されている場合

		if (IsMotionLoop())
		{ // ループするモーション中の場合

			if (nAnimMotion != nMotion)
			{ // 現在のモーションが再生中のモーションと一致しない場合

				// 現在のモーションの設定
				SetMotion(nMotion, BLEND_FRAME_OTHER);
			}
		}
	}

	// オブジェクトキャラクターの更新
	CObjectChara::Update(fDeltaTime);

	switch (GetMotionType())
	{ // モーションの種類ごとの処理
	case MOTION_IDOL:	// 待機モーション
		break;

	case MOTION_RUN:	// 警戒モーション

#if 0
		if (GetMotionPose() % 4 == 0 && GetMotionCounter() == 0)
		{ // 足がついたタイミングの場合

			switch (m_land)
			{ // 着地物ごとの処理
			case LAND_OBSTACLE:

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_WALK_OBS);	// 歩行音（障害物）

				break;

			default:

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_WALK_BUILD);	// 歩行音（ビル）

				break;
			}
		}
#endif
		break;

	case MOTION_FOUND:	// 発見モーション

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_BITE:	// 噛みつきモーション

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_TURN:	// 振り向きモーション

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(MOTION_IDOL, BLEND_FRAME_TURN);

			// 巡回状態にする
			m_state = STATE_CRAWL;
		}

		break;

	case MOTION_FALL:	// 落下モーション

		if (!IsJump())
		{ // ジャンプ中ではない場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_LANDING:	// 着地モーション

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_LAND);
		}

		break;
	}
}

//============================================================
//	着地の更新処理
//============================================================
void CEnemyWolf::UpdateLanding(D3DXVECTOR3* pPos)
{
	// 親クラスの着地更新
	CEnemy::UpdateLanding(pPos);

	if (!IsJump())
	{ // 空中にいない場合

		//  TODO：なぜかここで待機モーションになってる→ループモーションだからだ
		if (GetMotionType() == MOTION_FALL)
		{ // モーションが落下中の場合

			// 着地モーションを指定
			SetMotion(MOTION_LANDING);
		}
	}
	else
	{ // 空中にいる場合

		// 落下モーションを指定
		SetMotion(MOTION_FALL);
	}
}

//============================================================
//	巡回状態時の更新処理
//============================================================
int CEnemyWolf::UpdateCrawl(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// 重力の更新
	UpdateGravity();

	// 位置更新
	UpdatePosition(*pPos, fDeltaTime);

	// 着地判定
	UpdateLanding(pPos);

	// 向き更新
	UpdateRotation(*pRot, fDeltaTime);

	if (SearchClone(&m_posTarget, nullptr))
	{ // 分身が目に入った場合

		// 警告状態にする
		m_state = STATE_CAVEAT;

		// 標的を分身にする
		m_target = TARGET_CLONE;

		// 発見モーションを返す
		return MOTION_FOUND;
	}

	if (SearchPlayer(&m_posTarget))
	{ // プレイヤーが目に入った場合

		// 警告状態にする
		m_state = STATE_CAVEAT;

		// 標的をプレイヤーにする
		m_target = TARGET_PLAYER;

		// 発見モーションを返す
		return MOTION_FOUND;
	}

	// 巡回状態にする
	m_state = STATE_CRAWL;

	// 標的を未設定にする
	m_target = TARGET_NONE;

	// 待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
//	警告状態時の更新処理
//============================================================
int CEnemyWolf::UpdateCaveat(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// 重力の更新
	UpdateGravity();

	// 位置更新
	UpdatePosition(*pPos, fDeltaTime);

	// 着地判定
	UpdateLanding(pPos);

	// 目標位置の更新
	if		(SearchClone(&m_posTarget, nullptr)) {}	// 分身を見つける
	else if	(SearchPlayer(&m_posTarget)) {}			// プレイヤーを見つける
	else
	{ // 誰も見つけていない場合

		// 動揺状態にする
		m_state = STATE_UPSET;

		// 標的を未設定にする
		m_target = TARGET_NONE;

		// 動揺モーションにする
		return MOTION_TURN;
	}

	// 目標位置の視認
	LookTarget(*pPos);

	// 向き更新
	UpdateRotation(*pRot, REV_ROTA_LOOK, fDeltaTime);

	if (GetMotionType() != MOTION_FOUND)
	{ // 発見モーションじゃなかった場合

		// 追跡状態にする
		m_state = STATE_FOUND;
	}

	// 走行状態を返す
	return MOTION_RUN;
}

//============================================================
//	追跡状態時の更新処理
//============================================================
int CEnemyWolf::UpdateFound(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	if (SearchClone(&m_posTarget, nullptr))
	{ // 分身が目に入った場合

		// 標的を分身にする
		m_target = TARGET_CLONE;
	}
	else if (SearchPlayer(&m_posTarget))
	{ // プレイヤーが目に入った場合

		// 標的をプレイヤーにする
		m_target = TARGET_PLAYER;
	}
	else
	{ // 誰も見つけていない場合

		// 動揺状態にする
		m_state = STATE_UPSET;

		// 標的を未設定にする
		m_target = TARGET_NONE;

		// 動揺モーションにする
		return MOTION_TURN;
	}

	// デバッグ
	DebugProc::Print(DebugProc::POINT_RIGHT, "発見!!目的地：%f %f %f", m_posTarget.x, m_posTarget.y, m_posTarget.z);

	// 移動処理
	UpdateMove(*pPos, *pRot, fDeltaTime);

	if (Approach(*pPos))
	{ // 接近した場合

		// 攻撃状態にする
		m_state = STATE_ATTACK;

		// 噛みつきモーションを返す
		return MOTION_BITE;
	}

	// 重力の更新
	UpdateGravity();

	// 位置更新
	UpdatePosition(*pPos, fDeltaTime);

	// 着地判定
	UpdateLanding(pPos);

	// 向き更新
	UpdateRotation(*pRot, fDeltaTime);

	// 走行モーションを返す
	return MOTION_RUN;
}

//============================================================
//	攻撃状態時の更新処理
//============================================================
int CEnemyWolf::UpdateAttack(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// 重力の更新
	UpdateGravity();

	// 位置更新
	UpdatePosition(*pPos, fDeltaTime);

	// 着地判定
	UpdateLanding(pPos);

	// 向き更新
	UpdateRotation(*pRot, fDeltaTime);

	switch (m_target)
	{ // ターゲットごとの処理
	case CEnemyWolf::TARGET_PLAYER:

		// プレイヤーの当たり判定処理
		//HitPlayer(*pPos);

		if (GetMotionType() != MOTION_BITE)
		{ // 攻撃モーションじゃない場合

			// 巡回状態にする
			m_state = STATE_CRAWL;
		}

		// 待機モーションにする
		return MOTION_IDOL;

	case CEnemyWolf::TARGET_CLONE:

		// 分身の当たり判定処理
		//HitClone(*pPos);

		// 動揺状態にする
		m_state = STATE_UPSET;

		// 動揺モーションにする
		return MOTION_TURN;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
//	動揺状態時の更新処理
//============================================================
int CEnemyWolf::UpdateUpset(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// 重力の更新
	UpdateGravity();

	// 位置更新
	UpdatePosition(*pPos, fDeltaTime);

	// 着地判定
	UpdateLanding(pPos);

	// 向き更新
	UpdateRotation(*pRot, fDeltaTime);

	// 動揺モーションにする
	return MOTION_TURN;
}

//============================================================
//	移動量・目標向きの更新処理
//============================================================
void CEnemyWolf::UpdateMove(D3DXVECTOR3& rPos, D3DXVECTOR3& rRot, const float fDeltaTime)
{
	D3DXVECTOR3 move = GetMovePosition();		// 移動量

	// 目標向きを目標位置方向にする
	LookTarget(rPos);

	// 移動量を設定する
	move.x = sinf(rRot.y - D3DX_PI) * MOVE * fDeltaTime;
	move.z = cosf(rRot.y - D3DX_PI) * MOVE * fDeltaTime;

	// 位置を移動する
	rPos += move;

	SetMovePosition(move);		// 移動量を反映
}

//============================================================
//	位置の更新処理
//============================================================
void CEnemyWolf::UpdatePosition(D3DXVECTOR3& rPos, const float fDeltaTime)
{
	D3DXVECTOR3 move = GetMovePosition();	// 移動量

	// 移動量を加算
	rPos += move * fDeltaTime;

	// 移動量を減衰
	if (IsJump())
	{ // 空中の場合

		move.x += (0.0f - move.x) * JUMP_REV;
		move.z += (0.0f - move.z) * JUMP_REV;
	}
	else
	{ // 地上の場合

		move.x += (0.0f - move.x) * LAND_REV;
		move.z += (0.0f - move.z) * LAND_REV;
	}

	SetMovePosition(move);	// 移動量を反映
}

//============================================================
//	向きの更新処理
//============================================================
void CEnemyWolf::UpdateRotation(D3DXVECTOR3& rRot, const float fDeltaTime)
{
	// 向きの更新
	UpdateRotation(rRot, REV_ROTA, fDeltaTime);
}

//============================================================
//	向きの更新処理 (補正量設定)
//============================================================
void CEnemyWolf::UpdateRotation(D3DXVECTOR3& rRot, const float fRevRota, const float fDeltaTime)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// 目標向き
	float fDiffRot = 0.0f;	// 差分向き

	// 目標向きの正規化
	useful::NormalizeRot(destRot.y);

	// 目標向きまでの差分を計算
	fDiffRot = destRot.y - rRot.y;
	useful::NormalizeRot(fDiffRot);	// 差分向きの正規化

	// 向きの更新
	rRot.y += fDiffRot * fDeltaTime * fRevRota;
	useful::NormalizeRot(rRot.y);	// 向きの正規化

	SetDestRotation(destRot);	// 目標向きを反映
}

//============================================================
//	接近の判定処理
//============================================================
bool CEnemyWolf::Approach(const D3DXVECTOR3& rPos)
{
	// 目標位置と自身位置の距離を求める
	float fDis = sqrtf((rPos.x - m_posTarget.x) * (rPos.x - m_posTarget.x) + (rPos.z - m_posTarget.z) * (rPos.z - m_posTarget.z));
	if (fDis <= ATTACK_DISTANCE)
	{ // 一定距離の場合

		// 接近を返す
		return true;
	}

	// 非接近を返す
	return false;
}

//============================================================
//	目標位置の視認処理
//============================================================
void CEnemyWolf::LookTarget(const D3DXVECTOR3& rPos)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// 目標向き

	// 目標向きを求める
	destRot.y = atan2f(rPos.x - m_posTarget.x, rPos.z - m_posTarget.z);

	SetDestRotation(destRot);	// 目標向きを反映
}
