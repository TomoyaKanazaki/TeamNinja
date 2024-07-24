//============================================================
//
//	点滅オブジェクト2Dヘッダー [blink2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BLINK2D_H_
#define _BLINK2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D.h"

//************************************************************
//	クラス定義
//************************************************************
// 点滅オブジェクト2Dクラス
class CBlink2D : public CObject2D
{
public:
	// 定数
	static constexpr float LEVEL = 1.0f;	// フェードα値加減量

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない
		STATE_FADEOUT,	// フェードアウト
		STATE_DISP,		// 表示
		STATE_FADEIN,	// フェードイン
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CBlink2D();

	// デストラクタ
	~CBlink2D() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CBlink2D *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,		// 位置
		const D3DXVECTOR3& rSize,		// 大きさ
		const float fSubIn = LEVEL,		// インのα値減少量
		const float fAddOut = LEVEL,	// アウトのα値増加量
		const float fMaxWait = 0.0f,	// 余韻時間
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXCOLOR& rCol = XCOL_AWHITE		// 色
	);

	// メンバ関数
	void SetDisp(void);	// 表示設定
	bool IsDisp(void)	{ return (m_state == STATE_DISP); }	// 表示取得
	void SetSubIn(const float fLevel)		{ m_fSubIn = fLevel; }		// α値減少量設定
	float GetSubIn(void) const				{ return m_fSubIn; }		// α値減少量取得
	void SetAddOut(const float fLevel)		{ m_fAddOut = fLevel; }		// α値増加量設定
	float GetAddOut(void) const				{ return m_fAddOut; }		// α値増加量取得
	void SetMaxWait(const float fMaxWait)	{ m_fMaxWait = fMaxWait; }	// 余韻時間設定
	float GetMaxWait(void) const			{ return m_fMaxWait; }		// 余韻時間取得

private:
	// メンバ変数
	EState m_state;		// 状態
	float m_fSubIn;		// インのα値減少量
	float m_fAddOut;	// アウトのα値増加量
	float m_fWaitTime;	// 現在の余韻時間
	float m_fMaxWait;	// 余韻時間
};

#endif	// _BLINK2D_H_
