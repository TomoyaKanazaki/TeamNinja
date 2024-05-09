//============================================================
//
//	敵ヘッダー [enemy.h]
//	Author：小原立暉
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
//	クラス定義
//************************************************************
// 敵クラス
class CEnemy : public CObjectChara
{
public:

	// コンストラクタ
	explicit CEnemy();

	// デストラクタ
	virtual ~CEnemy();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画

	// 静的メンバ関数
	static CEnemy* Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);
	static CListManager<CEnemy>* GetList(void);			// リスト取得

	// メンバ関数
	inline void UpdateOldPosition(void)						{ m_oldPos = GetVec3Position(); }	// 過去位置更新
	inline D3DXVECTOR3 GetOldPosition(void) const			{ return m_oldPos; }				// 過去位置取得
	inline void SetMovePosition(const D3DXVECTOR3& rMove)	{ m_move = rMove; }					// 位置移動量設定
	inline D3DXVECTOR3 GetMovePosition(void) const			{ return m_move; }					// 位置移動量取得

protected:

private:

	// 静的メンバ変数
	static CListManager<CEnemy>* m_pList;			// オブジェクトリスト

	// メンバ変数
	CListManager<CEnemy>::AIterator m_iterator;	// イテレーター
	D3DXVECTOR3	m_oldPos;			// 過去位置
	D3DXVECTOR3	m_move;				// 移動量
};

#endif	// _ENEMY_H_
