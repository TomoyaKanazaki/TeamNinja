//============================================================
//
//	フォント処理 [font.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "font.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *LOAD_FOLDER = "data\\FONT";	// フォントフォルダ相対パス
	const CFont::SFont ZERO_FONT;	// フォント初期値
	const int FONT_HEIGHT = 240;	// フォント縦幅
}

//************************************************************
//	親クラス [CFont] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFont::CFont()
{
	// フォント連想配列をクリア
	m_mapFont.clear();

	// 読込済みファイルパスをクリア
	m_vecFilePass.clear();
}

//============================================================
//	デストラクタ
//============================================================
CFont::~CFont()
{

}

//============================================================
//	フォント初期化処理
//============================================================
HRESULT CFont::Init(void)
{
	// フォント連想配列を初期化
	m_mapFont.clear();

	// 読込済みファイルパスを初期化
	m_vecFilePass.clear();

	// 成功を返す
	return S_OK;
}

//============================================================
//	フォント終了処理
//============================================================
void CFont::Uninit(void)
{
	for (auto& rMap : m_mapFont)
	{ // フォントの要素数分繰り返す

		// フォント文字の破棄
		SAFE_REF_RELEASE(rMap.second.pFontChar);

		// フォントハンドルの破棄
		SAFE_DEL_OBJECT(rMap.second.pFont);
	}

	for (auto& rVec : m_vecFilePass)
	{ // フォントパスの要素数分繰り返す

		// フォントの破棄
		int nError = RemoveFontResourceEx
		( // 引数
			rVec.c_str(),	// フォントファイルパス
			FR_PRIVATE,		// フォント特性
			nullptr
		);

		// 破棄失敗
		if (nError == 0) { assert(false); }
	}

	// フォント連想配列をクリア
	m_mapFont.clear();

	// 読込済みファイルパスをクリア
	m_vecFilePass.clear();
}

//============================================================
//	フォント全読込処理
//============================================================
HRESULT CFont::LoadAll(void)
{
	// フォントの全読込
	if (FAILED(SearchFolderAll(LOAD_FOLDER)))
	{ // 読込に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	フォント読込処理
//============================================================
HRESULT CFont::Load(const std::string &rFilePass)
{
	// 既に読込済みかを検索
	auto itr = std::find(m_vecFilePass.begin(), m_vecFilePass.end(), rFilePass);	// 引数のフォントパスを検索
	if (itr != m_vecFilePass.end())
	{ // 読込済みの場合

		// 成功を返す
		return S_OK;
	}

	// フォントの読込
	int nError = AddFontResourceEx
	( // 引数
		rFilePass.c_str(),	// フォントファイルパス
		FR_PRIVATE,			// フォント特性
		nullptr
	);
	if (nError == 0)
	{ // 読込に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 読み込んだフォントパスを保存
	m_vecFilePass.push_back(rFilePass);

	// 成功を返す
	return S_OK;
}

//============================================================
//	フォント登録処理
//============================================================
CFont::SFont CFont::Regist(const std::string &rFontName, bool bItalic)
{
	// 既に生成済みかを検索
	auto itr = m_mapFont.find(SKey(rFontName, bItalic));	// 引数のフォントを検索
	if (itr != m_mapFont.end())
	{ // 生成済みの場合

		// 読込済みのフォント情報を返す
		return itr->second;
	}

	// フォント属性の設定
	LOGFONT lf;
	lf.lfHeight			= FONT_HEIGHT;					// フォントの縦幅
	lf.lfWidth			= 0;							// 平均文字幅
	lf.lfEscapement		= 0;							// 描画角度
	lf.lfOrientation	= 0;							// テキスト方向
	lf.lfWeight			= FW_NORMAL;					// フォントの太さ
	lf.lfItalic			= bItalic;						// イタリック
	lf.lfUnderline		= false;						// アンダーライン
	lf.lfStrikeOut		= false;						// 打ち消し線
	lf.lfCharSet		= SHIFTJIS_CHARSET;				// 文字セット
	lf.lfOutPrecision	= OUT_TT_ONLY_PRECIS;			// 出力精度
	lf.lfClipPrecision	= CLIP_DEFAULT_PRECIS;			// クリッピング精度
	lf.lfQuality		= PROOF_QUALITY;				// 出力品質
	lf.lfPitchAndFamily	= (DEFAULT_PITCH | FF_MODERN);	// ピッチとファミリー
	strcpy(&lf.lfFaceName[0], rFontName.c_str());		// フォント名

	// キー情報を設定
	SKey tempKey = SKey(rFontName, bItalic);

	// フォント情報を初期化
	SFont tempFont = ZERO_FONT;

	// フォントハンドルの生成
	tempFont.pFont = CreateFontIndirectA(&lf);
	if (tempFont.pFont == nullptr)
	{ // 生成に失敗した場合

		// 初期値を返す
		assert(false);
		return ZERO_FONT;
	}

	// フォント文字の生成
	tempFont.pFontChar = CFontChar::Create(tempFont.pFont);
	if (tempFont.pFontChar == nullptr)
	{ // 生成に失敗した場合

		// 初期値を返す
		assert(false);
		return ZERO_FONT;
	}

	// フォント情報を保存
	m_mapFont.insert(std::make_pair(tempKey, tempFont));

	// 生成したフォント情報を返す
	return tempFont;
}

//============================================================
//	フォント文字登録 (名前)
//============================================================
CFontChar::SChar CFont::RegistChar
(
	const wchar_t wcChar,			// 指定文字
	const std::string &rFontName,	// フォント名
	bool bItalic					// イタリック
)
{
	// 生成したフォントの文字テクスチャインデックスを返す
	return Regist(rFontName, bItalic).pFontChar->Regist(wcChar);
}

//============================================================
//	生成処理
//============================================================
CFont *CFont::Create(void)
{
	// フォントの生成
	CFont *pFont = new CFont;
	if (pFont == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// フォントの初期化
		if (FAILED(pFont->Init()))
		{ // フォント初期化に失敗した場合

			// フォントの破棄
			SAFE_DELETE(pFont);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pFont;
	}
}

//============================================================
//	破棄処理
//============================================================
void CFont::Release(CFont *&prFont)
{
	// フォントの終了
	assert(prFont != nullptr);
	prFont->Uninit();

	// メモリ開放
	SAFE_DELETE(prFont);
}

//============================================================
//	フォルダ全検索処理
//============================================================
HRESULT CFont::SearchFolderAll(const std::string &rFolderPath)
{
	HANDLE hFile;	// 検索ハンドル
	WIN32_FIND_DATA findFileData;	// ファイル情報

	// 引数パスのディレクトリを取得
	std::string sAllLoadPath = rFolderPath + "\\*.*";	// 全読込パス
	hFile = FindFirstFile(sAllLoadPath.c_str(), &findFileData);
	if (INVALID_HANDLE_VALUE == hFile)
	{ // ハンドルが無効の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	do
	{ // ファイル内の情報全てを読み込む

		// 現在のディレクトリ、親ディレクトリの場合次のループに移行
		if (strcmp(findFileData.cFileName, ".") == 0)	{ continue; }
		if (strcmp(findFileData.cFileName, "..") == 0)	{ continue; }

		// ファイル名を相対パスに変換
		std::string sFullPath = rFolderPath;	// 現在の相対パスを設定
		sFullPath += "\\";						// パス区切り文字を追加
		sFullPath += findFileData.cFileName;	// ファイル名を追加

		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{ // ディレクトリだった場合

			// 新たなディレクトリを全検索
			SearchFolderAll(sFullPath);
		}
		else
		{ // ファイルだった場合

			// フォントを読込
			if (FAILED(Load(sFullPath.c_str())))
			{ // 登録に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}
		}

	} while (FindNextFile(hFile, &findFileData));	// 次のファイルを検索

	// 検索ハンドルを閉じる
	FindClose(hFile);

	// 成功を返す
	return S_OK;
}
