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
// 前方宣言
//************************************************************
class CEnemyNav;		// 敵のナビゲーション

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
		STATE_BLANKATTACK,	// 空白攻撃状態
		STATE_UPSET,		// 動揺状態
		STATE_STANCE,		// 構え状態
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

	// 静的メンバ関数
	static CEnemyWolf* Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const float fMoveWidth,		// 移動幅
		const float fMoveDepth,		// 移動奥行
		const float fChaseWidth,	// 追跡幅
		const float fChaseDepth		// 追跡奥行
	);
	static CEnemyWolf* Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rRot,				// 向き
		const std::vector<D3DXVECTOR3> route,	// ルートの配列
		const float fChaseWidth,				// 追跡幅
		const float fChaseDepth					// 追跡奥行
	);

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
	int UpdateAttack(const D3DXVECTOR3& rPos);											// 攻撃状態時の更新
	int UpdateBlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime);					// 空白攻撃処理
	int UpdateUpset(void);		// 動揺状態時の更新
	int UpdateStance(void);		// 構え処理
	bool BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight) override;		// 元の位置に戻る処理
	void SetState(const EState state);			// 状態の設定処理
	void UpdatePosition(D3DXVECTOR3& rPos, const float fDeltaTime);	// 位置の更新

	// メンバ変数
	CEnemyNav* m_pNav;					// ナビゲーションの情報
	EState m_state;						// 状態
	int m_nStateCount;					// 状態カウント
	int m_nRegressionCount;				// 回帰カウント
};

#endif	// _ENEMY_CHASE_H_
