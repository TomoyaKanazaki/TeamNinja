//============================================================
//
//	敵のナビゲーション(ランダム座標)ヘッダー [enemyNavRandom.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_NAV_RANDOM_H_
#define _ENEMY_NAV_RANDOM_H_

//************************************************************
// インクルードファイル
//************************************************************
#include "enemyNavigation.h"

//************************************************************
//	クラス定義
//************************************************************
// 敵のナビゲーション(ランダム座標)クラス
class CEnemyNavRandom : public CEnemyNav
{
public:

	// コンストラクタ
	CEnemyNavRandom();

	// デストラクタ
	~CEnemyNavRandom() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update						// 更新
	(
		D3DXVECTOR3* pPos,		// 位置
		D3DXVECTOR3* pRot,		// 向き
		D3DXVECTOR3* pMove,		// 移動量
		D3DXVECTOR3* pRotDest,	// 目的の向き
		const float fSpeed,		// 速度
		const float fDeltaTime	// デルタタイム
	) override;

	// 静的メンバ関数
	static CEnemyNavRandom* Create(const D3DXVECTOR3& rPosInit, const float fWidth, const float fDepth);		// 生成処理

private:

	// 純粋仮想メンバ関数
	void StopFunc		// 停止状態処理
	(
		const D3DXVECTOR3& rPos,		// 位置
		const D3DXVECTOR3& rRot,		// 向き
		D3DXVECTOR3* pRotDest			// 目的の向き
	) override;
	void TurnFunc		// ターン状態処理
	(
		D3DXVECTOR3* pRot,				// 向き
		D3DXVECTOR3* pMove,				// 移動量
		const D3DXVECTOR3& rRotDest,	// 目的の向き
		const float fSpeed,				// 速度
		const float fDeltaTime			// デルタタイム
	) override;
	void MoveFunc		// 移動状態処理
	(
		D3DXVECTOR3* pPos,				// 位置
		const D3DXVECTOR3& rMove		// 移動量
	) override;

	// メンバ関数
	void DestPosRandom(void);			// 位置のランダム処理
	bool PosCorrect(const float fDest, float* fTarget, const float fMove);	// 位置の補正処理

	// メンバ変数
	CObjectMeshCube* m_pRangeCube;	// 範囲のブロック
};

#endif	// _ENEMY_NAV_RANDOM_H_
