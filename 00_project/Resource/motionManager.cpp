//============================================================
//
//	モーションマネージャー処理 [motionManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "motionManager.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	親クラス [CMotionManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CMotionManager::CMotionManager()
{
	// モーション連想配列をクリア
	m_mapMotion.clear();
}

//============================================================
//	デストラクタ
//============================================================
CMotionManager::~CMotionManager()
{

}

//============================================================
//	モーションマネージャー初期化処理
//============================================================
HRESULT CMotionManager::Init(void)
{
	// モーション連想配列を初期化
	m_mapMotion.clear();

	// 成功を返す
	return S_OK;
}

//============================================================
//	モーションマネージャー終了処理
//============================================================
void CMotionManager::Uninit(void)
{
	for (auto& rMap : m_mapMotion)
	{ // モーションの要素数分繰り返す

		// パーツ情報の破棄
		SAFE_DEL_ARRAY(rMap.second.infoParts.pInfo);
	}

	// モーション連想配列をクリア
	m_mapMotion.clear();
}

//============================================================
//	キャラクター情報登録
//============================================================
CMotionManager::SCharaData CMotionManager::Regist(const char *pTextPass)
{
	// 既に生成済みかを検索
	auto itr = m_mapMotion.find(pTextPass);	// 引数のキャラクター情報を検索
	if (itr != m_mapMotion.end())
	{ // 生成済みの場合

		// 読込済みのキャラクター情報を返す
		return itr->second;
	}

	// キャラクター情報を読込
	SCharaData tempCharaData;	// キャラクター情報
	if (FAILED(LoadSetup(&tempCharaData, pTextPass)))
	{ // 読込に失敗した場合

		// 初期化されたキャラクター情報を返す
		assert(false);
		return tempCharaData;
	}

	// キャラクター情報を保存
	m_mapMotion.insert(std::make_pair(pTextPass, tempCharaData));

	// 生成したキャラクター情報を返す
	return tempCharaData;
}

//============================================================
//	生成処理
//============================================================
CMotionManager *CMotionManager::Create(void)
{
	// モーションマネージャーの生成
	CMotionManager *pMotionManager = new CMotionManager;
	if (pMotionManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// モーションマネージャーの初期化
		if (FAILED(pMotionManager->Init()))
		{ // モーションマネージャー初期化に失敗した場合

			// モーションマネージャーの破棄
			SAFE_DELETE(pMotionManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pMotionManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CMotionManager::Release(CMotionManager *&prMotionManager)
{
	// モーションマネージャーの終了
	assert(prMotionManager != nullptr);
	prMotionManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prMotionManager);
}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CMotionManager::LoadSetup(SCharaData *pInfoChara, const char *pTextPass)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	int nNumParts	= 0;			// パーツ数の代入用
	int nID			= 0;			// インデックスの代入用
	int nCurMotion	= 0;			// 現在のモーション番号
	int nCurKey		= 0;			// 現在のキー番号
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

					if (strcmp(&aString[0], "NUM_PARTS") == 0)
					{ // 読み込んだ文字列が NUM_PARTS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nNumParts);	// パーツ数を読み込む

						// 既に情報が存在する場合抜ける
						if (pPartsInfo->pInfo != nullptr) { assert(false); return E_FAIL; }

						// パーツ数分メモリ確保
						pPartsInfo->pInfo = new SParts[nNumParts];

						// 生成に失敗した場合抜ける
						if (pPartsInfo->pInfo == nullptr) { assert(false); return E_FAIL; }

						for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
						{ // 読み込んだパーツ数分繰り返す

							// パーツ情報の初期化
							pPartsInfo->pInfo[nCntParts].nParentID = NONE_IDX;	// 親インデックス
							pPartsInfo->pInfo[nCntParts].pos = VEC3_ZERO;		// 位置オフセット
							pPartsInfo->pInfo[nCntParts].rot = VEC3_ZERO;		// 向きオフセット
						}
					}
					else if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // 読み込んだ文字列が PARTSSET の場合

						do
						{ // 読み込んだ文字列が END_PARTSSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // 読み込んだ文字列が INDEX の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nID);			// モデルのインデックスを読み込む

								// パーツ数を加算
								pPartsInfo->nNumParts++;
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // 読み込んだ文字列が PARENT の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%d", &pPartsInfo->pInfo[nID].nParentID);	// モデルの親のインデックスを読み込む
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f", &pPartsInfo->pInfo[nID].pos.x);		// X位置オフセットを読み込む
								fscanf(pFile, "%f", &pPartsInfo->pInfo[nID].pos.y);		// Y位置オフセットを読み込む
								fscanf(pFile, "%f", &pPartsInfo->pInfo[nID].pos.z);		// Z位置オフセットを読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f", &pPartsInfo->pInfo[nID].rot.x);		// X向きオフセットを読み込む
								fscanf(pFile, "%f", &pPartsInfo->pInfo[nID].rot.y);		// Y向きオフセットを読み込む
								fscanf(pFile, "%f", &pPartsInfo->pInfo[nID].rot.z);		// Z向きオフセットを読み込む
							}
							else if (strcmp(&aString[0], "FILEPASS") == 0)
							{ // 読み込んだ文字列が FILEPASS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%s", &aString[0]);	// モデルパスを読み込む

								// モデルのファイルパスを保存
								pPartsInfo->pInfo[nID].strPass = &aString[0];
								useful::StandardizePathPart(&pPartsInfo->pInfo[nID].strPass);	// ファイルパス標準化
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// 読み込んだ文字列が END_PARTSSET ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);	// 読み込んだ文字列が END_CHARACTERSET ではない場合ループ
			}

			// モーションの設定
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // 読み込んだ文字列が MOTIONSET の場合

				int nType = pInfoChara->infoMotion.nNumType;	// 現在の種類の数
				CMotion::SMotionInfo *pMotionInfo = &pInfoChara->infoMotion.aMotionInfo[nType];	// 現在のモーション情報

				// 現在のモーション番号を初期化
				nCurMotion = 0;

				// モーション代入用の変数を初期化
				memset(pMotionInfo, 0, sizeof(*pMotionInfo));

				// キャンセル・コンボフレームをなしにする
				pMotionInfo->nCancelFrame = NONE_IDX;
				pMotionInfo->nComboFrame  = NONE_IDX;

				// 攻撃判定情報を初期化
				pMotionInfo->collLeft.nMin  = NONE_IDX;
				pMotionInfo->collLeft.nMax  = NONE_IDX;
				pMotionInfo->collRight.nMin = NONE_IDX;
				pMotionInfo->collRight.nMax = NONE_IDX;

				// 武器表示をOFFにする
				pMotionInfo->bWeaponDisp = false;

				do
				{ // 読み込んだ文字列が END_MOTION ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "WEAPON") == 0)
					{ // 読み込んだ文字列が WEAPON の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nWeapon);		// 武器表示のON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						pMotionInfo->bWeaponDisp = (nWeapon == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "LOOP") == 0)
					{ // 読み込んだ文字列が LOOP の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nLoop);		// ループのON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						pMotionInfo->bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // 読み込んだ文字列が NUM_KEY の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%d", &pMotionInfo->nNumKey);	// キーの総数を読み込む
					}
					else if (strcmp(&aString[0], "CANCEL") == 0)
					{ // 読み込んだ文字列が CANCEL の場合

						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%d", &pMotionInfo->nCancelFrame);	// キャンセル可能フレームを読み込む
					}
					else if (strcmp(&aString[0], "COMBO") == 0)
					{ // 読み込んだ文字列が COMBO の場合

						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%d", &pMotionInfo->nComboFrame);		// コンボ可能フレームを読み込む
					}
					else if (strcmp(&aString[0], "LEFT_COLL") == 0)
					{ // 読み込んだ文字列が LEFT_COLL の場合

						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%d", &pMotionInfo->collLeft.nMin);	// 判定を出す開始フレームを読み込む
						fscanf(pFile, "%d", &pMotionInfo->collLeft.nMax);	// 判定を消す終了フレームを読み込む
					}
					else if (strcmp(&aString[0], "RIGHT_COLL") == 0)
					{ // 読み込んだ文字列が RIGHT_COLL の場合

						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%d", &pMotionInfo->collRight.nMin);	// 判定を出す開始フレームを読み込む
						fscanf(pFile, "%d", &pMotionInfo->collRight.nMax);	// 判定を消す終了フレームを読み込む
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // 読み込んだ文字列が KEYSET の場合

						CMotion::SKeyInfo *pKeyInfo = &pMotionInfo->pKeyInfo[nCurMotion];	// 現在のキー情報

						// 現在のキー番号を初期化
						nCurKey = 0;

						do
						{ // 読み込んだ文字列が END_KEYSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // 読み込んだ文字列が FRAME の場合

								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &pKeyInfo->nFrame);	// キーが切り替わるまでのフレーム数を読み込む
							}
							else if (strcmp(&aString[0], "MOVE") == 0)
							{ // 読み込んだ文字列が MOVE の場合

								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%f", &pKeyInfo->move.x);	// キーが切り替わるまでの移動量を読み込む
								fscanf(pFile, "%f", &pKeyInfo->move.y);	// キーが切り替わるまでの移動量を読み込む
								fscanf(pFile, "%f", &pKeyInfo->move.z);	// キーが切り替わるまでの移動量を読み込む
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // 読み込んだ文字列が KEY の場合

								CMotion::SKey *pKey = &pKeyInfo->aKey[nCurKey];	// 現在のキー

								do
								{ // 読み込んだ文字列が END_KEY ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // 読み込んだ文字列が POS の場合

										fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
										fscanf(pFile, "%f", &pKey->pos.x);	// X位置を読み込む
										fscanf(pFile, "%f", &pKey->pos.y);	// Y位置を読み込む
										fscanf(pFile, "%f", &pKey->pos.z);	// Z位置を読み込む

										// 読み込んだ位置にパーツの初期位置を加算
										pKey->pos += pPartsInfo->pInfo[nCurKey].pos;
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // 読み込んだ文字列が ROT の場合

										fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
										fscanf(pFile, "%f", &pKey->rot.x);	// X向きを読み込む
										fscanf(pFile, "%f", &pKey->rot.y);	// Y向きを読み込む
										fscanf(pFile, "%f", &pKey->rot.z);	// Z向きを読み込む

										// 読み込んだ向きにパーツの初期向きを加算
										pKey->rot += pPartsInfo->pInfo[nCurKey].rot;

										// 初期向きを正規化
										useful::NormalizeRot(pKey->rot);
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// 読み込んだ文字列が END_KEY ではない場合ループ

								// 現在のキー番号を加算
								nCurKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// 読み込んだ文字列が END_KEYSET ではない場合ループ

						// 現在のモーション番号を加算
						nCurMotion++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// 読み込んだ文字列が END_MOTIONSET ではない場合ループ

				// モーション数を加算
				pInfoChara->infoMotion.nNumType++;
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
