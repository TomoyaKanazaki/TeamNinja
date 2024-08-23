//============================================================
//
//	敵の追跡範囲処理 [enemyChaseRange.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "enemyChaseRange.h"
#include "collision.h"

#ifdef _DEBUG

#include "objectMeshCube.h"

#endif // _DEBUG


//************************************************************
//	子クラス [CEnemyChaseRange] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyChaseRange::CEnemyChaseRange() :
	m_fWidth(0.0f),		// 幅
	m_fDepth(0.0f)		// 奥行
#ifdef _DEBUG
	, m_pRangeCube(nullptr)		// キューブの情報
#endif // _DEBUG

{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyChaseRange::~CEnemyChaseRange()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyChaseRange::Init(void)
{
	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyChaseRange::Uninit(void)
{
#ifdef _DEBUG

	// キューブの破棄処理
	SAFE_UNINIT(m_pRangeCube);

#endif // _DEBUG

	// 自身を破棄
	delete this;
}

//============================================================
//	更新処理
//============================================================
bool CEnemyChaseRange::ChaseRange(const D3DXVECTOR3& rInitPos, D3DXVECTOR3* pPos)
{
	// 範囲を超えたかどうか
	bool bOver = false;

	if (pPos->x >= rInitPos.x + m_fWidth)
	{ // 右端を超えた場合

		// 位置を補正する
		pPos->x = rInitPos.x + m_fWidth;

		// 範囲超えた
		bOver = true;
	}

	if (pPos->x <= rInitPos.x - m_fWidth)
	{ // 左端を超えた場合

		// 位置を補正する
		pPos->x = rInitPos.x - m_fWidth;

		// 範囲超えた
		bOver = true;
	}

	if (pPos->z >= rInitPos.z + m_fDepth)
	{ // 奥端を超えた場合

		// 位置を補正する
		pPos->z = rInitPos.z + m_fDepth;

		// 範囲超えた
		bOver = true;
	}

	if (pPos->z <= rInitPos.z - m_fDepth)
	{ // 手前端を超えた場合

		// 位置を補正する
		pPos->z = rInitPos.z - m_fDepth;

		// 範囲超えた
		bOver = true;
	}

	// 範囲状況を返す
	return bOver;
}

//============================================================
// ターゲットが範囲内にいるかの処理
//============================================================
bool CEnemyChaseRange::InsideTargetPos(const D3DXVECTOR3& rInitPos, const D3DXVECTOR3& rTargetPos)
{
	// 幅を設定
	D3DXVECTOR3 centerSize = D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth);

	// 当たり判定を返す
	return collision::Box2D(rInitPos, rTargetPos, centerSize, centerSize, VEC3_ZERO, VEC3_ZERO);
}

//============================================================
// 生成処理
//============================================================
CEnemyChaseRange* CEnemyChaseRange::Create(const D3DXVECTOR3& pos, const float fWidth, const float fDepth)
{
	// ナビゲーションの生成
	CEnemyChaseRange* pNav = new CEnemyChaseRange;

	if (pNav == nullptr)
	{ // 生成に失敗した場合

		// NULL を返す
		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// アクターの初期化
		if (FAILED(pNav->Init()))
		{ // 初期化に失敗した場合

			// アクターの破棄
			SAFE_DELETE(pNav);
			return nullptr;
		}

		// 情報を設定
		pNav->m_fWidth = fWidth;	// 幅
		pNav->m_fDepth = fDepth;	// 奥行

#if 1
#ifdef _DEBUG

		// キューブを生成
		pNav->m_pRangeCube = CObjectMeshCube::Create
		(
			pos,
			VEC3_ZERO,
			D3DXVECTOR3(fWidth, 10.0f, fDepth),
			XCOL_CYAN
		);

		// 当たり判定ラベルに変える
		pNav->m_pRangeCube->SetLabel(CObject::LABEL_COLLISION);

#endif // _DEBUG
#endif // 0

		// 確保したアドレスを返す
		return pNav;
	}
}