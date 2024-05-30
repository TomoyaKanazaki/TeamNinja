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
#include "collision.h"

#ifdef _DEBUG
#include "objectMeshTube.h"
#endif // _DEBUG

//************************************************************
// 定数定義
//************************************************************
namespace
{
	const int TUBE_PRIORITY = 5;			// チューブの優先順位
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// 色
}

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
// ヒット処理
//============================================================
bool CCollisionCylinder::Hit
(
	D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight,			// 高さ
	D3DXVECTOR3& rMove,				// 移動量
	bool& rJump						// ジャンプ状況
)
{
	// 変数を宣言
	D3DXVECTOR3 posColl = GetPos();	// 位置
	float fLength = 0.0f;			// 判定位置と判定目標位置の間の距離

	// 判定位置と判定目標位置の距離を求める
	fLength = (rPos.x - posColl.x) * (rPos.x - posColl.x)
		+ (rPos.z - posColl.z) * (rPos.z - posColl.z);

	if (fLength < (fRadius + m_fRadius) * (fRadius + m_fRadius) &&
		rPos.y <= posColl.y + m_fHeight &&
		rPos.y + fHeight >= posColl.y)
	{ // 判定内の場合

		if (rPosOld.y >= posColl.y + m_fHeight &&
			rPos.y <= posColl.y + m_fHeight)
		{ // 上からの当たり判定

			// 位置を補正する
			rPos.y = posColl.y + m_fHeight;

			// 移動量を0にする
			rMove.y = 0.0f;

			// ジャンプしていない
			rJump = false;

			// 真を返す
			return true;
		}

		// 変数を宣言
		float fCenterRot = atan2f(rPos.x - posColl.x, rPos.z - posColl.z);	// 判定目標から見た判定向き

		// 位置を補正
		rPos.x = posColl.x + sinf(fCenterRot) * (fRadius + m_fRadius);
		rPos.z = posColl.z + cosf(fCenterRot) * (fRadius + m_fRadius);

		// 真を返す
		return true;
	}

	// 偽を返す
	return false;
}

//============================================================
// 生成処理
//============================================================
CCollisionCylinder* CCollisionCylinder::Create(const D3DXVECTOR3& rPos, const float fLength, const float fAngle, const float fRadius, const float fHeight)
{
	// 当たり判定の生成
	CCollisionCylinder* pColl = new CCollisionCylinder();

	// 生成出来ていない場合 nullptr を返す
	if (pColl == nullptr) { return nullptr; }

	// 位置
	D3DXVECTOR3 pos = VEC3_ZERO;

	// 位置を作成する
	pos.x = rPos.x + sinf(fAngle) * fLength;
	pos.y = rPos.y;
	pos.z = rPos.z + cosf(fAngle) * fLength;

	// 位置を設定
	pColl->SetPos(pos);

	// 半径を設定
	pColl->m_fRadius = fRadius;

	// 高さを設定
	pColl->m_fHeight = fHeight;

#ifdef _DEBUG

	// チューブを生成
	pColl->m_pTube = CObjectMeshTube::Create(pos, VEC3_ZERO, COL, POSGRID2(8, 2), POSGRID2(8, 2), fRadius, fHeight);
	pColl->m_pTube->SetPriority(TUBE_PRIORITY);

#endif // _DEBUG

	// 当たり判定を返す
	return pColl;
}
