//============================================================
//
//	ギミック処理 [gimmick.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "gimmick.h"

#include "gimmick_jumptable.h"
#include "gimmick_step.h"
#include "gimmick_fall.h"
#include "gimmick_post.h"
#include "gimmick_malti.h"
#include "gimmick_decayed.h"
#include "gimmick_jumpoff.h"
#include "gimmick_heavydoor.h"

#include "gimmick_gravel.h"
#include "gimmick_boob.h"
#include "gimmick_water.h"
#include "gimmick_cleen.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 2;				// ギミック範囲ポリゴンの優先順位
	const int INIT_NUM_ACTIVE = -1;		// 発動可能人数の初期値
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CGimmick>* CGimmick::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CGimmick] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGimmick::CGimmick() : CObject3D(CObject::LABEL_GIMMICK, CObject::DIM_3D, PRIORITY),
m_type(TYPE_JUMPTABLE),			// 種類
m_nNumActive(INIT_NUM_ACTIVE)	// 発動可能な分身の数
{

}

//============================================================
//	デストラクタ
//============================================================
CGimmick::~CGimmick()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGimmick::Init(void)
{
	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CGimmick>::Create();
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
void CGimmick::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGimmick::Update(const float fDeltaTime)
{
	// オブジェクト3Dの更新
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CGimmick::Draw(CShader* pShader)
{
	// オブジェクト3Dの描画
	CObject3D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CGimmick* CGimmick::Create
(
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rSize,		// サイズ
	const EType type,				// 種類
	const int nNumActive			// 発動可能な分身の数
)
{
	// 分身のUIの生成
	CGimmick* pGimmick = nullptr;

	switch (type)
	{
	case CGimmick::TYPE_JUMPTABLE:		// ジャンプ台

		pGimmick = new CGimmickJumpTable;

		break;

	case CGimmick::TYPE_STEP:			// 踏み台

		pGimmick = new CGimmickStep;

		break;

	case CGimmick::TYPE_FALL:			// 落とし穴

		pGimmick = new CGimmickFall;

		break;

	case CGimmick::TYPE_POST:			// 設置

		pGimmick = new CGimmickPost;

		break;

	case CGimmick::TYPE_MALTI:			// 複数管理

		pGimmick = new CGimmickMalti;

		break;

	case CGimmick::TYPE_DECAED:			// 朽ちた床

		pGimmick = new CGimmickDecaed;

		break;

	case CGimmick::TYPE_GRAVEL:			// 砂利道

		pGimmick = new CGimmickGravel;

		break;

	case CGimmick::TYPE_BOOB:			// ドブ

		pGimmick = new CGimmickBoob;

		break;

	case CGimmick::TYPE_WATER:			// 水場

		pGimmick = new CGimmickWater;

		break;

	case CGimmick::TYPE_JUMPOFF:			// 飛び降り

		pGimmick = new CGimmickJumpOff;

		break;

	case CGimmick::TYPE_HEAVYDOOR:			// 重い扉

		pGimmick = new CGimmickHeavyDoor;

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	if (pGimmick == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 分身のUIの初期化
		if (FAILED(pGimmick->Init()))
		{ // 初期化に失敗した場合

			// 分身のUIの破棄
			SAFE_DELETE(pGimmick);
			return nullptr;
		}

		// 位置を設定
		pGimmick->SetVec3Position(rPos);

		// サイズを設定
		pGimmick->SetVec3Sizing(rSize);

		// 種類を設定
		pGimmick->m_type = type;

		// 発動可能な分身の数を設定
		pGimmick->m_nNumActive = nNumActive;

#ifdef _DEBUG

		// 色の設定
		pGimmick->SetColor(XCOL_YELLOW);

#else

		// 色の設定
		pGimmick->SetColor(XCOL_AWHITE);

#endif // _DEBUG


		// 確保したアドレスを返す
		return pGimmick;
	}
}

//============================================================
// リスト取得
//============================================================
CListManager<CGimmick>* CGimmick::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}
