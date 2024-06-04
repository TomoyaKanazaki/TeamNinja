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

	// 状態
	enum EState
	{
		STATE_PLAYER = 0,	// プレイヤー追跡状態
		STATE_CLONE,		// 分身追跡状態
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
	void TargetSelect(void);		// 標的選択処理
	bool Search(const D3DXVECTOR3& posTarget);		// 探索処理
	void Chase(const D3DXVECTOR3& posTarget);		// 追跡処理
	bool Approach(const D3DXVECTOR3& posTarget);	// 接近処理

	// メンバ変数
	EState m_state;			// 状態
};

#endif	// _ENEMY_CHASE_H_
