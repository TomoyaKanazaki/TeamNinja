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
#include "sound.h"

#include "enemyNavigation.h"
#include "enemyNavStreet.h"
#include "enemyNavRandom.h"
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

	// 状態管理関係
	const int CAVEAT_STATE_COUNT = 36;		// 発見状態のカウント数
	const int ATTACK_STATE_COUNT = 34;		// 攻撃状態のカウント数
	const int BLANKBITE_STATE_COUNT = 340;	// 動揺状態のカウント数
	const int BLANKBITE_CYCLE_COUNT = 18;	// 動揺状態の回転カウント
	const int CAUTION_STATE_COUNT = 180;	// 警戒状態のカウント数

	// 音管理関係
	const int WALK_SOUND_COUNT = 32;			// 歩行音を鳴らすカウント数
	const int FOUND_SOUND_COUNT = 37;			// 発見音を鳴らすカウント数
	const int UPSET_SOUND_COUNT = 200;			// 動揺音を鳴らすカウント数
}

//************************************************************
//	子クラス [CEnemyWolf] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyWolf::CEnemyWolf() : CEnemyAttack(),
m_pNav(nullptr),		// ナビゲーションの情報
m_state(STATE_CRAWL),	// 状態
m_nStateCount(0)		// 状態カウント
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
	// ナビゲーションの終了処理
	SAFE_UNINIT(m_pNav);

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
//	生成処理(一定範囲移動敵)
//============================================================
CEnemyWolf* CEnemyWolf::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const float fMoveWidth,		// 移動幅
	const float fMoveDepth,		// 移動奥行
	const float fChaseWidth,	// 追跡幅
	const float fChaseDepth		// 追跡奥行
)
{
	// 敵を生成
	CEnemyWolf* pEnemy = new CEnemyWolf;

	if (pEnemy == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 敵の初期化
		if (FAILED(pEnemy->Init()))
		{ // 初期化に失敗した場合

			// 敵の破棄
			SAFE_DELETE(pEnemy);
			return nullptr;
		}

		// 位置を設定
		pEnemy->SetVec3Position(rPos);

		// 向きを設定
		pEnemy->SetVec3Rotation(rRot);

		// 種類を設定
		pEnemy->SetType(TYPE_WOLF);

		// 初期位置を設定
		pEnemy->SetPosInit(rPos);

		// 情報の設定処理
		pEnemy->SetData();

		// ナビゲーションを生成
		pEnemy->m_pNav = CEnemyNavRandom::Create(rPos, fMoveWidth, fMoveDepth);

		// 追跡範囲を生成
		pEnemy->SetChaseRange(CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth));

		// 確保したアドレスを返す
		return pEnemy;
	}
}

//============================================================
//	生成処理(ルート巡回移動敵)
//============================================================
CEnemyWolf* CEnemyWolf::Create
(
	const D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rRot,				// 向き
	const std::vector<D3DXVECTOR3> route,	// ルートの配列
	const float fChaseWidth,				// 追跡幅
	const float fChaseDepth					// 追跡奥行
)
{
	// 敵を生成
	CEnemyWolf* pEnemy = new CEnemyWolf;

	if (pEnemy == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 敵の初期化
		if (FAILED(pEnemy->Init()))
		{ // 初期化に失敗した場合

			// 敵の破棄
			SAFE_DELETE(pEnemy);
			return nullptr;
		}

		// 位置を設定
		pEnemy->SetVec3Position(rPos);

		// 向きを設定
		pEnemy->SetVec3Rotation(rRot);

		// 種類を設定
		pEnemy->SetType(TYPE_WOLF);

		// 初期位置を設定
		pEnemy->SetPosInit(rPos);

		// 情報の設定処理
		pEnemy->SetData();

		// ナビゲーションを生成
		pEnemy->m_pNav = CEnemyNavStreet::Create(route);

		// 追跡範囲を生成
		pEnemy->SetChaseRange(CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth));

		// 確保したアドレスを返す
		return pEnemy;
	}
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
		nCurMotion = UpdateCaveat(pPos, fDeltaTime);
		break;

	case STATE_FOUND:	// 追跡状態

		// 追跡状態時の更新
		nCurMotion = UpdateFound(pPos, pRot, fDeltaTime);
		break;

	case STATE_ATTACK:	// 攻撃状態

		// 攻撃状態時の更新
		nCurMotion = UpdateAttack(*pPos);
		break;

	case STATE_BLANKATTACK:

		// 空白攻撃状態の更新
		nCurMotion = UpdateBlankAttack(pRot, fDeltaTime);
		break;

	case STATE_UPSET:	// 動揺状態

		// 動揺状態時の更新
		nCurMotion = UpdateUpset();
		break;

	case STATE_CAUTION:	// 警戒状態

		// 警戒状態時の更新
		nCurMotion = UpdateCaution();
		break;

	case STATE_FADEOUT:	// フェードアウト状態

		// フェードアウト時の更新
		nCurMotion = UpdateFadeOut(pPos, pRot);
		break;

	case STATE_FADEIN:	// フェードイン状態

		// フェードイン時の更新
		nCurMotion = UpdateFadeIn();
		break;

	case STATE_STANCE:	// 構え状態

		// 構え時の更新
		nCurMotion = UpdateStance();
		break;

	case STATE_THREAT:	// 威嚇状態

		// 威嚇状態の更新
		nCurMotion = UpdateThreat();

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 重力の更新
	UpdateGravity();

	// 敵を落下させる
	pPos->y += GetMovePosition().y * fDeltaTime;

	if (Collision(*pPos))
	{ // 当たり判定が true かつ、移動状態の場合

		// ナビゲーションのリセット処理
		m_pNav->NavReset();
	}

	// 着地判定処理
	UpdateLanding(pPos);

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

			// TODO：警戒モーションの設定
			SetMotion(MOTION_IDOL, BLEND_FRAME_TURN);

			// 警戒状態にする
			SetState(STATE_CAUTION);
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
	switch (m_pNav->GetState())
	{
	case CEnemyNav::STATE_MOVE:

		// 移動モーションを設定
		*pMotion = MOTION_RUN;

		// 歩行音処理
		WalkSound();

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
	// 状態カウントを加算する
	m_nStateCount++;

	int motion = MOTION_IDOL;	// モーション

	// 向き更新
	RotMove(*pRot, REV_ROTA, fDeltaTime);

	if (m_pNav != nullptr)
	{ // ナビゲーションが NULL じゃない場合

		// ナビの更新処理
		m_pNav->Update
		(
			pPos,		// 位置
			pRot,		// 向き
			this,		// 敵の情報
			SPEED,		// 速度
			fDeltaTime	// デルタタイム
		);

		// ナビゲーションによるモーションの設定処理
		NavMoitonSet(&motion);
	}

	if (JudgeClone() ||
		JudgePlayer())
	{ // 分身かプレイヤーが目に入った場合

		// ナビゲーションリセット処理
		m_pNav->NavReset();

		if (GetChaseRange()->InsideTargetPos(GetPosInit(), GetTargetPos()))
		{ // 範囲内に入った場合

			// 警告状態にする
			SetState(STATE_CAVEAT);

			// 発見モーションを返す
			return MOTION_FOUND;
		}

		// 構え状態にする
		SetState(STATE_STANCE);

		// 構えた音を鳴らす
		PLAY_SOUND(CSound::LABEL_SE_STALKSTANCE_000);

		// TODO：構えモーションを返す
		return MOTION_LANDING;
	}

	// 標的を未設定にする
	SetTarget(TARGET_NONE);

	// 待機モーションを返す
	return motion;
}

//============================================================
//	警告状態時の更新処理
//============================================================
int CEnemyWolf::UpdateCaveat(D3DXVECTOR3* pPos, const float fDeltaTime)
{
	// 状態カウントを加算する
	m_nStateCount++;

	if (m_nStateCount % CAVEAT_STATE_COUNT == 0)
	{ // 一定時間経過した場合

		// 追跡状態にする
		SetState(STATE_FOUND);
	}

	// 走行モーションを返す
	return MOTION_RUN;
}

//============================================================
//	追跡状態時の更新処理
//============================================================
int CEnemyWolf::UpdateFound(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// 歩行カウントを加算する
	m_nStateCount++;

	// 歩行音処理
	WalkSound();

	if (!ShakeOffClone() &&
		!ShakeOffPlayer())
	{ // 分身が目に入っていない場合

		// 巡回状態にする
		SetState(STATE_CRAWL);

		// 標的を未設定にする
		SetTarget(TARGET_NONE);

		// 動揺モーションにする
		return MOTION_IDOL;
	}

	// 目標向きを目標位置方向にする
	LookTarget(*pPos);

	// 回避受付フラグを false にする
	SetEnableDodge(false);

	// 攻撃カウントをリセットする
	SetAttackCount(0);

	// 向きの移動処理
	RotMove(*pRot, REV_ROTA, fDeltaTime);

	// 移動処理
	Move(pPos, *pRot, SPEED, fDeltaTime);

	if (Approach(*pPos))
	{ // 接近した場合

		if (GetTarget() == CEnemyAttack::TARGET_PLAYER)
		{ // 目標がプレイヤーの場合

			// 回避受付フラグを true にする
			SetEnableDodge(true);

			// 攻撃カウントをリセットする
			SetAttackCount(0);
		}

		// 攻撃状態にする
		SetState(STATE_ATTACK);

		// 噛みつきモーションを返す
		return MOTION_BITE;
	}

	// 位置更新
	UpdatePosition(*pPos, fDeltaTime);

	if (GetChaseRange() != nullptr &&
		GetChaseRange()->ChaseRange(GetPosInit(), pPos))
	{ // 追跡範囲から出た場合

		// フェードアウト状態にする
		SetState(STATE_FADEOUT);

		// ナビゲーションリセット処理
		m_pNav->NavReset();

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
int CEnemyWolf::UpdateAttack(const D3DXVECTOR3& rPos)
{
	switch (GetTarget())
	{ // ターゲットごとの処理
	case CEnemyAttack::TARGET_PLAYER:

		// プレイヤーの当たり判定処理
		if (HitPlayer(rPos))
		{
			// 攻撃音を鳴らす
			PLAY_SOUND(CSound::LABEL_SE_STALKATTACK_000);
		}

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount % ATTACK_STATE_COUNT == 0)
		{ // 一定カウント経過した場合

			// 巡回状態にする
			SetState(STATE_CRAWL);
		}

		// 待機モーションにする
		return MOTION_IDOL;

	case CEnemyWolf::TARGET_CLONE:

		if (HitClone(rPos))
		{ // 分身に当たった場合

			// 空白攻撃状態にする
			SetState(STATE_BLANKATTACK);

			// 噛みつきモーションにする
			return MOTION_BITE;
		}
		else
		{ // 上記以外

			// 巡回状態にする
			SetState(STATE_CRAWL);
		}

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
// 空白攻撃処理
//============================================================
int CEnemyWolf::UpdateBlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// 状態カウントを加算する
	m_nStateCount++;

	// 向きの移動処理
	RotMove(*pRot, REV_ROTA_LOOK, fDeltaTime);

	if (m_nStateCount <= BLANKBITE_STATE_COUNT)
	{ // 一定カウント以下の場合

		if (m_nStateCount % BLANKBITE_CYCLE_COUNT == 0)
		{ // 一定カウントごとに

			// 目的の向きを取得
			D3DXVECTOR3 rotDest = GetDestRotation();

			// 目的の向きを設定する
			rotDest.y = useful::RandomRot();

			// 目的の向きを適用
			SetDestRotation(rotDest);
		}
	}
	else
	{ // 上記以外

		// 動揺状態にする
		SetState(STATE_UPSET);
	}

	// 攻撃モーションにする
	return MOTION_BITE;
}

//============================================================
//	動揺状態時の更新処理
//============================================================
int CEnemyWolf::UpdateUpset(void)
{
	// 状態カウントを加算する
	m_nStateCount++;

	if (m_nStateCount == UPSET_SOUND_COUNT)
	{ // 状態カウントが一定数になったとき

		// 動揺音を鳴らす
		PLAY_SOUND(CSound::LABEL_SE_STALKUPSET_000);
	}

	// 動揺モーションにする
	return MOTION_TURN;
}

//============================================================
// 警戒処理
//============================================================
int CEnemyWolf::UpdateCaution(void)
{
	// 状態カウントを加算する
	m_nStateCount++;

	if (m_nStateCount % CAUTION_STATE_COUNT == 0)
	{ // 状態カウントが一定数になった場合

		// フェードアウト状態にする
		SetState(STATE_FADEOUT);
	}

	if (JudgeClone() ||
		JudgePlayer())
	{ // 分身かプレイヤーが目に入った場合

		// ナビゲーションリセット処理
		m_pNav->NavReset();

		// 警告状態にする
		SetState(STATE_CAVEAT);

		// 発見モーションを返す
		return MOTION_FOUND;
	}
	else
	{ // 上記以外

		// 無対象にする
		SetTarget(TARGET_NONE);
	}

	// TODO：待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
// フェードアウト状態時の更新
//============================================================
int CEnemyWolf::UpdateFadeOut(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot)
{
	// 透明度を取得
	float fAlpha = GetAlpha();

	// 透明度を減算する
	fAlpha -= FADE_ALPHA_TRANS;

	if (fAlpha <= 0.0f)
	{ // 透明度が0以下になった場合

		// フェードイン状態にする
		SetState(STATE_FADEIN);

		// 位置を設定する
		*pPos = GetPosInit();

		// 過去の位置を適用する(こうしないと当たり判定に引っかかってしまう)
		SetOldPosition(*pPos);

		// 向きを設定する
		*pRot = VEC3_ZERO;

		// 目的の向きを設定する(復活後に無意味に向いてしまうため)
		SetDestRotation(*pRot);

		// 透明度を補正する
		fAlpha = 0.0f;
	}

	// 透明度を適用
	CObjectChara::SetAlpha(fAlpha);
	CEnemy::SetAlpha(fAlpha);

	// 待機モーションにする
	return MOTION_IDOL;
}

//============================================================
// フェードイン状態時の更新
//============================================================
int CEnemyWolf::UpdateFadeIn(void)
{
	// 透明度を取得
	float fAlpha = GetAlpha();

	// 透明度を減算する
	fAlpha += FADE_ALPHA_TRANS;

	if (fAlpha >= 1.0f)
	{ // 透明度が一定数以上になった場合

		// 巡回状態にする
		SetState(STATE_CRAWL);

		// 透明度を補正する
		fAlpha = 1.0f;
	}

	// 透明度を適用
	CObjectChara::SetAlpha(fAlpha);
	CEnemy::SetAlpha(fAlpha);

	// 待機モーションにする
	return MOTION_IDOL;
}

//============================================================
// 構え処理
//============================================================
int CEnemyWolf::UpdateStance(void)
{
	if (!JudgeClone() &&
		!JudgePlayer())
	{ // 分身もプレイヤーも視界から居なくなった場合

		// 威嚇状態にする
		SetState(STATE_THREAT);

		// TODO：威嚇モーションを返す
		return MOTION_FALL;
	}

	if (GetChaseRange()->InsideTargetPos(GetPosInit(), GetTargetPos()))
	{ // 範囲内に入った場合

		// 警告状態にする
		SetState(STATE_CAVEAT);

		// 発見モーションを返す
		return MOTION_FOUND;
	}

	// TODO：構えモーションを返す
	return MOTION_FALL;
}

//============================================================
// 威嚇処理
//============================================================
int CEnemyWolf::UpdateThreat(void)
{
	// フェードアウト状態にする
	SetState(STATE_FADEOUT);

	// TODO：歩行状態を返す
	return MOTION_RUN;
}

//============================================================
// 状態の設定処理
//============================================================
void CEnemyWolf::SetState(const EState state)
{
	// 状態を設定する
	m_state = state;

	// 状態カウントを0にする
	m_nStateCount = 0;
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
// 歩行音処理
//============================================================
void CEnemyWolf::WalkSound(void)
{
	if (m_nStateCount % WALK_SOUND_COUNT == 0)
	{ // 一定カウントごとに

		// 歩行音を鳴らす
		PLAY_SOUND(CSound::LABEL_SE_STALKWALK_000);
	}
}
