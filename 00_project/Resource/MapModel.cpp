//<================================================
//マップモデル処理(MapModel.cpp)
//
//Author:kazuki watanabe
//<================================================
//<**********************************************
//インクルードファイル
//<**********************************************
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
	"data\\MODEL\\Tree\\TreeStump000.x",							//木の幹
	"data\\MODEL\\Plant\\Plant000.x",								//4つしかつながっていない草
	"data\\MODEL\\Plant\\Plant001.x",								//多くつながっている草
	"data\\MODEL\\Plant\\Bush000.x",								//草むら
	"data\\MODEL\\House\\House000.x",								//草むら
	"data\\MODEL\\House\\House001.x",								//草むら
	"data\\MODEL\\House\\House002.x",								//草むら
	"data\\MODEL\\House\\House003.x",								//草むら
	"data\\MODEL\\House\\House004.x",								//草むら
	"data\\MODEL\\Lantern\\Lantern001.x",								//草むら
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
	//値のクリア
	m_nNumAll++;
	m_nModelId = 0;
}
//<==============================================
//デストラクタ
//<==============================================
CMapModel::~CMapModel()
{
	//減算
	m_nNumAll--;
}
//<==============================================
//生成処理
//<==============================================
CMapModel* CMapModel::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const int& nModelId,const D3DXVECTOR3& rSize)
{
	//インスタンス生成
	CMapModel* pMapModel = new CMapModel;

	//中身と初期化チェック
	assert(pMapModel);

	pMapModel->Init();

	pMapModel->SetVec3Position(rPos);				//位置情報
	pMapModel->SetVec3Rotation(rRot);				//向き情報
	pMapModel->SetVec3Scaling(rSize);				//サイズ情報
	pMapModel->BindModel(mc_apModelFile[nModelId]);	//番号でモデル割り当て
	pMapModel->m_nModelId = nModelId;				//モデル番号保存

	return pMapModel;
}
//<==============================================
//初期化処理
//<==============================================
HRESULT CMapModel::Init()
{
	//親クラスの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//ラベルの設定
	SetLabel(LABEL_MAPMODEL);

	return S_OK;
}
//<==============================================
//更新処理
//<==============================================
void CMapModel::Update(const float fDeltaTime)
{
	//デバッグ専用のモデル変更処理
	ChangeModel();

	DebugRotate();

	//親クラスの更新
	CObjectModel::Update(fDeltaTime);
}
//<==============================================
//描画処理
//<==============================================
void CMapModel::Draw(CShader* pShader)
{
	//トゥーンシェーダー情報を取得
	CToonShader* pToonShader = CToonShader::GetInstance();

	//エフェクトの使用ができる場合
	if (pToonShader->IsEffectOK())
	{

		//トゥーンシェーダーを使った描画をする
		CObjectModel::Draw(pToonShader);
	}
	//使用できない場合
	else
	{ 
		//エフェクトエラー
		assert(false);

		//オブジェクトモデルの描画
		CObjectModel::Draw(pShader);
	}
}
//<==============================================
//モデル変更処理
//<==============================================
void CMapModel::ChangeModel(void)
{
#ifdef _DEBUG

	//モデル番号を進める
	if (GET_INPUTKEY->IsTrigger(DIK_F9)){m_nModelId += 1;}

	//モデルの最大に達したら、最初のモデルに戻す
	if (m_nModelId >= MODEL_TYPE_MAX){m_nModelId = MODEL_TYPE_MAX - MODEL_TYPE_MAX;}

	//モデルの割り当てをする
	BindModel(mc_apModelFile[m_nModelId]);

#endif
}
//<==============================================
//デバッグ用の回転処理
//<==============================================
void CMapModel::DebugRotate(void)
{
	// くるくるしてみる
	D3DXVECTOR3 rot = GetVec3Rotation();

	//モデル番号を進める
	if (GET_INPUTKEY->IsPress(DIK_F8)) { rot.y += 0.1f; }


	SetVec3Rotation(rot);

}
//<==============================================
//当たり判定(いったん作っておきます)
//<==============================================
void CMapModel::Collision(void)
{
	
}