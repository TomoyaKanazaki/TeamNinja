//===========================================
//
//  植物管理クラス(multi_plant.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "multi_plant.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	// TODO ちゃんとした植物のテクスチャ用意する
	const char* TEXTURE_FILE[] = // テクスチャファイル
	{
		"data\\TEXTURE\\test.png", // テストテクスチャ
	};
}

//===========================================
//  静的警告処理
//===========================================
static_assert(NUM_ARRAY(TEXTURE_FILE) == CMultiPlant::TYPE_MAX, "ERROR : Type Count Mismatch");

//===========================================
//  コンストラクタ
//===========================================
CMultiPlant::CMultiPlant() : CObject3D(),
m_Type(CMultiPlant::TYPE_MAX),
m_pos(VEC3_ZERO),
m_size(VEC3_ZERO)
{
}

//===========================================
//  デストラクタ
//===========================================
CMultiPlant::~CMultiPlant()
{
}

//===========================================
//  初期化処理
//===========================================
HRESULT CMultiPlant::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CObject3D::Init())){ assert(false); return E_FAIL; }

	// 成功を返す
	return S_OK;
}

//===========================================
//  終了処理
//===========================================
void CMultiPlant::Uninit(void)
{
	// 親クラスの終了
	CObject3D::Uninit();
}

//===========================================
//  更新処理
//===========================================
void CMultiPlant::Update(const float fDeltaTime)
{
	// 親クラスの更新
	CObject3D::Update(fDeltaTime);
}

//===========================================
//  描画処理
//===========================================
void CMultiPlant::Draw(CShader* pShader)
{
	// 親クラスの描画
	CObject3D::Draw(pShader);
}

//===========================================
//  生成処理
//===========================================
CMultiPlant* CMultiPlant::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const EType type)
{
	// ギミックの生成
	CMultiPlant* pPlant = new CMultiPlant;

	// メモリの確保に失敗した場合関数を抜ける
	if (pPlant == nullptr) { assert(false); return nullptr; }

	// 初期化
	if (FAILED(pPlant->Init())) { assert(false); SAFE_DELETE(pPlant); return nullptr; }

	// 位置を設定
	pPlant->SetVec3Position(rPos);

	// サイズを設定
	pPlant->SetVec3Sizing(rSize);

	// タイプの設定
	pPlant->m_Type = type;

	// 確保したアドレスを返す
	return pPlant;
}
