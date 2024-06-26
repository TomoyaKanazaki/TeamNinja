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

	// 種類
	enum EType
	{
		TYPE_STALK = 0,		// しつこい敵
		TYPE_CAVEAT,		// 警告敵
		TYPE_WOLF,			// 狼敵
		TYPE_MAX			// この列挙型の総数
	};

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
	static CEnemy* Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const EType type			// 種類
	);
	static CListManager<CEnemy>* GetList(void);			// リスト取得

	// メンバ関数
	inline D3DXVECTOR3 GetOldPosition(void) const			{ return m_oldPos; }				// 過去位置取得
	inline void SetDestRotation(const D3DXVECTOR3& rRot)	{ m_destRot = rRot; }				// 目的の向き設定
	inline D3DXVECTOR3 GetDestRotation(void) const			{ return m_destRot; }				// 目的の向き取得
	inline void SetMovePosition(const D3DXVECTOR3& rMove)	{ m_move = rMove; }					// 位置移動量設定
	inline D3DXVECTOR3 GetMovePosition(void) const			{ return m_move; }					// 位置移動量取得
	inline void SetItem(CEnemyItem* pItem)					{ m_pItem = pItem; }				// 敵の持ち物設定
	inline CEnemyItem* GetItem(void) const					{ return m_pItem; }					// 敵の持ち物取得

protected:

	// 純粋仮想関数
	virtual int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) = 0;	// 状態の更新処理
	virtual void UpdateMotion(int nMotion, const float fDeltaTime) = 0;	// モーションの更新処理
	virtual void UpdateLanding(D3DXVECTOR3* pPos) = 0;	// 着地更新

	// メンバ関数
	bool SearchPlayer(D3DXVECTOR3* pPos = nullptr);	// プレイヤーの探索処理
	bool SearchClone(D3DXVECTOR3* pPos = nullptr, CPlayerClone** pClone = nullptr);	// 分身の探索処理

	void UpdateOldPosition(void) { m_oldPos = GetVec3Position(); }	// 過去位置更新
	void UpdateGravity(void);	// 重力更新

	bool IsJump(void) { return m_bJump; }	// ジャンプ状況取得

private:

	// 静的メンバ変数
	static CListManager<CEnemy>* m_pList;		// オブジェクトリスト

	// メンバ変数
	CListManager<CEnemy>::AIterator m_iterator;	// イテレーター
	CEnemyItem* m_pItem;			// 持ち物の情報
	D3DXVECTOR3	m_oldPos;			// 過去位置
	D3DXVECTOR3 m_destRot;			// 目的の向き
	D3DXVECTOR3	m_move;				// 移動量
	EType m_type;					// 種類
	bool m_bJump;					// ジャンプ状況
};

#endif	// _ENEMY_H_
