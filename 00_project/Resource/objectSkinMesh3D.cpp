#if 0
//============================================================
//
//	オブジェクトスキンメッシュ3D処理 [objectSkinMesh3D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectSkinMesh3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\TXT\\skinmesh.txt";		// スキンメッシュセットアップテキスト
}

//************************************************************
//	子クラス [CObjectSkinMesh3D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectSkinMesh3D::CObjectSkinMesh3D(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_pVtxBuff		(nullptr),			// 頂点バッファへのポインタ
	m_pBone			(nullptr),			// ボーン情報
	m_pVtxPlus		(nullptr),			// 頂点追加情報
	m_pRenderState	(nullptr),			// レンダーステートの情報
	m_pos			(VEC3_ZERO),		// 位置
	m_rot			(VEC3_ZERO),		// 向き
	m_size			(VEC3_ZERO),		// 大きさ
	m_col			(XCOL_WHITE),		// 色
	m_nTextureID	(0),				// テクスチャインデックス
	m_nNumBone		(0),				// ボーン数
	m_nNumVtx		(0)					// 頂点数
{
	// メンバ変数をクリア
	D3DXMatrixIdentity(&m_mtxWorld);	// ワールドマトリックス
}

//============================================================
//	デストラクタ
//============================================================
CObjectSkinMesh3D::~CObjectSkinMesh3D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectSkinMesh3D::Init(void)
{
	// メンバ変数を初期化
	D3DXMatrixIdentity(&m_mtxWorld);	// ワールドマトリックス
	m_pVtxBuff		= nullptr;			// 頂点バッファへのポインタ
	m_pBone			= nullptr;			// ボーン情報
	m_pVtxPlus		= nullptr;			// 頂点追加情報
	m_pRenderState	= nullptr;			// レンダーステートの情報
	m_pos			= VEC3_ZERO;		// 位置
	m_rot			= VEC3_ZERO;		// 向き
	m_size			= VEC3_ZERO;		// 大きさ
	m_col			= XCOL_WHITE;		// 色
	m_nTextureID	= NONE_IDX;			// テクスチャインデックス
	m_nNumBone		= 0;				// ボーン数
	m_nNumVtx		= 0;				// 頂点数

	// レンダーステートの生成
	m_pRenderState = CRenderState::Create();
	if (m_pRenderState == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// カリングをOFFにする
	m_pRenderState->SetCulling(D3DCULL_NONE);

	// ライティングをOFFにする
	m_pRenderState->SetLighting(false);

	// TODO
	// テクスチャを設定
	m_nTextureID = GET_MANAGER->GetTexture()->Regist("data\\TEXTURE\\player000.jpg");

	// セットアップ処理
	LoadSetup();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectSkinMesh3D::Uninit(void)
{
	// 頂点バッファの破棄
	SAFE_RELEASE(m_pVtxBuff);

	// 頂点追加情報の破棄
	SAFE_DEL_ARRAY(m_pVtxPlus);

	// ボーンの破棄
	SAFE_DEL_ARRAY(m_pBone);

	// レンダーステートの破棄
	SAFE_REF_RELEASE(m_pRenderState);

	// オブジェクトスキンメッシュ3Dを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectSkinMesh3D::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CObjectSkinMesh3D::Draw(CShader *pShader)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxParent;	// 親のマトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// レンダーステートを設定
	m_pRenderState->Set();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntBone = 0; nCntBone < m_nNumBone; nCntBone++)
	{ // ボーン数分繰り返す

		// ワールドマトリックスの初期化
		SBone *pBone = &m_pBone[nCntBone];
		D3DXMatrixIdentity(&pBone->mtx);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pBone->rot.y, pBone->rot.x, pBone->rot.z);
		D3DXMatrixMultiply(&pBone->mtx, &pBone->mtx, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pBone->pos.x, pBone->pos.y, pBone->pos.z);
		D3DXMatrixMultiply(&pBone->mtx, &pBone->mtx, &mtxTrans);

		// 親マトリックスを設定
		if (pBone->nParentID > NONE_IDX && pBone->nParentID < m_nNumBone)
		{ // 親が存在する場合

			// 親ボーンのマトリックスを設定
			mtxParent = m_pBone[pBone->nParentID].mtx;
		}
		else
		{ // 親が存在しない場合

			// 親オブジェクトのマトリックスを設定
			mtxParent = m_mtxWorld;
		}

		// ワールドマトリックスと親マトリックスを掛け合わせる
		D3DXMatrixMultiply(&pBone->mtx, &pBone->mtx, &mtxParent);
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
 
	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{ // 頂点数分繰り返す

		D3DXVECTOR3 posSet = VEC3_ZERO;

		// ワールドマトリックスの初期化
		SVtx *pVtxPlus = &m_pVtxPlus[nCntVtx];
		D3DXMatrixIdentity(&pVtxPlus->mtx);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pVtxPlus->pos.x, pVtxPlus->pos.y, pVtxPlus->pos.z);
		D3DXMatrixMultiply(&pVtxPlus->mtx, &pVtxPlus->mtx, &mtxTrans);

#if 1
		for (int nCntBone = 0; nCntBone < m_nNumBone; nCntBone++)
		{
			// 親ボーンのマトリックスを設定
			mtxParent = m_pBone[nCntBone].mtx;

			// ワールドマトリックスと親マトリックスを掛け合わせる
			D3DXMATRIX mtxOut;
			D3DXMatrixMultiply(&mtxOut, &pVtxPlus->mtx, &mtxParent);

			// マトリックスから位置を取得
			posSet += useful::GetMatrixPosition(mtxOut) * pVtxPlus->aWeight[nCntBone];
		}

		// マトリックスから位置を取得
		pVtx->pos = posSet;
#else
		// 親ボーンのマトリックスを設定
		mtxParent = m_pBone[0].mtx;

		// ワールドマトリックスと親マトリックスを掛け合わせる
		D3DXMATRIX mtxOut;
		D3DXMatrixMultiply(&mtxOut, &pVtxPlus->mtx, &mtxParent);

		// マトリックスから位置を取得
		pVtx->pos = useful::GetMatrixPosition(mtxOut);
#endif

		// 頂点データを1つ進める
		pVtx += 1;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(object::FVF_VERTEX_3D);

	if (pShader == nullptr)
	{ // シェーダーが使用されていない場合

		// 通常描画
		DrawNormal();
	}
	else
	{ // シェーダーが使用されている場合

		// シェーダー描画
		DrawShader(pShader);
	}

	// レンダーステートを再設定
	m_pRenderState->Reset();
}

//============================================================
//	生成処理
//============================================================
CObjectSkinMesh3D *CObjectSkinMesh3D::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot		// 向き
)
{
	// オブジェクトスキンメッシュ3Dの生成
	CObjectSkinMesh3D *pObjectSkinMesh3D = new CObjectSkinMesh3D;
	if (pObjectSkinMesh3D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// オブジェクトスキンメッシュ3Dの初期化
		if (FAILED(pObjectSkinMesh3D->Init()))
		{ // 初期化に失敗した場合

			// オブジェクトスキンメッシュ3Dの破棄
			SAFE_DELETE(pObjectSkinMesh3D);
			return nullptr;
		}

		// 位置を設定
		pObjectSkinMesh3D->SetVec3Position(rPos);

		// 向きを設定
		pObjectSkinMesh3D->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pObjectSkinMesh3D;
	}
}

//============================================================
//	テクスチャ割当処理 (インデックス)
//============================================================
void CObjectSkinMesh3D::BindTexture(const int nTextureID)
{
	if (nTextureID >= NONE_IDX)
	{ // テクスチャインデックスが使用可能な場合

		// テクスチャインデックスを代入
		m_nTextureID = nTextureID;
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	テクスチャ割当処理 (パス)
//============================================================
void CObjectSkinMesh3D::BindTexture(const char *pTexturePass)
{
	// ポインタを宣言
	CTexture *pTexture = GET_MANAGER->GetTexture();	// テクスチャへのポインタ

	if (pTexturePass != nullptr)
	{ // 割り当てるテクスチャパスがある場合

		// テクスチャインデックスを設定
		m_nTextureID = pTexture->Regist(pTexturePass);
	}
	else
	{ // 割り当てるテクスチャパスがない場合

		// テクスチャなしインデックスを設定
		m_nTextureID = NONE_IDX;
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CObjectSkinMesh3D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;
}

//============================================================
//	位置取得処理
//============================================================
D3DXVECTOR3 CObjectSkinMesh3D::GetVec3Position(void) const
{
	// 位置を返す
	return m_pos;
}

//============================================================
//	向きの設定処理
//============================================================
void CObjectSkinMesh3D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 引数の向きを設定
	m_rot = rRot;

	// 向きの正規化
	useful::NormalizeRot(m_rot);
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CObjectSkinMesh3D::GetVec3Rotation(void) const
{
	// 向きを返す
	return m_rot;
}

//============================================================
//	ボーンの生成処理
//============================================================
HRESULT CObjectSkinMesh3D::CreateBone(const int nNumBone)
{
	// ボーン数が自然数ではない場合抜ける
	if (nNumBone <= 0) { assert(false); return E_FAIL; }

	// ボーンの破棄
	SAFE_DEL_ARRAY(m_pBone);
	if (m_pBone != nullptr)
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ボーンの生成
	m_pBone = new SBone[nNumBone];
	if (m_pBone == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	頂点の生成処理
//============================================================
HRESULT CObjectSkinMesh3D::CreateVertex(const int nNumVtx)
{
	// 頂点数が自然数ではない場合抜ける
	if (nNumVtx <= 0) { assert(false); return E_FAIL; }

	// 頂点バッファの破棄
	SAFE_RELEASE(m_pVtxBuff);
	if (m_pVtxBuff != nullptr)
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 頂点バッファの生成
	if (FAILED(GET_DEVICE->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * nNumVtx,	// 必要頂点数
		D3DUSAGE_WRITEONLY,		// 使用方法
		object::FVF_VERTEX_3D,	// 頂点フォーマット
		D3DPOOL_MANAGED,		// メモリの指定
		&m_pVtxBuff,			// 頂点バッファへのポインタ
		nullptr
	)))
	{ // 頂点バッファの生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 頂点追加情報の破棄
	SAFE_DEL_ARRAY(m_pVtxPlus);
	if (m_pVtxPlus != nullptr)
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 頂点追加情報の生成
	m_pVtxPlus = new SVtx[nNumVtx];
	if (m_pVtxPlus == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	ボーン情報の設定処理
//============================================================
void CObjectSkinMesh3D::SetBoneInfo
(
	const int nID,				// パーツインデックス
	const int nParentID,		// 親インデックス
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot		// 向き
)
{
	if (nID < m_nNumBone && nParentID < m_nNumBone)
	{ // インデックスがボーン総数の範囲内の場合

		// オフセット位置・向きの設定
		m_pBone[nID].pos = rPos;
		m_pBone[nID].rot = rRot;

#if 0
		// 親モデルの設定
		if (nParentID == NONE_IDX)
		{ // 親がない場合

			// nullptrを設定
			m_apMultiModel[nID]->SetParentModel(nullptr);
		}
		else
		{ // 親がいる場合

			// 親のアドレスを設定
			m_apMultiModel[nID]->SetParentModel(m_apMultiModel[nParentID]);
		}
#else
		// 親インデックスの設定
		m_pBone[nID].nParentID = nParentID;
#endif
	}
	else { assert(false); }
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObjectSkinMesh3D::SetVertexInfo
(
	const int nVtxID,		// 頂点インデックス
	const SVtx& rVtx,		// 頂点追加情報
	const D3DXVECTOR2& rTex	// テクスチャ座標
)
{
	if (m_pVtxBuff != nullptr && nVtxID < m_nNumVtx)
	{ // 頂点が生成済み且つ、頂点総数の範囲内の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		VERTEX_3D *pVtx;	// 頂点情報へのポインタ
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点データを引数インデックスまで進める
		pVtx += nVtxID;

		// 頂点情報を設定
		pVtx->pos = VEC3_ZERO;	// 頂点座標
		pVtx->nor = VEC3_ZERO;	// 法線ベクトル
		pVtx->col = XCOL_WHITE;	// 頂点カラー
		pVtx->tex = rTex;		// テクスチャ座標

		// 頂点追加情報を設定
		m_pVtxPlus[nVtxID] = rVtx;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
	else { assert(false); }
}

//============================================================
//	レンダーステート情報の取得処理
//============================================================
CRenderState *CObjectSkinMesh3D::GetRenderState(void)
{
	// インスタンス未使用
	assert(m_pRenderState != nullptr);

	// レンダーステートの情報を返す
	return m_pRenderState;
}

//============================================================
//	破棄処理
//============================================================
void CObjectSkinMesh3D::Release(void)
{
	// オブジェクトの破棄
	CObject::Release();
}

//============================================================
//	通常描画処理
//============================================================
void CObjectSkinMesh3D::DrawNormal(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// テクスチャの設定
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(m_nTextureID));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);
}

//============================================================
//	シェーダー描画処理
//============================================================
void CObjectSkinMesh3D::DrawShader(CShader *pShader)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// 描画開始
	pShader->Begin();
	pShader->BeginPass(0);

	// マトリックス情報を設定
	pShader->SetMatrix(&m_mtxWorld);

	// ライト方向を設定
	pShader->SetLightDirect(&m_mtxWorld, 0);

	// 拡散光を設定
	pShader->SetOnlyDiffuse(m_col);

	// テクスチャを設定
	pShader->SetTexture(m_nTextureID);

	// 状態変更の伝達
	pShader->CommitChanges();

	// テクスチャの設定
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(m_nTextureID));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);

	// 描画終了
	pShader->EndPass();
	pShader->End();
}

//============================================================
//	セットアップ処理
//============================================================
void CObjectSkinMesh3D::LoadSetup(void)
{
	// 変数を宣言
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向き
	D3DXVECTOR2 tex = VEC2_ZERO;	// テクスチャ座標
	SVtx vtx;	// 頂点情報

	int nBoneID		= 0;	// ボーンインデックス
	int nVtxID		= 0;	// 頂点インデックス
	int nParentID	= 0;	// 親インデックス
	int nEnd		= 0;	// 読込終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;	// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			// ボーンの設定
			if (strcmp(&aString[0], "BONESET") == 0)
			{ // 読み込んだ文字列が BONESET の場合

				do
				{ // 読み込んだ文字列が END_BONESET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_BONE") == 0)
					{ // 読み込んだ文字列が NUM_BONE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &m_nNumBone);	// ボーン数を読み込む

						// ボーンの生成
						CreateBone(m_nNumBone);
					}
					else if (strcmp(&aString[0], "BONE") == 0)
					{ // 読み込んだ文字列が BONE の場合

						do
						{ // 読み込んだ文字列が END_BONE ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "PARENT") == 0)
							{ // 読み込んだ文字列が PARENT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%d", &nParentID);	// 親インデックスを読み込む
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &pos.x);		// X座標を読み込む
								fscanf(pFile, "%f", &pos.y);		// Y座標を読み込む
								fscanf(pFile, "%f", &pos.z);		// Z座標を読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &rot.x);		// X向きを読み込む
								fscanf(pFile, "%f", &rot.y);		// Y向きを読み込む
								fscanf(pFile, "%f", &rot.z);		// Z向きを読み込む
							}
						} while (strcmp(&aString[0], "END_BONE") != 0);	// 読み込んだ文字列が END_BONE ではない場合ループ

						// ボーン情報の設定
						SetBoneInfo(nBoneID, nParentID, pos, rot);

						// 次のインデックスに移行
						nBoneID++;
					}
				} while (strcmp(&aString[0], "END_BONESET") != 0);	// 読み込んだ文字列が END_BONESET ではない場合ループ
			}

			// 頂点の設定
			if (strcmp(&aString[0], "VERTEXSET") == 0)
			{ // 読み込んだ文字列が VERTEXSET の場合

				do
				{ // 読み込んだ文字列が END_VERTEXSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_VERTEX") == 0)
					{ // 読み込んだ文字列が NUM_VERTEX の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &m_nNumVtx);	// 頂点数を読み込む

						// 頂点の生成
						CreateVertex(m_nNumVtx);
					}
					else if (strcmp(&aString[0], "VERTEX") == 0)
					{ // 読み込んだ文字列が VERTEX の場合

						do
						{ // 読み込んだ文字列が END_VERTEX ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &vtx.pos.x);	// X座標を読み込む
								fscanf(pFile, "%f", &vtx.pos.y);	// Y座標を読み込む
								fscanf(pFile, "%f", &vtx.pos.z);	// Z座標を読み込む
							}
							else if (strcmp(&aString[0], "BONE_REF") == 0)
							{ // 読み込んだ文字列が BONE_REF の場合

								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &vtx.aBoneRef[0]);	// ボーン参照値0を読み込む
								fscanf(pFile, "%d", &vtx.aBoneRef[1]);	// ボーン参照値1を読み込む
							}
							else if (strcmp(&aString[0], "WEIGHT_VALUE") == 0)
							{ // 読み込んだ文字列が WEIGHT_VALUE の場合

								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%f", &vtx.aWeight[0]);	// ボーン影響度0を読み込む
								fscanf(pFile, "%f", &vtx.aWeight[1]);	// ボーン影響度1を読み込む
							}
							else if (strcmp(&aString[0], "TEX") == 0)
							{ // 読み込んだ文字列が TEX の場合

								fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
								fscanf(pFile, "%f", &tex.x);		// テクスチャX座標を読み込む
								fscanf(pFile, "%f", &tex.y);		// テクスチャY座標を読み込む
							}
						} while (strcmp(&aString[0], "END_VERTEX") != 0);	// 読み込んだ文字列が END_VERTEX ではない場合ループ

						// 頂点情報の設定
						SetVertexInfo(nVtxID, vtx, tex);

						// 次のインデックスに移行
						nVtxID++;
					}
				} while (strcmp(&aString[0], "END_VERTEXSET") != 0);	// 読み込んだ文字列が END_VERTEXSET ではない場合ループ
			}
		} while (nEnd != EOF);	// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "スキンメッシュセットアップファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}
#endif
