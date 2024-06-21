//============================================================
//
//	地面処理 [field.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "gimmick_gravel.h"
#include "gimmick_boob.h"
#include "gimmick_cleen.h"
#include "gimmick_fall.h"
#include "gimmick_decayed.h"
#include "gimmick_water.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// テクスチャファイル
	{
		"data\\TEXTURE\\soil005.png",			// 土テクスチャ
		"data\\TEXTURE\\soil001.png",			// 草土テクスチャ
		"data\\TEXTURE\\soil004.png",			// 草テクスチャ
		"data\\TEXTURE\\soil001.png",			// 砂利道テクスチャ
		"data\\TEXTURE\\FIELD2\\Mud000.jpg",	// 泥テクスチャ
		"data\\TEXTURE\\soil001.png",			// 掃除床テクスチャ
		"data\\TEXTURE\\soil001.png",			// 落とし穴テクスチャ
		"data\\TEXTURE\\soil001.png",			// 朽ちた床テクスチャ
		"data\\TEXTURE\\soil001.png",			// 水テクスチャ
		"data\\TEXTURE\\soil001.png",			// ドブテクスチャ
	};
	const char FLAG[] =	// フラグ配列
	{
		' ',	// 土フラグ
		' ',	// 草土フラグ
		' ',	// 草フラグ
		'g',	// 砂利フラグ
		'b',	// 泥
		'c',	// 掃除床
		'f',	// 落ちた床
		'd',	// 朽ちた床
		'w',	// 水
		'u',	// ドブ
	};

#ifdef _DEBUG
	const D3DXCOLOR COLOR[] =
	{
		D3DXCOLOR(0.70f, 0.37f, 0.00f, 1.0f),
		D3DXCOLOR(0.11f, 0.02f, 0.00f, 1.0f),
		D3DXCOLOR(0.02f, 0.96f, 0.27f, 1.0f),
		D3DXCOLOR(0.38f, 0.38f, 0.38f, 1.0f),
		D3DXCOLOR(0.18f, 0.10f, 0.00f, 1.0f),
		D3DXCOLOR(0.90f, 0.90f, 1.00f, 1.0f),
		D3DXCOLOR(0.81f, 0.90f, 0.85f, 1.0f),
		D3DXCOLOR(0.28f, 0.00f, 0.18f, 1.0f),
		D3DXCOLOR(0.28f, 0.87f, 0.95f, 1.0f),
		D3DXCOLOR(0.33f, 0.40f, 0.26f, 1.0f)
	};
#endif

	const char *SETUP_TXT = "data\\TXT\\field.txt";	// セットアップテキスト相対パス

	const int PRIORITY = 0;	// 地面の優先順位
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CField> *CField::m_pList = nullptr;	// オブジェクトリスト
CField::STerrainInfo CField::m_aTerrainInfo[TERRAIN_MAX] = {};	// 地形情報

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CField::TYPE_MAX, "ERROR : Type Count Mismatch");
static_assert(NUM_ARRAY(FLAG) == CField::TYPE_MAX, "ERROR : Type Count Mismatch");
#ifdef _DEBUG
static_assert(NUM_ARRAY(COLOR) == CField::TYPE_MAX, "ERROR : Type Count Mismatch");
#endif

//************************************************************
//	子クラス [CField] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CField::CField() : CObjectMeshField(CObject::LABEL_FIELD, CObject::DIM_3D, PRIORITY),
	m_terrain	(TERRAIN_120x120),	// 地形
	m_type		(TYPE_SOIL)			// 種類
{

}

//============================================================
//	デストラクタ
//============================================================
CField::~CField()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CField::Init(void)
{
	// メンバ変数を初期化
	m_terrain	= TERRAIN_120x120;	// 地形
	m_type		= TYPE_SOIL;		// 種類

	// オブジェクトメッシュフィールドの初期化
	if (FAILED(CObjectMeshField::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// セットアップの読込
	LoadSetup();

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CField>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CField::Uninit(void)
{
	for (int nCntField = 0; nCntField < TERRAIN_MAX; nCntField++)
	{ // 地形情報の最大数分繰り返す

		// 地形情報の破棄
		SAFE_DEL_ARRAY(m_aTerrainInfo[nCntField].pPosGap);
	}

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトメッシュフィールドの終了
	CObjectMeshField::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CField::Update(const float fDeltaTime)
{
	// オブジェクトメッシュフィールドの更新
	CObjectMeshField::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CField::Draw(CShader *pShader)
{
	// オブジェクトメッシュフィールドの描画
	CObjectMeshField::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CField *CField::Create
(
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR2& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const POSGRID2& rTexPart	// テクスチャ分割数
)
{
	// 地面の生成
	CField *pField = nullptr;	// 地面
	switch (type)
	{ // 種類ごとの処理
	case TYPE_GRAVEL:
		pField = new CGimmickGravel;
		break;

	case TYPE_BOOB:
		pField = new CGimmickBoob;
		break;

	case TYPE_CLEEN:
		pField = new CGimmickCleen;
		break;

	case TYPE_FALL:
		pField = new CGimmickFall;
		break;

	case TYPE_DECAYED:
		pField = new CGimmickDecayed;
		break;

	case TYPE_WATER:
		pField = new CGimmickWater;
		break;

	default:
		pField = new CField;
		break;
	}

	if (pField == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 地面の初期化
		if (FAILED(pField->Init()))
		{ // 初期化に失敗した場合

			// 地面の破棄
			SAFE_DELETE(pField);
			return nullptr;
		}

		// 種類を設定
		pField->SetType(type);

		// 位置を設定
		pField->SetVec3Position(rPos);

		// 向きを設定
		pField->SetVec3Rotation(rRot);

		// 大きさを設定
		pField->SetVec2Sizing(rSize);

		// 色を設定
		pField->SetColor(rCol);

		// 分割数を設定
		if (FAILED(pField->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// 地面の破棄
			SAFE_DELETE(pField);
			return nullptr;
		}

#ifdef _DEBUG
		pField->SetColor(COLOR[type]);
#endif

		// テクスチャ分割数を設定
		pField->SetTexPattern(rTexPart);

		// 確保したアドレスを返す
		return pField;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CField> *CField::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	地形の設定処理
//============================================================
void CField::SetTerrain(const ETerrain terrain)
{
	if (terrain > NONE_IDX && terrain < TERRAIN_MAX)
	{ // インデックスが範囲内の場合

		// 地形を保存
		m_terrain = terrain;

		// 地形を設定
		CObjectMeshField::SetTerrain(m_aTerrainInfo[terrain].part, m_aTerrainInfo[terrain].pPosGap);
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	種類の設定処理
//============================================================
void CField::SetType(const EType type)
{
	if (type > NONE_IDX && type < TYPE_MAX)
	{ // インデックスが範囲内の場合

		// 種類を保存
		m_type = type;

		// テクスチャを登録・割当
		BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[type]));
#ifdef _DEBUG
		BindTexture(GET_MANAGER->GetTexture()->Regist("data\\TEXTURE\\testfield.png"));
#endif
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	分身に当たっていた時の処理
//============================================================
void CField::Hit(CPlayerClone* pClone)
{

}

//============================================================
//	分身に当たっていない時の処理
//============================================================
void CField::Miss(CPlayerClone* pClone)
{

}

//===========================================
//  自身のフラグを返す
//===========================================
const char CField::GetFlag() const
{
	return FLAG[m_type];
}

//===========================================
//  指定のフラグ取得
//===========================================
const char CField::GetFlag(EType type)
{
	// 指定されたフラグを返す
	return FLAG[type];
}

//============================================================
//	セットアップ処理
//============================================================
void CField::LoadSetup(void)
{
	// 変数を宣言
	int nID = 0;		// インデックスの代入用
	int nNumVtx = 0;	// 頂点数の代入用
	int nEnd = 0;		// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	for (int nCntField = 0; nCntField < TERRAIN_MAX; nCntField++)
	{ // 地形情報の最大数分繰り返す

		// 地形情報の破棄
		SAFE_DEL_ARRAY(m_aTerrainInfo[nCntField].pPosGap);
	}

	// 静的メンバ変数の情報をクリア
	memset(&m_aTerrainInfo[0], 0, sizeof(m_aTerrainInfo));	// 地形情報

	// ファイルを読み込み形式で開く
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// 地形の設定
			if (strcmp(&aString[0], "TERRAINSET") == 0)
			{ // 読み込んだ文字列が TERRAINSET の場合

				do
				{ // 読み込んだ文字列が END_TERRAINSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "FIELDSET") == 0)
					{ // 読み込んだ文字列が FIELDSET の場合

						do
						{ // 読み込んだ文字列が END_FIELDSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "PART") == 0)
							{ // 読み込んだ文字列が PART の場合

								fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
								fscanf(pFile, "%d", &m_aTerrainInfo[nID].part.x);	// 分割数Xを読み込む
								fscanf(pFile, "%d", &m_aTerrainInfo[nID].part.y);	// 分割数Yを読み込む

								// 頂点数を設定
								nNumVtx = (m_aTerrainInfo[nID].part.x + 1) * (m_aTerrainInfo[nID].part.y + 1);

								if (m_aTerrainInfo[nID].pPosGap == nullptr)
								{ // ポインタが使用されていない場合

									// 頂点数分メモリ確保
									m_aTerrainInfo[nID].pPosGap = new D3DXVECTOR3[nNumVtx];

									// 例外処理
									assert(m_aTerrainInfo[nID].pPosGap != nullptr);	// 非使用中
								}
								else { assert(false); }	// 使用中
							}
							else if (strcmp(&aString[0], "GAPSET") == 0)
							{ // 読み込んだ文字列が GAPSET の場合

								for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
								{ // 頂点数分繰り返す

									fscanf(pFile, "%f", &m_aTerrainInfo[nID].pPosGap[nCntVtx].x);	// 頂点座標のずれ量Xを読み込む
									fscanf(pFile, "%f", &m_aTerrainInfo[nID].pPosGap[nCntVtx].y);	// 頂点座標のずれ量Yを読み込む
									fscanf(pFile, "%f", &m_aTerrainInfo[nID].pPosGap[nCntVtx].z);	// 頂点座標のずれ量Zを読み込む
									fscanf(pFile, "%s", &aString[0]);								// , を読み込む (不要)
								}
							}
						} while (strcmp(&aString[0], "END_FIELDSET") != 0);	// 読み込んだ文字列が END_FIELDSET ではない場合ループ

						// インデックスを加算
						nID++;
					}
				} while (strcmp(&aString[0], "END_TERRAINSET") != 0);	// 読み込んだ文字列が END_TERRAINSET ではない場合ループ
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "地面セットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
