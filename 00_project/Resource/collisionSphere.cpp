//============================================================
//
//	球の当たり判定処理 [collisionSphere.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "collisionSphere.h"
#include "collision.h"

#ifdef _DEBUG
#include "objectMeshSphere.h"
#endif // _DEBUG

//************************************************************
// 定数定義
//************************************************************
namespace
{
	const int SPHERE_PRIORITY = 5;			// 球の優先順位
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// 色
}

//============================================================
// コンストラクタ
//============================================================
CCollisionSphere::CCollisionSphere() : CCollision(),
m_fRadius(0.0f)			// 半径
#ifdef _DEBUG
, m_pSphere(nullptr)	// メッシュスフィア
#endif // _DEBUG

{

}

//============================================================
// デストラクタ
//============================================================
CCollisionSphere::~CCollisionSphere()
{

}

//============================================================
// 終了処理
//============================================================
void CCollisionSphere::Uninit(void)
{
#ifdef _DEBUG

	// メッシュスフィアの終了
	SAFE_UNINIT(m_pSphere);

#endif // _DEBUG

	// 終了処理
	CCollision::Uninit();
}

//============================================================
// ヒット処理
//============================================================
bool CCollisionSphere::Hit
(
	D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight,			// 高さ
	D3DXVECTOR3& rMove,				// 移動量
	bool& rJump						// ジャンプ状況
)
{
	// 当たり判定の結果を返す
	return collision::ResponseCircle3D(rPos, GetPos(), fRadius, m_fRadius);
}

//============================================================
// 生成処理
//============================================================
CCollisionSphere* CCollisionSphere::Create(const D3DXVECTOR3& rOffset, const float fRadius)
{
	// 当たり判定の生成
	CCollisionSphere* pColl = new CCollisionSphere();

	// 生成出来ていない場合 nullptr を返す
	if (pColl == nullptr) { return nullptr; }

	pColl->SetPos(rOffset);

	// オフセット位置を設定
	pColl->SetOffset(rOffset);

	// 半径を設定
	pColl->m_fRadius = fRadius;

#ifdef _DEBUG

	// スフィアを生成
	pColl->m_pSphere = CObjectMeshSphere::Create(rOffset, VEC3_ZERO, COL, POSGRID2(8, 2), POSGRID2(8, 2), fRadius);
	pColl->m_pSphere->SetPriority(SPHERE_PRIORITY);

#endif // _DEBUG

	// 当たり判定を返す
	return pColl;
}
