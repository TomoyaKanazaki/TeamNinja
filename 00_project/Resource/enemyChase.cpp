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
#include "enemyChase.h"
#include "renderer.h"
#include "deltaTime.h"

#include "player.h"
#include "player_clone.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\player.txt";	// セットアップテキスト相対パス
	const float MOVE = -420.0f;								// 移動量
	const float ROT_REV = 0.5f;								// 向きの補正係数
}

//************************************************************
//	子クラス [CEnemyChase] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyChase::CEnemyChase(const EType type) : CEnemy(type),
m_state(STATE_PLAYER)			// 状態
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyChase::~CEnemyChase()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyChase::Init(void)
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

	// マテリアルの設定処理
	SetAllMaterial(material::DamageRed());

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyChase::Uninit(void)
{
	// 敵の終了
	CEnemy::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyChase::Update(const float fDeltaTime)
{
	// 過去位置更新
	UpdateOldPosition();

	// 標的選択処理
	TargetSelect();

	// 敵の更新
	CEnemy::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CEnemyChase::Draw(CShader* pShader)
{
	// 敵の描画
	CEnemy::Draw(pShader);
}

//============================================================
// 標的選択処理
//============================================================
void CEnemyChase::TargetSelect(void)
{
	D3DXVECTOR3 posTarget = VEC3_ZERO;		// 標的の位置

	switch (m_state)
	{
	case CEnemyChase::STATE_PLAYER:

		// プレイヤーの位置を取得する
		posTarget = CScene::GetPlayer()->GetVec3Position();

		break;

	case CEnemyChase::STATE_CLONE:

		// 分身のリストが無い場合抜け出す
		if (CPlayerClone::GetList() == nullptr ||
			*CPlayerClone::GetList()->GetBegin() == nullptr) { return; }

		// 分身の位置を取得する
		posTarget = (*CPlayerClone::GetList()->GetBegin())->GetVec3Position();

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 追跡処理
	Chase(posTarget);
}

//============================================================
// 追跡処理
//============================================================
void CEnemyChase::Chase(const D3DXVECTOR3 posTarget)
{
	D3DXVECTOR3 pos = GetVec3Position();		// 位置
	D3DXVECTOR3 destRot = GetDestRotation();	// 目的の向き
	D3DXVECTOR3 rot = GetVec3Rotation();		// 向き
	D3DXVECTOR3 move = GetMovePosition();		// 移動量
	float fDiff;

	// 目的の向きを取得
	destRot.y = atan2f(pos.x - posTarget.x, pos.z - posTarget.z);

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