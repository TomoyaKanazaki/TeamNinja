//============================================================
//
//	勾玉マークUIヘッダー [goditem_mark.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GODITEM_MARK_H_
#define _GODITEM_MARK_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "anim2D.h"

//************************************************************
//	クラス定義
//************************************************************
// 神器マーククラス
class CGodItemMark : public CAnim2D
{
public:

	// 状態
	enum EState
	{
		STATE_NONE = 0,	// 初期状態
		STATE_SCALING,	// 拡縮状態
		STATE_STOP,		// 停止状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CGodItemMark(const int nPriority);

	// デストラクタ
	~CGodItemMark();

	// メンバ関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	void SetData					// 情報の設定処理
	(
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const int nIdx				// インデックス
	);

	// セット・ゲット関係
	void SetState(const EState state)	{ m_state = state; }	// 状態設定処理
	EState GetState(void) const			{ return m_state; }		// 状態取得処理

	// 静的メンバ関数
	static CGodItemMark* Create		// 生成
	(
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const int nIdx,				// インデックス
		const int nPriority			// 優先順位
	);

private:

	// メンバ関数
	void UpdateScaling(void);		// 拡縮処理

	// メンバ変数
	EState m_state;		// 状態
	float m_fScaling;	// 拡縮率
};

#endif	// _GODITEM_MARK_H_
