//============================================================
//
//	待ち伏せ敵処理 [enemyAmbush.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "enemyAmbush.h"
#include "renderer.h"
#include "deltaTime.h"

#include "multiModel.h"
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
	const float SPEED = -370.0f;			// 速度
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
}

//************************************************************
//	子クラス [CEnemyAmbush] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyAmbush::CEnemyAmbush() : CEnemyAttack(),
m_state(STATE_AMBUSH),
m_nStateCount(0)
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyAmbush::~CEnemyAmbush()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyAmbush::Init(void)
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
void CEnemyAmbush::Uninit(void)
{
	// 敵の終了
	CEnemyAttack::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyAmbush::Update(const float fDeltaTime)
{
	// 敵の更新
	CEnemyAttack::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CEnemyAmbush::Draw(CShader* pShader)
{
	// 敵の描画
	CEnemyAttack::Draw(pShader);
}

//============================================================
// 情報の設定処理
//============================================================
void CEnemyAmbush::SetData(void)
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
float CEnemyAmbush::GetRadius(void) const
{
	// 半径を返す
	return RADIUS;
}

//============================================================
// 高さの取得処理
//============================================================
float CEnemyAmbush::GetHeight(void) const
{
	// 身長を返す
	return HEIGHT;
}

//============================================================
//	生成処理(ルート待ち伏せ移動敵)
//============================================================
CEnemyAmbush* CEnemyAmbush::Create
(
	const D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rRot,				// 向き
	const float fChaseWidth,				// 追跡幅
	const float fChaseDepth					// 追跡奥行
)
{
	// 敵を生成
	CEnemyAmbush* pEnemy = new CEnemyAmbush;

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
		pEnemy->SetType(TYPE_AMBUSH);

		// 初期位置を設定
		pEnemy->SetPosInit(rPos);

		// 情報の設定処理
		pEnemy->SetData();

		// 追跡範囲を生成
		pEnemy->SetChaseRange(CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth));

		// 確保したアドレスを返す
		return pEnemy;
	}
}

//============================================================
// 状態の更新処理
//============================================================
int CEnemyAmbush::UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// 現在のモーション
	switch (m_state)
	{
	case CEnemyAmbush::STATE_AMBUSH:

		// 待ち伏せ処理
		nCurMotion = Ambush(pPos, fDeltaTime);

		break;

	case CEnemyAmbush::STATE_WARNING:

		// 警告処理
		nCurMotion = Warning(pPos, fDeltaTime);

		break;

	case CEnemyAmbush::STATE_STALK:

		// 追跡処理
		nCurMotion = Stalk(pPos, pRot, fDeltaTime);

		break;

	case CEnemyAmbush::STATE_ATTACK:

		// 攻撃処理
		nCurMotion = Attack(*pPos);

		break;

	case CEnemyAmbush::STATE_BLANKATTACK:

		// 空白攻撃処理
		nCurMotion = BlankAttack(pRot, fDeltaTime);

		break;

	case CEnemyAmbush::STATE_UPSET:

		// 動揺処理
		nCurMotion = Upset();

		break;

	case CEnemyAmbush::STATE_CAUTION:

		// 警戒処理
		nCurMotion = Caution();

		break;

	case CEnemyAmbush::STATE_FADEOUT:

		// フェードアウト処理
		nCurMotion = FadeOut(pPos, pRot);

		break;

	case CEnemyAmbush::STATE_FADEIN:

		// フェードイン処理
		nCurMotion = FadeIn();

		break;

	case CEnemyAmbush::STATE_STANCE:

		// 構え処理
		nCurMotion = Stance();

		break;

	case CEnemyAmbush::STATE_THREAT:

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

	// 当たり判定処理
	Collision(*pPos);

	// 着地判定処理
	UpdateLanding(pPos);

	// 現在のモーションを返す
	return nCurMotion;
}

//============================================================
// モーションの更新処理
//============================================================
void CEnemyAmbush::UpdateMotion(int nMotion, const float fDeltaTime)
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
	case CEnemyAmbush::MOTION_IDOL:		// 待機
		break;

	case CEnemyAmbush::MOTION_WALK:		// 歩行
		break;

	case CEnemyAmbush::MOTION_FOUND:		// 発見

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyAmbush::MOTION_ATTACK:	// 攻撃

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyAmbush::MOTION_UPSET:		// 動揺

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 待機モーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyAmbush::MOTION_FALL:		// 落下

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyAmbush::MOTION_LANDING:	// 着地

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
void CEnemyAmbush::UpdateLanding(D3DXVECTOR3* pPos)
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
// 待ち伏せ処理
//============================================================
CEnemyAmbush::EMotion CEnemyAmbush::Ambush(D3DXVECTOR3* pPos, const float fDeltaTime)
{
	if (JudgeClone() ||
		JudgePlayer())
	{ // 分身かプレイヤーが目に入った場合

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
	return MOTION_IDOL;
}

//============================================================
// 警告処理
//============================================================
CEnemyAmbush::EMotion CEnemyAmbush::Warning(D3DXVECTOR3* pPos, const float fDeltaTime)
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
CEnemyAmbush::EMotion CEnemyAmbush::Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	if (!ShakeOffClone() &&
		!ShakeOffPlayer())
	{ // 分身かプレイヤーが視界内にいない場合

		// 待ち伏せ状態にする
		SetState(STATE_AMBUSH);

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
CEnemyAmbush::EMotion CEnemyAmbush::Attack(const D3DXVECTOR3& rPos)
{
	switch (GetTarget())
	{
	case CEnemyAttack::TARGET_PLAYER:

		// プレイヤーの当たり判定処理
		HitPlayer(rPos);

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount % ATTACK_STATE_COUNT == 0)
		{ // 一定カウント経過した場合

			// 待ち伏せ状態にする
			SetState(STATE_AMBUSH);
		}

		// 待機モーションにする
		return MOTION_IDOL;

		break;

	case CEnemyAttack::TARGET_CLONE:

		// 分身の当たり判定処理
		HitClone(rPos);

		if (HitClone(rPos))
		{ // 分身に当たった場合

			// 空白攻撃状態にする
			SetState(STATE_BLANKATTACK);

			// 攻撃モーションにする
			return MOTION_ATTACK;
		}
		else
		{ // 上記以外

			// 待ち伏せ状態にする
			SetState(STATE_AMBUSH);
		}

		// 動揺状態にする
		SetState(STATE_UPSET);

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
CEnemyAmbush::EMotion CEnemyAmbush::BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime)
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
CEnemyAmbush::EMotion CEnemyAmbush::Upset(void)
{
	// 動揺モーションにする
	return MOTION_UPSET;
}

//============================================================
// 警戒処理
//============================================================
CEnemyAmbush::EMotion CEnemyAmbush::Caution(void)
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
CEnemyAmbush::EMotion CEnemyAmbush::FadeOut(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot)
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
CEnemyAmbush::EMotion CEnemyAmbush::FadeIn(void)
{
	// 透明度を取得
	float fAlpha = GetAlpha();

	// 透明度を減算する
	fAlpha += FADE_ALPHA_TRANS;

	if (fAlpha >= 1.0f)
	{ // 透明度が一定数以上になった場合

		// 待ち伏せ状態にする
		SetState(STATE_AMBUSH);

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
CEnemyAmbush::EMotion CEnemyAmbush::Stance(void)
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
CEnemyAmbush::EMotion CEnemyAmbush::Threat(void)
{
	// フェードアウト状態にする
	SetState(STATE_FADEOUT);

	// TODO：歩行状態を返す
	return MOTION_WALK;
}

//============================================================
// 状態の設定処理
//============================================================
void CEnemyAmbush::SetState(const EState state)
{
	// 状態を設定する
	m_state = state;

	// 状態カウントを0にする
	m_nStateCount = 0;
}
