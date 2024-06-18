#if 1
//============================================================
//
//	円判定統括処理 [chiefCollSphere.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "chiefCollSphere.h"
#include "collisionSphere.h"

//************************************************************
//	親クラス [CChiefCollSphere] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CChiefCollSphere::CChiefCollSphere()
{
	// 判定情報配列をクリア
	m_vecColl.clear();
}

//============================================================
//	デストラクタ
//============================================================
CChiefCollSphere::~CChiefCollSphere()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CChiefCollSphere::Init(void)
{
	// 判定情報配列を初期化
	m_vecColl.clear();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CChiefCollSphere::Uninit(void)
{
	// 判定情報配列をクリア
	m_vecColl.clear();
}

//============================================================
//	オフセット更新処理
//============================================================
void CChiefCollSphere::OffSet(const D3DXMATRIX& rMtx)
{
	for (auto& rSphere : m_vecColl)
	{ // パーツの最大数分繰り返す

		// オフセットの更新
		rSphere->OffSet(rMtx);
	}
}

//============================================================
//	判定位置の取得処理
//============================================================
D3DXVECTOR3 CChiefCollSphere::GetWorldPosition(const int nCollID) const
{
	auto info = m_vecColl.begin();	// 配列の先頭イテレーター

	// イテレーターをインデックス分動かす
	info += nCollID;

	// 判定位置を返す
	return (*info)->GetPos();
}

#if 0
//============================================================
//	判定情報の設定処理
//============================================================
void CChiefCollSphere::SetInfo(const SInfo& rInfo, const int nID)
{
	auto info = m_coll.begin();	// 配列の先頭イテレーター
	info += nID;	// イテレーターをインデックス分動かす

	// イテレーターの中身を設定
	*info = rInfo;
}

//============================================================
//	判定情報の取得処理
//============================================================
CChiefCollSphere::SInfo CChiefCollSphere::GetInfo(const int nID) const
{
	auto info = m_coll.begin();	// 配列の先頭イテレーター
	info += nID;	// イテレーターをインデックス分動かす

	// イテレーターの中身を返す
	return *info;
}

//============================================================
//	配列の設定処理
//============================================================
void CChiefCollSphere::SetVector(const std::vector<SInfo>& rVector)
{
	// 配列を設定
	m_coll = rVector;
}

//============================================================
//	配列の取得処理
//============================================================
std::vector<CChiefCollSphere::SInfo> CChiefCollSphere::GetVector(void) const
{
	// 配列を返す
	return m_coll;
}
#endif

//============================================================
//	判定の追加処理
//============================================================
HRESULT CChiefCollSphere::AddColl
(
	const D3DXVECTOR3& rOffset,	// 判定位置オフセット
	const float fRadius			// 判定半径
)
{
	// 情報を設定
	CCollisionSphere *pColl = CCollisionSphere::Create
	( // 引数
		VEC3_ZERO,	// 位置
		rOffset,	// オフセット
		fRadius		// 半径
	);
	if (pColl == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 配列最後尾に追加情報を設定
	m_vecColl.push_back(pColl);

	// 成功を返す
	return S_OK;
}

//============================================================
//	判定の削除処理
//============================================================
void CChiefCollSphere::SubColl(const int nCollID)
{
	auto info = m_vecColl.begin();	// 配列の先頭イテレーター

	// イテレーターをインデックス分動かす
	info += nCollID;

	// 判定の終了
	SAFE_UNINIT(*info);

	// 配列の要素から削除
	m_vecColl.erase(info);
}

//============================================================
//	生成処理
//============================================================
CChiefCollSphere *CChiefCollSphere::Create(void)
{
	// 円判定統括の生成
	CChiefCollSphere *pChiefCollSphere = new CChiefCollSphere;
	if (pChiefCollSphere == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 円判定統括の初期化
		if (FAILED(pChiefCollSphere->Init()))
		{ // 初期化に失敗した場合

			// 円判定統括の破棄
			SAFE_DELETE(pChiefCollSphere);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pChiefCollSphere;
	}
}

//============================================================
//	破棄処理
//============================================================
void CChiefCollSphere::Release(CChiefCollSphere *&prChiefCollSphere)
{
	// 円判定統括の終了
	assert(prChiefCollSphere != nullptr);
	prChiefCollSphere->Uninit();

	// メモリ開放
	SAFE_DELETE(prChiefCollSphere);
}
#endif
