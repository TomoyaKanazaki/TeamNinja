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

#ifdef _DEBUG

//-----------------------------------
// 前方宣言
//-----------------------------------
class CObjectMeshSphere;		// メッシュスフィア

#endif // _DEBUG

// クラス定義(当たり判定クラス)
class CCollisionSphere : public CCollision
{
public:

	CCollisionSphere();			// コンストラクタ
	~CCollisionSphere();		// デストラクタ

	void Uninit(void);			// 終了処理

	bool Hit					// ヒット処理
	(
		D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rPosOld,		// 前回の位置
		const float fRadius,			// 半径
		const float fHeight,			// 高さ
		D3DXVECTOR3& rMove,				// 移動量
		bool& rJump						// ジャンプ状況
	) override;

	// 静的メンバ関数
	static CCollisionSphere* Create(const D3DXVECTOR3& rPos, const float fLength, const float fAngle, const float fRadius);		// 生成処理

private:

	// メンバ変数
	float m_fRadius;	// 半径

#ifdef _DEBUG

	CObjectMeshSphere* m_pSphere;	// メッシュスフィア

#endif // _DEBUG
};

#endif	// _COLLISION_SPHERE_H_
