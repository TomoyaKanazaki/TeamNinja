//============================================================
//
//	壁処理 [wall.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "wall.h"
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
		"data\\TEXTURE\\soil000.png",	// 土テクスチャ
		"data\\TEXTURE\\soil001.png",	// 草土テクスチャ
		"data\\TEXTURE\\soil002.png",	// 草テクスチャ
	};

	const int PRIORITY = 0;	// 壁の優先順位
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CWall::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CWall> *CWall::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CWall] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CWall::CWall() : CObjectMeshWall(CObject::LABEL_WALL, CObject::DIM_3D, PRIORITY),
	m_type	(TYPE_SOIL)	// 種類
{

}

//============================================================
//	デストラクタ
//============================================================
CWall::~CWall()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CWall::Init(void)
{
	// メンバ変数を初期化
	m_type = TYPE_SOIL;	// 種類

	// オブジェクトメッシュウォールの初期化
	if (FAILED(CObjectMeshWall::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CWall>::Create();
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
void CWall::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトメッシュウォールの終了
	CObjectMeshWall::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CWall::Update(const float fDeltaTime)
{
	// オブジェクトメッシュウォールの更新
	CObjectMeshWall::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CWall::Draw(CShader *pShader)
{
	// オブジェクトメッシュウォールの描画
	CObjectMeshWall::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CWall *CWall::Create
(
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR2& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const POSGRID2& rTexPart	// テクスチャ分割数
)
{
	// 壁の生成
	CWall *pWall = new CWall;
	if (pWall == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 壁の初期化
		if (FAILED(pWall->Init()))
		{ // 初期化に失敗した場合

			// 壁の破棄
			SAFE_DELETE(pWall);
			return nullptr;
		}

		// 種類を設定
		pWall->SetType(type);

		// 位置を設定
		pWall->SetVec3Position(rPos);

		// 向きを設定
		pWall->SetVec3Rotation(rRot);

		// 大きさを設定
		pWall->SetVec2Sizing(rSize);

		// 色を設定
		pWall->SetColor(rCol);

		// 分割数を設定
		if (FAILED(pWall->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// 壁の破棄
			SAFE_DELETE(pWall);
			return nullptr;
		}

		// テクスチャ分割数を設定
		pWall->SetTexPattern(rTexPart);

		// 確保したアドレスを返す
		return pWall;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CWall> *CWall::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	種類の設定処理
//============================================================
void CWall::SetType(const EType type)
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
