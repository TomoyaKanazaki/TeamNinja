//============================================================
//
//	攻撃系敵処理 [enemyAttack.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "enemyAttack.h"
#include "renderer.h"
#include "deltaTime.h"

#include "player.h"
#include "player_clone.h"
#include "multiModel.h"

#include "collision.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 ATTACK_COLLUP = D3DXVECTOR3(30.0f, 100.0f, 30.0f);	// 攻撃判定(上)
	const D3DXVECTOR3 ATTACK_COLLDOWN = D3DXVECTOR3(30.0f, 0.0f, 30.0f);	// 攻撃判定(下)
	const int DODGE_COUNT = 17;				// 回避カウント数
}

//************************************************************
//	子クラス [CEnemyAttack] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyAttack::CEnemyAttack() : CEnemy(),
m_pClone(nullptr),			// 分身の情報
m_posTarget(VEC3_ZERO),		// 目標の位置
m_target(TARGET_NONE),		// 標的
m_nAttackCount(0),			// 攻撃カウント
m_bAttack(false)			// 攻撃状況
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyAttack::~CEnemyAttack()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyAttack::Init(void)
{
	// 敵の初期化
	if (FAILED(CEnemy::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyAttack::Uninit(void)
{
	// 敵の終了
	CEnemy::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyAttack::Update(const float fDeltaTime)
{
	// 敵の更新
	CEnemy::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CEnemyAttack::Draw(CShader* pShader)
{
	// 敵の描画
	CEnemy::Draw(pShader);
}

//============================================================
// 情報の設定処理
//============================================================
void CEnemyAttack::SetData(void)
{

}

//============================================================
// 移動処理
//============================================================
void CEnemyAttack::Move(D3DXVECTOR3* pPos, const D3DXVECTOR3& rRot, const float fSpeed, const float fDeltaTime)
{
	D3DXVECTOR3 move = GetMovePosition();		// 移動量

	// 移動量を設定する
	move.x = sinf(rRot.y) * fSpeed * fDeltaTime;
	move.z = cosf(rRot.y) * fSpeed * fDeltaTime;

	// 位置を移動する
	*pPos += move;

	// 情報を適用
	SetMovePosition(move);
}

//============================================================
// 向きの移動処理
//============================================================
void CEnemyAttack::RotMove(D3DXVECTOR3& rRot, const float fRevRota, const float fDeltaTime)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// 目標向き
	float fDiffRot = 0.0f;	// 差分向き

	// 目標向きまでの差分を計算
	fDiffRot = destRot.y - rRot.y;
	useful::NormalizeRot(fDiffRot);	// 差分向きの正規化

	// 向きの更新
	rRot.y += fDiffRot * fDeltaTime * fRevRota;
	useful::NormalizeRot(rRot.y);	// 向きの正規化
}

//============================================================
//	目標位置の視認処理
//============================================================
void CEnemyAttack::LookTarget(const D3DXVECTOR3& rPos)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// 目標向き

	// 目標向きを求める
	destRot.y = atan2f(rPos.x - GetTargetPos().x, rPos.z - GetTargetPos().z);

	SetDestRotation(destRot);	// 目標向きを反映
}

//============================================================
// 接近処理
//============================================================
bool CEnemyAttack::Approach(const D3DXVECTOR3& rPos, const float fDis)
{
	float fDistance = 0.0f;					// 距離

	// 距離を測る
	fDistance = sqrtf((rPos.x - m_posTarget.x) * (rPos.x - m_posTarget.x) + (rPos.z - m_posTarget.z) * (rPos.z - m_posTarget.z));

	if (fDistance <= fDis)
	{ // 一定の距離に入った場合

		// 接近した
		return true;
	}

	// 接近してない
	return false;
}

//============================================================
// プレイヤーの探索処理
//============================================================
bool CEnemyAttack::JudgePlayer(void)
{
	// プレイヤーが見つからなかった場合 false を返す
	if (!SearchPlayer(&m_posTarget)) { return false; }

	// 標的をプレイヤーにする
	m_target = TARGET_PLAYER;

	// true を返す
	return true;
}

//============================================================
// 分身の探索処理
//============================================================
bool CEnemyAttack::JudgeClone(void)
{
	// 分身が見つからなかった場合 false を返す
	if (!SearchClone(&m_posTarget, &m_pClone)) { return false; }

	// 標的を分身にする
	m_target = TARGET_CLONE;

	// true を返す
	return true;
}

//====================================================================================================================================================================================
// TODO：ここから下はう〇ちカス判定だから後で修正
//====================================================================================================================================================================================

//============================================================
// プレイヤーのヒット処理
//============================================================
void CEnemyAttack::HitPlayer(const D3DXVECTOR3& rPos)
{
	// 攻撃判定が true の場合抜ける
	if (m_bAttack == true) { return; }

	// ヒット処理
	D3DXVECTOR3 posPlayer = CScene::GetPlayer()->GetVec3Position();
	D3DXVECTOR3 sizeUpPlayer =				// プレイヤーの判定(右・上・後)
	{
		CScene::GetPlayer()->GetRadius(),
		CScene::GetPlayer()->GetHeight(),
		CScene::GetPlayer()->GetRadius()
	};
	D3DXVECTOR3 sizeDownPlayer =			// プレイヤーの判定(左・下・前)
	{
		CScene::GetPlayer()->GetRadius(),
		0.0f,
		CScene::GetPlayer()->GetRadius()
	};

	// ボックスの当たり判定
	if (!collision::Box3D
	(
		rPos,				// 判定位置
		posPlayer,			// 判定目標位置
		ATTACK_COLLUP,		// 判定サイズ(右・上・後)
		ATTACK_COLLDOWN,	// 判定サイズ(左・下・前)
		sizeUpPlayer,		// 判定目標サイズ(右・上・後)
		sizeDownPlayer		// 判定目標サイズ(左・下・前)
	))
	{ // 当たってなかった場合

		return;
	}

	if (m_nAttackCount <= DODGE_COUNT)
	{ // 回避カウント中

		// 青色に変えておく
		SetAllMaterial(material::Blue());

		// 回避カウントを加算する
		m_nAttackCount++;
	}
	else
	{ // 上記以外

		// 回避カウントを初期化する
		m_nAttackCount = 0;

		// マテリアルをリセット
		ResetMaterial();

		// ヒット処理
		CScene::GetPlayer()->Hit(500);

		// 攻撃状況を true にする
		m_bAttack = true;
	}
}

//============================================================
// 分身のヒット処理
//============================================================
void CEnemyAttack::HitClone(const D3DXVECTOR3& rPos)
{
	// 分身の情報が存在しない場合抜ける
	if (CPlayerClone::GetList() == nullptr ||
		*CPlayerClone::GetList()->GetBegin() == nullptr ||
		m_pClone == nullptr ||
		m_bAttack == true)
	{
		return;
	}

	CPlayerClone* pClone = nullptr;	// 分身の情報

	for (auto& rClone : CPlayerClone::GetList()->GetList())
	{
		// 分身の情報じゃ無かった場合次へ
		if (m_pClone != rClone) { continue; }

		// 分身の情報を設定
		pClone = rClone;

		// for文を抜ける
		break;
	}

	// 分身が NULL の場合抜ける
	if (pClone == nullptr) { return; }

	// ヒット処理
	D3DXVECTOR3 sizeUpClone =
	{
		pClone->GetRadius(),
		pClone->GetHeight(),
		pClone->GetRadius()
	};
	D3DXVECTOR3 sizeDownClone =
	{
		pClone->GetRadius(),
		0.0f,
		pClone->GetRadius()
	};

	// ボックスの当たり判定
	if (!collision::Box3D
	(
		rPos,						// 判定位置
		pClone->GetVec3Position(),	// 判定目標位置
		ATTACK_COLLUP,				// 判定サイズ(右・上・後)
		ATTACK_COLLDOWN,			// 判定サイズ(左・下・前)
		sizeUpClone,				// 判定目標サイズ(右・上・後)
		sizeDownClone				// 判定目標サイズ(左・下・前)
	))
	{ // 当たってなかったら抜ける

		return;
	}

	// ヒット処理
	pClone->Hit(20);

	// 攻撃状況を true にする
	m_bAttack = true;
}