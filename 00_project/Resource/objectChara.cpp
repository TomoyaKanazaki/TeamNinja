//============================================================
//
//	オブジェクトキャラクター処理 [objectChara.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectChara.h"
#include "manager.h"
#include "renderer.h"
#include "multiModel.h"
#include "motion.h"

//************************************************************
//	子クラス [CObjectChara] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectChara::CObjectChara(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_pMotion	(nullptr),		// モーションの情報
	m_pos		(VEC3_ZERO),	// 位置
	m_rot		(VEC3_ZERO),	// 向き
	m_nNumModel	(0)				// パーツの総数
{
	// メンバ変数をクリア
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// モデルの情報
	memset(&m_apCollision[0], 0, sizeof(m_apCollision));	// 当たり判定の情報
	D3DXMatrixIdentity(&m_mtxWorld);	// ワールドマトリックス
}

//============================================================
//	デストラクタ
//============================================================
CObjectChara::~CObjectChara()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectChara::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// モデルの情報
	memset(&m_apCollision[0], 0, sizeof(m_apCollision));	// 当たり判定の情報
	D3DXMatrixIdentity(&m_mtxWorld);	// ワールドマトリックス
	m_pMotion	= nullptr;		// モーションの情報
	m_pos		= VEC3_ZERO;	// 位置
	m_rot		= VEC3_ZERO;	// 向き
	m_nNumModel	= 0;			// パーツの総数

	// モーションの生成
	m_pMotion = CMotion::Create(this);
	if (m_pMotion == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectChara::Uninit(void)
{
	for (int nCntChara = 0; nCntChara < motion::MAX_PARTS; nCntChara++)
	{ // パーツの最大数分繰り返す

		// マルチモデルの終了
		SAFE_UNINIT(m_apMultiModel[nCntChara]);
	}

	// モーションの破棄
	SAFE_REF_RELEASE(m_pMotion);

	// オブジェクトキャラクターを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectChara::Update(void)
{
	if (m_pMotion != nullptr)
	{ // モーションが存在する場合

		// モーションの更新
		m_pMotion->Update();
	}
}

//============================================================
//	描画処理
//============================================================
void CObjectChara::Draw(CShader *pShader)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

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

	for (int nCntChara = 0; nCntChara < m_nNumModel; nCntChara++)
	{ // パーツの総数分繰り返す

		// パーツの描画
		m_apMultiModel[nCntChara]->Draw(pShader);
	}
}

//============================================================
//	更新状況の設定処理
//============================================================
void CObjectChara::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);	// 自身
	for (int nCntChara = 0; nCntChara < m_nNumModel; nCntChara++)
	{ // パーツの総数分繰り返す

		m_apMultiModel[nCntChara]->SetEnableUpdate(bUpdate);
	}
}

//============================================================
//	描画状況の設定処理
//============================================================
void CObjectChara::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);	// 自身
	for (int nCntChara = 0; nCntChara < m_nNumModel; nCntChara++)
	{ // パーツの総数分繰り返す

		m_apMultiModel[nCntChara]->SetEnableDraw(bDraw);
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CObjectChara::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;
}

//============================================================
//	向きの設定処理
//============================================================
void CObjectChara::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 引数の向きを設定
	m_rot = rRot;

	// 向きの正規化
	useful::NormalizeRot(m_rot);
}

//============================================================
//	生成処理
//============================================================
CObjectChara *CObjectChara::Create(const D3DXVECTOR3 &rPos, const D3DXVECTOR3 &rRot)
{
	// オブジェクトキャラクターの生成
	CObjectChara *pObjectChara = new CObjectChara;
	if (pObjectChara == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// オブジェクトキャラクターの初期化
		if (FAILED(pObjectChara->Init()))
		{ // 初期化に失敗した場合

			// オブジェクトキャラクターの破棄
			SAFE_DELETE(pObjectChara);
			return nullptr;
		}

		// 位置を設定
		pObjectChara->SetVec3Position(rPos);

		// 向きを設定
		pObjectChara->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pObjectChara;
	}
}

//============================================================
//	モーションの設定処理
//============================================================
void CObjectChara::SetMotion(const int nType, const int nBlendFrame)
{
	// モーションの設定
	m_pMotion->Set(nType, nBlendFrame);
}

//============================================================
//	パーツ情報の設定処理
//============================================================
void CObjectChara::SetPartsInfo
(
	const int nID,				// パーツインデックス
	const int nParentID,		// 親インデックス
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const char *pFileName		// ファイル名
)
{
	if (nID < motion::MAX_PARTS && nParentID < motion::MAX_PARTS && pFileName != nullptr)
	{ // パーツ・親インデックスが配列範囲内且つ、ファイル名が存在する場合

		// モデルの生成
		m_apMultiModel[nID] = CMultiModel::Create(rPos, rRot);

		// モデルの原点位置・向きを設定
		m_pMotion->SetOriginPosition(rPos, nID);
		m_pMotion->SetOriginRotation(rRot, nID);

		// モデルを割当
		m_apMultiModel[nID]->BindModel(pFileName);

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

		// パーツの総数を加算
		m_nNumModel++;
	}
}

//============================================================
//	マトリックスの設定処理
//============================================================
void CObjectChara::SetMtxWorld(const D3DXMATRIX &rMtxWorld)
{
	// 引数のマトリックスを設定
	m_mtxWorld = rMtxWorld;
}

//============================================================
//	パーツ位置の設定処理
//============================================================
void CObjectChara::SetPartsPosition(const int nPartsID, const D3DXVECTOR3& rPos)
{
	if (nPartsID < m_nNumModel)
	{ // 使用可能なインデックスの場合

		// 引数のインデックスの位置を設定
		m_apMultiModel[nPartsID]->SetVec3Position(rPos);
	}
	else { assert(false); }
}

//============================================================
//	パーツ向きの設定処理
//============================================================
void CObjectChara::SetPartsRotation(const int nPartsID, const D3DXVECTOR3& rRot)
{
	if (nPartsID < m_nNumModel)
	{ // 使用可能なインデックスの場合

		// 引数のインデックスの向きを設定
		m_apMultiModel[nPartsID]->SetVec3Rotation(rRot);
	}
	else { assert(false); }
}

//============================================================
//	パーツ位置取得処理
//============================================================
D3DXVECTOR3 CObjectChara::GetPartsPosition(const int nPartsID) const
{
	if (nPartsID < m_nNumModel)
	{ // 使用可能なインデックスの場合

		// 引数インデックスのパーツの位置を返す
		return m_apMultiModel[nPartsID]->GetVec3Position();
	}

	// インデックスエラー
	assert(false);
	return VEC3_ZERO;
}

//============================================================
//	パーツ向き取得処理
//============================================================
D3DXVECTOR3 CObjectChara::GetPartsRotation(const int nPartsID) const
{
	if (nPartsID < m_nNumModel)
	{ // 使用可能なインデックスの場合

		// 引数インデックスのパーツの向きを返す
		return m_apMultiModel[nPartsID]->GetVec3Rotation();
	}

	// インデックスエラー
	assert(false);
	return VEC3_ZERO;
}

//============================================================
//	マルチモデル取得処理
//============================================================
CMultiModel *CObjectChara::GetMultiModel(const int nPartsID) const
{
	if (nPartsID < m_nNumModel)
	{ // 使用可能なインデックスの場合

		// マルチモデルの情報を返す
		return m_apMultiModel[nPartsID];
	}
	else { assert(false); return m_apMultiModel[0]; }
}

//============================================================
//	モーション取得処理
//============================================================
CMotion *CObjectChara::GetMotion(void) const
{
	// インスタンス未使用
	assert(m_pMotion != nullptr);

	// モーションの情報を返す
	return m_pMotion;
}

//============================================================
//	マテリアルの設定処理
//============================================================
void CObjectChara::SetMaterial(const D3DXMATERIAL& rMat, const int nPartsID, const int nMatID)
{
	if (nPartsID < m_nNumModel)
	{ // 使用可能なインデックスの場合

		// 引数のマテリアルを設定
		m_apMultiModel[nPartsID]->SetMaterial(rMat, nMatID);
	}
	else { assert(false); }
}

//============================================================
//	マテリアル全設定処理
//============================================================
void CObjectChara::SetAllMaterial(const D3DXMATERIAL& rMat)
{
	for (int nCntChara = 0; nCntChara < m_nNumModel; nCntChara++)
	{ // パーツの総数分繰り返す

		// 引数のマテリアルを全マテリアルに設定
		m_apMultiModel[nCntChara]->SetAllMaterial(rMat);
	}
}

//============================================================
//	マテリアル再設定処理
//============================================================
void CObjectChara::ResetMaterial(void)
{
	for (int nCntChara = 0; nCntChara < m_nNumModel; nCntChara++)
	{ // パーツの総数分繰り返す

		// 全マテリアルに初期マテリアルを再設定
		m_apMultiModel[nCntChara]->ResetMaterial();
	}
}

//============================================================
//	透明度の設定処理
//============================================================
void CObjectChara::SetAlpha(const float fAlpha)
{
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // パーツの最大数分繰り返す

		// 引数の透明度を設定
		m_apMultiModel[nCntParts]->SetAlpha(fAlpha);
	}
}

//============================================================
//	透明度取得処理
//============================================================
float CObjectChara::GetAlpha(void) const
{
	// 変数を宣言
	float fAlpha = 0.0f;	// 最も不透明なマテリアルの透明度

	// 最も不透明な透明度を探す
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // パーツの最大数分繰り返す

		float fCurAlpha = m_apMultiModel[nCntParts]->GetAlpha();	// 現在のモデルの透明度
		if (fCurAlpha > fAlpha)
		{ // マテリアルの透明度がより不透明だった場合

			// 現在のマテリアルの透明度を保存
			fAlpha = fCurAlpha;
		}
	}

	// 全パーツ内で最も不透明だったマテリアルの透明度を返す
	return fAlpha;
}

//============================================================
//	最大透明度取得処理
//============================================================
float CObjectChara::GetMaxAlpha(void) const
{
	// 変数を宣言
	float fAlpha = 0.0f;	// 最も不透明なマテリアルの透明度

	// 最も不透明な透明度を探す
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // パーツの最大数分繰り返す

		if (m_apMultiModel[nCntParts]->GetMaxAlpha() > fAlpha)
		{ // マテリアルの透明度がより不透明だった場合

			// 現在のマテリアルの透明度を保存
			fAlpha = m_apMultiModel[nCntParts]->GetMaxAlpha();
		}
	}

	// 全パーツ内で最も不透明だったマテリアルの透明度を返す
	return fAlpha;
}
