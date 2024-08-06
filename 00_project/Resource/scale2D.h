#if 0
//============================================================
//
//	拡縮オブジェクト2Dヘッダー [scale2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCALE2D_H_
#define _SCALE2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D.h"

//************************************************************
//	クラス定義
//************************************************************
// 拡縮オブジェクト2Dクラス
class CScale2D : public CObject2D
{
public:
	// 定数
	static constexpr float ADD_ROT = 2.0f;	// 向きの加算量
	static constexpr float LEVEL = 1.0f;	// フェードα値加減量

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない
		STATE_FADEOUT,	// フェードアウト
		STATE_SCALE,	// 拡縮
		STATE_FADEIN,	// フェードイン
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CScale2D();

	// デストラクタ
	~CScale2D() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CScale2D *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,		// 位置
		const D3DXVECTOR3& rOriginSize,	// 原点大きさ
		const float fMinScale = 0.0f,	// 最低拡大率
		const float fMaxScale = 1.0f,	// 最大拡大率
		const float fInitScale = 0.0f,	// 初期拡大率
		const float fCalcScale = ADD_ROT,		// 拡大率の加減量
		const float fSubIn = LEVEL,				// 拡縮前のインの拡大率減少量
		const float fAddOut = LEVEL,			// 拡縮後のアウトの拡大率増加量
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXCOLOR& rCol = XCOL_AWHITE		// 色
	);

	// メンバ関数
	void SetScale(const bool bScale);	// 拡縮設定
	bool IsScale(void) const { return (m_state == STATE_SCALE); }	// 拡縮取得

	void SetOriginSizing(const D3DXVECTOR3& rSize)	{ m_sizeOrigin = rSize; }	// 原点大きさ設定
	D3DXVECTOR3 GetOriginSizing(void) const			{ return m_sizeOrigin; }	// 原点大きさ取得
	void SetCalcScale(const float fCalc)	{ m_fAddSinRot = fCalc; }	// 拡縮向きの加算量設定
	float GetCalcScale(void) const			{ return m_fAddSinRot; }	// 拡縮向きの加算量取得
	void SetMinScale(const float fMin)		{ m_fMinScale = fMin; }		// 最低拡大率設定
	float GetMinScale(void) const			{ return m_fMinScale; }		// 最低拡大率取得
	void SetMaxScale(const float fMax)		{ m_fMaxScale = fMax; }		// 最大拡大率設定
	float GetMaxScale(void) const			{ return m_fMaxScale; }		// 最大拡大率取得
	void SetInitScale(const float fInit)	{ m_fInitScale = fInit; }	// 初期拡大率設定
	float GetInitScale(void) const			{ return m_fInitScale; }	// 初期拡大率取得
	void SetSubIn(const float fLevel)		{ m_fSubIn = fLevel; }		// α値減少量設定
	float GetSubIn(void) const				{ return m_fSubIn; }		// α値減少量取得
	void SetAddOut(const float fLevel)		{ m_fAddOut = fLevel; }		// α値増加量設定
	float GetAddOut(void) const				{ return m_fAddOut; }		// α値増加量取得

private:
	// メンバ変数
	D3DXVECTOR3 m_sizeOrigin;	// 原点大きさ
	EState m_state;		// 状態
	float m_fSinScale;	// 拡縮向き
	float m_fAddSinRot;	// 拡縮向きの加算量
	float m_fCurScale;	// 現在拡大率
	float m_fMinScale;	// 最低拡大率
	float m_fMaxScale;	// 最大拡大率
	float m_fInitScale;	// 初期拡大率
	float m_fSubIn;		// インの拡大率減少量
	float m_fAddOut;	// アウトの拡大率増加量
};

#endif	// _SCALE2D_H_
#endif
