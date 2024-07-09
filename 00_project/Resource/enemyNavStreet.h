//============================================================
//
//	敵のナビゲーション(往来)ヘッダー [enemyNavStreet.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_NAV_STREET_H_
#define _ENEMY_NAV_STREET_H_

//************************************************************
// インクルードファイル
//************************************************************
#include "enemyNavigation.h"

//************************************************************
//	クラス定義
//************************************************************
// 敵のナビゲーション(往来)クラス
class CEnemyNavStreet : public CEnemyNav
{
public:

	// コンストラクタ
	CEnemyNavStreet();

	// デストラクタ
	~CEnemyNavStreet() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update						// 更新
	(
		D3DXVECTOR3* pPos,			// 位置
		D3DXVECTOR3* pRot,			// 向き
		CEnemy* pEnemy,				// 敵の情報
		const float fSpeed,			// 速度
		const float fDeltaTime		// デルタタイム
	) override;

	// 静的メンバ関数
	static CEnemyNavStreet* Create(const std::vector<D3DXVECTOR3>& rRoute);		// 生成処理

private:

	// メンバ関数
	void StopFunc			// 停止状態処理
	(
		const D3DXVECTOR3& rPos,	// 位置
		CEnemy* pEnemy				// 敵の情報
	) override;

	void TurnFunc			// ターン状態処理
	(
		D3DXVECTOR3* pRot,			// 向き
		CEnemy* pEnemy,				// 敵の情報
		const float fSpeed,			// 速度
		const float fDeltaTime		// デルタタイム
	) override;

	void MoveFunc			// 移動状態処理
	(
		D3DXVECTOR3* pPos,			// 位置
		CEnemy* pEnemy				// 敵の情報
	) override;

	bool PosCorrect(const float fDest, float* fTarget, const float fMove);	// 位置の補正処理

	// メンバ変数
	std::vector<D3DXVECTOR3> m_Route;	// 経路配列
	int m_nNumRoute;					// 現在向かうルートの番号
};

#endif	// _ENEMY_NAV_STREET_H_

