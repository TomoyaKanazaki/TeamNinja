//============================================================
//
//	タッチアクター(枝)処理 [touchActorBranch.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "touchActorBranch.h"
#include "manager.h"
#include "sound.h"

#include "collision.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float FLY_SPEED = 900.0f;		// 飛んでいく速度
	const float FLY_HEIGHT = 300.0f;	// 飛んでいく高さ
	const float GRAVITY = 900.0f;		// 重力
	const float CYCLE_Y = 9.0f;			// Y軸の回転速度
	const float CYCLE_Z = 12.1f;		// Z軸の回転速度
}

//************************************************************
//	子クラス [CTouchBranch] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTouchBranch::CTouchBranch() : CTouchActor(),
m_bSound(false)
{

}

//============================================================
//	デストラクタ
//============================================================
CTouchBranch::~CTouchBranch()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTouchBranch::Init(void)
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
void CTouchBranch::Uninit(void)
{
	// オブジェクトモデルの終了
	CTouchActor::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CTouchBranch::Update(const float fDeltaTime)
{
	// オブジェクトモデルの更新
	CTouchActor::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CTouchBranch::Draw(CShader* pShader)
{
	// オブジェクトモデルの描画
	CTouchActor::Draw(pShader);
}

//============================================================
// 情報の設定処理
//============================================================
void CTouchBranch::SetData
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
bool CTouchBranch::Collision
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
		D3DXVECTOR3 move = VEC3_ZERO;

		// 飛んでいく方向を設定
		rot.y = atan2f(posCan.x - rPos.x, posCan.z - rPos.z);

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
void CTouchBranch::UpdateNone(const float /*fDeltaTime*/)
{
	// 位置を取得
	D3DXVECTOR3 pos = GetVec3Position();
	pos.y -= GRAVITY;

	// 床との当たり判定
	CollisionFieid(pos);

	// 下の分を足す
	pos.y -= GetModelData().vtxMin.y;

	// 位置を反映
	SetVec3Position(pos);
}

//============================================================
// アクション状態更新処理
//============================================================
void CTouchBranch::UpdateAct(const float fDeltaTime)
{
	D3DXVECTOR3 pos = GetVec3Position();	// 位置
	D3DXVECTOR3 rot = GetVec3Rotation();	// 向き
	D3DXVECTOR3 move = GetVec3Move();		// 移動量

	// 移動する
	pos += move * fDeltaTime;

	// 重力をかける
	move.y -= GRAVITY * fDeltaTime;

	// 向きを加算する
	rot.y += CYCLE_Y * fDeltaTime;
	rot.z += CYCLE_Z * fDeltaTime;

	// フィールドとの当たり判定
	if (CollisionFieid(pos))
	{ // フィールドに当たった場合

		if (!m_bSound)
		{ // 音が鳴っていなかった場合

			// 落ちる音を鳴らす
			PLAY_SOUND(CSound::LABEL_SE_TOUCHFLY);

			// サウンド状況を true にする
			m_bSound = true;
		}

		// 半径分上げる
		pos.y += GetModelData().fRadius;

		// 移動量を重力以外0にする
		move.x = 0.0f;
		move.z = 0.0f;

		// 向きを横にする
		rot.z = 0.0f;
	}

	// 位置と向きと移動量を反映
	SetVec3Position(pos);
	SetVec3Rotation(rot);
	SetVec3Move(move);
}
