//============================================================
//
//	セレクト画面プレイヤー処理 [playerSelect.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "playerSelect.h"
#include "manager.h"
#include "stage.h"
#include "camera.h"
#include "transpoint.h"
#include "sound.h"
#include "sceneSelect.h"
#include "selectManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{

}

//************************************************************
//	子クラス [CPlayerSelect] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayerSelect::CPlayerSelect()
{

}

//============================================================
//	デストラクタ
//============================================================
CPlayerSelect::~CPlayerSelect()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayerSelect::Init(void)
{
	// プレイヤーの初期化
	if (FAILED(CPlayer::Init()))
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
void CPlayerSelect::Uninit(void)
{
	// プレイヤーの終了
	CPlayer::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayerSelect::Update(const float fDeltaTime)
{
	// プレイヤーの更新
	CPlayer::Update(fDeltaTime);
}

//============================================================
//	状態の更新処理
//============================================================
CPlayer::EMotion CPlayerSelect::UpdateState(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション
	switch (GetState())
	{ // 状態ごとの処理
	case STATE_NONE:

		// 何もしない状態時の更新
		currentMotion = UpdateNone(fDeltaTime);
		break;

	case STATE_SELECT_SPAWN:

		// スポーン状態時の更新
		currentMotion = UpdateSpawn(fDeltaTime);
		break;

	case STATE_SELECT_NORMAL:

		// 通常状態時の更新
		currentMotion = UpdateNormal(fDeltaTime);
		break;

	case STATE_SELECT_WAIT:

		// 待機状態時の更新
		currentMotion = UpdateWait(fDeltaTime);
		break;

	case STATE_SELECT_ENTER:

		// 入場状態時の更新
		currentMotion = UpdateEnter(fDeltaTime);
		break;

	case STATE_SELECT_OPEN:

		// 解放状態時の更新
		currentMotion = UpdateOpen(fDeltaTime);
		break;

	default:
		assert(false);
		break;
	}

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	スポーン状態時の更新処理
//============================================================
CPlayer::EMotion CPlayerSelect::UpdateSpawn(const float fDeltaTime)
{
	if (IsMotionFinish())
	{ // 選択モーションが終了した場合

		// 待機モーションを設定
		SetMotion(MOTION_IDOL);

		if (!GET_STAGE->GetOpenMapDirectory().empty()
		&&  GET_RETENTION->GetWin() == CRetentionManager::WIN_SUCCESS)
		{ // 解放したマップがある場合

			// 解放カメラに遷移
			GET_CAMERA->SetState(CCamera::STATE_OPEN);
		}
		else
		{ // 解放したマップがない場合

			// 回り込みカメラに遷移
			GET_CAMERA->SetState(CCamera::STATE_AROUND);

			// 通常状態を設定
			SetState(STATE_SELECT_NORMAL);
		}
	}

	// 待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
//	通常状態時の更新処理
//============================================================
CPlayer::EMotion CPlayerSelect::UpdateNormal(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;		// 現在のモーション
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// 移動操作
	currentMotion = UpdateMove();

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 位置更新
	UpdatePosition(posPlayer, fDeltaTime);

	// 着地判定
	UpdateLanding(posPlayer, fDeltaTime);

	// 向き更新
	UpdateRotation(rotPlayer, fDeltaTime);

	// 壁の当たり判定
	CollisionWall(posPlayer);

	// 大人の壁の判定
	GET_STAGE->LimitPosition(posPlayer, GetRadius());

	// ステージ遷移の更新
	UpdateTrans(posPlayer);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// 分身の処理
	if(ControlClone(posPlayer, rotPlayer, fDeltaTime))
	{ currentMotion = MOTION_DODGE; }

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	待機状態時の更新処理
//============================================================
CPlayer::EMotion CPlayerSelect::UpdateWait(const float fDeltaTime)
{
	D3DXVECTOR3 posPlayer		= GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 oldPosPlayer	= GetOldPosition();		// プレイヤー過去位置
	D3DXVECTOR3 rotPlayer		= GetVec3Rotation();	// プレイヤー向き
	CStage *pStage	= GET_STAGE;	// ステージ情報
	bool	bLand	= false;		// 着地フラグ

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 重力を与える
	D3DXVECTOR3 movePlayer = GetMove();	// プレイヤー移動量
	movePlayer.x = movePlayer.z = 0.0f;	// 横移動量を初期化
	posPlayer += movePlayer * fDeltaTime;

	// アクターとの当たり判定
	CollisionActor(posPlayer, bLand);

	// 地面・制限位置・アクターの着地判定
	if (pStage->LandFieldPosition(posPlayer, oldPosPlayer, movePlayer)
	||  pStage->LandLimitPosition(posPlayer, movePlayer, 0.0f))
	{ // プレイヤーが着地していた場合

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		SetJump(false);
	}

	// 向き更新
	UpdateRotation(rotPlayer, fDeltaTime);

	// 重力を反映
	SetMove(movePlayer);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// 待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
//	入場状態時の更新処理
//============================================================
CPlayer::EMotion CPlayerSelect::UpdateEnter(const float fDeltaTime)
{
	D3DXVECTOR3 posPlayer		= GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 oldPosPlayer	= GetOldPosition();		// プレイヤー過去位置
	CStage *pStage	= GET_STAGE;	// ステージ情報
	bool	bLand	= false;		// 着地フラグ

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 重力を与える
	D3DXVECTOR3 movePlayer = GetMove();	// プレイヤー移動量
	movePlayer.x = movePlayer.z = 0.0f;	// 横移動量を初期化
	posPlayer += movePlayer * fDeltaTime;

	// アクターとの当たり判定
	CollisionActor(posPlayer, bLand);

	// 地面・制限位置・アクターの着地判定
	if (pStage->LandFieldPosition(posPlayer, oldPosPlayer, movePlayer)
	||  pStage->LandLimitPosition(posPlayer, movePlayer, 0.0f))
	{ // プレイヤーが着地していた場合

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		SetJump(false);
	}

	// 重力を反映
	SetMove(movePlayer);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(GetDestRotation());

	if (GetMotionWholeCounter() == 58)
	{
		// 開始エフェクトを生成
		GET_EFFECT->Create("data\\EFFEKSEER\\toonsmoke.efkefc", GetVec3Position(), VEC3_ZERO, VEC3_ZERO, 17.0f);

		// 煙音を鳴らす
		PLAY_SOUND(CSound::LABEL_SE_CLONELOST_000);
	}

	if (IsMotionFinish())
	{ // 選択モーションが終了した場合

		// ラムダ式の作成
		auto func = [](void) -> D3DXVECTOR3
		{
			CPlayer* pPlayer = GET_PLAYER;	// プレイヤー情報
			CCamera* pCamera = GET_CAMERA;	// カメラ情報

			if (pPlayer == nullptr || pCamera == nullptr)
			{ // プレイヤーかカメラがない場合

				return SCREEN_CENT;
			}

			if (!pCamera->OnScreen(pPlayer->GetVec3Position()))
			{ // プレイヤーが画面外の場合

				return SCREEN_CENT;
			}

			D3DXVECTOR3 posScreen = pCamera->CalcPlayerPos();	// プレイヤー画面座標
			posScreen.z = 0.0f;	// Z値の値を消す
			return posScreen;
		};

		// 設定済みマップパスに遷移
		GET_MANAGER->SetIrisLoadScene(CScene::MODE_GAME, std::bind(func), 0.0f, 0.6f, 1.2f);
	}

	// セレクト開始モーションを返す
	return MOTION_SELECT_IN;
}

//============================================================
//	解放状態時の更新処理
//============================================================
CPlayer::EMotion CPlayerSelect::UpdateOpen(const float fDeltaTime)
{
	// 待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
//	ステージ遷移の更新処理
//============================================================
void CPlayerSelect::UpdateTrans(D3DXVECTOR3& rPos)
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// キーボード情報
	CInputPad*		pPad = GET_INPUTPAD;	// パッド情報

	// 触れている遷移ポイントを取得
	CTransPoint *pHitTrans = CTransPoint::Collision(rPos, GetRadius());

	// 遷移ポイントに触れていない場合抜ける
	if (pHitTrans == nullptr) { return; }

	if (pKey->IsTrigger(DIK_RETURN)
	||  pPad->IsTrigger(CInputPad::KEY_A))
	{
		// 遷移ポイントインデックスを保存
		GET_RETENTION->SetTransIdx(CTransPoint::GetList()->GetIndex(pHitTrans));

		// 待機の設定
		SetWait(pHitTrans);
	}
}

//============================================================
//	スポーンの設定処理
//============================================================
void CPlayerSelect::SetSpawn(void)
{
	// 情報を初期化
	SetState(STATE_SELECT_SPAWN);	// スポーン状態の設定
	SetMotion(MOTION_SELECT_IN);	// セレクト開始モーションを設定

	// 移動量を初期化
	SetMove(VEC3_ZERO);

	// マテリアルを再設定
	ResetMaterial();

	// 透明度を不透明に設定
	SetAlpha(1.0f);

	// 描画を再開
	SetEnableDraw(true);

	// 回り込みカメラの設定
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_SELECT);
	GET_MANAGER->GetCamera()->SetDestSelect();

	// プレイヤーの向きをカメラ方向に設定
	D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, GET_CAMERA->GetDestRotation().y, 0.0f);	// カメラ向き
	SetVec3Rotation(rotCamera);
	SetDestRotation(rotCamera);
}

//===========================================================
//	待機の設定処理
//===========================================================
void CPlayerSelect::SetWait(CTransPoint* pHit)
{
	// 待機状態にする
	SetState(STATE_SELECT_WAIT);

	// ランキング表示をONにする
	CSceneSelect::GetSelectManager()->SetDispRanking(pHit);

	// 選択中の遷移先のマップパスを保存
	m_sSelectPath = pHit->GetTransMapPass().c_str();

	// 移動量を初期化
	D3DXVECTOR3 move = GetMove();	// 移動量
	move.x = move.z = 0.0f;	// 横移動量を初期化
	SetMove(move);

	// プレイヤーの向きをカメラ方向に設定
	D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, GET_CAMERA->GetDestRotation().y, 0.0f);	// カメラ向き
	SetVec3Rotation(rotCamera);
	SetDestRotation(rotCamera);
}

//============================================================
//	入場の設定処理
//============================================================
void CPlayerSelect::SetEnter(void)
{
	// 遷移ポイントのマップパスを保存
	GET_STAGE->SetInitMapPass(m_sSelectPath.c_str());

	// セレクト終了モーションにする
	SetMotion(MOTION_SELECT_OUT);

	// 入場状態にする
	SetState(STATE_SELECT_ENTER);

	// 選択カメラにする
	GET_CAMERA->SetState(CCamera::STATE_SELECT);

	// 尺八音の再生
	PLAY_SOUND(CSound::LABEL_SE_SYAKUHATI);
}
