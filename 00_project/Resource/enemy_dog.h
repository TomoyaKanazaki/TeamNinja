//============================================================
//
//	犬敵ヘッダー [enemyDog.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_DOG_H_
#define _ENEMY_DOG_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemy.h"

//************************************************************
//	クラス定義
//************************************************************
// 犬敵クラス
class CEnemyDog : public CEnemy
{
public:

	// モーション列挙
	enum EMotion
	{
		MOTION_IDOL = 0,	// 待機モーション
		MOTION_WALK,		// 歩行モーション
		MOTION_FOUND,		// 発見モーション
		MOTION_ATTACK,		// 攻撃モーション
		MOTION_UPSET,		// 動揺モーション
		MOTION_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEnemyDog();

	// デストラクタ
	~CEnemyDog() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	void SetData(void) override;	// 情報の設定処理

private:

};

#endif	// _ENEMY_CHASE_H_
