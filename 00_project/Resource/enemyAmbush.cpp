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
#include "stage.h"
#include "sound.h"

#include "multiModel.h"
#include "enemyChaseRange.h"
#include "enemy_item.h"
#include "camera.h"

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
	const float SPEED = -490.0f;			// 速度
	const float ROT_REV = 9.0f;				// 向きの補正係数
	const float FADE_ALPHA_TRANS = 0.02f;	// フェードの透明度の遷移定数

	const int ITEM_PART_NUMBER = 8;			// アイテムを持つパーツの番号
	const D3DXVECTOR3 ITEM_OFFSET = D3DXVECTOR3(-3.0f, -1.0f, 10.0f);		// アイテムのオフセット座標
	const D3DXVECTOR3 ITEM_ROT = D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f);	// アイテムの向き
}

//************************************************************
//	子クラス [CEnemyAmbush] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyAmbush::CEnemyAmbush() : CEnemyAttack()
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

	// 情報の設定処理
	CEnemyAttack::SetData();
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
// 速度の取得処理
//============================================================
float CEnemyAmbush::GetSpeed(void) const
{
	// 速度を返す
	return SPEED;
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

		// 初期向きを設定
		pEnemy->SetRotInit(rRot);

		// 追跡範囲を生成
		pEnemy->SetChaseRange(CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth));

		// 情報の設定処理
		pEnemy->SetData();

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

	// 元の位置に戻る処理が true の場合、抜ける
	if (!BackOriginPos(pPos, pRot, HEIGHT))
	{
		switch (GetState())
		{
		case CEnemyAmbush::STATE_ORIGIN:

			// 待ち伏せ処理
			nCurMotion = Original(pPos, pRot, fDeltaTime);

			break;

		case CEnemyAmbush::STATE_WARNING:

			// 警告処理
			nCurMotion = Warning(pPos, pRot, fDeltaTime, ROT_REV);

			break;

		case CEnemyAmbush::STATE_STALK:

			// 追跡処理
			nCurMotion = Stalk(pPos, pRot, fDeltaTime, ROT_REV);

			break;

		case CEnemyAmbush::STATE_ATTACK:

			// 攻撃処理
			nCurMotion = Attack(*pPos);

			break;

		case CEnemyAmbush::STATE_BLANKATTACK:

			// 空白攻撃処理
			nCurMotion = BlankAttack(pRot, fDeltaTime, ROT_REV);

			break;

		case CEnemyAmbush::STATE_UPSET:

			// 動揺処理
			nCurMotion = Upset();

			break;

		case CEnemyAmbush::STATE_STANCE:

			// 構え処理
			nCurMotion = Stance();

			break;

		default:

			// 停止
			assert(false);

			break;
		}
	}

	// 重力の更新
	UpdateGravity();

	// 敵を落下させる
	pPos->y += GetMovePosition().y * fDeltaTime;

	// 当たり判定処理
	Collision(*pPos);

	// 着地判定処理
	UpdateLanding(pPos);

	// 大人の壁の判定
	GET_STAGE->LimitPosition(*pPos, RADIUS);

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

		// ブレンド中の場合抜ける
		if (GetMotionBlendFrame() != 0) { break; }

		if (GetMotionKey() % 2 == 0 && GetMotionKeyCounter() == 0)
		{ // 足がついたタイミングの場合

			// 歩行音を鳴らす
			PLAY_SOUND(CSound::LABEL_SE_STALKWALK_000);
		}

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

			// TODO：警戒モーションの設定
			SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);

			// 独自状態にする
			SetState(STATE_ORIGIN);
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

	case CEnemyAmbush::MOTION_STANDBY:	// 構え

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyAmbush::MOTION_BATTLE:	// 威嚇

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
// それぞれの独自処理
//============================================================
int CEnemyAmbush::Original(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
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

		// 構えモーションを返す
		return MOTION_STANDBY;
	}

	// 無対象にする
	SetTarget(TARGET_NONE);

	// 待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
// 警告処理
//============================================================
int CEnemyAmbush::Warning
(
	D3DXVECTOR3* pPos,		// 位置
	D3DXVECTOR3* pRot,		// 向き
	const float fDeltaTime,	// デルタタイム
	const float fRotRev		// 向きの補正数
)
{
	// 警告処理
	CEnemyAttack::Warning(pPos, pRot, fDeltaTime, fRotRev);

	// 歩行状態を返す
	return MOTION_WALK;
}

//============================================================
// 追跡処理
//============================================================
int CEnemyAmbush::Stalk
(
	D3DXVECTOR3* pPos,		// 位置
	D3DXVECTOR3* pRot, 		// 向き
	const float fDeltaTime,	// デルタタイム
	const float fRotRev		// 向きの補正数
)
{
	// 追跡処理
	switch (CEnemyAttack::Stalk(pPos, pRot, fDeltaTime, fRotRev))
	{
	case STATE_ORIGIN:

		// 待機モーションを返す
		return MOTION_IDOL;

	case STATE_ATTACK:

		// 攻撃モーションを返す
		return MOTION_ATTACK;

	default:

		// 歩行モーションを返す
		return MOTION_WALK;
	}
}

//============================================================
// 攻撃処理
//============================================================
int CEnemyAmbush::Attack(const D3DXVECTOR3& rPos)
{
	switch (CEnemyAttack::Attack(rPos))
	{
	case STATE_ORIGIN:

		// 待機モーションを返す
		return MOTION_IDOL;

	case STATE_BLANKATTACK:

		// 分身に対する攻撃モーションを返す
		return MOTION_BATTLE;

	default:

		// 待機モーションにする
		return MOTION_IDOL;
	}
}

//============================================================
// 空白攻撃処理
//============================================================
int CEnemyAmbush::BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime, const float fRotRev)
{
	switch (CEnemyAttack::BlankAttack(pRot, fDeltaTime, fRotRev))
	{
	case STATE_UPSET:

		// 動揺モーションにする
		return MOTION_UPSET;

	default:

		// 分身に対する攻撃モーションにする
		return MOTION_BATTLE;
	}
}

//============================================================
// 動揺処理
//============================================================
int CEnemyAmbush::Upset(void)
{
	// 動揺処理
	CEnemyAttack::Upset();

	// 動揺モーションを返す
	return MOTION_UPSET;
}

//============================================================
// 構え処理
//============================================================
int CEnemyAmbush::Stance(void)
{
	switch (CEnemyAttack::Stance())
	{
	case STATE_WARNING:

		// 発見モーションを返す
		return MOTION_FOUND;

	default:

		// 構えモーションを返す
		return MOTION_STANDBY;
	}
}

//============================================================
// 元の位置に戻る処理
//============================================================
bool CEnemyAmbush::BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight)
{
	// 一定の状態の場合、false を返す
	if (GetState() == STATE_BLANKATTACK || GetState() == STATE_UPSET) { SetRegressionCount(0); return false; }

	// 初期位置回帰処理に失敗した場合、false を返す
	if (!CEnemyAttack::BackOriginPos(pPos, pRot, fHeight)) { return false; }

	// 独自状態にする
	SetState(STATE_ORIGIN);

	// true を返す
	return true;
}
