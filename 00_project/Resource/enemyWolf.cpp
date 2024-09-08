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
#include "stage.h"
#include "sound.h"

#include "enemyNavigation.h"
#include "enemyNavStreet.h"
#include "enemyNavRandom.h"
#include "enemyChaseRange.h"
#include "camera.h"

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
m_pNav(nullptr)		// ナビゲーションの情報
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
	// 情報の設定処理
	CEnemyAttack::SetData();
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
// 速度の取得処理
//============================================================
float CEnemyWolf::GetSpeed(void) const
{
	// 速度を返す
	return SPEED;
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

		// 初期向きを設定
		pEnemy->SetRotInit(rRot);

		// ナビゲーションを生成
		pEnemy->m_pNav = CEnemyNavRandom::Create(rPos, fMoveWidth, fMoveDepth);

		// 追跡範囲を生成
		pEnemy->SetChaseRange(CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth));

		// 情報の設定処理
		pEnemy->SetData();

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

		// 初期向きを設定
		pEnemy->SetRotInit(rRot);

		// ナビゲーションを生成
		pEnemy->m_pNav = CEnemyNavStreet::Create(route);

		// 追跡範囲を生成
		pEnemy->SetChaseRange(CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth));

		// 情報の設定処理
		pEnemy->SetData();

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

	// 元の位置に戻る処理が true の場合、抜ける
	if (!BackOriginPos(pPos, pRot, HEIGHT))
	{
		switch (GetState())
		{ // 状態ごとの処理
		case STATE_ORIGIN:	// 独自状態

			// 独自状態時の更新
			nCurMotion = Original(pPos, pRot, fDeltaTime);
			break;

		case STATE_WARNING:	// 警告状態

			// 警告状態時の更新
			nCurMotion = Warning(pPos, pRot, fDeltaTime, REV_ROTA);
			break;

		case STATE_STALK:	// 追跡状態

			// 追跡状態時の更新
			nCurMotion = Stalk(pPos, pRot, fDeltaTime, REV_ROTA);
			break;

		case STATE_ATTACK:	// 攻撃状態

			// 攻撃状態時の更新
			nCurMotion = Attack(*pPos);
			break;

		case STATE_BLANKATTACK:

			// 空白攻撃状態の更新
			nCurMotion = BlankAttack(pRot, fDeltaTime, REV_ROTA_LOOK);
			break;

		case STATE_UPSET:	// 動揺状態

			// 動揺状態時の更新
			nCurMotion = Upset();
			break;

		case STATE_STANCE:	// 構え状態

			// 構え時の更新
			nCurMotion = Stance();
			break;

		default:	// 例外処理
			assert(false);
			break;
		}
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

	// 大人の壁の判定
	GET_STAGE->LimitPosition(*pPos, RADIUS);

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

		// ブレンド中の場合抜ける
		if (GetMotionBlendFrame() != 0) { break; }

		if (GetMotionKey() % 2 == 0 && GetMotionKeyCounter() == 0)
		{ // 足がついたタイミングの場合

			// 歩行音を鳴らす
			PLAY_SOUND(CSound::LABEL_SE_STALKWALK_000);
		}

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

			// 独自状態にする
			SetState(STATE_ORIGIN);
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

		break;

	default:

		break;
	}
}

//============================================================
// それぞれの独自処理
//============================================================
 int CEnemyWolf::Original(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
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

			// 独自状態にする
			SetState(STATE_ORIGIN);

			// 発見モーションを返す
			return MOTION_FOUND;
		}

		// 構え状態にする
		SetState(STATE_STANCE);

		// 構えた音を鳴らす
		PLAY_SOUND(CSound::LABEL_SE_WOLFSTANCE_000);

		// TODO：構えモーションを返す
		return MOTION_FOUND;
	}

	// 標的を未設定にする
	SetTarget(TARGET_NONE);

	// 待機モーションを返す
	return motion;
}

//============================================================
//	警告状態時の更新処理
//============================================================
int CEnemyWolf::Warning				// 警告処理
(
	D3DXVECTOR3* pPos,		// 位置
	D3DXVECTOR3* pRot,		// 向き
	const float fDeltaTime,	// デルタタイム
	const float fRotRev		// 向きの補正数
)
{
	// 警告処理
	CEnemyAttack::Warning(pPos, pRot, fDeltaTime, REV_ROTA);

	// 走行モーションを返す
	return MOTION_RUN;
}

//============================================================
// 追跡処理
//============================================================
int CEnemyWolf::Stalk
(
	D3DXVECTOR3* pPos,		// 位置
	D3DXVECTOR3* pRot, 		// 向き
	const float fDeltaTime,	// デルタタイム
	const float fRotRev		// 向きの補正数
)
{
	// 追跡処理
	switch (CEnemyAttack::Stalk(pPos, pRot, fDeltaTime, REV_ROTA))
	{
	case STATE_ORIGIN:

		// 待機モーションを返す
		return MOTION_IDOL;

		break;

	case STATE_ATTACK:

		// 噛みつきモーションを返す
		return MOTION_BITE;

		break;

	case STATE_STANCE:

		// ナビゲーションリセット処理
		m_pNav->NavReset();

		// 走行モーションを返す
		return MOTION_RUN;

		break;

	default:

		// 走行モーションを返す
		return MOTION_RUN;

		break;
	}
}

//============================================================
// 攻撃処理
//============================================================
int CEnemyWolf::Attack(const D3DXVECTOR3& rPos)
{
	switch (CEnemyAttack::Attack(rPos))
	{
	case STATE_ORIGIN:

		// 待機モーションを返す
		return MOTION_IDOL;

	case STATE_BLANKATTACK:

		// 攻撃モーションを返す
		return MOTION_BITE;

	default:

		// 待機モーションにする
		return MOTION_IDOL;
	}
}

//============================================================
// 空白攻撃処理
//============================================================
int CEnemyWolf::BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime, const float fRotRev)
{
	switch (CEnemyAttack::BlankAttack(pRot, fDeltaTime, fRotRev))
	{
	case STATE_UPSET:

		// 振り向きモーションにする
		return MOTION_TURN;

	default:

		// 攻撃モーションにする
		return MOTION_BITE;
	}
}

//============================================================
// 動揺処理
//============================================================
int CEnemyWolf::Upset(void)
{
	// 動揺処理
	CEnemyAttack::Upset();

	// 振り向きモーションを返す
	return MOTION_TURN;
}

//============================================================
// 構え処理
//============================================================
int CEnemyWolf::Stance(void)
{
	switch (CEnemyAttack::Stance())
	{
	case STATE_WARNING:

		// 発見モーションを返す
		return MOTION_FOUND;

	default:

		// 構えモーションを返す
		return MOTION_FOUND;
	}
}

//============================================================
// 元の位置に戻る処理
//============================================================
bool CEnemyWolf::BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight)
{
	// 一定の状態の場合、false を返す
	if (GetState() == STATE_BLANKATTACK || GetState() == STATE_UPSET) { SetRegressionCount(0); return false; }

	// 初期位置回帰処理に失敗した場合、false を返す
	if (!CEnemyAttack::BackOriginPos(pPos, pRot, fHeight)) { return false; }

	// 独自状態にする
	SetState(STATE_ORIGIN);

	// ナビゲーションリセット処理
	m_pNav->NavReset();

	// true を返す
	return true;
}
