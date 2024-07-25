//============================================================
//
//	敵の持ち物処理 [enemy_item.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "enemy_item.h"
#include "renderer.h"
#include "model.h"

#include "enemy_katana.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* MODEL[] =		// モデルのパス
	{
		"data\\MODEL\\ENEMY\\SAMURAI\\13_katana.x",		// 刀
	};

	const int PRIORITY = 3;		// 優先度
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(MODEL) == CEnemyItem::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	子クラス [CEnemyItem] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyItem::CEnemyItem() : CMultiModel(CObject::LABEL_NONE, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
m_type(TYPE_KATANA)		// 種類
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyItem::~CEnemyItem()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyItem::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CMultiModel::Init()))
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
void CEnemyItem::Uninit(void)
{
	// オブジェクトモデルの終了
	CMultiModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyItem::Update(const float fDeltaTime)
{
	// オブジェクトモデルの更新
	CMultiModel::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CEnemyItem::Draw(CShader* pShader)
{
	// オブジェクトモデルの描画
	CMultiModel::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CEnemyItem* CEnemyItem::Create
( // 引数
	const EType type,				// 種類
	const D3DXVECTOR3& rOffset,		// オフセット
	const D3DXVECTOR3& rRot			// 向き
)
{
	// モデルUIの生成
	CEnemyItem* pItem = nullptr;

	switch (type)
	{
	case CEnemyItem::TYPE_KATANA:

		// 刀を生成
		pItem = new CEnemyKatana;

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	if (pItem == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// アクターの初期化
		if (FAILED(pItem->Init()))
		{ // 初期化に失敗した場合

			// アクターの破棄
			SAFE_DELETE(pItem);
			return nullptr;
		}

		// モデルの割り当て処理
		pItem->BindModel(MODEL[type]);

		// オフセット座標を設定
		pItem->SetVec3Position(rOffset);

		// 向きを設定
		pItem->SetVec3Rotation(rRot);

		// 種類を設定
		pItem->m_type = type;

		// 確保したアドレスを返す
		return pItem;
	}
}
