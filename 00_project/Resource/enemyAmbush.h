//============================================================
//
//	待ち伏せ敵ヘッダー [enemyAmbush.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_AMBUSH_H_
#define _ENEMY_AMBUSH_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	クラス定義
//************************************************************
// 待ち伏せ敵クラス
class CEnemyAmbush : public CEnemyAttack
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
		MOTION_STANDBY,		// 構えモーション
		MOTION_BATTLE,		// 分身に対する攻撃モーション
		MOTION_MAX			// この列挙型の総数
	};

	// 状態
	enum EState
	{
		STATE_AMBUSH = 0,	// 待ち伏せ状態
		STATE_WARNING,		// 警告状態
		STATE_STALK,		// 追跡状態
		STATE_ATTACK,		// 攻撃状態
		STATE_BLANKATTACK,	// 空白攻撃状態
		STATE_UPSET,		// 動揺状態
		STATE_STANCE,		// 構え状態
		STATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEnemyAmbush();

	// デストラクタ
	~CEnemyAmbush() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	void SetData(void) override;	// 情報の設定処理

	float GetRadius(void) const override;			// 半径の取得処理
	float GetHeight(void) const override;			// 高さの取得処理

	// 静的メンバ関数
	static CEnemyAmbush* Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rRot,				// 向き
		const float fChaseWidth,				// 追跡幅
		const float fChaseDepth					// 追跡奥行
	);

private:

	// オーバーライド関数
	int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override;	// 状態の更新処理
	void UpdateMotion(int nMotion, const float fDeltaTime) override;	// モーションの更新処理
	void UpdateLanding(D3DXVECTOR3* pPos) override;	// 着地更新

	// メンバ関数
	EMotion Ambush(D3DXVECTOR3* pPos, const float fDeltaTime);			// 待ち伏せ処理
	EMotion Warning(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);	// 警告処理
	EMotion Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);	// 追跡処理
	EMotion Attack(const D3DXVECTOR3& rPos);	// 攻撃処理
	EMotion BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime);		// 空白攻撃処理
	EMotion Upset(void);						// 動揺処理
	EMotion Stance(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot);				// 構え処理
	bool BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot);			// 元の位置に戻る処理
	void SetState(const EState state);			// 状態の設定処理
	void WalkSound(void);						// 歩行音処理

	// メンバ変数
	EState m_state;						// 状態
	int m_nStateCount;					// 状態カウント
	int m_nRegressionCount;				// 回帰カウント
};

#endif	// _ENEMY_CHASE_H_
