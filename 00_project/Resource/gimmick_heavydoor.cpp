//============================================================
//
// 重い扉の処理 [gimmick_heavydoor.cpp]
// Author：佐藤根詩音
// Adder ：藤田勇一
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
	const char* MODEL_ROOF_PASS	= "data\\MODEL\\DOOR\\gate000.x";	// 重い扉(屋根)
	const char* MODEL_DOOR_PASS	= "data\\MODEL\\DOOR\\gate001.x";	// 重い扉(扉)

	const D3DXVECTOR3 MOVEUP	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 扉が上がる移動量
	const D3DXVECTOR3 MOVEDOWN	= D3DXVECTOR3(0.0f, 12.0f, 0.0f);	// 扉が下がる移動量
	const float GRAVITY	= 60.0f;	// 重力
}

//============================================================
//	コンストラクタ
//============================================================
CGimmickHeavyDoor::CGimmickHeavyDoor() : CGimmickAction(),
m_pRoofModel(nullptr),		// オブジェクトモデル(屋根)の情報
m_pDoorModel(nullptr),		// オブジェクトモデル(扉)の情報
m_oldPosDoor(VEC3_ZERO),	// 過去位置
m_move(VEC3_ZERO),			// 移動量
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
	m_pRoofModel = nullptr;		// 屋根モデルの情報
	m_pDoorModel = nullptr;		// 扉モデルの情報
	m_oldPosDoor = VEC3_ZERO;	// 過去位置
	m_move = VEC3_ZERO;			// 移動量
	m_state = STATE_CLOSE;		// 扉の状態

	// ギミックアクションの初期化
	if (FAILED(CGimmickAction::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//-----------------------------------------------------------
	//	モデルの生成
	//-----------------------------------------------------------
	// 屋根の生成
	m_pRoofModel = CObjectModel::Create(VEC3_ZERO, VEC3_ZERO);
	if (m_pRoofModel == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 屋根モデルを割当
	m_pRoofModel->BindModel(MODEL_ROOF_PASS);

	// 自動更新・破棄されるラベルを適応
	m_pRoofModel->SetLabel(CObject::LABEL_GIMMICK);

	// 扉の生成
	m_pDoorModel = CObjectModel::Create(VEC3_ZERO, VEC3_ZERO);
	if (m_pDoorModel == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 扉モデルを割当
	m_pDoorModel->BindModel(MODEL_DOOR_PASS);

	// 自動更新・破棄されるラベルを適応
	m_pDoorModel->SetLabel(CObject::LABEL_GIMMICK);

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

	// ギミックアクションの終了
	CGimmickAction::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGimmickHeavyDoor::Update(const float fDeltaTime)
{
	// 過去位置を更新
	m_oldPosDoor = m_pDoorModel->GetVec3Position();

	switch (m_state)
	{
	case CGimmickHeavyDoor::STATE_NONE:		// 何もしてない
		break;

	case CGimmickHeavyDoor::STATE_CLOSE:	// 扉閉じてる

		if (IsActive())
		{ // 必要な人数が揃ってる場合

			m_state = STATE_OPEN;	// 扉上げる
			m_move = MOVEUP;		// 移動量
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
// 位置の設定処理
//============================================================
void CGimmickHeavyDoor::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 親クラスの位置設定
	CGimmickAction::SetVec3Position(rPos);

	// 屋根の位置設定
	m_pRoofModel->SetVec3Position(rPos + D3DXVECTOR3(0.0f, 0.0f, 100.0f));

	// 扉の位置設定
	m_pDoorModel->SetVec3Position(rPos + D3DXVECTOR3(0.0f, 0.0f, 100.0f));
}

//============================================================
// 大きさの設定処理
//============================================================
void CGimmickHeavyDoor::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// 親クラスの大きさ設定
	CGimmickAction::SetVec3Sizing(rSize);
}

//============================================================
// 扉を上げる
//============================================================
void CGimmickHeavyDoor::OpenTheDoor(void)
{
	D3DXVECTOR3 posDoor = m_pDoorModel->GetVec3Position();	// 位置

	// 移動量加算
	posDoor += m_move;

	// 位置設定
	m_pDoorModel->SetVec3Position(posDoor);

	if (posDoor.y >= GetVec3Position().y + 100.0f || IsActive() == false)	// TODO：上げる量は定数に
	{ // 一定時間経ったら

		m_state = STATE_FULLY;	// 扉全開状態
	}
}

//============================================================
// 扉を下げる
//============================================================
void CGimmickHeavyDoor::CloseTheDoor(void)
{
	CStage *pStage = GET_MANAGER->GetScene()->GetStage();	// ステージ情報
	D3DXVECTOR3 posDoor = m_pDoorModel->GetVec3Position();	// 位置

	// 重力
	m_move.y -= GRAVITY;

	// 移動量加算
	posDoor += m_move;

	// 範囲外の着地判定
	if (pStage->LandFieldPosition(posDoor, m_oldPosDoor, m_move))	// TODO：バウンドさせよう
	{
		m_state = STATE_CLOSE;	// 扉閉じてる状態
	}

	// 位置設定
	m_pDoorModel->SetVec3Position(posDoor);
}
