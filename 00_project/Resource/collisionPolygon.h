//============================================================
//
//	ポリゴンの当たり判定ヘッダー [collisionPolygon.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COLLISION_POLYGON_H_
#define _COLLISION_POLYGON_H_

//***********************************
// インクルードファイル
//***********************************
#include "collisionModel.h"
#include "object3D.h"

// クラス定義(当たり判定クラス)
class CCollisionPolygon : public CCollision
{
public:

	CCollisionPolygon();		// コンストラクタ
	~CCollisionPolygon();		// デストラクタ

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
	void SetRot(const D3DXVECTOR3& rRot) { m_rot = rRot; }		// 向きの設定処理
	D3DXVECTOR3 GetRot(void) const { return m_rot; }			// 向きの取得処理
	void SetSize(const D3DXVECTOR3& rSize) { m_size = rSize; }	// サイズの設定処理
	D3DXVECTOR3 GetSize(void) const { return m_size; }			// サイズの取得処理
	CObject3D* GetPolygon(void) const { return m_pPolygon; }	// 当たり判定ポリゴンの取得処理

	// 静的メンバ関数
	static CCollisionPolygon* Create			// 生成処理
	(
		const D3DXVECTOR3& rPos,		// 位置
		const D3DXVECTOR3& rOffset,		// オフセット
		const D3DXVECTOR3& rRot,		// 向き
		const D3DXVECTOR3& rSize		// サイズ
	);

private:

	// メンバ関数
	bool Convert(const float fWidth, const float fDepth, const float fRot);		// 向きによる変換処理

#ifdef _DEBUG

	bool RotWarning(const float fRot);		// 向きの警告処理

#endif

	// メンバ変数
	CObject3D* m_pPolygon;	// ポリゴン
	D3DXVECTOR3 m_rot;	// 向き
	D3DXVECTOR3 m_size;	// サイズ
};

#endif	// _COLLISION_POLYGON_H_
