//===================================
//
// 敵の攻撃状態ヘッダー[enemyStateAttack.h]
// Author 小原立暉
//
//===================================
#ifndef _ENEMY_STATE_ATTACK_H_
#define _ENEMY_STATE_ATTACK_H_

//***********************************
// インクルードファイル
//***********************************
#include "enemyStateNone.h"

// クラス定義(敵の攻撃状態クラス)
class CEnemyStateAttack : public CEnemyStateNone
{
public:

	CEnemyStateAttack(const CEnemy* pEnemy);	// コンストラクタ
	~CEnemyStateAttack();						// デストラクタ

	void Process(void) override;				// 状態処理

private:


};

#endif