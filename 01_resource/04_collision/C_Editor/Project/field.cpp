//============================================================
//
//	地面処理 [field.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// テクスチャファイル
	{
		"data\\TEXTURE\\field000.png",	// 火山灰テクスチャ
	};
	const int PRIORITY = 0;	// 地面の優先順位
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CField::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	子クラス [CField] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CField::CField() : CObjectMeshField(CObject::LABEL_FIELD, CObject::DIM_3D, PRIORITY)
{

}

//============================================================
//	デストラクタ
//============================================================
CField::~CField()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CField::Init(void)
{
	// オブジェクトメッシュフィールドの初期化
	if (FAILED(CObjectMeshField::Init()))
	{ // 初期化に失敗した場合

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
void CField::Uninit(void)
{
	// オブジェクトメッシュフィールドの終了
	CObjectMeshField::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CField::Update(void)
{
	// オブジェクトメッシュフィールドの更新
	CObjectMeshField::Update();
}

//============================================================
//	描画処理
//============================================================
void CField::Draw(void)
{
	// オブジェクトメッシュフィールドの描画
	CObjectMeshField::Draw();
}

//============================================================
//	生成処理
//============================================================
CField *CField::Create
(
	const ETexture texture,		// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR2& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart		// 分割数
)
{
	// 地面の生成
	CField *pField = new CField;
	if (pField == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 地面の初期化
		if (FAILED(pField->Init()))
		{ // 初期化に失敗した場合

			// 地面の破棄
			SAFE_DELETE(pField);
			return nullptr;
		}

		// テクスチャを登録・割当
		pField->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[texture]));

		// 位置を設定
		pField->SetVec3Position(rPos);

		// 向きを設定
		pField->SetVec3Rotation(rRot);

		// 大きさを設定
		pField->SetVec2Sizing(rSize);

		// 色を設定
		pField->SetColor(rCol);

		// 分割数を設定
		if (FAILED(pField->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// 地面の破棄
			SAFE_DELETE(pField);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pField;
	}
}
