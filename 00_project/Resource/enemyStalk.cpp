//============================================================
//
//	追跡敵処理 [enemy_chase.cpp]
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

#include "player.h"
#include "player_clone.h"
#include "multiModel.h"
#include "enemy_item.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\enemy.txt";	// セットアップテキスト相対パス
	const float MOVE = -300.0f;								// 移動量
	const float ROT_REV = 0.5f;								// 向きの補正係数
	const float ATTACK_DISTANCE = 50.0f;					// 攻撃判定に入る距離

	const D3DXVECTOR3 OFFSET = D3DXVECTOR3(-3.0f, -15.0f, 0.0f);	// オフセット座標
}

//************************************************************
//	子クラス [CEnemyStalk] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyStalk::CEnemyStalk() : CEnemy(),
m_posTarget(VEC3_ZERO),		// 目標の位置
m_target(TARGET_PLAYER),	// 標的
m_state(STATE_CRAWL),		// 状態
m_fSpeed(0.0f)				// 速度
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
void CEnemyStalk::Uninit(void)
{
	// 敵の終了
	CEnemy::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyStalk::Update(const float fDeltaTime)
{
	// 過去位置更新
	UpdateOldPosition();

	// 状態処理
	State();

	// 敵の更新
	CEnemy::Update(fDeltaTime);

	if (GetItem() != nullptr)
	{ // アイテムを持っている場合

		// アイテムのオフセット処理
		GetItem()->Offset(GetParts(8)->GetMtxWorld());
	}
}

//============================================================
//	描画処理
//============================================================
void CEnemyStalk::Draw(CShader* pShader)
{
	// 敵の描画
	CEnemy::Draw(pShader);
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
		OFFSET,
		GetParts(8)->GetMtxWorld()
	));
}

//============================================================
// 状態処理
//============================================================
void CEnemyStalk::State(void)
{
	switch (m_state)
	{
	case CEnemyStalk::STATE_CRAWL:

		// 巡回処理
		Crawl();

		break;

	case CEnemyStalk::STATE_STALK:

		// 追跡処理
		Stalk();

		break;

	case CEnemyStalk::STATE_ATTACK:

		break;

	default:		// 例外処理
		assert(false);
		break;
	}
}

//============================================================
// 巡回処理
//============================================================
void CEnemyStalk::Crawl(void)
{
	if (SearchClone(&m_posTarget))
	{ // 分身が目に入った場合

		// 追跡状態にする
		m_state = STATE_STALK;

		// 標的を分身にする
		m_target = TARGET_CLONE;

		return;
	}

	if (SearchPlayer(&m_posTarget))
	{ // 分身が目に入った場合

		// 追跡状態にする
		m_state = STATE_STALK;

		// 標的をプレイヤーにする
		m_target = TARGET_PLAYER;

		return;
	}

	// 巡回状態にする
	m_state = STATE_CRAWL;
}

//============================================================
// 追跡処理
//============================================================
void CEnemyStalk::Stalk(void)
{
	// 巡回処理
	Crawl();

	// 移動処理
	Move();

	if (Approach())
	{ // 接近した場合

		// 攻撃状態にする
		m_state = STATE_ATTACK;

		switch (m_target)
		{
		case CEnemyStalk::TARGET_PLAYER:

			// ヒット処理
			CScene::GetPlayer()->Hit(1);

			break;

		case CEnemyStalk::TARGET_CLONE:

			// ヒット処理
			(*CPlayerClone::GetList()->GetBegin())->Hit(1);

			break;

		default:		// 例外処理
			assert(false);
			break;
		}
	}

	// デバッグ
	DebugProc::Print(DebugProc::POINT_RIGHT, "発見!!目的地：%f %f %f", m_posTarget.x, m_posTarget.y, m_posTarget.z);
}

//============================================================
// 移動処理
//============================================================
void CEnemyStalk::Move(void)
{
	D3DXVECTOR3 pos = GetVec3Position();		// 位置
	D3DXVECTOR3 destRot = GetDestRotation();	// 目的の向き
	D3DXVECTOR3 rot = GetVec3Rotation();		// 向き
	D3DXVECTOR3 move = GetMovePosition();		// 移動量
	float fDiff;

	// 目的の向きを取得
	destRot.y = atan2f(pos.x - m_posTarget.x, pos.z - m_posTarget.z);

	// 向きの差分
	fDiff = destRot.y - rot.y;

	// 向きの正規化
	useful::NormalizeRot(fDiff);

	// 向きを補正
	rot.y += fDiff * ROT_REV;

	// 向きの正規化
	useful::NormalizeRot(rot.y);

	// 移動量を設定する
	move.x = sinf(rot.y) * MOVE * GET_MANAGER->GetDeltaTime()->GetTime();
	move.z = cosf(rot.y) * MOVE * GET_MANAGER->GetDeltaTime()->GetTime();

	// 位置を移動する
	pos += move;

	// 情報を適用
	SetVec3Position(pos);
	SetDestRotation(destRot);
	SetVec3Rotation(rot);
	SetMovePosition(move);
}

//============================================================
// 接近処理
//============================================================
bool CEnemyStalk::Approach(void)
{
	float fDistance = 0.0f;					// 距離
	D3DXVECTOR3 pos = GetVec3Position();	// 位置

	// 距離を測る
	fDistance = sqrtf((pos.x - m_posTarget.x) * (pos.x - m_posTarget.x) + (pos.z - m_posTarget.z) * (pos.z - m_posTarget.z));

	if (fDistance <= ATTACK_DISTANCE)
	{ // 一定の距離に入った場合

		// 接近した
		return true;
	}

	// 接近してない
	return false;
}
