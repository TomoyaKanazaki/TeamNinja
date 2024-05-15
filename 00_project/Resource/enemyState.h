//===================================
//
// 敵の状態(基盤)ヘッダー[enemyState.h]
// Author 小原立暉
//
//===================================
#ifndef _ENEMY_STATE_H_
#define _ENEMY_STATE_H_
//***********************************
// インクルードファイル
//***********************************
#include "main.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CEnemy;		// 敵

// クラス定義(敵の状態の親クラス)
class CEnemyState
{
public:

	explicit CEnemyState(CEnemy* pEnemy);	// コンストラクタ
	virtual ~CEnemyState();			// デストラクタ

	virtual void Uninit(void);			// 終了処理

	// メンバ関数
	virtual void Process(void) = 0;		// 状態処理
	CEnemy* GetEnemy(void) { return m_pEnemy; }		// 敵の情報の取得処理

private:

	// メンバ変数
	CEnemy* m_pEnemy;				// 本体の情報
};

#endif