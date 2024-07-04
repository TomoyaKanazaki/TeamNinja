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

#include "multiModel.h"
#include "enemyNavigation.h"
#include "enemy_item.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\enemy.txt";	// セットアップテキスト相対パス
	const int	BLEND_FRAME_OTHER = 5;		// モーションの基本的なブレンドフレーム
	const int	BLEND_FRAME_LAND = 15;		// モーション着地のブレンドフレーム
	const int	CAUTIOUS_TRANS_LOOP = 7;	// 警戒モーションに遷移する待機ループ数
	const float	RADIUS = 20.0f;				// 半径
	const float HEIGHT = 80.0f;				// 身長
	const float SPEED = -290.0f;			// 速度
	const float ROT_REV = 0.5f;				// 向きの補正係数

	const int ITEM_PART_NUMBER = 8;			// アイテムを持つパーツの番号
	const D3DXVECTOR3 ITEM_OFFSET = D3DXVECTOR3(-3.0f, -1.0f, 10.0f);		// アイテムのオフセット座標
	const D3DXVECTOR3 ITEM_ROT = D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f);	// アイテムの向き
}

//************************************************************
//	子クラス [CEnemyStalk] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyStalk::CEnemyStalk() : CEnemyAttack(),
m_pNav(nullptr),
m_state(STATE_CRAWL)
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
	if (m_pNav != nullptr)
	{ // ナビが NULL じゃない場合

		// ナビの終了処理
		m_pNav->Uninit();
		m_pNav = nullptr;
	}

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

	// ナビゲーションを生成
	m_pNav = CEnemyNav::Create(GetVec3Position(), 700.0f, 700.0f);
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
		nCurMotion = Warning();

		break;

	case CEnemyStalk::STATE_STALK:

		// 追跡処理
		nCurMotion = Stalk(pPos, pRot, fDeltaTime);

		break;

	case CEnemyStalk::STATE_ATTACK:

		// 攻撃処理
		nCurMotion = Attack(*pPos);

		break;

	case CEnemyStalk::STATE_UPSET:

		// 動揺処理
		nCurMotion = Upset();

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	if (Collision(*pPos))
	{ // 当たり判定が true だった場合

		// 停止状態にする
		m_pNav->SetState(CEnemyNav::STATE_STOP);
	}

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

			// 待機モーションの設定
			SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);

			// 巡回状態にする
			m_state = STATE_CRAWL;
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
// 巡回処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Crawl(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	D3DXVECTOR3 rotDest = GetDestRotation();	// 目的の向き
	D3DXVECTOR3 Move = GetMovePosition();

	if (m_pNav != nullptr)
	{ // ナビゲーションが NULL じゃない場合

		// 向きの移動処理
		RotMove(*pRot, ROT_REV, fDeltaTime);

		// ナビの更新処理
		m_pNav->Update
		(
			pPos,		// 位置
			pRot,		// 向き
			&Move,		// 移動量
			&rotDest,	// 目的の向き
			RADIUS,		// 半径
			HEIGHT,		// 高さ
			SPEED,		// 速度
			fDeltaTime	// デルタタイム
		);
	}

	if (JudgeClone() ||
		JudgePlayer())
	{ // 分身かプレイヤーが目に入った場合

		// 警告状態にする
		m_state = STATE_WARNING;

		// 発見モーションを返す
		return MOTION_FOUND;
	}
	else
	{ // 上記以外

		// 無対象にする
		SetTarget(TARGET_NONE);
	}

	SetDestRotation(rotDest);
	SetMovePosition(Move);

	// 待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
// 警告処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Warning(void)
{
	if (GetMotionType() != MOTION_FOUND)
	{ // 発見モーションじゃなかった場合

		// 追跡状態にする
		m_state = STATE_STALK;
	}

	// 歩行状態を返す
	return MOTION_WALK;
}

//============================================================
// 追跡処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	if (JudgeClone() ||
		JudgePlayer())
	{ // 分身かプレイヤーが目に入った場合

		// 攻撃判定を false にする
		SetEnableAttack(false);
	}
	else
	{ // 上記以外

		// 巡回状態にする
		m_state = STATE_CRAWL;

		// 待機モーションを返す
		return MOTION_IDOL;
	}

	// 移動処理
	Move(pPos, *pRot, SPEED, fDeltaTime);

	if (Approach(*pPos))
	{ // 接近した場合

		// 攻撃状態にする
		m_state = STATE_ATTACK;

		// 攻撃モーションを返す
		return MOTION_ATTACK;
	}

	// デバッグ
	DebugProc::Print(DebugProc::POINT_RIGHT, "発見!!目的地：%f %f %f", GetTargetPos().x, GetTargetPos().y, GetTargetPos().z);

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

		// プレイヤーの当たり判定処理
		HitPlayer(rPos);

		if (GetMotionType() != MOTION_ATTACK)
		{ // 攻撃モーションじゃない場合

			// 巡回状態にする
			m_state = STATE_CRAWL;
		}

		// 待機モーションにする
		return MOTION_IDOL;

		break;

	case CEnemyAttack::TARGET_CLONE:

		// 分身の当たり判定処理
		HitClone(rPos);

		// 動揺状態にする
		m_state = STATE_UPSET;

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
// 動揺処理
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Upset(void)
{
	// 動揺モーションにする
	return MOTION_UPSET;
}