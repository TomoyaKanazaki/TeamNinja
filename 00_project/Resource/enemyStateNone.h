//===================================
//
// 敵の通常状態ヘッダー[enemyStateNone.h]
// Author 小原立暉
//
//===================================
#ifndef _ENEMY_STATE_NONE_H_
#define _ENEMY_STATE_NONE_H_

//***********************************
// インクルードファイル
//***********************************
#include "enemyState.h"

// クラス定義(敵の通常状態クラス)
class CEnemyStateNone : public CEnemyState
{
public:

	CEnemyStateNone(CEnemy* pEnemy);	// コンストラクタ
	~CEnemyStateNone();						// デストラクタ

	void Process(void) override;			// 状態処理

private:

};

#endif