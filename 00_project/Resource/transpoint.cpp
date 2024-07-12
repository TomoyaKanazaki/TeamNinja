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
#include "collision.h"
#include "manager.h"
#include "stage.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char*	MODEL_FILE	= "data\\MODEL\\MAP\\arena000.x";	// モデルファイル
	const int	PRIORITY	= 3;		// 遷移ポイントの優先順位
	const float	RADIUS		= 120.0f;	// 遷移ポイントに触れられる半径
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CTransPoint>* CTransPoint::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CTransPoint] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTransPoint::CTransPoint(const char* pPass) : CObjectModel(CObject::LABEL_MAP, CObject::DIM_3D, PRIORITY),
	m_sTransMapPass	(pPass)	// 遷移先マップパス
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

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CTransPoint>::Create();
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
void CTransPoint::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

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

//============================================================
//	リスト取得処理
//============================================================
CListManager<CTransPoint> *CTransPoint::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	遷移ポイントとの当たり判定
//============================================================
void CTransPoint::Collision
(
	const D3DXVECTOR3& rPos,	// 位置
	const float fRadius			// 半径
)
{
	// 遷移ポイントがない場合抜ける
	if (m_pList == nullptr) { return; }

	std::list<CTransPoint*> list = m_pList->GetList();	// 内部リスト
	for (const auto& rList : list)
	{ // 要素数分繰り返す

		D3DXVECTOR3 posTrans = rList->GetVec3Position();	// 遷移ポイント位置

		// XZ平面の円の当たり判定
		bool bHit = collision::Circle2D
		(
			rPos,		// 判定位置
			posTrans,	// 判定目標位置
			fRadius,	// 判定半径
			RADIUS		// 判定目標半径
		);
		if (bHit)
		{ // 当たっている場合

#if 0
			// 遷移ポイントのマップパスに遷移
			GET_STAGE->SetInitMapPass(rList->m_sTransMapPass.c_str());
			GET_MANAGER->SetLoadScene(CScene::MODE_GAME);
#endif
			return;
		}
	}
}
