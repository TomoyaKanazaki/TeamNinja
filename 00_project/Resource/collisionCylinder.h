//============================================================
//
//	円柱の当たり判定ヘッダー [collisionCylinder.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COLLISION_CYLINDER_H_
#define _COLLISION_CYLINDER_H_

//***********************************
// インクルードファイル
//***********************************
#include "collisionModel.h"

#ifdef _DEBUG

//-----------------------------------
// 前方宣言
//-----------------------------------
class CObjectMeshTube;		// メッシュチューブ

#endif // _DEBUG

// クラス定義(当たり判定クラス)
class CCollisionCylinder : public CCollision
{
public:

	CCollisionCylinder();		// コンストラクタ
	~CCollisionCylinder();		// デストラクタ

	void Uninit(void);			// 終了処理

	// 静的メンバ関数
	static CCollisionCylinder* Create(const D3DXVECTOR3& rOffset, const float fRadius, const float fHeight);		// 生成処理

private:

	// メンバ変数
	float m_fRadius;		// 半径
	float m_fHeight;		// 高さ

#ifdef _DEBUG

	CObjectMeshTube* m_pTube;	// メッシュチューブ

#endif // _DEBUG

	// 
};

#endif	// _COLLISION_H_
