//============================================================
//
//	警告敵ヘッダー [enemyCaveat.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_CAVEAT_H_
#define _ENEMY_CAVEAT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemy.h"

//************************************************************
//	クラス定義
//************************************************************
// 警告敵クラス
class CEnemyCaveat : public CEnemy
{
public:

	// 状態
	enum EState
	{
		STATE_CRAWL = 0,	// 巡回状態
		STATE_CAVEAT,		// 警告状態
		STATE_MAX			// この列挙型の総数
	};

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
	CEnemyCaveat();

	// デストラクタ
	~CEnemyCaveat() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

private:

	// メンバ関数
	void State(void);		// 状態処理
	void Caveat(void);		// 警告処理

	// メンバ変数
	EState m_state;			// 状態
};

#endif	// _ENEMY_CHASE_H_
