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

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 MOVEUP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 扉が上がる移動量
	const D3DXVECTOR3 MOVEDOWN = D3DXVECTOR3(0.0f, 20.0f, 0.0f);	// 扉が下がる移動量
	const float GRAVITY = 60.0f;		// 重力

	const int OPEN_DOOR = 60 * 5;		// ドアが開くまでの時間
}
//============================================================
//	コンストラクタ
//============================================================
CGimmickHeavyDoor::CGimmickHeavyDoor() : CGimmickAction(),
m_pMapModel(nullptr),		// マップモデルの情報
m_pStage(nullptr),			// ステージの情報
m_move(VEC3_ZERO),			// 移動量
m_nDoorCounter(0),			// 扉の開閉カウンター
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
	m_pMapModel = nullptr;	// マップモデルの情報
	m_pStage = CManager::GetInstance()->GetScene()->GetStage();		// ステージの情報
	m_move = VEC3_ZERO;		// 移動量
	m_nDoorCounter = 0;		// 扉の開閉カウンター
	m_state = STATE_CLOSE;	// 扉の状態

	// 扉の生成
	m_pMapModel = CMapModel::Create
	(
		D3DXVECTOR3(-1400.0f, 0.0f, -200.0f),
		VEC3_ZERO,
		CMapModel::MODEL_TYPE_DOOR01
	);

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
	// マップモデルの終了
	SAFE_UNINIT(m_pMapModel);

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
	pos = m_pMapModel->GetVec3Position();

	// 位置更新
	pos += m_move;

	// 位置設定
	m_pMapModel->SetVec3Position(pos);

	if (m_nDoorCounter >= OPEN_DOOR || IsActive() == false)
	{ // 一定時間経ったら

		m_state = STATE_FULLY;		// 扉全開状態
		m_nDoorCounter = 0;			// カウンター初期化
		m_move = MOVEDOWN;			// 移動量

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
	pos = m_pMapModel->GetVec3Position();

	// 位置更新
	pos += m_move;

	// 範囲外の着地判定
	if (m_pStage->LandLimitPosition(pos, m_move, 0.0f))
	{
		m_state = STATE_CLOSE;	// 扉閉じてる状態
		m_nDoorCounter = 0;		// カウンター初期化

		// 扉開けてない
		SetMoment(false);
	}

	// 位置設定
	m_pMapModel->SetVec3Position(pos);

	// 重力
	m_move.y -= GRAVITY;
}
