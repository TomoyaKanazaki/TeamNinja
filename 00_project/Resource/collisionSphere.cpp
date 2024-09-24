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

//************************************************************
// 定数定義
//************************************************************
namespace
{
	const int SPHERE_PRIORITY = 5;			// 球の優先順位
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// 色
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CCollisionSphere>* CCollisionSphere::m_pList = nullptr;	// オブジェクトリスト

//============================================================
// コンストラクタ
//============================================================
CCollisionSphere::CCollisionSphere() : CCollision(),
m_fRadius(0.0f),			// 半径
m_bList(false)				// リスト状況
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
// 初期化処理
//============================================================
void CCollisionSphere::Init(void)
{
	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CCollisionSphere>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);
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

	if (m_bList == true)
	{ // リスト入りしている場合

		// リストから自身のオブジェクトを削除
		m_pList->DelList(m_iterator);

		if (m_pList->GetNumAll() == 0)
		{ // オブジェクトが一つもない場合

			// リストマネージャーの破棄
			m_pList->Release(m_pList);
		}
	}

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
// オフセット設定処理
//============================================================
void CCollisionSphere::OffSet(const D3DXMATRIX& mtx)
{
	// オフセット処理
	CCollision::OffSet(mtx);

#ifdef _DEBUG

	// スフィアの位置を設定
	m_pSphere->SetVec3Position(GetPos());

#endif // _DEBUG
}

//============================================================
// 生成処理
//============================================================
CCollisionSphere* CCollisionSphere::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rOffset, const float fRadius, const bool bList)
{
	// 当たり判定の生成
	CCollisionSphere* pColl = new CCollisionSphere();

	// 生成出来ていない場合 nullptr を返す
	if (pColl == nullptr) { return nullptr; }

	// リスト状況を設定する
	pColl->m_bList = bList;

	if (bList == true)
	{ // リスト状況が true の場合

		// 初期化処理
		pColl->Init();
	}

	// 位置を設定する
	pColl->SetPos(rPos);

	// オフセットの設定
	pColl->SetOffset(rOffset);

	// 半径を設定
	pColl->m_fRadius = fRadius;

#ifdef _DEBUG

	// スフィアを生成
	pColl->m_pSphere = CObjectMeshSphere::Create(rPos, VEC3_ZERO, COL, POSGRID2(8, 2), POSGRID2(8, 2), fRadius);
	pColl->m_pSphere->SetPriority(SPHERE_PRIORITY);
	pColl->m_pSphere->SetLabel(CObject::LABEL_COLLISION);

#endif // _DEBUG

	// 当たり判定を返す
	return pColl;
}

//============================================================
// リスト取得
//============================================================
CListManager<CCollisionSphere>* CCollisionSphere::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}
