//============================================================
//
//	敵処理 [enemy.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "camera.h"

#include "player.h"
#include "player_clone.h"
#include "stage.h"
#include "actor.h"

#include "enemy_item.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY = 3;				// 敵の優先順位
	const float	GRAVITY = 60.0f;			// 重力
	const float VIEW_RANGE = 700.0f;		// 視界の範囲
	const float SUB_VANISH_ALPHA = 0.02f;	// 消滅時の透明度の減算量
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CEnemy>* CEnemy::m_pList = nullptr;			// オブジェクトリスト

//************************************************************
//	親クラス [CEnemy] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemy::CEnemy() : CObjectChara(CObject::LABEL_ENEMY, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
m_pItem(nullptr),			// 持ち物の情報
m_oldPos(VEC3_ZERO),		// 過去位置
m_posInit(VEC3_ZERO),		// 初期位置
m_destRot(VEC3_ZERO),		// 目的の向き
m_move(VEC3_ZERO),			// 移動量
m_rotInit(VEC3_ZERO),		// 初期向き
m_bJump(false),				// 着地状況
m_bVanish(false),			// 消滅状況
m_bHitToPlayer(false)		// プレイヤーとの衝突判定
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemy::~CEnemy()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemy::Init(void)
{
	// オブジェクトキャラクターの初期化
	if (FAILED(CObjectChara::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CEnemy>::Create();
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
void CEnemy::Uninit(void)
{
	if (m_pItem != nullptr)
	{ // 持ち物が NULL じゃない場合

		// 持ち物の終了処理
		m_pItem->Uninit();
		m_pItem = nullptr;
	}

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトキャラクターの終了
	CObjectChara::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemy::Update(const float fDeltaTime)
{
	// 透明度を取得
	float fAlpha = GetAlpha();

	if (m_bVanish)
	{ // 消滅状況が true の場合

		// 消滅時の透明度を減算する
		fAlpha -= SUB_VANISH_ALPHA;

		// 透明度を適用する
		SetAlpha(fAlpha);

		if (m_pItem != nullptr)
		{ // アイテムを持っている場合

			// アイテムのオフセット処理
			m_pItem->Update(fDeltaTime);

			// 透明度を設定する
			m_pItem->SetAlpha(fAlpha);
		}

		// 透明度が 0.0f 超過の場合、抜ける
		if (fAlpha > 0.0f) { return; }

		// 破棄処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	D3DXVECTOR3 posEnemy = GetVec3Position();	// 敵位置
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// 敵向き

	// 過去位置更新
	UpdateOldPosition();

	// 状態更新
	int nCurMotion = UpdateState(&posEnemy, &rotEnemy, fDeltaTime);	// 現在のモーションを取得

	SetVec3Position(posEnemy);	// 位置を反映
	SetVec3Rotation(rotEnemy);	// 向きを反映

	// 遠距離判定
	bool bFar = useful::IsNearPosR(GetVec3Position());
	if (!bFar) { return; }

	// モーション・オブジェクトキャラクター更新
	UpdateMotion(nCurMotion, fDeltaTime);

	if (m_pItem != nullptr)
	{ // アイテムを持っている場合

		// アイテムのオフセット処理
		m_pItem->Update(fDeltaTime);

		// 透明度を設定する
		m_pItem->SetAlpha(fAlpha);
	}
}

//============================================================
//	描画処理
//============================================================
void CEnemy::Draw(CShader* pShader)
{
	// オブジェクトキャラクターの描画
	CObjectChara::Draw(pShader);

	if (m_pItem != nullptr)
	{ // アイテムを持っている場合

		// アイテムのオフセット処理
		m_pItem->Draw(pShader);
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CEnemy>* CEnemy::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
// 全敵の消滅処理
//============================================================
void CEnemy::VanishAll(void)
{
	// 敵のリストが NULL だった場合、抜ける
	if (m_pList == nullptr) { return; }

	// 全敵の消滅状況を true にする
	for (auto& rEnemy : m_pList->GetList())
	{
		rEnemy->m_bVanish = true;
	}
}

//============================================================
// 当たり判定処理
//============================================================
bool CEnemy::Collision(D3DXVECTOR3& rPos)
{
	bool bHit = false;		// ヒット状況

	// アクターの当たり判定処理
	CollisionActor(rPos, bHit);

	// ヒット状況を返す
	return bHit;
}

//============================================================
// プレイヤーの探索処理
//============================================================
bool CEnemy::SearchPlayer(D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 pos = VEC3_ZERO;				// 位置
	float fRot = GetVec3Rotation().y + D3DX_PI;	// 向き

	// 向きを正規化
	useful::NormalizeRot(fRot);

	// プレイヤー情報の取得
	CPlayer* pPlayer = GET_PLAYER;

	// プレイヤーがいない場合抜ける
	if (pPlayer == nullptr) { return false; }

	// 位置を取得する
	pos = pPlayer->GetVec3Position();

	// 視界内に居なかった場合 false を返す
	if (!collision::Sector(GetVec3Position(), pos, fRot, VIEW_RANGE, D3DX_PI)) { return false; }

	// プレイヤーの位置を取得する
	if (pPos != nullptr) { *pPos = pos; }

	// true を返す
	return true;
}

//============================================================
// 分身の探索処理
//============================================================
bool CEnemy::SearchClone(D3DXVECTOR3* pPos, CPlayerClone** pClone)
{
	D3DXVECTOR3 pos = VEC3_ZERO;					// 位置
	D3DXVECTOR3 posEnemy = GetVec3Position();		// 敵の位置
	float fRot = GetVec3Rotation().y + D3DX_PI;		// 向き
	float fLength = FLT_MAX;						// 距離
	float fLengthComp = FLT_MAX;					// 比較する距離
	CPlayerClone* pChaseClone = nullptr;			// 追跡する分身

	// 向きを正規化
	useful::NormalizeRot(fRot);

	if (CPlayerClone::GetList() == nullptr ||
		*CPlayerClone::GetList()->GetBegin() == nullptr)
	{ // 分身のリストが無い場合

		// falseを返す
		return false;
	}

	for (auto& clone : CPlayerClone::GetList()->GetList())
	{
		// 歩行状態じゃない場合、次に進む
		if (clone->GetAction() != CPlayerClone::ACTION_MOVE) { continue; }

		// 分身の位置を取得する
		pos = clone->GetVec3Position();

		// 距離を測る
		fLengthComp = sqrtf((posEnemy.x - pos.x) * (posEnemy.x - pos.x) + (posEnemy.z - pos.z) * (posEnemy.z - pos.z));

		// 比較した距離の方が長い場合、次に進む
		if (fLength < fLengthComp) { continue; }

		// 距離を更新する
		fLength = fLengthComp;

		// 追跡する分身を設定
		pChaseClone = clone;
	}

	// 追跡する分身がいない場合、抜ける
	if (pChaseClone == nullptr) { return false; }

	// 位置を設定する
	pos = pChaseClone->GetVec3Position();

	// 視界内に居なかった場合 false を返す
	if (!collision::Sector(GetVec3Position(), pos, fRot, VIEW_RANGE, D3DX_PI)) { return false; }

	// プレイヤーの位置を取得する
	if (pPos != nullptr) { *pPos = pos; }

	// 分身の情報を取得する
	if (pClone != nullptr) { *pClone = pChaseClone; }

	// true を返す
	return true;
}

//============================================================
// 重力処理
//============================================================
void CEnemy::UpdateGravity(void)
{
	// 重力を加算する
	m_move.y -= GRAVITY;
}

//============================================================
// 着地処理
//============================================================
void CEnemy::UpdateLanding(D3DXVECTOR3* pPos)
{
	CStage *pStage = GET_STAGE;	// ステージ情報

	// ジャンプしている状態にする
	m_bJump = true;

	// 地面・制限位置の着地判定
	if (pStage->LandFieldPosition(*pPos, m_oldPos, m_move)
	||  pStage->LandLimitPosition(*pPos, m_move, 0.0f))
	{ // 着地していた場合

		// ジャンプしていない状態にする
		m_bJump = false;
	}
}

//============================================================
// アクターの当たり判定処理
//============================================================
void CEnemy::CollisionActor(D3DXVECTOR3& rPos, bool& bHit)
{
	// アクターのリスト構造が無ければ抜ける
	if (CActor::GetList() == nullptr) { return; }

	// リストを取得
	std::list<CActor*> list = CActor::GetList()->GetList();

	for (auto actor : list)
	{
		// 当たり判定処理
		actor->Collision
		(
			rPos,			// 位置
			m_oldPos,		// 前回の位置
			GetRadius(),	// 半径
			GetHeight(),	// 高さ
			m_move,			// 移動量
			m_bJump,		// ジャンプ状況
			bHit,			// ヒット状況
			false			// 消去状態
		);
	}
}

//============================================================
// プレイヤーとの当たり判定処理
//============================================================
void CEnemy::CollisionToPlayer(D3DXVECTOR3& rPos, const float fRadius, const float fHeight)
{
	D3DXVECTOR3 pos = GetVec3Position();
	float fHeightEnemy = GetHeight();

	// 高さがあっていない場合、抜ける
	if (pos.y + fHeightEnemy < rPos.y || pos.y > rPos.y + fHeight) { m_bHitToPlayer = false; return; }

	// 敵に当たっていない場合、抜ける
	if (!collision::CirclePillar(rPos, pos, fRadius, GetRadius())) { m_bHitToPlayer = false; return; }

	// プレイヤーとの衝突判定を true にする
	m_bHitToPlayer = true;
}
