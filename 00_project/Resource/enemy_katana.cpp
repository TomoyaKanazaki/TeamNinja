//============================================================
//
//	敵の刀処理 [enemy_katana.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "enemy_katana.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{

}

//************************************************************
//	子クラス [CEnemyKatana] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyKatana::CEnemyKatana() : CEnemyItem()
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyKatana::~CEnemyKatana()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyKatana::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CEnemyItem::Init()))
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
void CEnemyKatana::Uninit(void)
{
	// オブジェクトモデルの終了
	CEnemyItem::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyKatana::Update(const float fDeltaTime)
{
	// オブジェクトモデルの更新
	CEnemyItem::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CEnemyKatana::Draw(CShader* pShader)
{
	// オブジェクトモデルの描画
	CEnemyItem::Draw(pShader);
}