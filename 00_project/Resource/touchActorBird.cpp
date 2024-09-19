//============================================================
//
//	タッチアクター(鳥)処理 [touchActorBird.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "touchActorBird.h"

#include "collision.h"
#include "scene.h"
#include "player.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int NONE_MOVE_COUNT = 90;				// 通常状態の移動カウント数
	const float COLLISION_RADIUS = 150.0f;		// 当たり判定の半径
	const float NONE_SPEED = 200.0f;			// 通常時の移動量
	const float NONE_HEIGHT = 80.0f;			// 通常時の高さ
	const float NONE_GRAVITY = 500.0f;			// 通常時の重力
	const float FLY_SPEED = 700.0f;				// 飛んでいく速度
	const float FLY_HEIGHT = 300.0f;			// 飛んでいく高さ
	const float FLY_GRAVITY = 200.0f;			// 飛行時の重力
}

//************************************************************
//	子クラス [CTouchBird] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTouchBird::CTouchBird() : CTouchActor()
{

}

//============================================================
//	デストラクタ
//============================================================
CTouchBird::~CTouchBird()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTouchBird::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CTouchActor::Init()))
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
void CTouchBird::Uninit(void)
{
	// オブジェクトモデルの終了
	CTouchActor::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CTouchBird::Update(const float fDeltaTime)
{
	// オブジェクトモデルの更新
	CTouchActor::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CTouchBird::Draw(CShader* pShader)
{
	// オブジェクトモデルの描画
	CTouchActor::Draw(pShader);
}

//============================================================
// 当たり判定処理
//============================================================
bool CTouchBird::Collision
(
	const D3DXVECTOR3& rPos,
	const D3DXVECTOR3& rPosOld,
	const float fRadius,
	const float fHeight
)
{
	// 通常状態以外、falseを返す
	if (GetState() != STATE_NONE) { return false; }

	D3DXVECTOR3 posBird = GetVec3Position();		// 位置
	float fBirdMax = GetModelData().vtxMax.y;	// 高さ(最大値)
	float fBirdMin = GetModelData().vtxMin.y;	// 高さ(最小値)

	if (posBird.y + fBirdMax > rPos.y &&
		posBird.y + fBirdMin < rPos.y + fHeight &&
		collision::Circle2D(posBird, rPos, COLLISION_RADIUS, fRadius))
	{ // 鳥の範囲に入った場合

		D3DXVECTOR3 rot = GetVec3Rotation();
		D3DXVECTOR3 move = VEC3_ZERO;

		// 飛んでいく方向を設定
		rot.y = atan2f(posBird.x - rPos.x, posBird.z - rPos.z);

		// アクション状態にする
		SetState(STATE_ACT);

		// 移動量を設定する
		move.x = sinf(rot.y) * FLY_SPEED;
		move.y = FLY_HEIGHT;
		move.z = cosf(rot.y) * FLY_SPEED;

		// 向きと移動量を反映
		SetVec3Rotation(rot);
		SetVec3Move(move);

		// true を返す
		return true;
	}

	// false を返す
	return false;
}

//============================================================
// 通常状態更新処理
//============================================================
void CTouchBird::UpdateNone(const float fDeltaTime)
{
	D3DXVECTOR3 pos = GetVec3Position();
	D3DXVECTOR3 rot = GetVec3Rotation();
	D3DXVECTOR3 move = GetVec3Move();

	// 重力をかける
	move.y -= NONE_GRAVITY * fDeltaTime;

	if (GetStateCount() % NONE_MOVE_COUNT == 0)
	{ // 一定カウントごとに

		// プレイヤーの位置を取得する
		D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();

		// 向きをランダムで設定
		rot.y = atan2f(pos.x - posPlayer.x, pos.z - posPlayer.z) + (float)(rand() % 315 - 157) * 0.01f;

		// 移動量を設定する
		move.x = sinf(rot.y) * NONE_SPEED;
		move.y = NONE_HEIGHT;
		move.z = cosf(rot.y) * NONE_SPEED;
	}

	// 移動する
	pos += move * fDeltaTime;

	// 床との当たり判定
	if (CollisionFieid(pos))
	{
		// 移動量を0にする
		move = VEC3_ZERO;
	}

	// 位置と向きと移動量を反映
	SetVec3Position(pos);
	SetVec3Rotation(rot);
	SetVec3Move(move);
}

//============================================================
// アクション状態更新処理
//============================================================
void CTouchBird::UpdateAct(const float fDeltaTime)
{
	D3DXVECTOR3 pos = GetVec3Position();	// 位置
	D3DXVECTOR3 move = GetVec3Move();		// 移動量

	// 移動する
	pos += move * fDeltaTime;

	// 重力をかける
	move.y -= FLY_GRAVITY * fDeltaTime;

	// 移動量が 0.0f 以下になった場合、0に補正する
	if (move.y <= 0.0f) { move.y = 0.0f; }

	// 位置と移動量を反映
	SetVec3Position(pos);
	SetVec3Move(move);
}
