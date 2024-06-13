//============================================================
//
// 重い扉の処理 [gimmick_heavydoor.cpp]
// Author：佐藤根詩音
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "gimmick_heavydoor.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"
#include "debugproc.h"
#include "MapModel.h"
#include "stage.h"
#include "scene.h"
#include "debug.h"
#include "debugproc.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 MOVEUP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 扉が上がる移動量
	const D3DXVECTOR3 MOVEDOWN = D3DXVECTOR3(0.0f, 20.0f, 0.0f);	// 扉が下がる移動量
	const float GRAVITY = 60.0f;		// 重力

	const int OPEN_DOOR = 60 * 5;		// ドアが開くまでの時間
	const int NUM_BOUND = 5;			// 扉が跳ねる回数
}
//============================================================
//	コンストラクタ
//============================================================
CGimmickHeavyDoor::CGimmickHeavyDoor() : CGimmickAction(),
m_pRoofModel(nullptr),	// オブジェクトモデル(屋根)の情報
m_pDoorModel(nullptr),	// オブジェクトモデル(扉)の情報
m_pStage(nullptr),			// ステージの情報
m_pos(VEC3_ZERO),			// 扉の位置
m_posSave(VEC3_ZERO),		// 位置保存用
m_move(VEC3_ZERO),			// 移動量
m_nDoorCounter(0),			// 扉の開閉カウンター
m_nNumBound(0),				// 扉が跳ねた回数
m_state(STATE_NONE)			// 扉の状態
{

}

//============================================================
//	デストラクタ
//============================================================
CGimmickHeavyDoor::~CGimmickHeavyDoor()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGimmickHeavyDoor::Init(void)
{
	// 変数初期化
	m_pRoofModel = nullptr;	// 屋根モデルの情報
	m_pDoorModel = nullptr;	// 扉モデルの情報
	m_pStage = CManager::GetInstance()->GetScene()->GetStage();		// ステージの情報
	m_pos = D3DXVECTOR3(-1400.0f, 0.0f, -200.0f);	// 扉の位置
	m_posSave = VEC3_ZERO;	// 位置保存用
	m_move = VEC3_ZERO;		// 移動量
	m_nDoorCounter = 0;		// 扉の開閉カウンター
	m_nNumBound = 1;		// 扉が跳ねた回数
	m_state = STATE_CLOSE;	// 扉の状態


	//-----------------------------------------------------------
	//	モデルの生成
	//-----------------------------------------------------------

	// 屋根の生成
	m_pRoofModel = CMapModel::Create
	(
		m_pos,
		VEC3_ZERO,
		CMapModel::MODEL_TYPE_DOOR00
	);
	if (m_pRoofModel == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 扉の生成
	m_pDoorModel = CMapModel::Create
	(
		m_pos,
		VEC3_ZERO,
		CMapModel::MODEL_TYPE_DOOR01
	);
	if (m_pDoorModel == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ギミックアクションの初期化
	if (FAILED(CGimmickAction::Init()))
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
void CGimmickHeavyDoor::Uninit(void)
{
	// 屋根モデルの終了
	SAFE_UNINIT(m_pRoofModel);

	// 扉モデルの終了
	SAFE_UNINIT(m_pDoorModel);

	// ステージの終了
	SAFE_UNINIT(m_pStage);

	// ギミックアクションの終了
	CGimmickAction::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGimmickHeavyDoor::Update(const float fDeltaTime)
{
	switch (m_state)
	{
	case CGimmickHeavyDoor::STATE_NONE:		// 何もしてない
		break;
	case CGimmickHeavyDoor::STATE_CLOSE:	// 扉閉じてる

		if (IsActive())
		{ // 必要な人数が揃ってる場合

			m_state = STATE_OPEN;		// 扉上げる
			m_move = MOVEUP;			// 移動量

			// 扉上げてる
			SetMoment(true);
		}

		break;

	case CGimmickHeavyDoor::STATE_OPEN:		// 扉上げてる

		// 扉開く
		OpenTheDoor();

		break;

	case CGimmickHeavyDoor::STATE_FULLY:	// 扉全開

		if (IsActive() == false)
		{ // 必要な人数が揃ってない場合

			// 扉閉める
			CloseTheDoor();
		}

		break;

	default:
		assert(false);
		break;
	}

	// ギミックアクションの更新
	CGimmickAction::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CGimmickHeavyDoor::Draw(CShader* pShader)
{
	// ギミックアクションの描画
	CGimmickAction::Draw(pShader);
}

//============================================================
// 扉を上げる
//============================================================
void CGimmickHeavyDoor::OpenTheDoor(void)
{
	D3DXVECTOR3 pos;		// 位置

	// 位置取得
	pos = m_pDoorModel->GetVec3Position();

	// 移動量加算
	pos += m_move;
	m_posSave += m_move;	// 位置保存

	// 位置設定
	m_pDoorModel->SetVec3Position(pos);

	if (m_nDoorCounter >= OPEN_DOOR || IsActive() == false)
	{ // 一定時間経ったら

		m_state = STATE_FULLY;		// 扉全開状態
		m_nDoorCounter = 0;			// カウンター初期化
		m_move = m_posSave;			// 移動量

	}
	else
	{ // 時間経ってない場合

		m_nDoorCounter++;
	}
}

//============================================================
// 扉を下げる
//============================================================
void CGimmickHeavyDoor::CloseTheDoor(void)
{
	D3DXVECTOR3 pos;		// 位置

	// 位置取得
	pos = m_pDoorModel->GetVec3Position();

	// 重力
	m_move.y -= GRAVITY;

	// 移動量加算
	pos += m_move;

	// 範囲外の着地判定
	if (m_pStage->LandLimitPosition(pos, m_move, 0.0f))
	{
		if (m_nNumBound <= NUM_BOUND)
		{ // 跳ねた回数が目標の回数じゃない場合

			m_nNumBound++;		// 跳ねた回数加算

			// 跳ねる高さ設定
			m_move.y = m_posSave.y / m_nNumBound;
		}
		else
		{ // 跳ね切ってない場合

			m_state = STATE_CLOSE;	// 扉閉じてる状態
			m_posSave = VEC3_ZERO;	// 保存用の位置を初期化

			m_nNumBound = 1;

			// 扉開けてない
			SetMoment(false);
		}
	}

	// 位置設定
	m_pDoorModel->SetVec3Position(pos);
}
