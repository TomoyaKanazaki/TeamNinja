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
#include "transpoint.h"

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

	// 情報を初期化
	SetState(STATE_SELECT_SPAWN);	// スポーン状態の設定
	SetMotion(MOTION_LANDING);		// 着地モーションを設定

	// 移動量を初期化
	SetMove(VEC3_ZERO);

	// マテリアルを再設定
	ResetMaterial();

	// 透明度を不透明に設定
	SetAlpha(1.0f);

	// 描画を再開
	SetEnableDraw(true);

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
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション
	//if (GetType)
	{ // 場合

		// 通常状態を設定
		SetState(STATE_SELECT_NORMAL);
	}

	// 現在のモーションを返す
	return currentMotion;
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

//==========================================
//	ステージ遷移の更新処理
//==========================================
void CPlayerSelect::UpdateTrans(D3DXVECTOR3& rPos)
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// キーボード情報
	CInputPad*		pPad = GET_INPUTPAD;	// パッド情報

	// 触れている遷移ポイントを取得
	CTransPoint *pHitTrans = CTransPoint::Collision(rPos, GetRadius());

	// 遷移ポイントに触れていない場合抜ける
	if (pHitTrans == nullptr) { return; }

	if (pKey->IsTrigger(DIK_SPACE)
	||  pKey->IsTrigger(DIK_RETURN)
	||  pPad->IsTrigger(CInputPad::KEY_A)
	||  pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X)
	||  pPad->IsTrigger(CInputPad::KEY_Y))
	{
		// 遷移ポイントのマップパスに遷移
		GET_STAGE->SetInitMapPass(pHitTrans->GetTransMapPass().c_str());
		GET_MANAGER->SetLoadScene(CScene::MODE_GAME);
	}
}
