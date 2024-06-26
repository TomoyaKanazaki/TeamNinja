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
#include "enemyAttack.h"

//************************************************************
// 前方宣言
//************************************************************
class CEnemyNav;		// 敵のナビゲーション

//************************************************************
//	クラス定義
//************************************************************
// しつこい敵クラス
class CEnemyStalk : public CEnemyAttack
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
		MOTION_FALL,		// 落下モーション
		MOTION_LANDING,		// 着地モーション
		MOTION_MAX			// この列挙型の総数
	};

	// 状態
	enum EState
	{
		STATE_CRAWL = 0,	// 巡回状態
		STATE_WARNING,		// 警告状態
		STATE_STALK,		// 追跡状態
		STATE_ATTACK,		// 攻撃状態
		STATE_UPSET,		// 動揺状態
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

	float GetRadius(void) const override;			// 半径の取得処理
	float GetHeight(void) const override;			// 高さの取得処理

private:

	// オーバーライド関数
	int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override;	// 状態の更新処理
	void UpdateMotion(int nMotion, const float fDeltaTime) override;	// モーションの更新処理
	void UpdateLanding(D3DXVECTOR3* pPos) override;	// 着地更新

	// メンバ関数
	EMotion Crawl(void);				// 巡回処理
	EMotion Warning(void);				// 警告処理
	EMotion Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot);	// 追跡処理
	EMotion Attack(const D3DXVECTOR3& rPos);				// 攻撃処理
	EMotion Upset(void);				// 動揺処理

	// メンバ変数
	CEnemyNav* m_pNav;			// ナビゲーション
	EState m_state;				// 状態
};

#endif	// _ENEMY_CHASE_H_
