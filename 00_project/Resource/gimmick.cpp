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

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY = 2;			// ギミック範囲ポリゴンの優先順位
	const D3DXVECTOR3 RADIUS = D3DXVECTOR3(70.0f, 0.0f, 70.0f);		// 半径(サイズ)
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
m_type(TYPE_JUMPTABLE),
m_pClonelist(nullptr)
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
CGimmick* CGimmick::Create(const D3DXVECTOR3& rPos, const EType type)
{
	// 分身のUIの生成
	CGimmick* pGimmick = nullptr;

	switch (type)
	{
	case CGimmick::TYPE_JUMPTABLE:		// ジャンプ台

		pGimmick = new CGimmickJumpTable;

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
		pGimmick->SetVec3Sizing(RADIUS);

		// 種類を設定
		pGimmick->SetType(type);

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

////==========================================
//// ギミックの当たり判定
////==========================================
//void CPlayer::CollisionGimmick(void)
//{
//	// ギミックがなかった場合抜ける
//	if (CGimmick::GetList() == nullptr) { return; }
//
//	// リストを取得
//	std::list<CGimmick*> list = CGimmick::GetList()->GetList();
//	auto gimBegin = list.begin();
//	auto gimEnd = list.end();
//	D3DXVECTOR3 pos = GetVec3Position();
//	D3DXVECTOR3 size = D3DXVECTOR3(RADIUS, 0.0f, RADIUS);
//	D3DXVECTOR3 posGim = VEC3_ZERO;
//	D3DXVECTOR3 sizeGim = VEC3_ZERO;
//
//	for (auto gim : list)
//	{
//		posGim = gim->GetVec3Position();
//		sizeGim = gim->GetVec3Sizing();
//
//		if (collision::Box2D
//		(
//			pos,		// 判定位置
//			posGim,		// 判定目標位置
//			size,		// 判定サイズ(右・上・後)
//			-size,		// 判定サイズ(左・下・前)
//			sizeGim,	// 判定目標サイズ(右・上・後)
//			-sizeGim	// 判定目標サイズ(左・下・前)
//		))
//		{ // 四角の中に入った場合
//
//
//		}
//	}
//}