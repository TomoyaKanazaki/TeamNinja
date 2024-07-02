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
class CObjectMeshCylinder;		// メッシュシリンダー

//************************************************************
//	クラス定義
//************************************************************
// 敵のナビゲーションクラス
class CEnemyNav
{
public:

	// 状態
	enum STATE
	{
		STATE_STOP = 0,		// 停止状態
		STATE_TURN,			// ターン状態
		STATE_WALK,			// 歩行状態
		STATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEnemyNav();

	// デストラクタ
	~CEnemyNav();

	// オーバーライド関数
	HRESULT Init(void);					// 初期化
	void Uninit(void);					// 終了
	void Update(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosOld, D3DXVECTOR3* pRot, D3DXVECTOR3* pRotDest, D3DXVECTOR3* pMove);		// 更新

	// 静的メンバ関数
	static CEnemyNav* Create(const D3DXVECTOR3& rPosInit);		// 生成処理

private:

	// メンバ関数
	void Stop(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosOld, D3DXVECTOR3* pRotDest);							// 停止状態処理
	void Turn(D3DXVECTOR3* pRot, D3DXVECTOR3* pRotDest, D3DXVECTOR3* pMove);		// ターン状態処理
	void Walk(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosOld, D3DXVECTOR3* pRot, D3DXVECTOR3* pMove);		// 歩行状態処理

	void CollisionActor(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosOld);	// アクターの当たり判定処理
	void CollisionWall(D3DXVECTOR3* pPos);	// 壁の当たり判定処理

	// メンバ変数
	CObjectMeshCylinder* m_pCylinder;		// メッシュシリンダーの情報
	D3DXVECTOR3 m_posInit;	// 初期位置
	D3DXVECTOR3 m_posDest;	// 目標地点
	STATE m_state;			// 状態
	int m_nStopCount;		// 停止カウント
	float m_fRotMove;		// 向きの移動量
};

#endif	// _ACTOR_H_
