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

#include "sceneGame.h"
#include "collManager.h"
#include "collisionCube.h"
#include "collisionCylinder.h"
#include "collisionSphere.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* MODEL[] =	// モデルのパス
	{
		"data\\MODEL\\Rock\\Moss-Rock000.x",		// 岩(小)
	};
	const int PRIORITY = 4;	// アクターの優先順位
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(MODEL) == CActor::TYPE_MAX, "ERROR : Type Count Mismatch");

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
CActor::CActor() : CObjectModel(CObject::LABEL_ACTOR, CObject::DIM_3D, PRIORITY),
m_type(TYPE_ROCK_S)		// 種類
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
	for (auto cube : m_cube)
	{
		// 終了処理
		cube->Uninit();
	}

	for (auto cylinder : m_cylinder)
	{
		// 終了処理
		cylinder->Uninit();
	}

	for (auto sphere : m_sphere)
	{
		// 終了処理
		sphere->Uninit();
	}

	// クリア処理
	m_cube.clear();
	m_cylinder.clear();
	m_sphere.clear();

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
	for (auto cube : m_cube)
	{
		// オフセット処理
		cube->OffSet(GetMtxWorld());
	}

	for (auto cylinder : m_cylinder)
	{
		// 終了処理
		cylinder->OffSet(GetMtxWorld());
	}

	for (auto sphere : m_sphere)
	{
		// 終了処理
		sphere->OffSet(GetMtxWorld());
	}

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
	const EType type,			// 種類
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
		pActor->BindModel(MODEL[type]);

		// 種類を設定
		pActor->m_type = type;

		// 当たり判定の割り当て処理
		pActor->BindCollision();

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
	bool& bJump,					// ジャンプ状況
	bool* pHit						// 当たったかどうかの判定
)
{
	D3DXVECTOR3 pos = GetVec3Position();	// 位置

	for (auto cube : m_cube)
	{
		// ヒット処理
		cube->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);

		if (pHit != nullptr)
		{ // ヒット状況が NULL じゃない場合

			// ヒット状況を true にする
			*pHit = true;
		}
	}

	for (auto cylinder : m_cylinder)
	{
		// ヒット処理
		cylinder->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);

		if (pHit != nullptr)
		{ // ヒット状況が NULL じゃない場合

			// ヒット状況を true にする
			*pHit = true;
		}
	}

	for (auto sphere : m_sphere)
	{
		// ヒット処理
		sphere->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);

		if (pHit != nullptr)
		{ // ヒット状況が NULL じゃない場合

			// ヒット状況を true にする
			*pHit = true;
		}
	}
}

//============================================================
// 当たり判定の割り当て処理
//============================================================
void CActor::BindCollision(void)
{
	// 当たり判定を取得
	CCollManager::SCollision coll = CSceneGame::GetCollManager()->GetCollInfo(m_type);

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_cube.size()); nCnt++)
	{
		// キューブの情報を追加する
		m_cube.push_back(CCollisionCube::Create
		(
			GetVec3Position(),				// 位置
			coll.m_cube[nCnt].offset,		// オフセット座標
			coll.m_cube[nCnt].fWidth,		// 幅
			coll.m_cube[nCnt].fHeight,		// 高さ
			coll.m_cube[nCnt].fDepth,		// 奥行
			GetVec3Rotation().y)			// 向き
		);
	}

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_cylinder.size()); nCnt++)
	{
		// シリンダーの情報を追加する
		m_cylinder.push_back(CCollisionCylinder::Create
		(
			GetVec3Position(),				// 位置
			coll.m_cylinder[nCnt].offset,	// オフセット座標
			coll.m_cylinder[nCnt].fRadius,	// 半径
			coll.m_cylinder[nCnt].fHeight	// 高さ
		));
	}

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_sphere.size()); nCnt++)
	{
		// スフィアの情報を追加する
		m_sphere.push_back(CCollisionSphere::Create
		(
			GetVec3Position(),				// 位置
			coll.m_sphere[nCnt].offset,		// オフセット座標
			coll.m_sphere[nCnt].fRadius		// 半径
		));
	}
}
