//============================================================
//
//	遷移ポイント処理 [transpoint.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "transpoint.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char*	MODEL_FILE = "data\\MODEL\\MAP\\arena000.x";	// モデルファイル
	const int	PRIORITY = 3;	// 遷移ポイントの優先順位
}

//************************************************************
//	子クラス [CTransPoint] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTransPoint::CTransPoint(const char* pPass) : CObjectModel(CObject::LABEL_MAP, CObject::DIM_3D, PRIORITY),
	m_pTransMapPass	(pPass)	// 遷移先マップパス
{

}

//============================================================
//	デストラクタ
//============================================================
CTransPoint::~CTransPoint()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTransPoint::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 遷移ポイントモデルを登録・割当
	BindModel(MODEL_FILE);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTransPoint::Uninit(void)
{
	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CTransPoint::Update(const float fDeltaTime)
{
	// オブジェクトモデルの更新
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CTransPoint::Draw(CShader *pShader)
{
	CToonShader	*pToonShader = CToonShader::GetInstance();	// トゥーンシェーダー情報
	if (pToonShader->IsEffectOK())
	{ // エフェクトが使用可能な場合

		// オブジェクトモデルの描画
		CObjectModel::Draw(pToonShader);
	}
	else
	{ // エフェクトが使用不可能な場合

		// エフェクトエラー
		assert(false);

		// オブジェクトモデルの描画
		CObjectModel::Draw(pShader);
	}
}

//============================================================
//	生成処理
//============================================================
CTransPoint *CTransPoint::Create
(
	const char* pPass,			// 遷移先マップパス
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot		// 向き
)
{
	// 遷移ポイントの生成
	CTransPoint *pTransPoint = new CTransPoint(pPass);
	if (pTransPoint == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 遷移ポイントの初期化
		if (FAILED(pTransPoint->Init()))
		{ // 初期化に失敗した場合

			// 遷移ポイントの破棄
			SAFE_DELETE(pTransPoint);
			return nullptr;
		}

		// 位置を設定
		pTransPoint->SetVec3Position(rPos);

		// 向きを設定
		pTransPoint->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pTransPoint;
	}
}
