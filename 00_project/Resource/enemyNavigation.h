//============================================================
//
//	敵のナビゲーションヘッダー [enemyNavigation.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_NAVIGATION_H_
#define _ENEMY_NAVIGATION_H_

//************************************************************
// 前方宣言
//************************************************************
class CEnemy;				// 敵の情報

//************************************************************
//	クラス定義
//************************************************************
// 敵のナビゲーションクラス
class CEnemyNav
{
public:

	// 状態
	enum EState
	{
		STATE_STOP = 0,		// 停止状態
		STATE_TURN,			// ターン状態
		STATE_MOVE,			// 移動状態
		STATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEnemyNav();

	// デストラクタ
	virtual ~CEnemyNav();

	// オーバーライド関数
	virtual HRESULT Init(void);		// 初期化
	virtual void Uninit(void);		// 終了
	virtual void Update				// 更新
	(
		D3DXVECTOR3* pPos,			// 位置
		D3DXVECTOR3* pRot,			// 向き
		CEnemy* pEnemy,				// 敵の情報
		const float fSpeed,			// 速度
		const float fDeltaTime		// デルタタイム
	);

	virtual void NavReset(void);	// ナビゲーションのリセット処理

	// セット・ゲット関係
	void SetState(const EState state) { m_state = state; }				// 状態の設定処理
	EState GetState(void) const { return m_state; }						// 状態の取得処理
	void SetStateCount(const int nCount) { m_nStateCount = nCount; }	// 状態カウントの設定処理

protected:

	// セット・ゲット関係
	int GetStateCount(void) const { return m_nStateCount; }					// 状態カウントの取得処理
	void SetPosDest(const D3DXVECTOR3& rPosDest) { m_posDest = rPosDest; }	// 目標位置の設定処理
	D3DXVECTOR3 GetPosDest(void) const { return m_posDest; }				// 目標位置の取得処理

private:

	// 純粋仮想メンバ関数
	virtual void StopFunc	// 停止状態処理
	(
		const D3DXVECTOR3& rPos,	// 位置
		CEnemy* pEnemy				// 敵の情報
	) = 0;

	virtual void TurnFunc	// ターン状態処理
	(
		D3DXVECTOR3* pRot,			// 向き
		CEnemy* pEnemy,				// 敵の情報
		const float fSpeed,			// 速度
		const float fDeltaTime		// デルタタイム
	) = 0;

	virtual void MoveFunc	// 移動状態処理
	(
		D3DXVECTOR3* pPos,			// 位置
		CEnemy* pEnemy				// 敵の情報
	) = 0;

	// メンバ変数
	D3DXVECTOR3 m_posDest;			// 目標位置
	EState m_state;					// 状態
	int m_nStateCount;				// 状態カウント
};

#endif	// _ENEMY_NAVIGATION_H_
