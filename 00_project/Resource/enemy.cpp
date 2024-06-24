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

#include "player.h"
#include "player_clone.h"
#include "stage.h"

#include "enemy_item.h"
#include "enemyStalk.h"
#include "enemyCaveat.h"
#include "enemyWolf.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY = 3;			// 敵の優先順位
	const float	GRAVITY = 60.0f;		// 重力
	const float VIEW_RANGE = 400.0f;	// 視界の範囲
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
CEnemy::CEnemy() : CObjectChara(CObject::LABEL_ENEMY, CObject::DIM_3D, PRIORITY),
m_pItem(nullptr),			// 持ち物の情報
m_oldPos(VEC3_ZERO),		// 過去位置
m_destRot(VEC3_ZERO),		// 目的の向き
m_move(VEC3_ZERO),			// 移動量
m_type(TYPE_STALK),			// 種類
m_bJump(false)				// 着地状況
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
	D3DXVECTOR3 posEnemy = GetVec3Position();	// 敵位置
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// 敵向き

	// 過去位置更新
	UpdateOldPosition();

	// 状態更新
	int nCurMotion = UpdateState(&posEnemy, &rotEnemy, fDeltaTime);	// 現在のモーションを取得

	SetVec3Position(posEnemy);	// 位置を反映
	SetVec3Rotation(rotEnemy);	// 向きを反映

	// モーション・オブジェクトキャラクター更新
	UpdateMotion(nCurMotion, fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CEnemy::Draw(CShader* pShader)
{
	CToonShader	*pToonShader = CToonShader::GetInstance();	// トゥーンシェーダー情報
	if (pToonShader->IsEffectOK())
	{ // エフェクトが使用可能な場合

		// オブジェクトキャラクターの描画
		CObjectChara::Draw(pToonShader);
	}
	else
	{ // エフェクトが使用不可能な場合

		// エフェクトエラー
		assert(false);

		// オブジェクトキャラクターの描画
		CObjectChara::Draw(pShader);
	}
}

//============================================================
//	生成処理
//============================================================
CEnemy* CEnemy::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const EType type)
{
	// ポインタを宣言
	CEnemy* pEnemy = nullptr;	// 敵情報

	switch (type)
	{
	case TYPE_STALK:

		// 追跡敵を生成
		pEnemy = new CEnemyStalk;

		break;

	case TYPE_CAVEAT:

		// 警告敵を生成
		pEnemy = new CEnemyCaveat;

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

		// 情報の設定処理
		pEnemy->SetData();

		// 位置を設定
		pEnemy->SetVec3Position(rPos);

		// 向きを設定
		pEnemy->SetVec3Rotation(rRot);

		// 種類を設定
		pEnemy->m_type = type;

		// 確保したアドレスを返す
		return pEnemy;
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
// プレイヤーの探索処理
//============================================================
bool CEnemy::SearchPlayer(D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 pos = VEC3_ZERO;				// 位置
	float fRot = GetVec3Rotation().y + D3DX_PI;	// 向き

	// 向きを正規化
	useful::NormalizeRot(fRot);

	// 位置を取得する
	pos = CScene::GetPlayer()->GetVec3Position();

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
bool CEnemy::SearchClone(D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 pos = VEC3_ZERO;					// 位置
	D3DXVECTOR3 posEnemy = GetVec3Position();		// 敵の位置
	float fRot = GetVec3Rotation().y + D3DX_PI;		// 向き
	float fLength = FLT_MAX;						// 距離
	float fLengthComp = FLT_MAX;					// 比較する距離
	int nIdx = INT_MAX;								// 追わせるインデックス

	// 向きを正規化
	useful::NormalizeRot(fRot);

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

		// 距離を測る
		fLengthComp = sqrtf((posEnemy.x - pos.x) * (posEnemy.x - pos.x) + (posEnemy.z - pos.z) * (posEnemy.z - pos.z));

		// 比較した距離の方が長い場合、次に進む
		if (fLength < fLengthComp) { continue; }

		// 距離を更新する
		fLength = fLengthComp;

		// インデックスを設定する
		nIdx = nCnt;
	}

	// 位置を設定する
	pos = (*CPlayerClone::GetList()->GetIndex(nIdx))->GetVec3Position();

	// 視界内に居なかった場合 false を返す
	if (!collision::Sector(GetVec3Position(), pos, fRot, VIEW_RANGE, D3DX_PI)) { return false; }

	// プレイヤーの位置を取得する
	if (pPos != nullptr) { *pPos = pos; }

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
	CStage *pStage = CScene::GetStage();	// ステージ情報

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
