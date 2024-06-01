//============================================================
//
//	キューブの当たり判定ヘッダー [collisionCube.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COLLISION_CUBE_H_
#define _COLLISION_CUBE_H_

//***********************************
// インクルードファイル
//***********************************
#include "collisionModel.h"

#ifdef _DEBUG

#include "objectMeshCube.h"

#endif // _DEBUG

// クラス定義(当たり判定クラス)
class CCollisionCube : public CCollision
{
public:

	CCollisionCube();			// コンストラクタ
	~CCollisionCube();			// デストラクタ

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

	// セット・ゲット関係
	float GetWidth(void) const { return m_fWidth; }		// 幅の取得処理
	float GetHeight(void) const { return m_fHeight; }	// 高さの取得処理
	float GetDepth(void) const { return m_fDepth; }		// 奥行の取得処理

#ifdef _DEBUG

	// セット・ゲット関係
	CObjectMeshCube* GetCube(void) const { return m_pCube; }	// キューブの取得処理

#endif // _DEBUG

	// 静的メンバ関数
	static CCollisionCube* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rOffset, const float fWidth, const float fHeight, const float fDepth);		// 生成処理

private:

	// メンバ変数
	float m_fWidth;		// 幅
	float m_fHeight;	// 高さ
	float m_fDepth;		// 奥行

#ifdef _DEBUG

	CObjectMeshCube* m_pCube;	// メッシュキューブ

#endif // _DEBUG
};

#endif	// _COLLISION_CUBE_H_
