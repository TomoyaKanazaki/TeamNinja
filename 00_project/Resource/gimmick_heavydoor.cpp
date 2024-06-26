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
#include "actor.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 MOVEUP	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 扉が上がる移動量
	const D3DXVECTOR3 MOVEDOWN	= D3DXVECTOR3(0.0f, 12.0f, 0.0f);	// 扉が下がる移動量
	const float GRAVITY	= 360.0f;	// 重力
	const float CLONE_UP = 10.0f;	// 分身の身長に加算する値
}

//============================================================
//	コンストラクタ
//============================================================
CGimmickHeavyDoor::CGimmickHeavyDoor() : CGimmickAction(),
m_pGateModel(nullptr),		// オブジェクトモデル(枠)の情報
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
	m_pGateModel = nullptr;		// 枠モデルの情報
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
	// 枠の生成
	m_pGateModel = CActor::Create(CActor::TYPE_GATE, GetVec3Position(), VEC3_ZERO);
	if (m_pGateModel == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 自動更新・破棄されるラベルを適応
	m_pGateModel->SetLabel(CObject::LABEL_GIMMICK);

	// 扉の生成
	m_pDoorModel = CActor::Create(CActor::TYPE_DOOR, GetVec3Position(), VEC3_ZERO);
	if (m_pDoorModel == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

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
	// 枠モデルの終了
	SAFE_UNINIT(m_pGateModel);

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
	
	// 移動量加算
	D3DXVECTOR3 posDoor = m_pDoorModel->GetVec3Position();
	posDoor += m_move * fDeltaTime;

	// 位置設定
	m_pDoorModel->SetVec3Position(posDoor);

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

	// 枠の位置設定
	m_pGateModel->SetVec3Position(rPos + D3DXVECTOR3(0.0f, 0.0f, 100.0f));

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

//===========================================
//  各分身毎の待機位置を算出
//===========================================
D3DXVECTOR3 CGimmickHeavyDoor::CalcWaitPoint(const int Idx)
{
	// 受け取ったインデックスが最大値を超えている場合警告
	if (Idx > GetNumActive()) { assert(false); }

	// 自身の位置を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 最大数を取得
	int nNumActive = GetNumActive();

	// 最大数が0の場合関数を抜ける
	if (nNumActive == 0) { assert(false); return pos; }

	// 自身の横幅を取得
	D3DXVECTOR3 size = GetVec3Sizing();

	// 待機位置の相対値を算出
	float fValue = (size.x / (float)(nNumActive + 1)) * (Idx + 1);

	// 待機位置を設定
	D3DXVECTOR3 posWait = pos;
	posWait.x += fValue - (size.x * 0.5f);

	return posWait;
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

	if (posDoor.y >= GetVec3Position().y + CPlayerClone::GetHeight() + CLONE_UP || IsActive() == false)
	{ // 一定時間経ったら

		m_state = STATE_FULLY;	// 扉全開状態
	}
}

//============================================================
// 扉を下げる
//============================================================
void CGimmickHeavyDoor::CloseTheDoor(void)
{
	// 変数宣言
	CStage *pStage = GET_MANAGER->GetScene()->GetStage();	// ステージ情報
	D3DXVECTOR3 posDoor = m_pDoorModel->GetVec3Position();	// 位置

	// 重力
	m_move.y -= GRAVITY;

	// 範囲外の着地判定
	if (pStage->LandFieldPosition(posDoor, m_oldPosDoor, m_move))	// TODO：バウンドさせよう
	{
		m_pDoorModel->SetVec3Position(posDoor);	// 位置
		m_state = STATE_CLOSE;	// 扉閉じてる状態
	}
}
