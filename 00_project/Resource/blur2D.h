//============================================================
//
//	ブラー2Dヘッダー [blur2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BLUR2D_H_
#define _BLUR2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CAnim2D;	// アニメーション2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// ブラー2Dクラス
class CBlur2D : public CObject
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_NORMAL,	// 通常状態
		STATE_VANISH,	// 消失状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CBlur2D();

	// デストラクタ
	~CBlur2D();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CBlur2D *Create	// 生成
	( // 引数
		CAnim2D *pParent,			// 親オブジェクト
		const D3DXCOLOR& rCol,		// ブラー反映色
		const float fStartAlpha,	// ブラー開始透明度
		const int nMaxLength,		// 保持オブジェクト最大数
		const int nTextureID		// テクスチャインデックス
	);

	// メンバ関数
	void SetState(const EState state);			// 状態設定
	EState GetState(void) const					{ return m_state; }	// 状態取得
	void SetColor(const D3DXCOLOR& rCol)		{ m_col = rCol; }	// ブラー反映色設定
	D3DXCOLOR GetColor(void) const				{ return m_col; }	// ブラー反映色取得
	void SetStartAlpha(const float fStartAlpha)	{ m_fStartAlpha = fStartAlpha; }	// ブラーの開始透明度設定
	float GetStartAlpha(void) const				{ return m_fStartAlpha; }			// ブラーの開始透明度取得
	void SetMaxLength(const int nMaxLength)		{ m_nMaxLength = nMaxLength; }		// 保持オブジェクト数設定
	int GetMaxLength(void) const				{ return m_nMaxLength; }			// 保持オブジェクト数取得

private:
	// オーバーライド関数
	void Release(void) override { CObject::Release(); }	// 破棄

	// メンバ変数
	std::vector<CAnim2D*> m_oldObject;	// 過去オブジェクト情報
	CAnim2D *m_pParent;			// 親オブジェクト
	D3DXCOLOR m_col;			// ブラー反映色
	float	m_fStartAlpha;		// ブラーの開始透明度
	int		m_nTextureID;		// ブラーテクスチャインデックス
	int		m_nMaxLength;		// 保持する親オブジェクトの最大数
	EState	m_state;			// 状態
	int		m_nCounterState;	// 状態管理カウンター

};

#endif	// _BLUR2D_H_
