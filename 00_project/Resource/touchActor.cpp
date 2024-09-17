//============================================================
//
//	タッチアクター処理 [touchActor.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "touchActor.h"

#include "touchActorCan.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* MODEL[] =	// モデルのパス
	{
		"data\\MODEL\\Rock\\Rock000.x",		// 缶
		"data\\MODEL\\Rock\\Rock001.x",		// 鳥
	};
	const int PRIORITY = 4;	// アクターの優先順位
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(MODEL) == CTouchActor::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
// 静的メンバ変数宣言
//************************************************************
CListManager<CTouchActor>* CTouchActor::m_pList = nullptr;		// リスト構造

//************************************************************
//	子クラス [CTouchActor] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTouchActor::CTouchActor() : CObjectModel(CObject::LABEL_TOUCHACTOR, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
m_type(TYPE_CAN),		// 種類
m_state(STATE_NONE)		// 状態
{

}

//============================================================
//	デストラクタ
//============================================================
CTouchActor::~CTouchActor()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTouchActor::Init(void)
{
	// 値を初期化
	m_state = STATE_NONE;		// 状態

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
		m_pList = CListManager<CTouchActor>::Create();

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
void CTouchActor::Uninit(void)
{
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
void CTouchActor::Update(const float fDeltaTime)
{
	// 遠距離判定
	bool bFar = useful::IsNearPosR(GetVec3Position());
	SetEnableDraw(bFar);
	if (!bFar) { return; }

	switch (m_state)
	{
	case CTouchActor::STATE_NONE:

		// 通常状態更新処理
		UpdateNone(fDeltaTime);

		break;

	case CTouchActor::STATE_ACT:

		// アクション状態更新処理
		UpdateAct(fDeltaTime);

		break;

	default:
		assert(false);
		break;
	}

	// オブジェクトモデルの更新
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CTouchActor::Draw(CShader* pShader)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CTouchActor* CTouchActor::Create
( // 引数
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR3& rScale	// 拡大率
)
{
	// モデルUIの生成
	CTouchActor* pActor = nullptr;

	switch (type)
	{
	case CTouchActor::TYPE_CAN:

		pActor = new CTouchCan;

		break;

	case CTouchActor::TYPE_BIRD:

		//pActor = new CTouchActor;

		break;
	default:
		assert(false);
		break;
	}

	// アクターの初期化
	if (FAILED(pActor->Init()))
	{ // 初期化に失敗した場合

		// アクターの破棄
		SAFE_DELETE(pActor);
		return nullptr;
	}

	// モデルの割り当て処理
	pActor->BindModel(MODEL[type]);

	// 向きを設定
	pActor->SetVec3Rotation(rRot);

	// 位置を設定
	pActor->SetVec3Position(rPos);

	// 拡大率を設定
	pActor->SetVec3Scaling(rScale);

	// 種類を設定
	pActor->m_type = type;

	// 確保したアドレスを返す
	return pActor;
}

//============================================================
// リスト構造の取得処理
//============================================================
CListManager<CTouchActor>* CTouchActor::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}
