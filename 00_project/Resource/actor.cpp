//============================================================
//
//	アクター処理 [actor.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "actor.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"

#include "collisionCube.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* MODEL = "data\\MODEL\\Rock\\Moss-Rock000.x";	// モデルのパス
	const int PRIORITY = 4;	// アクターの優先順位
}

//************************************************************
// 静的メンバ変数宣言
//************************************************************
CListManager<CActor>* CActor::m_pList = nullptr;		// リスト構造

//************************************************************
//	子クラス [CActor] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CActor::CActor() : CObjectModel(CObject::LABEL_ACTOR, CObject::DIM_3D, PRIORITY)
{

}

//============================================================
//	デストラクタ
//============================================================
CActor::~CActor()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CActor::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CActor>::Create();

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
void CActor::Uninit(void)
{
	// 総数を取得
	int nNumColl = m_collision.size();

	for (int nCnt = 0; nCnt < nNumColl; nCnt++)
	{
		// 終了処理
		(*m_collision.begin())->Uninit();
	}

	// 当たり判定情報の破棄
	m_collision.clear();

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CActor::Update(const float fDeltaTime)
{
	// オブジェクトモデルの更新
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CActor::Draw(CShader* pShader)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CActor* CActor::Create
( // 引数
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR3& rScale	// 大きさ
)
{
	// モデルUIの生成
	CActor* pActor = new CActor;

	if (pActor == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// アクターの初期化
		if (FAILED(pActor->Init()))
		{ // 初期化に失敗した場合

			// アクターの破棄
			SAFE_DELETE(pActor);
			return nullptr;
		}

		// 位置を設定
		pActor->SetVec3Position(rPos);

		// 向きを設定
		pActor->SetVec3Rotation(rRot);

		// 拡大率を設定
		pActor->SetVec3Scaling(rScale);

		// モデルの割り当て処理
		pActor->BindModel(MODEL);

		// TODO：仮の当たり判定を一個追加
		pActor->m_collision.push_back(CCollisionCube::Create(rPos, 40.0f, 80.0f, 40.0f));

		// 確保したアドレスを返す
		return pActor;
	}
}

//============================================================
// リスト構造の取得処理
//============================================================
CListManager<CActor>* CActor::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}

//============================================================
// 当たり判定処理
//============================================================
void CActor::Collision
(
	D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight,			// 高さ
	D3DXVECTOR3& rMove,				// 移動量
	bool& bJump						// ジャンプ状況
)
{
	D3DXVECTOR3 pos = GetVec3Position();	// 位置

	for (auto collision : m_collision)
	{
		// ヒット処理
		collision->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);
	}
}
