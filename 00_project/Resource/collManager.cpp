//============================================================
//
//	当たり判定マネージャー処理 [collManager.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "collManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* COLLISION_TXT = "data\\TXT\\Collision.txt";		// 当たり判定のテキスト
}

//************************************************************
//	親クラス [CCollManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CCollManager::CCollManager()
{

}

//============================================================
//	デストラクタ
//============================================================
CCollManager::~CCollManager()
{

}

//============================================================
//	終了処理
//============================================================
void CCollManager::Uninit(void)
{
	// 全ての値をクリアする
	m_aCollInfo->m_cube.clear();		// キューブ
	m_aCollInfo->m_cylinder.clear();	// シリンダー
	m_aCollInfo->m_sphere.clear();		// スフィア

	// 自身を破棄する
	delete this;
}

//============================================================
// ロード処理
//============================================================
HRESULT CCollManager::Load(void)
{
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	SCollCube cubeInfo;				// キューブの情報
	SCollCylinder cylinderInfo;		// シリンダーの情報
	SCollSphere sphereInfo;			// スフィアの情報
	int nNum = 0;					// 総数

	// ファイルを開く
	std::ifstream file(COLLISION_TXT);		// ファイルストリーム

	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "当たり判定の読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// ファイルを読込
	std::string str;	// 読込文字列

	while (file >> str)
	{ // ファイルの終端ではない場合ループ

		if (str.front() == '#')
		{ // コメントアウトされている場合

			// 一行全て読み込む
			std::getline(file, str);
		}
		else if (str == "STAGE_COLLISIONSET")
		{ // ステージの当たり判定の設定を読み込んだ場合

			do
			{ // END_STAGE_COLLISIONSETを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str.front() == '#')
				{ // コメントアウトされている場合

					// 一行全て読み込む
					std::getline(file, str);
				}
				else if (str == "CUBE_COLLISIONSET")
				{ // キューブの当たり判定を読み込んだ場合

					do
					{ // END_CUBE_COLLISIONSETを読み込むまでループ

						// 文字列を読み込む
						file >> str;

						if (str.front() == '#')
						{ // コメントアウトされている場合

							// 一行全て読み込む
							std::getline(file, str);
						}
						else if (str == "CUBE_NUM")
						{ // キューブの数を読み込んだ場合

							file >> str;	// ＝を読込
							file >> nNum;	// 総数を読込

							for (int nCnt = 0; nCnt < nNum; nCnt++)
							{
								while (file >> str)
								{
									if (str == "COLLISIONSET")
									{ // コリジョン設定を読み込んだ場合

										// OFFSETを読込
										file >> str;
										file >> str;
										file >> cubeInfo.offset.x;
										file >> cubeInfo.offset.y;
										file >> cubeInfo.offset.z;

										// WIDTHを読み込み
										file >> str;	// WIDTHを読込
										file >> str;	// ＝を読込
										file >> cubeInfo.fWidth;	// 幅を読込

										// HEIGHTを読み込み
										file >> str;	// HEIGHTを読込
										file >> str;	// ＝を読込
										file >> cubeInfo.fHeight;	// 高さを読込

										// DEPTHを読み込み
										file >> str;	// DEPTHを読込
										file >> str;	// ＝を読込
										file >> cubeInfo.fDepth;	// 奥行を読込

										// END_COLLISIONSETを読込
										file >> str;

										// while文を抜け出す
										break;
									}
								}

								// キューブの情報を設定する
								m_aCollInfo->m_cube.push_back(cubeInfo);

								// キューブの当たり判定を初期化
								cubeInfo.offset = VEC3_ZERO;	// オフセット座標
								cubeInfo.fWidth = 0.0f;			// 幅
								cubeInfo.fHeight = 0.0f;		// 高さ
								cubeInfo.fDepth = 0.0f;			// 奥行
							}
						}
					} while (str != "END_CUBE_COLLISIONSET");	// END_CUBE_COLLISIONSETを読み込むまでループ
				}
				else if (str == "CYLINDER_COLLISIONSET")
				{ // シリンダーの当たり判定を読み込んだ場合

					do
					{ // END_CYLINDER_COLLISIONSETを読み込むまでループ

						// 文字列を読み込む
						file >> str;

						if (str.front() == '#')
						{ // コメントアウトされている場合

							// 一行全て読み込む
							std::getline(file, str);
						}
						else if (str == "CYLINDER_NUM")
						{ // シリンダーの数を読み込んだ場合

							file >> str;	// ＝を読込
							file >> nNum;	// 総数を読込

							for (int nCnt = 0; nCnt < nNum; nCnt++)
							{
								while (file >> str)
								{
									if (str == "COLLISIONSET")
									{ // コリジョン設定を読み込んだ場合

										// OFFSETを読込
										file >> str;
										file >> str;
										file >> cylinderInfo.offset.x;
										file >> cylinderInfo.offset.y;
										file >> cylinderInfo.offset.z;

										// RADIUSを読み込み
										file >> str;	// RADIUSを読込
										file >> str;	// ＝を読込
										file >> cylinderInfo.fRadius;	// 半径を読込

										// HEIGHTを読み込み
										file >> str;	// HEIGHTを読込
										file >> str;	// ＝を読込
										file >> cylinderInfo.fHeight;	// 高さを読込

										// END_COLLISIONSETを読込
										file >> str;

										// while文を抜け出す
										break;
									}
								}

								// シリンダーの情報を設定する
								m_aCollInfo->m_cylinder.push_back(cylinderInfo);

								// シリンダーの当たり判定を初期化
								cylinderInfo.offset = VEC3_ZERO;	// オフセット座標
								cylinderInfo.fRadius = 0.0f;		// 半径
								cylinderInfo.fHeight = 0.0f;		// 高さ
							}
						}
					} while (str != "END_CYLINDER_COLLISIONSET");	// END_CYLINDER_COLLISIONSETを読み込むまでループ
				}
				else if (str == "SPHERE_COLLISIONSET")
				{ // スフィアの当たり判定を読み込んだ場合

				do
				{ // END_SPHERE_COLLISIONSETを読み込むまでループ

					// 文字列を読み込む
					file >> str;

					if (str.front() == '#')
					{ // コメントアウトされている場合

						// 一行全て読み込む
						std::getline(file, str);
					}
					else if (str == "SPHERE_NUM")
					{ // スフィアの数を読み込んだ場合

						file >> str;	// ＝を読込
						file >> nNum;	// 総数を読込

						for (int nCnt = 0; nCnt < nNum; nCnt++)
						{
							while (file >> str)
							{
								if (str == "COLLISIONSET")
								{ // コリジョン設定を読み込んだ場合

									// OFFSETを読込
									file >> str;
									file >> str;
									file >> sphereInfo.offset.x;
									file >> sphereInfo.offset.y;
									file >> sphereInfo.offset.z;

									// RADIUSを読み込み
									file >> str;	// RADIUSを読込
									file >> str;	// ＝を読込
									file >> sphereInfo.fRadius;	// 半径を読込

									// END_COLLISIONSETを読込
									file >> str;

									// while文を抜け出す
									break;
								}
							}

							// スフィアの情報を設定する
							m_aCollInfo->m_sphere.push_back(sphereInfo);

							// スフィアの当たり判定を初期化
							sphereInfo.offset = VEC3_ZERO;	// オフセット座標
							sphereInfo.fRadius = 0.0f;		// 半径
						}
					}
				} while (str != "END_SPHERE_COLLISIONSET");	// END_SPHERE_COLLISIONSETを読み込むまでループ
				}
			} while (str != "END_STAGE_COLLISIONSET");	// END_STAGE_COLLISIONSETを読み込むまでループ
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}

//============================================================
// 情報の取得処理
//============================================================
CCollManager::SCollision CCollManager::GetCollInfo(const CActor::EType type)
{
	// 当たり判定の情報を返す
	return m_aCollInfo[type];
}

//============================================================
//	生成処理
//============================================================
CCollManager* CCollManager::Create(void)
{
	// 当たり判定の生成
	CCollManager* pColl = new CCollManager;

	if (pColl == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}

	if (FAILED(pColl->Load()))
	{ // 読込に失敗した場合

		return nullptr;
	}

	// 確保したアドレスを返す
	return pColl;
}
