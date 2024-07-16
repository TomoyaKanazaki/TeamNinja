//============================================================
//
//	狼敵ヘッダー [enemyWolf.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_WOLF_H_
#define _ENEMY_WOLF_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	クラス定義
//************************************************************
// 狼敵クラス
class CEnemyWolf : public CEnemyAttack
{
public:

	// 状態列挙
	enum EState
	{
		STATE_CRAWL = 0,	// 巡回状態
		STATE_CAVEAT,		// 警告状態
		STATE_FOUND,		// 追跡状態
		STATE_ATTACK,		// 攻撃状態
		STATE_UPSET,		// 動揺状態
		STATE_FADEOUT,		// フェードアウト状態
		STATE_FADEIN,		// フェードイン状態
		STATE_MAX			// この列挙型の総数
	};

	// モーション列挙
	enum EMotion
	{
		MOTION_IDOL = 0,	// 待機モーション
		MOTION_RUN,			// 走行モーション
		MOTION_FOUND,		// 発見モーション
		MOTION_BITE,		// 噛みつきモーション
		MOTION_TURN,		// 振り向きモーション
		MOTION_FALL,		// 落下モーション
		MOTION_LANDING,		// 着地モーション
		MOTION_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEnemyWolf();

	// デストラクタ
	~CEnemyWolf() override;

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
	void NavMoitonSet(int* pMotion);				// ナビゲーションによるモーションの設定処理

	// メンバ関数
	int UpdateCrawl(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);		// 巡回状態時の更新
	int UpdateCaveat(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);		// 警告状態時の更新
	int UpdateFound(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);		// 追跡状態時の更新
	int UpdateAttack(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);		// 攻撃状態時の更新
	int UpdateUpset(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);		// 動揺状態時の更新
	int UpdateFadeOut(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);	// フェードアウト状態時の更新
	int UpdateFadeIn(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);		// フェードイン状態時の更新
	void UpdatePosition(D3DXVECTOR3& rPos, const float fDeltaTime);	// 位置の更新
	void UpdateRotation(D3DXVECTOR3& rRot, const float fDeltaTime);	// 向きの更新

	// メンバ変数
	EState m_state;						// 状態
};

#endif	// _ENEMY_CHASE_H_
