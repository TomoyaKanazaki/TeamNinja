//============================================================
//
//	球の当たり判定ヘッダー [collisionSphere.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COLLISION_SPHERE_H_
#define _COLLISION_SPHERE_H_

//***********************************
// インクルードファイル
//***********************************
#include "collisionModel.h"
#include "listManager.h"

#ifdef _DEBUG

#include "objectMeshSphere.h"

#endif // _DEBUG

// クラス定義(当たり判定クラス)
class CCollisionSphere : public CCollision
{
public:

	CCollisionSphere();			// コンストラクタ
	~CCollisionSphere();		// デストラクタ

	void Init(void);			// 初期化処理
	void Uninit(void) override;	// 終了処理

	bool Hit					// ヒット処理
	(
		D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rPosOld,		// 前回の位置
		const float fRadius,			// 半径
		const float fHeight,			// 高さ
		D3DXVECTOR3& rMove,				// 移動量
		bool& rJump						// ジャンプ状況
	) override;

	void OffSet(const D3DXMATRIX& mtx) override;		// オフセット設定処理

	// セット・ゲット関係
	void SetRadius(const float fRadius) { m_fRadius = fRadius; }	// 半径の設定処理
	float GetRadius(void) const { return m_fRadius; }				// 半径の取得処理

#ifdef _DEBUG

	CObjectMeshSphere* GetSphere(void) const { return m_pSphere; }	// メッシュスフィアの取得処理

#endif // _DEBUG

	// 静的メンバ関数
	static CCollisionSphere* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rOffset, const float fRadius, const bool bList);		// 生成処理
	static CListManager<CCollisionSphere>* GetList(void);	// リスト取得

private:

	// メンバ変数
	CListManager<CCollisionSphere>::AIterator m_iterator;	// イテレーター

	float m_fRadius;	// 半径
	bool m_bList;		// リスト状況

#ifdef _DEBUG

	CObjectMeshSphere* m_pSphere;	// メッシュスフィア

#endif // _DEBUG

	// 静的メンバ変数
	static CListManager<CCollisionSphere>* m_pList;			// オブジェクトリスト
};

#endif	// _COLLISION_SPHERE_H_
