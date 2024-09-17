//============================================================
//
//	タッチアクター(缶)処理 [touchActorCan.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "touchActorCan.h"

#include "collision.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float FLY_SPEED = 300.0f;		// 飛んでいく速度
	const float FLY_HEIGHT = 100.0f;	// 飛んでいく高さ
	const float GRAVITY = 100.0f;		// 重力
	const float CYCLE_SPEED = 0.2f;		// 回転速度
}

//************************************************************
//	子クラス [CTouchCan] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTouchCan::CTouchCan() : CTouchActor(),
m_move(VEC3_ZERO)		// 移動量
{

}

//============================================================
//	デストラクタ
//============================================================
CTouchCan::~CTouchCan()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTouchCan::Init(void)
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
void CTouchCan::Uninit(void)
{
	// オブジェクトモデルの終了
	CTouchActor::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CTouchCan::Update(const float fDeltaTime)
{
	// オブジェクトモデルの更新
	CTouchActor::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CTouchCan::Draw(CShader* pShader)
{
	// オブジェクトモデルの描画
	CTouchActor::Draw(pShader);
}

//============================================================
// 当たり判定処理
//============================================================
bool CTouchCan::Collision
(
	const D3DXVECTOR3& rPos,
	const D3DXVECTOR3& rPosOld,
	const float fRadius,
	const float fHeight
)
{
	// 通常状態以外、falseを返す
	if (GetState() != STATE_NONE) { return false; }

	D3DXVECTOR3 posCan = GetVec3Position();		// 位置
	float fRadiusCan = GetModelData().fRadius;	// 半径
	float fCanMax = GetModelData().vtxMax.y;	// 高さ(最大値)
	float fCanMin = GetModelData().vtxMin.y;	// 高さ(最小値)

	if (posCan.y + fCanMax > rPos.y &&
		posCan.y + fCanMin < rPos.y + fHeight &&
		collision::CirclePillar(posCan, rPos, fRadius, fRadiusCan))
	{ // 缶を蹴った時

		D3DXVECTOR3 rot = GetVec3Rotation();

		// 飛んでいく方向を設定
		float fRot = atan2f(posCan.x - rPos.x, posCan.z - rPos.z);

		// アクション状態にする
		SetState(STATE_ACT);

		// 移動量を設定する
		m_move.x = sinf(fRot) * FLY_SPEED;
		m_move.y = FLY_HEIGHT;
		m_move.z = cosf(fRot) * FLY_SPEED;

		// 向きを設定する
		rot.y = fRot;

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
void CTouchCan::UpdateNone(const float /*fDeltaTime*/)
{
	// 特に無し
}

//============================================================
// アクション状態更新処理
//============================================================
void CTouchCan::UpdateAct(const float fDeltaTime)
{
	D3DXVECTOR3 pos = GetVec3Position();	// 位置
	D3DXVECTOR3 rot = GetVec3Rotation();	// 向き

	// 移動する
	pos += m_move * fDeltaTime;

	// 重力をかける
	m_move.y -= GRAVITY * fDeltaTime;

	// 向きを加算する
	rot.z += CYCLE_SPEED;

	// 位置と向きを反映
	SetVec3Position(pos);
	SetVec3Rotation(rot);
}
