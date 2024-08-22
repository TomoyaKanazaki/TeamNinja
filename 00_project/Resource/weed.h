//============================================================
//
//	草ヘッダー [weed.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _WEED_H_
#define _WEED_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object3D.h"

//************************************************************
//	クラス定義
//************************************************************
// 草クラス
class CWeed : public CObject3D
{
public:
	// コンストラクタ
	CWeed();

	// デストラクタ
	~CWeed() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CWeed *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);

	// メンバ関数
	void CollisionPlayer(void);	// プレイヤーとの当たり判定

private:
	// メンバ変数
	float m_fCurLength;		// 現在の距離
	float m_fDestLength;	// 目標の距離
	float m_fCurAngle;		// 現在の角度
	float m_fDestAngle;		// 目標の角度
};

#endif	// _WEED_H_
