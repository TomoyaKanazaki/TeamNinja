//============================================================
//
//	キューブの当たり判定処理 [collisionCube.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "collisionCube.h"
#include "collision.h"

#ifdef _DEBUG
#include "objectMeshCube.h"
#endif // _DEBUG

//************************************************************
// 定数定義
//************************************************************
namespace
{
	const int CUBE_PRIORITY = 5;			// キューブの優先順位
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// 色
}

//============================================================
// コンストラクタ
//============================================================
CCollisionCube::CCollisionCube() : CCollision(),
m_fWidth(0.0f),		// 幅
m_fHeight(0.0f),	// 高さ
m_fDepth(0.0f)		// 奥行
#ifdef _DEBUG
, m_pCube(nullptr)	// メッシュキューブ
#endif // _DEBUG

{

}

//============================================================
// デストラクタ
//============================================================
CCollisionCube::~CCollisionCube()
{

}

//============================================================
// 終了処理
//============================================================
void CCollisionCube::Uninit(void)
{
#ifdef _DEBUG

	// メッシュキューブの終了
	SAFE_UNINIT(m_pCube);

#endif // _DEBUG

	// 終了処理
	CCollision::Uninit();
}

//============================================================
// ヒット処理
//============================================================
bool CCollisionCube::Hit
(
	D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight,			// 高さ
	D3DXVECTOR3& rMove,				// 移動量
	bool& rJump						// ジャンプ状況
)
{
	// サイズ
	D3DXVECTOR3 centerSizeUp = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 centerSizeDown = D3DXVECTOR3(fRadius, 0.0f, fRadius);
	D3DXVECTOR3 targetSizeUp = D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth);
	D3DXVECTOR3 targetSizeDown = D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth);
	D3DXVECTOR3 posOld = rPosOld;
	D3DXVECTOR3 move = rMove;
	bool bJump = rJump;

	if (collision::ResponseBox3D(rPos, posOld, GetPos(), centerSizeUp, centerSizeDown, targetSizeUp, targetSizeDown, &move, &bJump))
	{ // 当たった場合

		// 移動量とジャンプ状況を設定する
		rMove = move;
		rJump = bJump;

		// true を返す
		return true;
	}

	// false を返す
	return false;
}

//============================================================
// 生成処理
//============================================================
CCollisionCube* CCollisionCube::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rOffset, const float fWidth, const float fHeight, const float fDepth)
{
	// 当たり判定の生成
	CCollisionCube* pColl = new CCollisionCube();

	// 生成出来ていない場合 nullptr を返す
	if (pColl == nullptr) { return nullptr; }

	// 位置を設定
	pColl->SetPos(rPos);

	// オフセットの設定
	pColl->SetOffset(rOffset);

	// 半径を設定
	pColl->m_fWidth = fWidth;

	// 高さを設定
	pColl->m_fHeight = fHeight;

	// 奥行を設定
	pColl->m_fDepth = fDepth;

#ifdef _DEBUG

	// キューブを生成
	pColl->m_pCube = CObjectMeshCube::Create
	(
		rPos,
		VEC3_ZERO,
		D3DXVECTOR3(pColl->m_fWidth, pColl->m_fHeight / 2, pColl->m_fDepth),
		COL,
		XCOL_BLACK,
		CObjectMeshCube::BORDER_OFF,
		0.0f,
		CObjectMeshCube::TEXSTATE_ONE,
		CObjectMeshCube::SFaceTex(NONE_IDX),
		CObjectMeshCube::ORIGIN_DOWN
	);
	pColl->m_pCube->SetPriority(CUBE_PRIORITY);

#endif // _DEBUG

	// 当たり判定を返す
	return pColl;
}
