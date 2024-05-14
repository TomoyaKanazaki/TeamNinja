//===================================
//
// 敵の状態(基盤)ヘッダー[enemyStateNone.h]
// Author 小原立暉
//
//===================================
#ifndef _ENEMY_STATE_NONE_H_
#define _ENEMY_STATE_NONE_H_
//***********************************
// インクルードファイル
//***********************************
#include "main.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CEnemy;		// 敵

// クラス定義(敵の状態の親クラス)
class CEnemyStateNone
{
public:

	explicit CEnemyStateNone(const CEnemy* pEnemy);	// コンストラクタ
	virtual ~CEnemyStateNone();			// デストラクタ

	virtual void Uninit(void);			// 終了処理

	virtual void Process(void) = 0;		// 状態処理

private:

	// メンバ変数
	const CEnemy* m_pEnemy;				// 本体の情報
};

#endif