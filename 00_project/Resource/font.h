//============================================================
//
//	フォントヘッダー [font.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FONT_H_
#define _FONT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "fontChar.h"

//************************************************************
//	クラス定義
//************************************************************
// フォントクラス
class CFont
{
public:
	// コンストラクタ
	CFont();

	// デストラクタ
	~CFont();

	// キー構造体
	struct SKey
	{
		// コンストラクタ
		SKey(std::string FontName, bool Italic) :
		sFontName(FontName), bItalic(Italic) {}

		// 比較演算子オーバーロード
		bool operator<(const SKey& rKey) const
		{
			// 情報をまとめて比較
			return std::tie(sFontName, bItalic) < std::tie(rKey.sFontName, rKey.bItalic);
		}

		std::string sFontName;	// フォント名
		bool bItalic;			// イタリック
	};

	// フォント構造体
	struct SFont
	{
		// コンストラクタ
		SFont() { memset(this, 0, sizeof(*this)); }

		CFontChar *pFontChar;	// フォント文字インスタンス
		HFONT pFont;			// フォントへのポインタ
	};

	// メンバ関数
	HRESULT Init(void);		// フォント初期化
	void Uninit(void);		// フォント終了
	HRESULT LoadAll(void);	// フォント全読込
	HRESULT Load(const std::string &rFilePass);	// フォント読込

	SFont Regist(const std::string &rFontName, bool bItalic = false);	// フォント登録
	CFontChar::SChar RegistChar	// フォント文字登録 (名前)
	( // 引数
		const wchar_t wcChar,			// 指定文字
		const std::string &rFontName,	// フォント名
		bool bItalic = false			// イタリック
	);

	// 静的メンバ関数
	static CFont *Create(void);				// 生成
	static void Release(CFont *&prFont);	// 破棄

private:
	// メンバ関数
	HRESULT SearchFolderAll(const std::string &rFolderPath);	// フォルダ全検索

	// メンバ変数
	std::map<SKey, SFont> m_mapFont;		// フォント連想配列
	std::vector<std::string> m_vecFilePass;	// 読込済みファイルパス
};

#endif	// _FONT_H_
