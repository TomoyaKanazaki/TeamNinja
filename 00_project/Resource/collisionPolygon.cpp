//============================================================
//
//	ポリゴンの当たり判定処理 [collisionPolygon.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "collisionPolygon.h"

//************************************************************
// 定数定義
//************************************************************
namespace
{
	const int POLYGON_PRIORITY = 5;			// ポリゴンの優先順位
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// 色
}

//============================================================
// コンストラクタ
//============================================================
CCollisionPolygon::CCollisionPolygon() : CCollision(),
m_rot(VEC3_ZERO),		// 幅
m_size(VEC3_ZERO),		// 高さ
m_pPolygon(nullptr)		// 当たり判定ポリゴン
{

}

//============================================================
// デストラクタ
//============================================================
CCollisionPolygon::~CCollisionPolygon()
{

}

//============================================================
// 終了処理
//============================================================
void CCollisionPolygon::Uninit(void)
{
	// メッシュポリゴンの終了
	SAFE_UNINIT(m_pPolygon);

	// 終了処理
	CCollision::Uninit();
}

//============================================================
// ヒット処理
//============================================================
bool CCollisionPolygon::Hit
(
	D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight,			// 高さ
	D3DXVECTOR3& rMove,				// 移動量
	bool& rJump						// ジャンプ状況
)
{
	// 高さを取得する
	float fPosY = m_pPolygon->GetPositionHeight(rPos);

	if (fPosY > rPos.y &&
		fPosY < rPos.y + fHeight)
	{ // 現在地がポリゴンより高かった場合

		// Y軸の位置を適用する
		rPos.y = fPosY;

		// 重力を0にする
		rMove.y = 0.0f;

		// ジャンプ状況を false にする
		rJump = false;
	}

	// false を返す
	return false;
}

//============================================================
// オフセット設定処理
//============================================================
void CCollisionPolygon::OffSet(const D3DXMATRIX& mtx)
{
	// オフセット処理
	CCollision::OffSet(mtx);

	// ポリゴンの位置を設定
	m_pPolygon->SetVec3Position(GetPos());
}

//============================================================
// 生成処理
//============================================================
CCollisionPolygon* CCollisionPolygon::Create
(
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rOffset,		// オフセット
	const D3DXVECTOR3& rRot,		// 向き
	const D3DXVECTOR3& rSize		// サイズ
)
{
	// 当たり判定の生成
	CCollisionPolygon* pColl = new CCollisionPolygon();

	// 生成出来ていない場合 nullptr を返す
	if (pColl == nullptr) { return nullptr; }

	// 位置を設定
	pColl->SetPos(rPos);

	// オフセットの設定
	pColl->SetOffset(rOffset);

	// 向きを設定
	pColl->m_rot = rRot;

	// サイズを設定
	pColl->m_size = rSize;

	// ポリゴンを生成
	pColl->m_pPolygon = CObject3D::Create
	(
		rPos,
		pColl->m_size,
		pColl->m_rot,
		COL
	);
	pColl->m_pPolygon->SetPriority(POLYGON_PRIORITY);
	pColl->m_pPolygon->SetLabel(CObject::LABEL_COLLISION);

#ifndef _DEBUG

	// 描画を切る
	pColl->m_pPolygon->SetEnableDraw(false);

#endif // !_DEBUG

	// 当たり判定を返す
	return pColl;
}