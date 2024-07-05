//============================================================
//
//	攻撃系敵ヘッダー [enemyAttack.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_ATTACK_H_
#define _ENEMY_ATTACK_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemy.h"

//************************************************************
// 前方宣言
//************************************************************
class CPlayerClone;		// 分身の情報

//************************************************************
//	クラス定義
//************************************************************
// 攻撃系敵クラス
class CEnemyAttack : public CEnemy
{
public:

	// ターゲット
	enum ETarget
	{
		TARGET_NONE = 0,	// 無対象
		TARGET_PLAYER,		// プレイヤー
		TARGET_CLONE,		// 分身
		TARGET_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CEnemyAttack();

	// デストラクタ
	~CEnemyAttack() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	void SetData(void) override;	// 情報の設定処理

	// セット・ゲット関係
	void SetClone(CPlayerClone* pClone) { m_pClone = pClone; }			// 分身の情報設定
	CPlayerClone* GetClone(void) const { return m_pClone; }				// 分身の情報取得
	void SetTargetPos(const D3DXVECTOR3& pos) { m_posTarget = pos; }	// 目標の位置設定
	D3DXVECTOR3 GetTargetPos(void) const { return m_posTarget; }		// 目標の位置取得
	void SetTarget(const ETarget target) { m_target = target; }			// 標的設定
	ETarget GetTarget(void) const { return m_target; }					// 標的取得
	int GetAttackCount(void) const { return m_nAttackCount; }			// 攻撃カウント取得
	void SetEnableAttack(const bool bAttack) { m_bAttack = bAttack; }	// 攻撃状況設定
	bool IsAttack(void) const { return m_bAttack; }						// 攻撃状況取得

	// メンバ関数
	void Move(D3DXVECTOR3* pPos, const D3DXVECTOR3& rRot, const float fSpeed, const float fDeltaTime);			// 移動処理
	void RotMove(D3DXVECTOR3& rRot, const float fRevRota, const float fDeltaTime);	// 向きの移動処理
	bool Approach(const D3DXVECTOR3& rPos, const float fDis = 50.0f);				// 接近処理
	bool JudgePlayer(void);		// プレイヤーの探索処理
	bool JudgeClone(void);		// 分身の探索処理

	// TODO：う〇ちカス判定だから後で修正
	void HitPlayer(const D3DXVECTOR3& rPos);		// プレイヤーのヒット処理
	void HitClone(const D3DXVECTOR3& rPos);			// 分身のヒット処理

private:

	// オーバーライド関数
	virtual int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override = 0;	// 状態の更新処理
	virtual void UpdateMotion(int nMotion, const float fDeltaTime) override = 0;	// モーションの更新処理

	// メンバ変数
	CPlayerClone* m_pClone;		// 分身の情報
	D3DXVECTOR3 m_posTarget;	// 目標の位置
	ETarget m_target;			// 標的
	int m_nAttackCount;			// 攻撃カウント
	bool m_bAttack;				// 攻撃状況TODO：後で変更
};

#endif	// _ENEMY_CHASE_H_
