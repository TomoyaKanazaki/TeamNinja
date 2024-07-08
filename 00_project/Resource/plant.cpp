//===========================================
//
//  植物クラス(plant.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "plant.h"
#include "manager.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const D3DXVECTOR3 POLYGON_SIZE = D3DXVECTOR3(20.0f, 60.0f, 0.0f);
}

//===========================================
//  静的メンバ変数宣言
//===========================================
CListManager<CPlant>* CPlant::m_pList = nullptr; // オブジェクトリスト

//===========================================
//  コンストラクタ
//===========================================
CPlant::CPlant()
{
}

//===========================================
//  デストラクタ
//===========================================
CPlant::~CPlant()
{
}

//===========================================
//  初期化処理
//===========================================
HRESULT CPlant::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CObjectBillboard::Init())) { assert(false); return E_FAIL; }

	// 原点設定
	SetOrigin(ORIGIN_DOWN);
	
	// 回転設定
	SetRotate(ROTATE_LATERAL);

	// サイズの設定
	SetVec3Sizing(POLYGON_SIZE);

	// ラベルの変更
	SetLabel(LABEL_GIMMICK);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CPlant>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//===========================================
//  終了処理
//===========================================
void CPlant::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 親クラスの終了
	CObjectBillboard::Uninit();
}

//===========================================
//  更新処理
//===========================================
void CPlant::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CObjectBillboard::Update(fDeltaTime);
}

//===========================================
//  描画処理
//===========================================
void CPlant::Draw(CShader* pShader)
{
	// 親クラスの描画
	CObjectBillboard::Draw(pShader);
}

//===========================================
//  生成処理
//===========================================
CPlant* CPlant::Create(const D3DXVECTOR3& rPos, const char* sPass)
{
	// ギミックの生成
	CPlant* pPlant = new CPlant;

	// メモリの確保に失敗した場合関数を抜ける
	if (pPlant == nullptr) { assert(false); return nullptr; }

	// 初期化
	if (FAILED(pPlant->Init())) { assert(false); SAFE_DELETE(pPlant); return nullptr; }

	// 位置を設定
	pPlant->SetVec3Position(rPos);

	// テクスチャ割り当て
	pPlant->BindTexture(GET_MANAGER->GetTexture()->Regist(sPass));

	// 確保したアドレスを返す
	return pPlant;
}

//===========================================
//  リスト取得
//===========================================
CListManager<CPlant>* CPlant::GetList(void)
{
	return m_pList;
}
