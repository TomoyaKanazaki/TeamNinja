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
		TARGET_PLAYER = 0,	// プレイヤー
		TARGET_CLONE,		// 分身
		TARGET_MAX			// この列挙型の総数
	};

	// モーション列挙
	enum EMotion
	{
		MOTION_IDOL = 0,	// 待機モーション
		MOTION_WALK,		// 歩行モーション
		MOTION_FOUND,		// 発見モーション
		MOTION_ATTACK,		// 攻撃モーション
		MOTION_UPSET,		// 動揺モーション
		MOTION_FALL,		// 落下モーション
		MOTION_LANDING,		// 着地モーション
		MOTION_MAX			// この列挙型の総数
	};

	// 状態
	enum EState
	{
		STATE_CRAWL = 0,	// 巡回状態
		STATE_STALK,		// 追跡状態
		STATE_ATTACK,		// 攻撃状態
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

	void SetData(void) override;	// 情報の設定処理

private:

	// オーバーライド関数
	int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot) override;	// 状態の更新処理
	void UpdateMotion(int nMotion, const float fDeltaTime) override;	// モーションの更新処理
	void UpdateLanding(D3DXVECTOR3* pPos) override;	// 着地更新

	// メンバ関数
	void Crawl(void);		// 巡回処理
	void Stalk(void);		// 追跡処理
	void Move(void);		// 移動処理
	bool Approach(void);	// 接近処理

	// メンバ変数
	D3DXVECTOR3 m_posTarget;	// 目標の位置
	ETarget m_target;			// 標的
	EState m_state;				// 状態
	float m_fSpeed;				// 速度
};

#endif	// _ENEMY_CHASE_H_
