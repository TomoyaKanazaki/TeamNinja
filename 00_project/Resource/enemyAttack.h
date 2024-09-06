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
class CEnemyChaseRange;	// 敵の追跡範囲
class CActor;			// アクター

//************************************************************
//	クラス定義
//************************************************************
// 攻撃系敵クラス
class CEnemyAttack : public CEnemy
{
public:

	// 種類
	enum EType
	{
		TYPE_STALK = 0,		// しつこい敵
		TYPE_WOLF,			// 狼敵
		TYPE_AMBUSH,		// 待ち伏せ敵
		TYPE_MAX			// この列挙型の総数
	};

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

	// 静的メンバ関数
	static CListManager<CEnemyAttack>* GetList(void);			// リスト取得
	static HRESULT LoadSetup(const char* pPass);		// セットアップ

	// セット・ゲット関係
	void SetChaseRange(CEnemyChaseRange* pChace)	{ assert(m_pChaseRange == nullptr); m_pChaseRange = pChace; }		// 追跡範囲の情報
	CEnemyChaseRange* GetChaseRange(void) const		{ return m_pChaseRange; }			// 追跡範囲の情報
	void SetType(const EType type)					{ m_type = type; }					// 種類設定
	EType GetType(void) const						{ return m_type; }					// 種類取得
	void SetClone(CPlayerClone* pClone)				{ m_pClone = pClone; }				// 分身の情報設定
	CPlayerClone* GetClone(void) const				{ return m_pClone; }				// 分身の情報取得
	void SetTargetPos(const D3DXVECTOR3& pos)		{ m_posTarget = pos; }				// 目標の位置設定
	D3DXVECTOR3 GetTargetPos(void) const			{ return m_posTarget; }				// 目標の位置取得
	void SetTarget(const ETarget target)			{ m_target = target; }				// 標的設定
	ETarget GetTarget(void) const					{ return m_target; }				// 標的取得
	void SetAttackCount(const int nCount)			{ m_nAttackCount = nCount; }		// 攻撃カウント設定
	int GetAttackCount(void) const					{ return m_nAttackCount; }			// 攻撃カウント取得
	void SetRegressionCount(const int nCount)		{ m_nRegressionCount = nCount; }	// 回帰カウント設定
	int GetRegressionCount(void) const				{ return m_nRegressionCount; }		// 回帰カウント取得
	void SetEnableDodge(const bool bAttack)			{ m_bDodge = bAttack; }				// 回避受付フラグ設定
	bool IsDodge(void) const						{ return m_bDodge; }				// 回避受付フラグ取得

	// メンバ関数
	void Move(D3DXVECTOR3* pPos, const D3DXVECTOR3& rRot, const float fSpeed, const float fDeltaTime);			// 移動処理
	void RotMove(D3DXVECTOR3& rRot, const float fRevRota, const float fDeltaTime);	// 向きの移動処理
	void LookTarget(const D3DXVECTOR3& rPos);	// 目標位置の視認
	bool Approach(const D3DXVECTOR3& rPos, const float fDis = 50.0f);				// 接近処理
	bool JudgePlayer(void);		// プレイヤーの探索処理
	bool JudgeClone(void);		// 分身の探索処理
	bool ShakeOffPlayer(void);	// プレイヤーの振り切り処理
	bool ShakeOffClone(void);	// 分身の振り切り処理

	// TODO：う〇ちカス判定だから後で修正
	bool HitPlayer(const D3DXVECTOR3& rPos);		// プレイヤーのヒット処理
	bool HitClone(const D3DXVECTOR3& rPos);			// 分身のヒット処理

	// 静的メンバ関数
	static D3DXVECTOR3 GetAttackUp();	// 当たり判定の取得
	static D3DXVECTOR3 GetAttackDown();	// 当たり判定の取得

protected:

	// オーバーライド関数
	virtual bool BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight);		// 元の位置に戻る処理

private:

	// オーバーライド関数
	virtual int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override = 0;	// 状態の更新処理
	virtual void UpdateMotion(int nMotion, const float fDeltaTime) override = 0;	// モーションの更新処理
	void CollisionActor(D3DXVECTOR3& rPos, bool& bHit) override;					// アクターの当たり判定処理

	// メンバ関数
	void ActorRegist(void);		// アクターの登録処理

	// 静的メンバ変数
	static CListManager<CEnemyAttack>* m_pList;		// オブジェクトリスト

	// メンバ変数
	CListManager<CEnemyAttack>::AIterator m_iterator;	// イテレーター
	CEnemyChaseRange* m_pChaseRange;	// 追跡範囲の情報
	std::vector<CActor*> m_actor;		// 当たり判定を取るアクターの情報
	CPlayerClone* m_pClone;		// 分身の情報
	D3DXVECTOR3 m_posTarget;	// 目標の位置
	ETarget m_target;			// 標的
	EType m_type;				// 種類
	int m_nAttackCount;			// 攻撃カウント
	int m_nRegressionCount;		// 回帰カウント
	bool m_bDodge;				// 回避受付フラグ
};

#endif	// _ENEMY_CHASE_H_
