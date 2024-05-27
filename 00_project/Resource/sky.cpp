//============================================================
//
//	空処理 [sky.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sky.h"
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
		"data\\TEXTURE\\sky000.png",	// 通常テクスチャ
	};

	const int	PRIORITY	= 0;		// 空の優先順位
	const float	SKY_ROTMOVE	= 0.001f;	// 向き変更量
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CSky::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CSky> *CSky::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CSky] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSky::CSky() : CObjectMeshDome(CObject::LABEL_SKY, CObject::DIM_3D, PRIORITY),
	m_type	(TYPE_NORMAL)	// 種類

{

}

//============================================================
//	デストラクタ
//============================================================
CSky::~CSky()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSky::Init(void)
{
	// メンバ変数を初期化
	m_type = TYPE_NORMAL;	// 種類

	// オブジェクトメッシュドームの初期化
	if (FAILED(CObjectMeshDome::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャ方向を設定
	SetTexDir(CObjectMeshDome::TEXDIR_INSIDE);

	// レンダーステートの情報を取得
	CRenderState *pRenderState = GetRenderState();

	// ポリゴンの裏面のみを表示状態にする
	pRenderState->SetCulling(D3DCULL_CW);

	// ライティングをOFFにする
	pRenderState->SetLighting(false);

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CSky>::Create();
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
void CSky::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトメッシュドームの終了
	CObjectMeshDome::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSky::Update(const float fDeltaTime)
{
	// 変数を宣言
	D3DXVECTOR3 rot = GetVec3Rotation();	// 向き

	// 向きを変更
	rot.y -= SKY_ROTMOVE;

	// 向きを設定
	SetVec3Rotation(rot);

	// オブジェクトメッシュドームの更新
	CObjectMeshDome::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CSky::Draw(CShader *pShader)
{
	// オブジェクトメッシュドームの描画
	CObjectMeshDome::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CSky *CSky::Create
(
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const POSGRID2& rTexPart,	// テクスチャ分割数
	const float fRadius			// 半径
)
{
	// 空の生成
	CSky *pSky = new CSky;
	if (pSky == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 空の初期化
		if (FAILED(pSky->Init()))
		{ // 初期化に失敗した場合

			// 空の破棄
			SAFE_DELETE(pSky);
			return nullptr;
		}

		// 種類を設定
		pSky->SetType(type);

		// 位置を設定
		pSky->SetVec3Position(rPos);

		// 向きを設定
		pSky->SetVec3Rotation(rRot);

		// 色を設定
		pSky->SetColor(rCol);

		// 半径を設定
		pSky->SetRadius(fRadius);

		// 分割数を設定
		if (FAILED(pSky->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// 空の破棄
			SAFE_DELETE(pSky);
			return nullptr;
		}

		// テクスチャ分割数を設定
		pSky->SetTexPattern(rTexPart);

		// 確保したアドレスを返す
		return pSky;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CSky> *CSky::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	種類の設定処理
//============================================================
void CSky::SetType(const EType type)
{
	if (type > NONE_IDX && type < TYPE_MAX)
	{ // インデックスが範囲内の場合

		// 種類を保存
		m_type = type;

		// テクスチャを登録・割当
		BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[type]));
	}
	else { assert(false); }	// 範囲外
}
