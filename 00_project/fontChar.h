//============================================================
//
//	フォント文字ヘッダー [fontChar.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FONT_CHAR_H_
#define _FONT_CHAR_H_

//************************************************************
//	クラス定義
//************************************************************
// フォント文字クラス
class CFontChar
{
public:
	// コンストラクタ
	CFontChar(const HFONT pFont);

	// デストラクタ
	~CFontChar();

	// フォント文字構造体
	struct SChar
	{
		// コンストラクタ
		SChar() { memset(this, 0, sizeof(*this)); }

		GLYPHMETRICS glyph;			// フォントグリフの情報
		OUTLINETEXTMETRIC outline;	// フォントアウトラインの情報
		TEXTMETRIC text;			// フォントテキストの情報
		POSGRID2 offsetBlackBox;	// ブラックボックスの左上オフセット
		int nTexID;					// テクスチャインデックス
	};

	// メンバ関数
	HRESULT Init(void);	// フォント文字初期化
	void Uninit(void);	// フォント文字終了
	SChar Regist(const wchar_t wcChar);	// フォント文字登録

	// 静的メンバ関数
	static CFontChar *Create(const HFONT pFont);	// 生成
	static void Release(CFontChar *&pFontChar);		// 破棄

private:
	// メンバ関数
	BYTE *CreateBitMap(SChar *pChar, HDC pDC, const wchar_t wcChar);	// ビットマップ生成・文字情報保存
	HRESULT CreateTexture(SChar *pChar, BYTE *pBitMap);					// テクスチャ生成・作成

	// メンバ変数
	std::map<wchar_t, SChar> m_mapChar;	// フォント文字連想配列
	const HFONT m_pFont;	// フォントへのポインタ
};

#endif	// _FONT_CHAR_H_
