//==========================================
//
//  マップ情報(map.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "map.h"

#include "checkpoint.h"
#include "goal.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const char* MAP_TXT = "data\\TXT\\map.txt"; // マップ情報のパス
}

//==========================================
//  マップの生成
//==========================================
HRESULT map::Create()
{
	//ローカル変数宣言
	FILE* pFile; // ファイルポインタ

	//ファイルを読み取り専用で開く
	pFile = fopen(MAP_TXT, "r");

	// ファイルが開けなかった場合
	if (pFile == NULL) { assert(false); return E_FAIL; }

	// 情報の読み込み
	while (1)
	{
		// 文字列の記録用
		char aStr[256];

		// 文字列読み込み
		fscanf(pFile, "%s", &aStr[0]);

		// 条件分岐
		if (strcmp(&aStr[0], "CHECKPOINT") == 0) // チェックポイントの生成
		{
			// データの取得用変数
			D3DXVECTOR3 pos, rot;

			// 文字列読み込み (POS)
			fscanf(pFile, "%s", &aStr[0]);

			// データ取得
			fscanf(pFile, "%f", &pos.x);
			fscanf(pFile, "%f", &pos.y);
			fscanf(pFile, "%f", &pos.z);

			// 文字列読み込み (ROT)
			fscanf(pFile, "%s", &aStr[0]);

			// データ取得
			fscanf(pFile, "%f", &rot.x);
			fscanf(pFile, "%f", &rot.y);
			fscanf(pFile, "%f", &rot.z);

			// チェックポイントを生成
			CCheckPoint::Create(pos, rot);
		}
		if (strcmp(&aStr[0], "GOAL") == 0) // チェックポイントの生成
		{
			// データの取得用変数
			D3DXVECTOR3 pos, rot;

			// 文字列読み込み (POS)
			fscanf(pFile, "%s", &aStr[0]);

			// データ取得
			fscanf(pFile, "%f", &pos.x);
			fscanf(pFile, "%f", &pos.y);
			fscanf(pFile, "%f", &pos.z);

			// 文字列読み込み (ROT)
			fscanf(pFile, "%s", &aStr[0]);

			// データ取得
			fscanf(pFile, "%f", &rot.x);
			fscanf(pFile, "%f", &rot.y);
			fscanf(pFile, "%f", &rot.z);

			// チェックポイントを生成
			CGoal::Create(pos, rot);
		}
		if (strcmp(&aStr[0], "END_OF_FILE") == 0) // 読み込み終了
		{
			break;
		}
	}

	return S_OK;
}
