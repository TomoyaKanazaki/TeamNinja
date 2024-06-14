//============================================================
//
//	敵の持ち物処理 [enemy_delongings.cpp]
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

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* MODEL[] =		// モデルのパス
	{
		"data\\MODEL\\Rock\\Moss-Rock000.x",		// 岩(小)
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
CEnemyItem::CEnemyItem() : CObjectModel(CObject::LABEL_ACTOR, CObject::DIM_3D, PRIORITY),
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
	if (FAILED(CObjectModel::Init()))
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
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyItem::Update(const float fDeltaTime)
{
	// オブジェクトモデルの更新
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CEnemyItem::Draw(CShader* pShader)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CEnemyItem* CEnemyItem::Create
( // 引数
	const EType type,				// 種類
	const D3DXVECTOR3& rOffset,		// オフセット
	const D3DXMATRIX& rMtx			// マトリックス情報
)
{
	// モデルUIの生成
	CEnemyItem* pItem = new CEnemyItem;

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
		pItem->m_offset = rOffset;

		// 種類を設定
		pItem->m_type = type;

		// オフセット処理
		pItem->Offset(rMtx);

		// 確保したアドレスを返す
		return pItem;
	}
}

//============================================================
// オフセット処理
//============================================================
void CEnemyItem::Offset(const D3DXMATRIX& rMtx)
{
	// 計算用マトリックス
	D3DXMATRIX mtxTrans, mtxColl;

	// マトリックスの初期化
	D3DXMatrixIdentity(&mtxColl);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_offset.x, m_offset.y, m_offset.z);
	D3DXMatrixMultiply(&mtxColl, &mtxColl, &mtxTrans);

	// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
	D3DXMatrixMultiply
	(
		&mtxColl,
		&mtxColl,
		&rMtx
	);

	// 位置を設定する
	SetVec3Position(D3DXVECTOR3(mtxColl._41, mtxColl._42, mtxColl._43));
}
