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
#if 0
	for (auto& rMap : m_mapMotion)
	{ // モーションマネージャーの要素数分繰り返す

		// モーションマネージャー文字の破棄
		SAFE_REF_RELEASE(rMap.second.pMotionManagerChar);

		// モーションマネージャーハンドルの破棄
		SAFE_DEL_OBJECT(rMap.second.pMotionManager);
	}
#endif

	// モーション連想配列をクリア
	m_mapMotion.clear();
}

//============================================================
//	キャラクター情報登録
//============================================================
CMotionManager::SCharaData CMotionManager::Regist
(
	const char *pTextPass,			// テキストパス
	const char **ppModelPassArray	// モデルパス配列
)
{
	// 既に生成済みかを検索
	auto itr = m_mapMotion.find(SKey(pTextPass, ppModelPassArray));	// 引数のキャラクター情報を検索
	if (itr != m_mapMotion.end())
	{ // 生成済みの場合

		// 読込済みのキャラクター情報を返す
		return itr->second;
	}

#if 1
	SCharaData tempCharaData;	// キャラクター情報
	SKey tempKey = SKey(pTextPass, ppModelPassArray);	// キー情報

	// キャラクター情報を読込
	LoadSetup(&tempCharaData, pTextPass, ppModelPassArray);
#endif

	// キャラクター情報を保存
	m_mapMotion.insert(std::make_pair(tempKey, tempCharaData));

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
void CMotionManager::LoadSetup(SCharaData *pInfoChara, const char *pTextPass, const char **ppModelPassArray)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	int nID			= 0;			// インデックスの代入用
	int nNowPose	= 0;			// 現在のポーズ番号
	int nNowKey		= 0;			// 現在のキー番号
	int nWeapon		= 0;			// 武器表示のON/OFFの変換用
	int nLoop		= 0;			// ループのON/OFFの変換用
	int nEnd		= 0;			// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// キャラクター情報を初期化
	memset(pInfoChara, 0, sizeof(*pInfoChara));

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

								// パーツ数を加算
								pInfoChara->infoParts.nNumParts++;
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // 読み込んだ文字列が PARENT の場合

								fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
								fscanf(pFile, "%d", &pInfoChara->infoParts.aInfo[nID].nParentID);	// モデルの親のインデックスを読み込む
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f", &pInfoChara->infoParts.aInfo[nID].pos.x);	// X座標を読み込む
								fscanf(pFile, "%f", &pInfoChara->infoParts.aInfo[nID].pos.y);	// Y座標を読み込む
								fscanf(pFile, "%f", &pInfoChara->infoParts.aInfo[nID].pos.z);	// Z座標を読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合

								fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
								fscanf(pFile, "%f", &pInfoChara->infoParts.aInfo[nID].rot.x);	// X向きを読み込む
								fscanf(pFile, "%f", &pInfoChara->infoParts.aInfo[nID].rot.y);	// Y向きを読み込む
								fscanf(pFile, "%f", &pInfoChara->infoParts.aInfo[nID].rot.z);	// Z向きを読み込む
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// 読み込んだ文字列が END_PARTSSET ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// 読み込んだ文字列が END_CHARACTERSET ではない場合ループ
			}

			// モーションの設定
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // 読み込んだ文字列が MOTIONSET の場合

				// 現在のポーズ番号を初期化
				nNowPose = 0;

				// ポーズ代入用の変数を初期化
				memset(&pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType], 0, sizeof(pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType]));

				// キャンセル・コンボフレームをなしにする
				pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].nCancelFrame = NONE_IDX;
				pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].nComboFrame  = NONE_IDX;

				// 攻撃判定情報を初期化
				pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].collLeft.nMin  = NONE_IDX;
				pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].collLeft.nMax  = NONE_IDX;
				pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].collRight.nMin = NONE_IDX;
				pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].collRight.nMax = NONE_IDX;

				// 武器表示をOFFにする
				pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].bWeaponDisp = false;

				do
				{ // 読み込んだ文字列が END_MOTION ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "WEAPON") == 0)
					{ // 読み込んだ文字列が WEAPON の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nWeapon);		// 武器表示のON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].bWeaponDisp = (nWeapon == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "LOOP") == 0)
					{ // 読み込んだ文字列が LOOP の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nLoop);		// ループのON/OFFを読み込む

						// 読み込んだ値をbool型に変換
						pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // 読み込んだ文字列が NUM_KEY の場合

						fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
						fscanf(pFile, "%d", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].nNumKey);		// キーの総数を読み込む
					}
					else if (strcmp(&aString[0], "CANCEL") == 0)
					{ // 読み込んだ文字列が CANCEL の場合

						fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
						fscanf(pFile, "%d", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].nCancelFrame);	// キャンセル可能フレームを読み込む
					}
					else if (strcmp(&aString[0], "COMBO") == 0)
					{ // 読み込んだ文字列が COMBO の場合

						fscanf(pFile, "%s", &aString[0]);									// = を読み込む (不要)
						fscanf(pFile, "%d", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].nComboFrame);	// コンボ可能フレームを読み込む
					}
					else if (strcmp(&aString[0], "LEFT_COLL") == 0)
					{ // 読み込んだ文字列が LEFT_COLL の場合

						fscanf(pFile, "%s", &aString[0]);										// = を読み込む (不要)
						fscanf(pFile, "%d", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].collLeft.nMin);		// 判定を出す開始フレームを読み込む
						fscanf(pFile, "%d", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].collLeft.nMax);		// 判定を消す終了フレームを読み込む
					}
					else if (strcmp(&aString[0], "RIGHT_COLL") == 0)
					{ // 読み込んだ文字列が RIGHT_COLL の場合

						fscanf(pFile, "%s", &aString[0]);										// = を読み込む (不要)
						fscanf(pFile, "%d", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].collRight.nMin);	// 判定を出す開始フレームを読み込む
						fscanf(pFile, "%d", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].collRight.nMax);	// 判定を消す終了フレームを読み込む
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // 読み込んだ文字列が KEYSET の場合

						// 現在のキー番号を初期化
						nNowKey = 0;

						do
						{ // 読み込んだ文字列が END_KEYSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // 読み込んだ文字列が FRAME の場合

								fscanf(pFile, "%s", &aString[0]);												// = を読み込む (不要)
								fscanf(pFile, "%d", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].aKeyInfo[nNowPose].nFrame);	// キーが切り替わるまでのフレーム数を読み込む
							}
							else if (strcmp(&aString[0], "MOVE") == 0)
							{ // 読み込んだ文字列が MOVE の場合

								fscanf(pFile, "%s", &aString[0]);												// = を読み込む (不要)
								fscanf(pFile, "%f", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].aKeyInfo[nNowPose].move.x);	// キーが切り替わるまでの移動量を読み込む
								fscanf(pFile, "%f", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].aKeyInfo[nNowPose].move.y);	// キーが切り替わるまでの移動量を読み込む
								fscanf(pFile, "%f", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].aKeyInfo[nNowPose].move.z);	// キーが切り替わるまでの移動量を読み込む
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // 読み込んだ文字列が KEY の場合

								do
								{ // 読み込んだ文字列が END_KEY ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // 読み込んだ文字列が POS の場合

										fscanf(pFile, "%s", &aString[0]);																// = を読み込む (不要)
										fscanf(pFile, "%f", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X位置を読み込む
										fscanf(pFile, "%f", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y位置を読み込む
										fscanf(pFile, "%f", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z位置を読み込む

										// 読み込んだ位置にパーツの初期位置を加算
										pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].aKeyInfo[nNowPose].aKey[nNowKey].pos += pInfoChara->infoParts.aInfo[nNowKey].pos;
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // 読み込んだ文字列が ROT の場合

										fscanf(pFile, "%s", &aString[0]);																// = を読み込む (不要)
										fscanf(pFile, "%f", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X向きを読み込む
										fscanf(pFile, "%f", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y向きを読み込む
										fscanf(pFile, "%f", &pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z向きを読み込む

										// 読み込んだ向きにパーツの初期向きを加算
										pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].aKeyInfo[nNowPose].aKey[nNowKey].rot += pInfoChara->infoParts.aInfo[nNowKey].rot;

										// 初期向きを正規化
										useful::NormalizeRot(pInfoChara->infoMotion.aMotionInfo[pInfoChara->infoMotion.nNumType].aKeyInfo[nNowPose].aKey[nNowKey].rot);
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// 読み込んだ文字列が END_KEY ではない場合ループ

								// 現在のキー番号を加算
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// 読み込んだ文字列が END_KEYSET ではない場合ループ

						// 現在のポーズ番号を加算
						nNowPose++;
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
	}
}
