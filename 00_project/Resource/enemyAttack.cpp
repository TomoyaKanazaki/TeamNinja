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
#include "enemyNavigation.h"
#include "enemyNavStreet.h"
#include "enemyNavRandom.h"
#include "enemyChaseRange.h"

#include "enemyStalk.h"
#include "enemyWolf.h"
#include "enemyAmbush.h"
#include "effekseerControl.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 ATTACK_COLLUP = D3DXVECTOR3(30.0f, 100.0f, 30.0f);	// 攻撃判定(上)
	const D3DXVECTOR3 ATTACK_COLLDOWN = D3DXVECTOR3(30.0f, 0.0f, 30.0f);	// 攻撃判定(下)
	const int DODGE_COUNT = 17;					// 回避カウント数
	const float SHAKEOFF_RANGE = 1000.0f;		// 振り切れる距離
	const float DIVERSION_EFFECT_SCALE = 10.0f;	// 分身との戦闘エフェクトの大きさ
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CEnemyAttack>* CEnemyAttack::m_pList = nullptr;			// オブジェクトリスト

//************************************************************
//	子クラス [CEnemyAttack] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyAttack::CEnemyAttack() : CEnemy(),
m_pNav(nullptr),			// ナビゲーションの情報
m_pChaseRange(nullptr),		// 追跡範囲の情報
m_pClone(nullptr),			// 分身の情報
m_posTarget(VEC3_ZERO),		// 目標の位置
m_target(TARGET_NONE),		// 標的
m_nAttackCount(0),			// 攻撃カウント
m_type(TYPE_STALK),			// 種類
m_fAlpha(1.0f),				// 透明度
m_bDodge(false)				// 回避受付フラグ
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

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CEnemyAttack>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyAttack::Uninit(void)
{
	// ナビゲーションの終了処理
	SAFE_UNINIT(m_pNav);

	// 追跡範囲の終了処理
	SAFE_UNINIT(m_pChaseRange);

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

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

	if (m_bDodge)
	{
		SetAllMaterial(material::Blue());
	}
	else
	{
		ResetMaterial();
	}
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
//	リスト取得処理
//============================================================
CListManager<CEnemyAttack>* CEnemyAttack::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
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

//============================================================
// プレイヤーの振り切り処理
//============================================================
bool CEnemyAttack::ShakeOffPlayer(void)
{
	// 位置を取得する
	m_posTarget = CScene::GetPlayer()->GetVec3Position();

	if (collision::Circle2D(GetVec3Position(), m_posTarget, GetRadius(), SHAKEOFF_RANGE) == true)
	{ // 範囲内に入っている場合

		// プレイヤーを標的にする
		m_target = TARGET_PLAYER;

		// true を返す
		return true;
	}

	// false を返す
	return false;
}

//============================================================
// 分身の振り切り処理
//============================================================
bool CEnemyAttack::ShakeOffClone(void)
{
	D3DXVECTOR3 pos = VEC3_ZERO;					// 位置
	D3DXVECTOR3 posEnemy = GetVec3Position();		// 敵の位置

	if (CPlayerClone::GetList() == nullptr ||
		*CPlayerClone::GetList()->GetBegin() == nullptr)
	{ // 分身のリストが無い場合

		// falseを返す
		return false;
	}

	for (int nCnt = 0; nCnt < CPlayerClone::GetList()->GetNumAll(); nCnt++)
	{
		// 分身の位置を取得する
		pos = (*CPlayerClone::GetList()->GetIndex(nCnt))->GetVec3Position();

		if (!collision::Circle2D(GetVec3Position(), pos, GetRadius(), SHAKEOFF_RANGE)) { continue; }

		// 位置を設定する
		m_posTarget = (*CPlayerClone::GetList()->GetIndex(nCnt))->GetVec3Position();

		// 分身の情報を設定する
		m_pClone = *CPlayerClone::GetList()->GetIndex(nCnt);

		// 分身を標的にする
		m_target = TARGET_CLONE;

		// true を返す
		return true;
	}

	// false を返す
	return false;
}

//====================================================================================================================================================================================
// TODO：ここから下はう〇ちカス判定だから後で修正
//====================================================================================================================================================================================

//============================================================
// プレイヤーのヒット処理
//============================================================
void CEnemyAttack::HitPlayer(const D3DXVECTOR3& rPos)
{
	// 回避受付フラグが false の場合、抜ける
	if (m_bDodge == false) { return; }

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

	// 回避カウントを加算する
	m_nAttackCount++;

	if (m_nAttackCount >= DODGE_COUNT)
	{ // 回避カウントを過ぎた場合

		// ボックスの当たり判定
		if (collision::Box3D
		(
			rPos,				// 判定位置
			posPlayer,			// 判定目標位置
			ATTACK_COLLUP,		// 判定サイズ(右・上・後)
			ATTACK_COLLDOWN,	// 判定サイズ(左・下・前)
			sizeUpPlayer,		// 判定目標サイズ(右・上・後)
			sizeDownPlayer		// 判定目標サイズ(左・下・前)
		))
		{ // 当たってなかった場合

			// 回避カウントを初期化する
			m_nAttackCount = 0;

			// ヒット処理
			CScene::GetPlayer()->Hit(500);
		}

		// 回避受付フラグを false にする
		m_bDodge = false;
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
		m_pClone == nullptr)
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

	// 分身との戦闘エフェクトを出す
	GET_EFFECT->Create("data\\EFFEKSEER\\diversion.efkefc", GetVec3Position(), GetVec3Rotation(), VEC3_ZERO, DIVERSION_EFFECT_SCALE);

	// 回避受付フラグを false にする
	m_bDodge = false;
}

//============================================================
//	生成処理(一定範囲移動敵)
//============================================================
CEnemyAttack* CEnemyAttack::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const EType type,			// 種類
	const float fMoveWidth,		// 移動幅
	const float fMoveDepth,		// 移動奥行
	const float fChaseWidth,	// 追跡幅
	const float fChaseDepth		// 追跡奥行
)
{
	// ポインタを宣言
	CEnemyAttack* pEnemy = nullptr;	// 敵情報

	switch (type)
	{
	case TYPE_STALK:

		// 追跡敵を生成
		pEnemy = new CEnemyStalk;

		break;

	case TYPE_WOLF:

		// 犬敵を生成
		pEnemy = new CEnemyWolf;

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

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
		pEnemy->m_type = type;

		// 初期位置を設定
		pEnemy->SetPosInit(rPos);

		// 情報の設定処理
		pEnemy->SetData();

		// ナビゲーションを生成
		pEnemy->m_pNav = CEnemyNavRandom::Create(rPos, fMoveWidth, fMoveDepth);

		// 追跡範囲を生成
		pEnemy->m_pChaseRange = CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth);

		// 確保したアドレスを返す
		return pEnemy;
	}
}

//============================================================
//	生成処理(ルート巡回移動敵)
//============================================================
CEnemyAttack* CEnemyAttack::Create
(
	const D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rRot,				// 向き
	const EType type,						// 種類
	const std::vector<D3DXVECTOR3> route,	// ルートの配列
	const float fChaseWidth,				// 追跡幅
	const float fChaseDepth					// 追跡奥行
)
{
	// ポインタを宣言
	CEnemyAttack* pEnemy = nullptr;	// 敵情報

	switch (type)
	{
	case TYPE_STALK:

		// 追跡敵を生成
		pEnemy = new CEnemyStalk;

		break;

	case TYPE_WOLF:

		// 犬敵を生成
		pEnemy = new CEnemyWolf;

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

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
		pEnemy->m_type = type;

		// 初期位置を設定
		pEnemy->SetPosInit(rPos);

		// 情報の設定処理
		pEnemy->SetData();

		// ナビゲーションを生成
		pEnemy->m_pNav = CEnemyNavStreet::Create(route);

		// 追跡範囲を生成
		pEnemy->m_pChaseRange = CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth);

		// 確保したアドレスを返す
		return pEnemy;
	}
}

//===========================================
//  当たり判定の取得
//===========================================
D3DXVECTOR3 CEnemyAttack::GetAttackUp()
{
	return ATTACK_COLLUP;
}

//===========================================
//  当たり判定の取得
//===========================================
D3DXVECTOR3 CEnemyAttack::GetAttackDown()
{
	return ATTACK_COLLDOWN;
}
