//============================================================
//
//	モデルの当たり判定ヘッダー [collisionModel.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COLLISION_MODEL_H_
#define _COLLISION_MODEL_H_

// クラス定義(当たり判定クラス)
class CCollision
{
public:

	CCollision();		// コンストラクタ
	~CCollision();		// デストラクタ

	void Uninit(void);	// 終了処理

	// セット・ゲット関係
	void SetOffset(const D3DXVECTOR3& rOffset) { m_offset = rOffset; }	// オフセット座標の設定処理
	D3DXVECTOR3 GetOffset(void) const { return m_offset; }				// オフセット座標の取得処理

private:

	// メンバ変数
	D3DXVECTOR3 m_offset;	// オフセット座標
};

#endif	// _COLLISION_MODEL_H_
