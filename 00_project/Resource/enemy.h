//============================================================
//
//	敵ヘッダー [enemy.h]
//	Author：小原立暉
//	Adder ：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectChara.h"
#include "motion.h"

//************************************************************
// 前方宣言
//************************************************************
class CEnemyItem;			// 敵の持ち物
class CPlayerClone;			// プレイヤーの分身

//************************************************************
// クラス定義
//************************************************************
// 敵クラス
class CEnemy : public CObjectChara
{
public:

	// コンストラクタ
	CEnemy();

	// デストラクタ
	virtual ~CEnemy();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画

	virtual void SetData(void) = 0;	// 情報の設定処理

	// 静的メンバ関数
	static CListManager<CEnemy>* GetList(void);			// リスト取得
	static void VanishAll(void);						// 全敵の消滅処理

	// メンバ関数
	inline void SetOldPosition(const D3DXVECTOR3& rPosOld)	{ m_oldPos = rPosOld; }				// 過去位置設定
	inline D3DXVECTOR3 GetOldPosition(void) const			{ return m_oldPos; }				// 過去位置取得
	inline D3DXVECTOR3 GetPosInit(void) const				{ return m_posInit; }				// 初期位置取得
	inline D3DXVECTOR3 GetRotInit(void) const				{ return m_rotInit; }				// 初期向き取得
	inline void SetDestRotation(D3DXVECTOR3& rRot)			{ useful::NormalizeRot(rRot); m_destRot = rRot; }	// 目的の向き設定
	inline D3DXVECTOR3 GetDestRotation(void) const			{ return m_destRot; }				// 目的の向き取得
	inline void SetMovePosition(const D3DXVECTOR3& rMove)	{ m_move = rMove; }					// 位置移動量設定
	inline D3DXVECTOR3 GetMovePosition(void) const			{ return m_move; }					// 位置移動量取得
	inline void SetItem(CEnemyItem* pItem)					{ m_pItem = pItem; }				// 敵の持ち物設定
	inline CEnemyItem* GetItem(void) const					{ return m_pItem; }					// 敵の持ち物取得
	virtual float GetRadius(void) const = 0;			// 半径の取得処理
	virtual float GetHeight(void) const = 0;			// 高さの取得処理

protected:

	// 純粋仮想関数
	virtual int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) = 0;	// 状態の更新処理
	virtual void UpdateMotion(int nMotion, const float fDeltaTime) = 0;	// モーションの更新処理
	virtual void UpdateLanding(D3DXVECTOR3* pPos) = 0;	// 着地更新

	// メンバ関数
	bool Collision(D3DXVECTOR3& rPos);		// 当たり判定処理
	bool SearchPlayer(D3DXVECTOR3* pPos = nullptr);	// プレイヤーの探索処理
	bool SearchClone(D3DXVECTOR3* pPos = nullptr, CPlayerClone** pClone = nullptr);	// 分身の探索処理

	void UpdateOldPosition(void) { m_oldPos = GetVec3Position(); }	// 過去位置更新
	void UpdateGravity(void);	// 重力更新

	inline void SetPosInit(const D3DXVECTOR3& rPosInit) { m_posInit = rPosInit; }	// 初期位置設定(生成時のみ設定)
	inline void SetRotInit(const D3DXVECTOR3& rRotInit) { m_rotInit = rRotInit; }	// 初期向き設定(生成時のみ設定)
	inline void SetEnableJump(const bool bJump)			{ m_bJump = bJump; }		// ジャンプ状況設定
	inline bool IsJump(void)							{ return m_bJump; }			// ジャンプ状況取得

private:

	// メンバ関数
	virtual void CollisionActor(D3DXVECTOR3& rPos, bool& bHit);		// アクターの当たり判定処理
	void CollisionPlayer(const D3DXVECTOR3& rPos, const float fRadius, const float fHeight);		// プレイヤーとの当たり判定処理

	// 静的メンバ変数
	static CListManager<CEnemy>* m_pList;		// オブジェクトリスト

	// メンバ変数
	CListManager<CEnemy>::AIterator m_iterator;	// イテレーター
	CEnemyItem* m_pItem;			// 持ち物の情報
	D3DXVECTOR3	m_oldPos;			// 過去位置
	D3DXVECTOR3 m_posInit;			// 初期位置
	D3DXVECTOR3 m_destRot;			// 目的の向き
	D3DXVECTOR3	m_move;				// 移動量
	D3DXVECTOR3 m_rotInit;			// 初期向き
	bool m_bJump;					// ジャンプ状況
	bool m_bVanish;					// 消滅状況
};

#endif	// _ENEMY_H_
