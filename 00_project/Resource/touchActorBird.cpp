//============================================================
//
//	タッチアクター(鳥)処理 [touchActorBird.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "touchActorBird.h"

#include "collision.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int NONE_MOVE_COUNT = 50;				// 通常状態の移動カウント数
	const float COLLISION_RADIUS = 150.0f;		// 当たり判定の半径
	const float FLY_SPEED = 600.0f;				// 飛んでいく速度
	const float FLY_HEIGHT = 180.0f;			// 飛んでいく高さ
	const float FLY_GRAVITY = 50.0f;			// 飛行時の重力
}

//************************************************************
//	子クラス [CTouchBird] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTouchBird::CTouchBird() : CTouchActor(),
m_move(VEC3_ZERO),		// 移動量
m_nStateCount(0)		// 状態カウント
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

		// 飛んでいく方向を設定
		rot.y = atan2f(posBird.x - rPos.x, posBird.z - rPos.z);

		// アクション状態にする
		SetState(STATE_ACT);

		// 移動量を設定する
		m_move.x = sinf(rot.y) * FLY_SPEED;
		m_move.y = FLY_HEIGHT;
		m_move.z = cosf(rot.y) * FLY_SPEED;

		// 向きを反映
		SetVec3Rotation(rot);

		// true を返す
		return true;
	}

	// false を返す
	return false;
}

//============================================================
// 通常状態更新処理
//============================================================
void CTouchBird::UpdateNone(const float /*fDeltaTime*/)
{
	// 状態カウントを加算する
	m_nStateCount++;

	if (m_nStateCount % NONE_MOVE_COUNT == 0)
	{ // 一定カウントごとに

		// ぴょんぴょん跳ねる処理を追加
	}
}

//============================================================
// アクション状態更新処理
//============================================================
void CTouchBird::UpdateAct(const float fDeltaTime)
{
	D3DXVECTOR3 pos = GetVec3Position();	// 位置

	// 移動する
	pos += m_move * fDeltaTime;

	// 重力をかける
	m_move.y -= FLY_GRAVITY * fDeltaTime;

	// 位置と向きを反映
	SetVec3Position(pos);
}
