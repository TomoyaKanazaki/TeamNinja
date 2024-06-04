//============================================================
//
//	しつこい敵ヘッダー [enemyStalk.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_STALK_H_
#define _ENEMY_STALK_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemy.h"

//************************************************************
//	クラス定義
//************************************************************
// しつこい敵クラス
class CEnemyStalk : public CEnemy
{
public:

	// ターゲット
	enum ETarget
	{
		TARGET_PLAYER = 0,	// プレイヤーターゲット
		TARGET_CLONE,		// 分身ターゲット
		TARGET_MAX			// この列挙型の総数
	};

	// 状態
	enum EState
	{
		STATE_CRAWL = 0,	// 巡回状態
		STATE_STALK,		// 追跡状態
		STATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEnemyStalk();

	// デストラクタ
	~CEnemyStalk() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

private:

	// メンバ関数
	void State(void);		// 状態処理
	void Crawl(void);		// 巡回処理
	void Stalk(void);		// 追跡処理
	void Move(void);		// 移動処理
	bool Approach(void);	// 接近処理

	// メンバ変数
	D3DXVECTOR3 m_posTarget;	// 目標の位置
	ETarget m_target;			// 目標
	EState m_state;				// 状態
};

#endif	// _ENEMY_CHASE_H_
