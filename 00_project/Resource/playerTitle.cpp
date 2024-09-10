//============================================================
//
//	タイトル画面プレイヤー処理 [playerTitle.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "playerTitle.h"
#include "player_clone.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	NUM_CLONE	= 2;		// 生成する分身数
	const float	MOVE_VALUE	= 500.0f;	// 移動スカラー
	const float	OFFSET_SIDE	= 100.0f;	// 分身生成横オフセット
	const float	OFFSET_BACK	= 100.0f;	// 分身生成後ろオフセット
}

//************************************************************
//	子クラス [CPlayerTitle] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayerTitle::CPlayerTitle()
{

}

//============================================================
//	デストラクタ
//============================================================
CPlayerTitle::~CPlayerTitle()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayerTitle::Init(void)
{
	// プレイヤーの初期化
	if (FAILED(CPlayer::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// スポーンの設定
	SetSpawn();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayerTitle::Uninit(void)
{
	// プレイヤーの終了
	CPlayer::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayerTitle::Update(const float fDeltaTime)
{
	// プレイヤーの更新
	CPlayer::Update(fDeltaTime);
}

//============================================================
//	状態の更新処理
//============================================================
CPlayer::EMotion CPlayerTitle::UpdateState(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション
	switch (GetState())
	{ // 状態ごとの処理
	case STATE_NONE:

		// 何もしない状態時の更新
		currentMotion = UpdateNone(fDeltaTime);
		break;

	case STATE_TITLE_IDOL:

		// 仁王立ち状態時の更新
		currentMotion = UpdateIdol(fDeltaTime);
		break;

	case STATE_TITLE_MOVE:

		// 移動状態時の更新
		currentMotion = UpdateMove(fDeltaTime);
		break;

	default:
		assert(false);
		break;
	}

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	仁王立ち状態時の更新処理
//============================================================
CPlayer::EMotion CPlayerTitle::UpdateIdol(const float fDeltaTime)
{
	// 仁王立ちモーションを返す
	return MOTION_STAND;
}

//============================================================
//	移動状態時の更新処理
//============================================================
CPlayer::EMotion CPlayerTitle::UpdateMove(const float fDeltaTime)
{
	// 位置を移動量分移動させる
	SetVec3Position(GetVec3Position() + GetMove() * fDeltaTime);

	// 移動モーションを返す
	return MOTION_DASH;
}

//============================================================
//	スポーンの設定処理
//============================================================
void CPlayerTitle::SetSpawn(void)
{
	// 情報を初期化
	SetState(STATE_TITLE_IDOL);	// 仁王立ち状態の設定
	SetMotion(MOTION_STAND);	// 仁王立ちモーションを設定

	// 移動量を初期化
	SetMove(VEC3_ZERO);

	// マテリアルを再設定
	ResetMaterial();

	// 透明度を不透明に設定
	SetAlpha(1.0f);

	// 描画を再開
	SetEnableDraw(true);
}

//============================================================
//	演出開始の設定処理
//============================================================
void CPlayerTitle::SetStart(void)
{
	const D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	const D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き
	const D3DXVECTOR3 moveChara = D3DXVECTOR3(sinf(rotPlayer.y), 0.0f, cosf(rotPlayer.y)) * -MOVE_VALUE;	// キャラ移動量

	// 情報を初期化
	SetState(STATE_TITLE_MOVE);	// 移動状態の設定
	SetMotion(MOTION_DASH);		// 移動モーションを設定

	for (int i = 0; i < NUM_CLONE; i++)
	{ // 生成する分身数分繰り返す

		D3DXVECTOR3 posClone = posPlayer;			// 分身生成位置
		float fRotSide = rotPlayer.y + HALF_PI;		// プレイヤー横方向向き
		float fSide = (1.0f - ((i % 2) * 2.0f));	// 生成方向係数

		// 生成位置をずらす
		posClone += D3DXVECTOR3(sinf(fRotSide), 0.0f, cosf(fRotSide)) * OFFSET_SIDE * fSide;	// 横にずらす
		posClone += D3DXVECTOR3(sinf(rotPlayer.y), 0.0f, cosf(rotPlayer.y)) * OFFSET_BACK;		// 後ろにずらす

		// 分身の生成
		CPlayerClone::Create(posClone, moveChara);
	}

	// 移動量の設定
	SetMove(moveChara);
}
