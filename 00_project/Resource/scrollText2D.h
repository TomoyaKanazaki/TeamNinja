//============================================================
//
//	文字送りテキスト2Dヘッダー [scrollText2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCROLL_TEXT2D_H_
#define _SCROLL_TEXT2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "text2D.h"
#include "sound.h"

//************************************************************
//	クラス定義
//************************************************************
// 文字送りテキスト2Dクラス
class CScrollText2D : public CText2D
{
public:
	// コンストラクタ
	CScrollText2D();

	// デストラクタ
	~CScrollText2D() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;			// 更新
	void Draw(CShader *pShader = nullptr) override;			// 描画
	void SetEnableDraw(const bool bDraw) override;			// 描画状況設定
	HRESULT AddString(const std::wstring& rStr) override;	// 文字列追加
	void DeleteString(const int nStrID) override;			// 文字列削除
	void DeleteStringAll(void) override;					// 文字列全削除

	// 静的メンバ関数
	static CScrollText2D *Create	// 生成
	( // 引数
		const std::string &rFilePass,		// フォントパス
		const bool bItalic,					// イタリック
		const D3DXVECTOR3& rPos,			// 原点位置
		const float fNextTime = 0.1f,		// 文字表示の待機時間
		const float fCharHeight = 100.0f,	// 文字縦幅
		const float fLineHeight = 100.0f,	// 行間縦幅
		const CString2D::EAlignX alignX = CString2D::XALIGN_CENTER,	// 横配置
		const EAlignY alignY = YALIGN_CENTER,						// 縦配置
		const D3DXVECTOR3& rRot = VEC3_ZERO,						// 原点向き
		const D3DXCOLOR& rCol = XCOL_WHITE							// 色
	);

	// メンバ関数
	void SetNextTime(const float fNextTime)			{ m_fNextTime = fNextTime; }	// 文字表示の待機時間設定
	float GetNextTime(void) const					{ return m_fNextTime; }			// 文字表示の待機時間取得
	void SetEnableScroll(const bool bScroll)		{ m_bScroll = bScroll; }		// 文字送り状況設定
	bool IsScroll(void) const						{ return m_bScroll; }			// 文字送り状況取得
	void SetScrollSE(const CSound::ELabel labelSE)	{ m_labelSE = labelSE; }		// 文字送り時の効果音設定
	CSound::ELabel GetScrollSE(void) const			{ return m_labelSE; }			// 文字送り時の効果音取得
	int GetNextCharID(void)			{ return m_nNextID; }				// 次の表示文字インデックス取得
	CChar2D *GetNextChar2D(void)	{ return m_vecChar[m_nNextID]; }	// 次の表示文字取得

private:
	// メンバ関数
	void UpdateScroll(const float fDeltaTime);	// 文字送り更新
	void PlayScrollSE(CChar2D *pChar2D);		// 文字送り効果音の再生

	// メンバ変数
	std::vector<CChar2D*> m_vecChar;	// 全文字情報
	CSound::ELabel m_labelSE;			// 文字送り再生SEラベル
	int m_nNextID;		// 次表示する文字インデックス
	float m_fNextTime;	// 次表示までの待機時間
	float m_fCurTime;	// 現在の待機時間
	bool m_bScroll;		// 文字送り状況
};

#endif	// _SCROLL_TEXT2D_H_
