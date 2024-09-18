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

#include "manager.h"
#include "collision.h"
#include "stage.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float FLY_SPEED = 900.0f;		// 飛んでいく速度
	const float FLY_HEIGHT = 300.0f;	// 飛んでいく高さ
	const float GRAVITY = 900.0f;		// 重力
	const float CYCLE_Y = 8.8f;			// Y軸の回転速度
	const float CYCLE_Z = 11.1f;		// Z軸の回転速度
}

//************************************************************
//	子クラス [CTouchCan] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTouchCan::CTouchCan() : CTouchActor(),
m_posOld(VEC3_ZERO),	// 前回の位置
m_move(VEC3_ZERO),		// 移動量
m_bDelete(false)		// 消去状況
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
	// 位置を設定する
	m_posOld = GetVec3Position();

	// オブジェクトモデルの更新
	CTouchActor::Update(fDeltaTime);

	if (m_bDelete)
	{ // 消去状況が true の場合

		// 終了処理
		Uninit();

		// 関数を抜ける
		return;
	}
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
// 情報の設定処理
//============================================================
void CTouchCan::SetData
(
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR3& rScale	// 拡大率
)
{
	// 情報を設定する
	CTouchActor::SetData(type, rPos, rRot, rScale);

	// 下がっている分だけ上げる
	D3DXVECTOR3 pos = rPos;
	pos.y -= GetModelData().vtxMin.y;
	SetVec3Position(pos);
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
		collision::CirclePillar(posCan, rPos, fRadiusCan, fRadius))
	{ // 缶を蹴った時

		D3DXVECTOR3 rot = GetVec3Rotation();

		// 飛んでいく方向を設定
		rot.y = atan2f(posCan.x - rPos.x, posCan.z - rPos.z);

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
void CTouchCan::UpdateNone(const float /*fDeltaTime*/)
{
	// 位置を取得
	D3DXVECTOR3 pos = GetVec3Position();
	pos.y -= GRAVITY;

	// フィールドとの当たり判定
	GET_STAGE->LandFieldPosition(pos, m_posOld, m_move);

	// 下の分を足す
	pos.y -= GetModelData().vtxMin.y;

	// 位置を反映
	SetVec3Position(pos);
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
	rot.y += CYCLE_Y * fDeltaTime;
	rot.z += CYCLE_Z * fDeltaTime;

	// フィールドとの当たり判定
	m_bDelete = GET_STAGE->LandFieldPosition(pos, m_posOld, m_move);

	// 位置と向きを反映
	SetVec3Position(pos);
	SetVec3Rotation(rot);
}
