//============================================================
//
//	円柱の当たり判定処理 [collisionCylinder.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "collisionCylinder.h"

#include "objectMeshTube.h"

//============================================================
// コンストラクタ
//============================================================
CCollisionCylinder::CCollisionCylinder() : CCollision(),
m_fRadius(0.0f),		// 半径
m_fHeight(0.0f)			// 高さ
#ifdef _DEBUG
,m_pTube(nullptr)		// メッシュチューブ
#endif // _DEBUG

{

}

//============================================================
// デストラクタ
//============================================================
CCollisionCylinder::~CCollisionCylinder()
{

}

//============================================================
// 終了処理
//============================================================
void CCollisionCylinder::Uninit(void)
{
#ifdef _DEBUG

	// メッシュチューブの終了
	SAFE_UNINIT(m_pTube);

#endif // _DEBUG


	// 終了処理
	CCollision::Uninit();
}

//============================================================
// 生成処理
//============================================================
CCollisionCylinder* CCollisionCylinder::Create(const D3DXVECTOR3& rOffset, const float fRadius, const float fHeight)
{
	// 当たり判定の生成
	CCollisionCylinder* pColl = new CCollisionCylinder();

	// 生成出来ていない場合 nullptr を返す
	if (pColl == nullptr) { return nullptr; }

	// オフセット位置を設定
	pColl->SetOffset(rOffset);

	// 半径を設定
	pColl->m_fRadius = fRadius;

	// 高さを設定
	pColl->m_fHeight = fHeight;

#ifdef _DEBUG

	// チューブを生成
	pColl->m_pTube = CObjectMeshTube::Create(rOffset, VEC3_ZERO, XCOL_RED, POSGRID2(8, 2), POSGRID2(8, 2), fRadius, fHeight);

#endif // _DEBUG

	// 当たり判定を返す
	return pColl;
}
