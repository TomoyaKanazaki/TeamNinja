//<================================================
//マップモデル処理(MapModel.cpp)
//
//Author:kazuki watanabe
//<================================================
#include "MapModel.h"
#include "manager.h"
#include "scene.h"

//<**********************************************
//静的メンバ変数
//<**********************************************
int CMapModel::m_nNumAll = 0;

//<**********************************************
//名前宣言
//<**********************************************
namespace
{
	const char* MODEL_NAME = "data\\MODEL\\Tree\\BirchTree_1.x"; //モデルパス
}

//<==============================================
//コンストラクタ
//<==============================================
CMapModel::CMapModel()
{

}
//<==============================================
//デストラクタ
//<==============================================
CMapModel::~CMapModel()
{

}
//<==============================================
//生成処理
//<==============================================
CMapModel* CMapModel::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const D3DXVECTOR3& rSize)
{
	CMapModel* pMapModel = new CMapModel;

	assert(pMapModel && pMapModel->Init()==S_OK);

	pMapModel->SetVec3Position(rPos);	//位置情報
	pMapModel->SetVec3Rotation(rRot);	//向き情報
	pMapModel->SetVec3Scaling(rSize);	//向き情報
	pMapModel->BindModel(MODEL_NAME);	//モデル割り当て

	return pMapModel;
}
//<==============================================
//初期化処理
//<==============================================
HRESULT CMapModel::Init()
{
	// 親クラスの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 自身のラベルを設定
	//SetLabel(LABEL_MAPMODEL);

	return S_OK;
}
//<==============================================
//終了処理
//<==============================================
void CMapModel::Uninit()
{
	CObjectModel::Uninit();
} 
//<==============================================
//更新処理
//<==============================================
void CMapModel::Update(const float fDeltaTime)
{
	//親クラスの更新
	CObjectModel::Update(fDeltaTime);
}
//<==============================================
//描画処理
//<==============================================
void CMapModel::Draw(CShader* pShader)
{
	CObjectModel::Draw();
}