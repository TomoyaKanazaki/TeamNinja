//============================================================
//
//	狼敵処理 [enemyWolf.cpp]
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

#include "enemyNavigation.h"
#include "enemyChaseRange.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\wolf.txt";	// セットアップテキスト相対パス
	const int	BLEND_FRAME_OTHER	= 5;	// モーションの基本的なブレンドフレーム
	const int	BLEND_FRAME_TURN	= 20;	// モーション動揺のブレンドフレーム
	const int	BLEND_FRAME_LAND	= 15;	// モーション着地のブレンドフレーム
	const float	RADIUS = 70.0f;				// 半径
	const float HEIGHT = 20.0f;				// 身長

	const float SPEED = -500.0f;			// 移動量
	const float	REV_ROTA		= 4.5f;		// 向き変更の補正係数
	const float	REV_ROTA_LOOK	= 9.0f;		// ガウガウしてる時の向き変更の補正係数
	const float ATTACK_DISTANCE	= 50.0f;	// 攻撃判定に入る距離
	const float FADE_ALPHA_TRANS = 0.02f;	// フェード時の透明度の遷移定数

	const float	JUMP_REV = 0.16f;	// 通常状態時の空中の移動量の減衰係数
	const float	LAND_REV = 0.16f;	// 通常状態時の地上の移動量の減衰係数
}

//************************************************************
//	子クラス [CEnemyWolf] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyWolf::CEnemyWolf() : CEnemyAttack(),
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
	if (FAILED(CEnemyAttack::Init()))
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
	CEnemyAttack::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyWolf::Update(const float fDeltaTime)
{
	// 敵の更新
	CEnemyAttack::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CEnemyWolf::Draw(CShader* pShader)
{
	// 敵の描画
	CEnemyAttack::Draw(pShader);
}

//============================================================
//	情報の設定処理
//============================================================
void CEnemyWolf::SetData(void)
{

}

//============================================================
// 半径の取得処理
//============================================================
float CEnemyWolf::GetRadius(void) const
{
	// 半径を返す
	return RADIUS;
}

//============================================================
// 高さの取得処理
//============================================================
float CEnemyWolf::GetHeight(void) const
{
	// 身長を返す
	return HEIGHT;
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

	case STATE_FADEOUT:	// フェードアウト状態

		// フェードアウト時の更新
		nCurMotion = UpdateFadeOut(pPos, pRot, fDeltaTime);

		break;

	case STATE_FADEIN:	// フェードイン状態

		// フェードイン時の更新
		nCurMotion = UpdateFadeIn(pPos, pRot, fDeltaTime);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 当たり判定処理
	Collision(*pPos);

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
	CEnemyAttack::UpdateLanding(pPos);

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
// ナビゲーションによるモーションの設定処理
//============================================================
void CEnemyWolf::NavMoitonSet(int* pMotion)
{
	switch (GetNavigation()->GetState())
	{
	case CEnemyNav::STATE_MOVE:

		// 移動モーションを設定
		*pMotion = MOTION_RUN;

		break;

	default:

		break;
	}
}

//============================================================
//	巡回状態時の更新処理
//============================================================
int CEnemyWolf::UpdateCrawl(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int motion = MOTION_IDOL;	// モーション

	// 重力の更新
	UpdateGravity();

	// 敵を落下させる
	pPos->y += GetMovePosition().y * fDeltaTime;

	// 向き更新
	UpdateRotation(*pRot, fDeltaTime);

	if (GetNavigation() != nullptr)
	{ // ナビゲーションが NULL じゃない場合

		// ナビの更新処理
		GetNavigation()->Update
		(
			pPos,		// 位置
			pRot,		// 向き
			this,		// 敵の情報
			SPEED,		// 速度
			fDeltaTime	// デルタタイム
		);

		switch (GetNavigation()->GetState())
		{
		case CEnemyNav::STATE_MOVE:

			// 移動モーションを設定
			motion = MOTION_RUN;

			break;

		default:

			break;
		}

		// ナビゲーションによるモーションの設定処理
		NavMoitonSet(&motion);
	}

	// 着地判定
	UpdateLanding(pPos);

	if (JudgeClone() || 
		JudgePlayer())
	{ // 分身かプレイヤーが目に入った場合

		// ナビゲーションリセット処理
		GetNavigation()->NavReset();

		// 警告状態にする
		m_state = STATE_CAVEAT;

		// 発見モーションを返す
		return MOTION_FOUND;
	}

	// 巡回状態にする
	m_state = STATE_CRAWL;

	// 標的を未設定にする
	SetTarget(TARGET_NONE);

	// 待機モーションを返す
	return motion;
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
	if (!JudgeClone() &&
		!JudgePlayer()) 
	{ // 分身もプレイヤーも見つけられなかった場合

		// 動揺状態にする
		m_state = STATE_UPSET;

		// 標的を未設定にする
		SetTarget(TARGET_NONE);

		// 動揺モーションにする
		return MOTION_TURN;
	}

	// 目標位置の視認
	LookTarget(*pPos);

	// 向き更新
	UpdateRotation(*pRot, fDeltaTime);

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
	if (!ShakeOffClone() &&
		!ShakeOffPlayer())
	{ // 分身が目に入っていない場合

		// 動揺状態にする
		m_state = STATE_UPSET;

		// 標的を未設定にする
		SetTarget(TARGET_NONE);

		// 動揺モーションにする
		return MOTION_TURN;
	}

	// 目標向きを目標位置方向にする
	LookTarget(*pPos);

	// 攻撃判定を false にする
	SetEnableAttack(false);

	// 移動処理
	Move(pPos, *pRot, SPEED, fDeltaTime);

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

	if (GetChaseRange() != nullptr &&
		GetChaseRange()->ChaseRange(pPos))
	{ // 追跡範囲から出た場合

		// フェードアウト状態にする
		m_state = STATE_FADEOUT;

		// ナビゲーションリセット処理
		GetNavigation()->NavReset();

		// 移動量をリセットする
		SetMovePosition(VEC3_ZERO);

		// ターゲットを無対象にする
		SetTarget(TARGET_NONE);
	}

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

	switch (GetTarget())
	{ // ターゲットごとの処理
	case CEnemyAttack::TARGET_PLAYER:

		// プレイヤーの当たり判定処理
		HitPlayer(*pPos);

		if (GetMotionType() != MOTION_BITE)
		{ // 攻撃モーションじゃない場合

			// 巡回状態にする
			m_state = STATE_CRAWL;
		}

		// 待機モーションにする
		return MOTION_IDOL;

	case CEnemyWolf::TARGET_CLONE:

		// 分身の当たり判定処理
		HitClone(*pPos);

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
// フェードアウト状態時の更新
//============================================================
int CEnemyWolf::UpdateFadeOut(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// 透明度を取得
	float fAlpha = GetAlpha();

	// 透明度を減算する
	fAlpha -= FADE_ALPHA_TRANS;

	if (fAlpha <= 0.0f)
	{ // 透明度が0以下になった場合

		// フェードイン状態にする
		m_state = STATE_FADEIN;

		// 位置を設定する
		*pPos = GetPosInit();

		// 過去の位置を適用する(こうしないと当たり判定に引っかかってしまう)
		SetOldPosition(*pPos);

		// 向きを設定する
		*pRot = VEC3_ZERO;

		// 透明度を補正する
		fAlpha = 0.0f;
	}

	// 透明度を適用
	CObjectChara::SetAlpha(fAlpha);
	CEnemyAttack::SetAlpha(fAlpha);

	// 待機モーションにする
	return MOTION_IDOL;
}

//============================================================
// フェードイン状態時の更新
//============================================================
int CEnemyWolf::UpdateFadeIn(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// 透明度を取得
	float fAlpha = GetAlpha();

	// 透明度を減算する
	fAlpha += FADE_ALPHA_TRANS;

	if (fAlpha >= 1.0f)
	{ // 透明度が一定数以上になった場合

		// 巡回状態にする
		m_state = STATE_CRAWL;

		// 透明度を補正する
		fAlpha = 1.0f;
	}

	// 透明度を適用
	CObjectChara::SetAlpha(fAlpha);
	CEnemyAttack::SetAlpha(fAlpha);

	// 待機モーションにする
	return MOTION_IDOL;
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
	RotMove(rRot, REV_ROTA, fDeltaTime);
}
