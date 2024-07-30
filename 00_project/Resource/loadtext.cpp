//============================================================
//
//	テキスト読込関数処理 [loadtext.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "loadtext.h"
#include "text2D.h"
#include "string2D.h"

//============================================================
//	文字列配列の読込処理
//============================================================
void loadtext::LoadVector(std::ifstream *pFile, std::vector<std::wstring> *pVecStr)
{
	// 動的配列ポインタがない場合抜ける
	if (pVecStr == nullptr) { assert(false); return; }

	// ファイルポインタがない場合抜ける
	if (pFile == nullptr)	{ assert(false); return; }

	// 開けてないファイルの場合抜ける
	if (!pFile->is_open())	{ assert(false); return; }

	std::string sStr;	// 読込文字列
	do { // END_TEXTを読み込むまでループ

		// 文字列を読み込む
		*pFile >> sStr;

		if (sStr.front() == '#')
		{ // コメントアウトされている場合

			// 一行全て読み込む
			std::getline(*pFile, sStr);
		}
		else if (sStr == "STR")
		{
			*pFile >> sStr;					// ＝を読込
			pFile->seekg(1, std::ios::cur);	// 読込位置を空白分ずらす
			std::getline(*pFile, sStr);		// 一行全て読み込む

			// 文字列を最後尾に追加
			pVecStr->push_back(useful::MultiByteToWide(sStr));	// ワイド文字列に変換
		}

	} while (sStr != "END_TEXT");	// END_TEXTを読み込むまでループ
}

//============================================================
//	テキストの読込処理 (文字列)
//============================================================
std::vector<std::wstring> loadtext::LoadText(const char *pFilePass, const char *pTextStr)
{
	std::vector<std::wstring> vecStr;	// 文字列の動的配列
	std::string sLoadStart;				// 読込開始の文字列

	// 読込の開始文字列を作成
	sLoadStart = "TEXT_";			// 認識用の先頭文字を設定
	sLoadStart.append(pTextStr);	// 引数の認識用文字列と連結

	// ファイルを開く
	std::ifstream file(pFilePass);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "テキストファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 初期値を返す
		return vecStr;
	}

	// ファイルを読込
	std::string sStr;	// 読込文字列
	while (file >> sStr)
	{ // ファイルの終端ではない場合ループ

		if (sStr.front() == '#')
		{ // コメントアウトされている場合

			// 一行全て読み込む
			std::getline(file, sStr);
		}
		else if (sStr == "TEXT_ELSE")
		{ // 汎用的な読込開始の文字列と一致した場合

			// 文字列の動的配列を読込
			LoadVector(&file, &vecStr);
		}
		else if (sStr == sLoadStart)
		{ // 読込開始の文字列と一致した場合

			// 文字列の動的配列を読込
			LoadVector(&file, &vecStr);

			break;	// ELSEを読み込まないよう繰り返しから抜ける
		}
	}

	// ファイルを閉じる
	file.close();

	// 文字列の動的配列を返す
	return vecStr;
}

//============================================================
//	テキストの読込処理 (インデックス)
//============================================================
std::vector<std::wstring> loadtext::LoadText(const char *pFilePass, const int nTextID)
{
	// テキストの読込
	return LoadText(pFilePass, std::to_string(nTextID).c_str());
}

//============================================================
//	テキストの割当処理
//============================================================
void loadtext::BindText(CText2D *pText2D, const std::vector<std::wstring>& rVecStr)
{
	for (int i = 0; i < (int)rVecStr.size(); i++)
	{ // 文字列数分繰り返す

		// 文字列を追加
		pText2D->AddString(rVecStr[i]);
	}
}

//============================================================
//	文字列の割当処理
//============================================================
void loadtext::BindString(CString2D *pString2D, const std::vector<std::wstring> &rVecStr)
{
	// 文字列が一つではない場合エラー
	assert((int)rVecStr.size() == 1);

	// 文字列を設定
	pString2D->SetString(rVecStr[0]);
}
