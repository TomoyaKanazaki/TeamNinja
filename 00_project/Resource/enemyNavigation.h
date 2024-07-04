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
class CObjectMeshCube;		// メッシュキューブ

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
	~CEnemyNav();

	// オーバーライド関数
	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了
	void Update				// 更新
	(
		D3DXVECTOR3* pPos,		// 位置
		D3DXVECTOR3* pRot,		// 向き
		D3DXVECTOR3* pMove,		// 移動量
		D3DXVECTOR3* pRotDest,	// 目的の向き
		const float fRadius,	// 半径
		const float fHeight,	// 高さ
		const float fSpeed,		// 速度
		const float fDeltaTime	// デルタタイム
	);

	// 静的メンバ関数
	static CEnemyNav* Create(const D3DXVECTOR3& rPosInit, const float fWidth, const float fDepth);		// 生成処理

private:

	// メンバ関数
	void StopFunc(D3DXVECTOR3* pRotDest);		// 停止状態処理
	void TurnFunc					// ターン状態処理
	(
		D3DXVECTOR3* pRot,				// 向き
		D3DXVECTOR3* pMove,				// 移動量
		const D3DXVECTOR3& rRotDest,	// 目的の向き
		const float fSpeed,				// 速度
		const float fDeltaTime			// デルタタイム
	);
	void MoveFunc(const D3DXVECTOR3& rPos);		// 移動状態処理

	// メンバ変数
	CObjectMeshCube* m_pRange;		// 範囲
	D3DXVECTOR3 m_posInit;			// 初期位置
	D3DXVECTOR3 m_posDest;			// 目標位置
	EState m_state;					// 状態
	int m_nStopCount;				// 停止カウント
};

#endif	// _ACTOR_H_
