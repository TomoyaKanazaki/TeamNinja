//============================================================
//
//	キャラクター処理 [character.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "character.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *LOAD_FOLDER = "data\\CHARACTER";	// キャラクターフォルダ相対パス
}

//************************************************************
//	親クラス [CCharacter] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CCharacter::CCharacter()
{
	// キャラクター連想配列をクリア
	m_mapCharacter.clear();
}

//============================================================
//	デストラクタ
//============================================================
CCharacter::~CCharacter()
{

}

//============================================================
//	キャラクター初期化処理
//============================================================
HRESULT CCharacter::Init(void)
{
	// キャラクター連想配列を初期化
	m_mapCharacter.clear();

	// 成功を返す
	return S_OK;
}

//============================================================
//	キャラクター終了処理
//============================================================
void CCharacter::Uninit(void)
{
	for (auto& rMap : m_mapCharacter)
	{ // キャラクターの要素数分繰り返す

		for (auto& rMotionInfo : rMap.second.infoMotion.vecMotion)
		{ // モーション情報の要素数分繰り返す

			for (auto& rKeyInfo : rMotionInfo.vecKey)
			{ // キー情報の要素数分繰り返す

				// パーツ情報をクリア
				rKeyInfo.vecParts.clear();
			}

			// キー情報をクリア
			rMotionInfo.vecKey.clear();
		}

		// モーション情報をクリア
		rMap.second.infoMotion.vecMotion.clear();

		// キー保存情報をクリア
		rMap.second.infoParts.vecParts.clear();
	}

	// キャラクター連想配列をクリア
	m_mapCharacter.clear();
}

//============================================================
//	キャラクター全読込処理
//============================================================
HRESULT CCharacter::LoadAll(void)
{
	// キャラクターの全読込
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
//	キャラクター登録
//============================================================
CCharacter::SCharaData CCharacter::Regist(const char *pCharaPass)
{
	// 既に生成済みかを検索
	auto itr = m_mapCharacter.find(pCharaPass);	// 引数のキャラクター情報を検索
	if (itr != m_mapCharacter.end())
	{ // 生成済みの場合

		// 読込済みのキャラクター情報を返す
		return itr->second;
	}

	// キャラクター情報を読込
	SCharaData tempCharaData;	// キャラクター情報
	if (FAILED(LoadSetup(&tempCharaData, pCharaPass)))
	{ // 読込に失敗した場合

		// 初期化されたキャラクター情報を返す
		assert(false);
		return tempCharaData;
	}

	// キャラクター情報を保存
	m_mapCharacter.insert(std::make_pair(pCharaPass, tempCharaData));

	// 生成したキャラクター情報を返す
	return tempCharaData;
}

//============================================================
//	生成処理
//============================================================
CCharacter *CCharacter::Create(void)
{
	// キャラクターの生成
	CCharacter *pCharacter = new CCharacter;
	if (pCharacter == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// キャラクターの初期化
		if (FAILED(pCharacter->Init()))
		{ // キャラクター初期化に失敗した場合

			// キャラクターの破棄
			SAFE_DELETE(pCharacter);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pCharacter;
	}
}

//============================================================
//	破棄処理
//============================================================
void CCharacter::Release(CCharacter *&prCharacter)
{
	// キャラクターの終了
	assert(prCharacter != nullptr);
	prCharacter->Uninit();

	// メモリ開放
	SAFE_DELETE(prCharacter);
}

//============================================================
//	フォルダ全検索処理
//============================================================
HRESULT CCharacter::SearchFolderAll(std::string sFolderPath)
{
	// 変数を宣言
	HANDLE hFile;	// 検索ハンドル
	WIN32_FIND_DATA findFileData;	// ファイル情報

	// 引数パスのディレクトリを取得
	std::string sAllLoadPath = sFolderPath + "\\*.*";	// 全読込パス
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
		std::string sFullPath = sFolderPath;	// 現在の相対パスを設定
		sFullPath += "\\";						// パス区切り文字を追加
		sFullPath += findFileData.cFileName;	// ファイル名を追加

		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{ // ディレクトリだった場合

			// 新たなディレクトリを全検索
			SearchFolderAll(sFullPath);
		}
		else
		{ // ファイルだった場合

			// テクスチャを登録
			Regist(sFullPath.c_str());
		}

	} while (FindNextFile(hFile, &findFileData));	// 次のファイルを検索

	// 検索ハンドルを閉じる
	FindClose(hFile);

	// 成功を返す
	return S_OK;
}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CCharacter::LoadSetup(SCharaData *pInfoChara, const char *pTextPass)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	int nID			= 0;			// インデックスの代入用
	int nCurKey		= 0;			// 現在のキー番号
	int nCurParts	= 0;			// 現在のパーツ番号
	int nWeapon		= 0;			// 武器表示のON/OFFの変換用
	int nLoop		= 0;			// ループのON/OFFの変換用
	int nEnd		= 0;			// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	SPartsInfo *pPartsInfo = &pInfoChara->infoParts;	// パーツ情報
	FILE *pFile;	// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(pTextPass, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// キャラクターの設定
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // 読み込んだ文字列が CHARACTERSET の場合

				do
				{ // 読み込んだ文字列が END_CHARACTERSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // 読み込んだ文字列が PARTSSET の場合

						do
						{ // 読み込んだ文字列が END_PARTSSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // 読み込んだ文字列が INDEX の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nID);			// モデルのインデックスを読み込む

								// 空の要素を最後尾に追加
								pPartsInfo->vecParts.emplace_back();
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // 読み込んだ文字列が PARENT の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%d", &pPartsInfo->vecParts[nID].nParentID);	// モデルの親のインデックスを読み込む
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f", &pPartsInfo->vecParts[nID].pos.x);	// X位置オフセットを読み込む
								fscanf(pFile, "%f", &pPartsInfo->vecParts[nID].pos.y);	// Y位置オフセットを読み込む
								fscanf(pFile, "%f", &pPartsInfo->vecParts[nID].pos.z);	// Z位置オフセットを読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f", &pPartsInfo->vecParts[nID].rot.x);	// X向きオフセットを読み込む
								fscanf(pFile, "%f", &pPartsInfo->vecParts[nID].rot.y);	// Y向きオフセットを読み込む
								fscanf(pFile, "%f", &pPartsInfo->vecParts[nID].rot.z);	// Z向きオフセットを読み込む
							}
							else if (strcmp(&aString[0], "FILEPASS") == 0)
							{ // 読み込んだ文字列が FILEPASS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%s", &aString[0]);	// モデルパスを読み込む

								// モデルのファイルパスを保存
								pPartsInfo->vecParts[nID].strPass = &aString[0];
								useful::StandardizePathPart(&pPartsInfo->vecParts[nID].strPass);	// ファイルパス標準化
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// 読み込んだ文字列が END_PARTSSET ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);	// 読み込んだ文字列が END_CHARACTERSET ではない場合ループ
			}

			// モーションの設定
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // 読み込んだ文字列が MOTIONSET の場合

				int nType = pInfoChara->infoMotion.GetNumMotion();	// 現在のモーション数
				pInfoChara->infoMotion.vecMotion.emplace_back();	// 空の要素を最後尾に追加
				CMotion::SMotion *pMotion = &pInfoChara->infoMotion.vecMotion[nType];	// 現在のモーション情報

				// 現在のキー番号を初期化
				nCurKey = 0;

				// キャンセル・コンボフレームをなしにする
				pMotion->nCancelFrame = NONE_IDX;
				pMotion->nComboFrame  = NONE_IDX;

				// 攻撃判定情報を初期化
				pMotion->collLeft.nMin  = NONE_IDX;
				pMotion->collLeft.nMax  = NONE_IDX;
				pMotion->collRight.nMin = NONE_IDX;
				pMotion->collRight.nMax = NONE_IDX;

				// 武器表示をOFFにする
				pMotion->bWeaponDisp = false;

				do
				{ // 読み込んだ文字列が END_MOTION ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "WEAPON") == 0)
					{ // 読み込んだ文字列が WEAPON の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nWeapon);		// 武器表示のON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						pMotion->bWeaponDisp = (nWeapon == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "LOOP") == 0)
					{ // 読み込んだ文字列が LOOP の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nLoop);		// ループのON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						pMotion->bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "CANCEL") == 0)
					{ // 読み込んだ文字列が CANCEL の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &pMotion->nCancelFrame);	// キャンセル可能フレームを読み込む
					}
					else if (strcmp(&aString[0], "COMBO") == 0)
					{ // 読み込んだ文字列が COMBO の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &pMotion->nComboFrame);		// コンボ可能フレームを読み込む
					}
					else if (strcmp(&aString[0], "LEFT_COLL") == 0)
					{ // 読み込んだ文字列が LEFT_COLL の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &pMotion->collLeft.nMin);	// 判定を出す開始フレームを読み込む
						fscanf(pFile, "%d", &pMotion->collLeft.nMax);	// 判定を消す終了フレームを読み込む
					}
					else if (strcmp(&aString[0], "RIGHT_COLL") == 0)
					{ // 読み込んだ文字列が RIGHT_COLL の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &pMotion->collRight.nMin);	// 判定を出す開始フレームを読み込む
						fscanf(pFile, "%d", &pMotion->collRight.nMax);	// 判定を消す終了フレームを読み込む
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // 読み込んだ文字列が KEYSET の場合

						pMotion->vecKey.emplace_back();	// 空の要素を最後尾に追加
						CMotion::SKey *pKey = &pMotion->vecKey[nCurKey];	// 現在のキー情報

						// 現在のパーツ番号を初期化
						nCurParts = 0;

						do
						{ // 読み込んだ文字列が END_KEYSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // 読み込んだ文字列が FRAME の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &pKey->nFrame);	// キーが切り替わるまでのフレーム数を読み込む
							}
							else if (strcmp(&aString[0], "MOVE") == 0)
							{ // 読み込んだ文字列が MOVE の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &pKey->move.x);	// キーが切り替わるまでの移動量を読み込む
								fscanf(pFile, "%f", &pKey->move.y);	// キーが切り替わるまでの移動量を読み込む
								fscanf(pFile, "%f", &pKey->move.z);	// キーが切り替わるまでの移動量を読み込む
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // 読み込んだ文字列が KEY の場合

								pKey->vecParts.emplace_back();	// 空の要素を最後尾に追加
								CMotion::SParts *pParts = &pKey->vecParts[nCurParts];	// 現在のパーツ情報

								do
								{ // 読み込んだ文字列が END_KEY ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // 読み込んだ文字列が POS の場合

										fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
										fscanf(pFile, "%f", &pParts->pos.x);	// X位置を読み込む
										fscanf(pFile, "%f", &pParts->pos.y);	// Y位置を読み込む
										fscanf(pFile, "%f", &pParts->pos.z);	// Z位置を読み込む

										// 読み込んだ位置にパーツの初期位置を加算
										pParts->pos += pPartsInfo->vecParts[nCurParts].pos;
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // 読み込んだ文字列が ROT の場合

										fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
										fscanf(pFile, "%f", &pParts->rot.x);	// X向きを読み込む
										fscanf(pFile, "%f", &pParts->rot.y);	// Y向きを読み込む
										fscanf(pFile, "%f", &pParts->rot.z);	// Z向きを読み込む

										// 読み込んだ向きにパーツの初期向きを加算
										pParts->rot += pPartsInfo->vecParts[nCurParts].rot;
										useful::NormalizeRot(pParts->rot);	// 向き正規化
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// 読み込んだ文字列が END_KEY ではない場合ループ

								// 現在のパーツ番号を加算
								nCurParts++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// 読み込んだ文字列が END_KEYSET ではない場合ループ

						// 現在のキー番号を加算
						nCurKey++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// 読み込んだ文字列が END_MOTIONSET ではない場合ループ
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "キャラクターセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}
