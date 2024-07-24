//============================================================
//
//	追跡敵処理 [enemyStalk.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "enemyStalk.h"
#include "renderer.h"
#include "deltaTime.h"
#include "sound.h"

#include "multiModel.h"
#include "enemyNavigation.h"
#include "enemyNavStreet.h"
#include "enemyNavRandom.h"
#include "enemyChaseRange.h"
#include "enemy_item.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\enemy.txt";	// セットアップテキスト相対パス
	const int	BLEND_FRAME_OTHER = 5;		// モーションの基本的なブレンドフレーム
	const int	BLEND_FRAME_UPSET = 8;		// モーション動揺のブレンドフレーム
	const int	BLEND_FRAME_LAND = 15;		// モーション着地のブレンドフレーム
	const int	CAUTIOUS_TRANS_LOOP = 7;	// 警戒モーションに遷移する待機ループ数
	const float	RADIUS = 20.0f;				// 半径
	const float HEIGHT = 80.0f;				// 身長
	const float SPEED = -290.0f;			// 速度
	const float ROT_REV = 4.0f;				// 向きの補正係数
	const float FADE_ALPHA_TRANS = 0.02f;	// フェードの透明度の遷移定数

	const int ITEM_PART_NUMBER = 8;			// アイテムを持つパーツの番号
	const D3DXVECTOR3 ITEM_OFFSET = D3DXVECTOR3(-3.0f, -1.0f, 10.0f);		// アイテムのオフセット座標
	const D3DXVECTOR3 ITEM_ROT = D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f);	// アイテムの向き

	// 状態管理関係
	const int FOUND_STATE_COUNT = 59;			// 発見状態のカウント数
	const int ATTACK_STATE_COUNT = 44;			// 攻撃状態のカウント数
	const int BLANKATTACK_STATE_COUNT = 340;	// 空白攻撃状態のカウント数
	const int BLANKATTACK_CYCLE_COUNT = 18;		// 空白攻撃状態の回転カウント
	const int CAUTION_STATE_COUNT = 180;		// 警戒状態のカウント数

	// 音管理関係
	const int WALK_SOUND_COUNT = 32;			// 歩行音を鳴らすカウント数
}

//************************************************************
//	子クラス [CEnemyStalk] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyStalk::CEnemyStalk() : CEnemyAttack(),
m_pNav(nullptr),
m_state(STATE_CRAWL),
m_nStateCount(0)
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyStalk::~CEnemyStalk()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyStalk::Init(void)
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
void CEnemyStalk::Uninit(void)
{
	// ナビゲーションの終了処理
	SAFE_UNINIT(m_pNav);

	// 敵の終了
	CEnemyAttack::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyStalk::Update(const float fDeltaTime)
{
	// 敵の更新
	CEnemyAttack::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CEnemyStalk::Draw(CShader* pShader)
{
	// 敵の描画
	CEnemyAttack::Draw(pShader);
}

//============================================================
// 情報の設定処理
//============================================================
void CEnemyStalk::SetData(void)
{
	// アイテムを設定する
	SetItem(CEnemyItem::Create
	(
		CEnemyItem::TYPE_KATANA,
		ITEM_OFFSET,
		ITEM_ROT
	));

	// 親オブジェクト (持ち手) の設定
	GetItem()->SetParentObject(GetParts(ITEM_PART_NUMBER));
}

//============================================================
// 半径の取得処理
//============================================================
float CEnemyStalk::GetRadius(void) const
{
	// 半径を返す
	return RADIUS;
}

//============================================================
// 高さの取得処理
//============================================================
float CEnemyStalk::GetHeight(void) const
{
	// 身長を返す
	return HEIGHT;
}

//============================================================
//	生成処理(一定範囲移動敵)
//============================================================
CEnemyStalk* CEnemyStalk::Create
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
	CEnemyStalk* pEnemy = new CEnemyStalk;

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
		pEnemy->SetType(TYPE_STALK);

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
CEnemyStalk* CEnemyStalk::Create
(
	const D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rRot,				// 向き
	const std::vector<D3DXVECTOR3> route,	// ルートの配列
	const float fChaseWidth,				// 追跡幅
	const float fChaseDepth					// 追跡奥行
)
{
	// 敵を生成
	CEnemyStalk* pEnemy = new CEnemyStalk;

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
		pEnemy->SetType(TYPE_STALK);

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
// 状態の更新処理
//============================================================
int CEnemyStalk::UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// 現在のモーション
	switch (m_state)
	{
	case CEnemyStalk::STATE_CRAWL:

		// 巡回処理
		nCurMotion = Crawl(pPos, pRot, fDeltaTime);

		break;

	case CEnemyStalk::STATE_WARNING:

		// 警告処理
		nCurMotion = Warning(pPos, fDeltaTime);

		break;

	case CEnemyStalk::STATE_STALK:

		// 追跡処理
		nCurMotion = Stalk(pPos, pRot, fDeltaTime);

		break;

	case CEnemyStalk::STATE_ATTACK:

		// 攻撃処理
		nCurMotion = Attack(*pPos);

		break;

	case CEnemyStalk::STATE_BLANKATTACK:

		// 空白攻撃処理
		nCurMotion = BlankAttack(pRot, fDeltaTime);

		break;

	case CEnemyStalk::STATE_UPSET:

		// 動揺処理
		nCurMotion = Upset();

		break;

	case CEnemyStalk::STATE_CAUTION:

		// 警戒処理
		nCurMotion = Caution();

		break;

	case CEnemyStalk::STATE_FADEOUT:

		// フェードアウト処理
		nCurMotion = FadeOut(pPos, pRot);

		break;

	case CEnemyStalk::STATE_FADEIN:

		// フェードイン処理
		nCurMotion = FadeIn();

		break;

	case CEnemyStalk::STATE_STANCE:

		// 構え処理
		nCurMotion = Stance();

		break;

	case CEnemyStalk::STATE_THREAT:

		// 威嚇処理
		nCurMotion = Threat();

		break;

	default:

		// 停止
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
// モーションの更新処理
//============================================================
void CEnemyStalk::UpdateMotion(int nMotion, const float fDeltaTime)
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

	// 現在のモーションの更新
	switch (GetMotionType())
	{
	case CEnemyStalk::MOTION_IDOL:		// 待機
		break;

	case CEnemyStalk::MOTION_WALK:		// 歩行
		break;

	case CEnemyStalk::MOTION_FOUND:		// 発見

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyStalk::MOTION_ATTACK:	// 攻撃

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyStalk::MOTION_UPSET:		// 動揺

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// TODO：警戒モーションの設定
			SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);

			// 警戒状態にする
			SetState(STATE_CAUTION);
		}

		break;

	case CEnemyStalk::MOTION_FALL:		// 落下

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyStalk::MOTION_LANDING:	// 着地

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//============================================================
// 着地の更新処理
//============================================================
void CEnemyStalk::UpdateLanding(D3DXVECTOR3* pPos)
{
	// 親クラスの着地更新
	CEnemy::UpdateLanding(pPos);

	// 現在のモーション種類を取得
	int nCurMotion = GetMotionType();

	// 落下モーションのフラグを設定
	bool bTypeFall = nCurMotion == MOTION_FALL;

	if (!IsJump())
	{ // 空中にいない場合

		if (bTypeFall)
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
// ナビによるモーションの設定処理
//============================================================
void CEnemyStalk::NavMotionSet(EMotion* pMotion)
{
	switch (m_pNav->GetState())
	{
	case CEnemyNav::STATE_MOVE:

		// 移動モーションを設定
		*pMotion = MOTION_WALK;

		// 歩行音処理
		WalkSound();

		break;

	default:

		break;
	}
}

//============================================================
// 巡回処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Crawl(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	EMotion motion = MOTION_IDOL;				// モーション

	// 向きの移動処理
	RotMove(*pRot, ROT_REV, fDeltaTime);

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

		// ナビによるモーション設定処理
		NavMotionSet(&motion);

		// 状態カウントを加算する
		m_nStateCount++;
	}

	if (JudgeClone() ||
		JudgePlayer())
	{ // 分身かプレイヤーが目に入った場合

		// ナビゲーションリセット処理
		m_pNav->NavReset();

		if (GetChaseRange()->InsideTargetPos(GetPosInit(), GetTargetPos()))
		{ // 範囲内に入った場合

			// 警告状態にする
			SetState(STATE_WARNING);

			// 発見モーションを返す
			return MOTION_FOUND;
		}

		// 構え状態にする
		SetState(STATE_STANCE);

		// TODO：構えモーションを返す
		return MOTION_WALK;
	}

	// 無対象にする
	SetTarget(TARGET_NONE);

	// 待機モーションを返す
	return motion;
}

//============================================================
// 警告処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Warning(D3DXVECTOR3* pPos, const float fDeltaTime)
{
	// 状態カウントを加算する
	m_nStateCount++;

	if (m_nStateCount % FOUND_STATE_COUNT == 0)
	{ // 一定時間経過した場合

		// 追跡状態にする
		SetState(STATE_STALK);
	}

	// 歩行状態を返す
	return MOTION_WALK;
}

//============================================================
// 追跡処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// 歩行カウントを加算する
	m_nStateCount++;

	// 歩行音処理
	WalkSound();

	if (!ShakeOffClone() &&
		!ShakeOffPlayer())
	{ // 分身かプレイヤーが視界内にいない場合

		// 巡回状態にする
		SetState(STATE_CRAWL);

		// 標的を未設定にする
		SetTarget(TARGET_NONE);

		// 待機モーションを返す
		return MOTION_IDOL;
	}

	// 目標位置の視認処理
	LookTarget(*pPos);

	// 回避受付フラグを false にする
	SetEnableDodge(false);

	// 攻撃カウントをリセットする
	SetAttackCount(0);

	// 向きの移動処理
	RotMove(*pRot, ROT_REV, fDeltaTime);

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

		// 攻撃モーションを返す
		return MOTION_ATTACK;
	}

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

	// 歩行モーションを返す
	return MOTION_WALK;
}

//============================================================
// 攻撃処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Attack(const D3DXVECTOR3& rPos)
{
	switch (GetTarget())
	{
	case CEnemyAttack::TARGET_PLAYER:

		if (HitPlayer(rPos))
		{ // プレイヤーに当たった場合

			// 分身攻撃音を鳴らす
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

		break;

	case CEnemyAttack::TARGET_CLONE:

		if (HitClone(rPos))
		{ // 分身に当たった場合

			// 空白攻撃状態にする
			SetState(STATE_BLANKATTACK);

			// 攻撃モーションにする
			return MOTION_ATTACK;
		}
		else
		{ // 上記以外

			// 巡回状態にする
			SetState(STATE_CRAWL);
		}

		// 動揺モーションにする
		return MOTION_UPSET;

		break;

	default:		// 例外処理
		assert(false);
		break;
	}

	// 待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
// 空白攻撃処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// 状態カウントを加算する
	m_nStateCount++;

	// 向きの移動処理
	RotMove(*pRot, ROT_REV, fDeltaTime);

	if (m_nStateCount <= BLANKATTACK_STATE_COUNT)
	{ // 一定カウント以下の場合

		if (m_nStateCount % BLANKATTACK_CYCLE_COUNT == 0)
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

	// 動揺モーションにする
	return MOTION_ATTACK;
}

//============================================================
// 動揺処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Upset(void)
{
	// 動揺モーションにする
	return MOTION_UPSET;
}

//============================================================
// 警戒処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Caution(void)
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
		SetState(STATE_WARNING);

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
// フェードアウト処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::FadeOut(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot)
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
// フェードイン処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::FadeIn(void)
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
CEnemyStalk::EMotion CEnemyStalk::Stance(void)
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
		SetState(STATE_WARNING);

		// 発見モーションを返す
		return MOTION_FOUND;
	}

	// TODO：構えモーションを返す
	return MOTION_WALK;
}

//============================================================
// 威嚇処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Threat(void)
{
	// フェードアウト状態にする
	SetState(STATE_FADEOUT);

	// TODO：歩行状態を返す
	return MOTION_WALK;
}

//============================================================
// 状態の設定処理
//============================================================
void CEnemyStalk::SetState(const EState state)
{
	// 状態を設定する
	m_state = state;

	// 状態カウントを0にする
	m_nStateCount = 0;
}

//============================================================
// 歩行音処理
//============================================================
void CEnemyStalk::WalkSound(void)
{
	if (m_nStateCount % WALK_SOUND_COUNT == 0)
	{ // 一定カウントごとに

		// 歩行音を鳴らす
		PLAY_SOUND(CSound::LABEL_SE_STALKWALK_000);
	}
}
