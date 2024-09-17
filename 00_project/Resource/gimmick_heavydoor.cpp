//============================================================
//
// 重い扉の処理 [gimmick_heavydoor.cpp]
// Author：佐藤根詩音
// Adder ：藤田勇一
// Adder ：金崎朋弥
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
#include "multi_plant.h"
#include "sound.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 WAIT_OFFSET	= D3DXVECTOR3(65.0f, 0.0f, 65.0f);	// 待機位置のオフセット
	const D3DXVECTOR3 MOVEUP		= D3DXVECTOR3(0.0f, 60.0f, 0.0f);	// 扉が上がる移動量
	const float GRAVITY	= 360.0f;	// 重力
	const float CLONE_UP = 2.0f;	// 分身の身長に加算する値

	const CCamera::SSwing OPEN_SWING = CCamera::SSwing(9.0f, 2.0f, 0.1f);		// 開扉時の揺れの値
}

//============================================================
//	コンストラクタ
//============================================================
CGimmickHeavyDoor::CGimmickHeavyDoor() : CGimmickAction(),
m_pGateModel(nullptr),		// オブジェクトモデル(枠)の情報
m_pDoorModel(nullptr),		// オブジェクトモデル(扉)の情報
m_oldPosDoor(VEC3_ZERO),	// 過去位置
m_move(VEC3_ZERO),			// 移動量
m_state(STATE_NONE),		// 扉の状態
m_nIdxWait(0),				// 待機中心のインデックス
m_vecToWait(VEC3_ZERO)		// 中心座標から待機中心へのベクトル(単位ベクトル)
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

	// 自身の座標
	D3DXVECTOR3 pos = GetVec3Position();

	//-----------------------------------------------------------
	//	モデルの生成
	//-----------------------------------------------------------
	// 枠の生成
	m_pGateModel = CActor::Create(CActor::TYPE_GATE, pos);
	if (m_pGateModel == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 自動更新・破棄されるラベルを適応
	m_pGateModel->SetLabel(CObject::LABEL_GIMMICK);

	// 扉の生成
	m_pDoorModel = CActor::Create(CActor::TYPE_DOOR, pos);
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
	// 遠距離判定
	bool bFar = useful::IsNearPosR(GetVec3Position());
	if (!bFar) { return; }

	// 過去位置を更新
	m_oldPosDoor = m_pDoorModel->GetVec3Position();
	
	// 移動量加算
	D3DXVECTOR3 posDoor = m_pDoorModel->GetVec3Position();
	posDoor += m_move * fDeltaTime;

	switch (m_state)
	{
	case CGimmickHeavyDoor::STATE_NONE:		// 何もしてない
		break;

	case CGimmickHeavyDoor::STATE_CLOSE:	// 扉閉じてる

		if (IsActive())
		{ // 必要な人数が揃ってる場合

			m_move = MOVEUP;		// 移動量
			m_state = STATE_OPEN;	// 扉上げる

			// カメラ揺れを設定する
			CManager::GetInstance()->GetCamera()->SetSwing(CCamera::TYPE_MAIN, OPEN_SWING);

			// ドアが開く音を鳴らす
			PLAY_SOUND(CSound::LABEL_SE_DOOROPEN);
		}

		break;

	case CGimmickHeavyDoor::STATE_OPEN:		// 扉上げてる

		// 扉開く
		OpenTheDoor(posDoor);

		break;

	case CGimmickHeavyDoor::STATE_FULLY:	// 扉全開

		if (IsActive() == false)
		{ // 必要な人数が揃ってない場合

			// 扉閉める
			CloseTheDoor(posDoor);
		}

		break;

	default:
		assert(false);
		break;
	}

	// 位置設定
	m_pDoorModel->SetVec3Position(posDoor);

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

	// 方向の設定
	D3DXVECTOR3 rot = VEC3_ZERO;
	switch (GetAngle())
	{
	case EAngle::ANGLE_0: // 0
		rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f);
		break;

	case EAngle::ANGLE_90: // 1.57
		rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
		break;

	case EAngle::ANGLE_180: // 3.14
		rot = D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f);
		break;

	case EAngle::ANGLE_270: // 4.71
		rot = D3DXVECTOR3(0.0f, D3DX_PI * 1.5f, 0.0f);
		break;

	default:
		assert(false);
		break;
	}

	// 見た目の方向設定
	m_pGateModel->SetVec3Rotation(rot);
	m_pDoorModel->SetVec3Rotation(rot);

	// 見た目の位置設定
	m_pGateModel->SetVec3Position(rPos);
	m_pDoorModel->SetVec3Position(rPos);

	// 見た目の当たり判定オフセット設定処理
	m_pGateModel->CollisionOffset();
	m_pDoorModel->CollisionOffset();
}

//============================================================
// 大きさの設定処理
//============================================================
void CGimmickHeavyDoor::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// 親クラスの大きさ設定
	CObject3D::SetVec3Sizing(rSize);

	// 座標を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 花の生成情報を設定する
	D3DXVECTOR3 posPlant[2] = {};
	D3DXVECTOR3 sizePlant = {};
	if ((int)GetAngle() % 2)
	{
		posPlant[0] = pos + D3DXVECTOR3(rSize.x * 0.25f, 0.0f, 0.0f);
		posPlant[1] = pos - D3DXVECTOR3(rSize.x * 0.25f, 0.0f, 0.0f);
		sizePlant = D3DXVECTOR3(rSize.x * 0.5f, 0.0f, rSize.z);
	}
	else
	{
		posPlant[0] = pos + D3DXVECTOR3(0.0f, 0.0f, rSize.z * 0.25f);
		posPlant[1] = pos - D3DXVECTOR3(0.0f, 0.0f, rSize.z * 0.25f);
		sizePlant = D3DXVECTOR3(rSize.x, 0.0f, rSize.z * 0.5f);
	}

	// 花を生成
	for (int i = 0; i < 2; ++i)
	{
		CMultiPlant::Create(posPlant[i], sizePlant, GetType(), GetNumActive());
	}
	
	// 待機中心の設定
	CalcConectPoint();
}

//===========================================
//  各分身毎の待機位置を算出
//===========================================
D3DXVECTOR3 CGimmickHeavyDoor::CalcWaitPoint(const int Idx, const CPlayerClone* pClone)
{
	// 受け取ったインデックスが最大値を超えている場合警告
	if (Idx > GetNumActive()) { assert(false); }

	// 自身の位置を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 角度を取得
	EAngle angle = GetAngle();

	// インデックス番号が0の場合2点のうちプレイヤーに近い方を待機中心とする
	if (Idx == 0 && !IsActive())
	{
		// プレイヤー座標を取得
		D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();

		// プレイヤーと2点を結ぶベクトルを算出
		D3DXVECTOR3 vecToPlayer[2] = { posPlayer - m_ConectPoint[0], posPlayer - m_ConectPoint[1] };

		// 2点を結ぶベクトル
		D3DXVECTOR3 vecToWait = VEC3_ZERO;

		// 距離の2乗が小さい方の配列番号を保存
		if (vecToPlayer[0].x * vecToPlayer[0].x + vecToPlayer[0].z * vecToPlayer[0].z <=
			vecToPlayer[1].x * vecToPlayer[1].x + vecToPlayer[1].z * vecToPlayer[1].z)
		{
			m_nIdxWait = 0;
			vecToWait = m_ConectPoint[0] - m_ConectPoint[1];
		}
		else
		{
			m_nIdxWait = 1;
			vecToWait = m_ConectPoint[1] - m_ConectPoint[0];
		}

		// 待機中心同士を結ぶベクトルを算出し正規化する
		D3DXVec3Normalize(&m_vecToWait, &vecToWait);
	}

	// 最大数を取得
	int nNumActive = GetNumActive();

	// 最大数が0の場合関数を抜ける
	if (nNumActive == 0) { assert(false); return pos; }

	// 自身の横幅を取得
	D3DXVECTOR3 size = GetVec3Sizing();

	// 待機位置の算出
	D3DXVECTOR3 posWait = m_ConectPoint[m_nIdxWait];
	float fValue = 0.0f;
	switch (angle)
	{
	case EAngle::ANGLE_0: // 0
	case EAngle::ANGLE_180: // 3.14

		// 待機位置の相対値を算出
		fValue = (size.x / (float)(nNumActive + 1)) * (Idx + 1);

		// 待機位置を設定
		posWait.x += fValue - (size.x * 0.5f);
		break;

	case EAngle::ANGLE_90: // 1.57
	case EAngle::ANGLE_270: // 4.71

		// 待機位置の相対値を算出
		fValue = (size.z / (float)(nNumActive + 1)) * (Idx + 1);

		// 待機位置を設定
		posWait.z += fValue - (size.z * 0.5f);
		break;

	default:
		assert(false);
		break;
	}

	// 待機位置を返す
	return posWait;
}

//===========================================
//  各分身毎の待機向きを算出
//===========================================
D3DXVECTOR3 CGimmickHeavyDoor::CalcWaitRotation(const int Idx, const CPlayerClone* pClone)
{
	// 受け取ったインデックスが最大値を超えている場合警告
	if (Idx > GetNumActive()) { assert(false); }

	// 向きを求める
	D3DXVECTOR3 rot = VEC3_ZERO;
	rot.y = atan2f(m_vecToWait.x, m_vecToWait.z);

	// 算出した向きを返す
	return rot;
}

//===========================================
//  待機位置の計算処理
//===========================================
void CGimmickHeavyDoor::CalcConectPoint()
{
	// 自身の位置を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 自身のサイズを取得
	D3DXVECTOR3 size = GetVec3Sizing();

	// 自身の方向を取得
	EAngle angle = GetAngle();

	// 計算を行う
	switch (angle)
	{
		// x軸方向に架ける
	case ANGLE_90:
	case ANGLE_270:

		m_ConectPoint[0] = pos + D3DXVECTOR3(size.x * 0.5f, 0.0f, 0.0f);
		m_ConectPoint[1] = pos - D3DXVECTOR3(size.x * 0.5f, 0.0f, 0.0f);
		break;

		// z軸方向に架ける
	case ANGLE_0:
	case ANGLE_180:

		m_ConectPoint[0] = pos + D3DXVECTOR3(0.0f, 0.0f, size.z * 0.5f);
		m_ConectPoint[1] = pos - D3DXVECTOR3(0.0f, 0.0f, size.z * 0.5f);
		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
// 扉を上げる
//============================================================
void CGimmickHeavyDoor::OpenTheDoor(D3DXVECTOR3& rPos)
{
	if (rPos.y >= GetVec3Position().y + CPlayerClone::GetHeight() + CLONE_UP || IsActive() == false)
	{ // 一定時間経ったら

		m_move = VEC3_ZERO;		// 移動量
		m_state = STATE_FULLY;	// 扉全開状態
	}
}

//============================================================
// 扉を下げる
//============================================================
void CGimmickHeavyDoor::CloseTheDoor(D3DXVECTOR3& rPos)
{
	// 変数宣言
	CStage *pStage = GET_STAGE;	// ステージ情報

	// 重力
	m_move.y -= GRAVITY;

	// 範囲外の着地判定
	if (pStage->LandFieldPosition(rPos, m_oldPosDoor, m_move))
	{
		m_move = VEC3_ZERO;		// 移動量
		m_state = STATE_CLOSE;	// 扉閉じてる状態

		// ドアが閉じる音を鳴らす
		PLAY_SOUND(CSound::LABEL_SE_DOORCLOSE);
	}
}
