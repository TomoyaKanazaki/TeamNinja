//<================================================
//マップモデル処理(MapModel.cpp)
//
//Author:kazuki watanabe
//<================================================
#include "MapModel.h"
#include "manager.h"
#include "scene.h"
#include "collision.h"

//<**********************************************
//静的メンバ変数
//<**********************************************
int CMapModel::m_nNumAll = 0;										//総数
const char* CMapModel::mc_apModelFile[MODEL_TYPE_MAX] =				//モデルファイル名(いったん定義)
{
	"data\\MODEL\\Tree\\BirchTree000.x",							//白樺の木
	"data\\MODEL\\Tree\\BirchTree001.x",							//普通サイズの白樺の木
	"data\\MODEL\\Tree\\CommonTree000.x",							//普通の木
	"data\\MODEL\\Tree\\CommonTree001.x",							//普通サイズの普通の木
	"data\\MODEL\\Tree\\Willow000.x",								//柳の木
	"data\\MODEL\\Tree\\Willow001.x",								//普通サイズの柳の木
	"data\\MODEL\\Rock\\Rock000.x",									//小さい岩
	"data\\MODEL\\Rock\\Rock001.x",									//普通の岩
	"data\\MODEL\\Rock\\Rock002.x",									//大きい岩
	"data\\MODEL\\Rock\\Moss-Rock000.x",							//小さい苔岩
	"data\\MODEL\\Rock\\Moss-Rock001.x",							//普通の苔岩
	"data\\MODEL\\Rock\\Moss-Rock002.x",							//大きい苔岩
	"data\\MODEL\\Tree\\WoodLog000.x",								//原木
	"data\\MODEL\\Tree\\TreeStump000.x",							//原木
	"data\\MODEL\\Plant\\Plant000.x",								//草

};	

//<**********************************************
//名前宣言(使うかもしれないので)
//<**********************************************
namespace
{
	
}

//<==============================================
//コンストラクタ
//<==============================================
CMapModel::CMapModel()
{
	m_nModelId = 0;
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
CMapModel* CMapModel::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const D3DXVECTOR3& rSize, const int& nModelId)
{
	CMapModel* pMapModel = new CMapModel;

	//中身と初期化チェック
	assert(pMapModel && pMapModel->Init()==S_OK);

	pMapModel->SetVec3Position(rPos);				//位置情報
	pMapModel->SetVec3Rotation(rRot);				//向き情報
	pMapModel->SetVec3Scaling(rSize);				//サイズ情報
	pMapModel->BindModel(mc_apModelFile[nModelId]);	//番号でモデル割り当て
	pMapModel->m_nModelId = nModelId;

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

	//自身のラベルを設定
	SetLabel(LABEL_MAPMODEL);

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
	ChangeModel();

	//親クラスの更新
	CObjectModel::Update(fDeltaTime);
}
//<==============================================
//描画処理
//<==============================================
void CMapModel::Draw(CShader* pShader)
{
	CToonShader* pToonShader = CToonShader::GetInstance();	// トゥーンシェーダー情報
	if (pToonShader->IsEffectOK())
	{ // エフェクトが使用可能な場合

		// オブジェクトキャラクターの描画
		CObjectModel::Draw(pToonShader);
	}
	else
	{ // エフェクトが使用不可能な場合

		// エフェクトエラー
		assert(false);

		// オブジェクトキャラクターの描画
		CObjectModel::Draw(pShader);
	}
}
//<==============================================
//描画処理
//<==============================================
void CMapModel::ChangeModel(void)
{
	if (GET_INPUTKEY->IsTrigger(DIK_F9))
	{
		m_nModelId += 1;
	}
	if (m_nModelId >= MODEL_TYPE_MAX)
	{
		m_nModelId = MODEL_TYPE_BIRCHTREE;
	}
	BindModel(mc_apModelFile[m_nModelId]);
}
//<==============================================
//当たり判定(いったん作っておきます)
//<==============================================
void CMapModel::Collision(void)
{

}