//==========================================
//
//  ふすまクラス(mash.cpp)
//  Author : Tomoya Kanazaki
//  Adder：Ritsuki Obara
//
//==========================================
#include "mash.h"
#include "collision.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float MOVE_SPEED = 120.0f; // 移動速度
}

//==========================================
//  コンストラクタ
//==========================================
CMash::CMash(const D3DXVECTOR3& rPos) :
	m_posDefault(rPos),
	m_offsetMove(VEC3_ZERO),
	m_move(VEC3_ZERO),
	m_state(STATE_CLOSE)
{
	// Do Nothing
}

//==========================================
//  デストラクタ
//==========================================
CMash::~CMash()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CMash::Init(void)
{
	// 親クラスの初期化処理
	return CActor::Init();
}

//==========================================
//  終了処理
//==========================================
void CMash::Uninit(void)
{
	// 親クラスの終了処理
	CActor::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CMash::Update(const float fDeltaTime)
{
	// 状態処理
	State(fDeltaTime);

	// 親クラスの更新処理
	CActor::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CMash::Draw(CShader* pShader)
{
	// 親クラスの描画処理
	CActor::Draw(pShader);
}

//==========================================
//  位置設定のオーバーライド
//==========================================
void CMash::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 移動する距離を取得する
	float offsetMax = GetModelData().vtxMax.x * 2.0f;

	// 向きから移動先オフセットを算出する
	float fRot = GetVec3Rotation().y + (D3DX_PI * 0.5f);
	m_offsetMove = D3DXVECTOR3
	(
		m_posDefault.x + offsetMax * sinf(fRot),
		0.0f,
		m_posDefault.z + offsetMax * cosf(fRot)
	);

	// 向きから移動量を算出する
	m_move = D3DXVECTOR3
	(
		MOVE_SPEED * sinf(fRot),
		0.0f,
		MOVE_SPEED * cosf(fRot)
	);

	// 親クラスの位置設定
	CActor::SetVec3Position(rPos);
}

//==========================================
// 当たり判定処理
//==========================================
void CMash::Collision
(
	D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight,			// 高さ
	D3DXVECTOR3& rMove,				// 移動量
	bool& bJump						// ジャンプ状況
)
{
	// 位置を取得
	D3DXVECTOR3 posMash = GetVec3Position();
	D3DXVECTOR3 rotMash = GetVec3Rotation();
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(fRadius, 0.0f, fRadius);
	D3DXVECTOR3 vtxMashMax = D3DXVECTOR3
	(
		GetModelData().vtxMax.x + sinf(rotMash.y) * GetModelData().vtxMax.x,
		fHeight,
		GetModelData().vtxMax.z + cosf(rotMash.y) * GetModelData().vtxMax.x
	);
	D3DXVECTOR3 vtxMashMin = D3DXVECTOR3
	(
		-GetModelData().vtxMin.x + sinf(rotMash.y) * -GetModelData().vtxMin.x,
		0.0f,
		-GetModelData().vtxMin.z + cosf(rotMash.y) * -GetModelData().vtxMin.x
	);

	// 向きの四方向変換
	useful::RotToFourDire(GetVec3Rotation().y);

	if (collision::Box3D
	(
		rPos,			// 判定位置
		m_posDefault,	// 判定目標位置
		vtxMax,			// 判定サイズ(右・上・後)
		vtxMin,			// 判定サイズ(左・下・前)
		vtxMashMax,		// 判定目標サイズ(右・上・後)
		vtxMashMin		// 判定目標サイズ(左・下・前)
	))
	{ // ふすまに近づいた場合

		// 開扉状態にする
		m_state = STATE_OPEN;
	}
	else
	{ // 上記以外

		// 閉扉状態にする
		m_state = STATE_CLOSE;
	}

	// アクターの当たり判定
	CActor::Collision
	(
		rPos,		// 位置
		rPosOld,	// 前回の位置
		fRadius,	// 半径
		fHeight,	// 高さ
		rMove,		// 移動量
		bJump		// ジャンプ状況
	);
}

//==========================================
// 当たり判定処理(判定を返すオーバーライド)
//==========================================
void CMash::Collision
(
	D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight,			// 高さ
	D3DXVECTOR3& rMove,				// 移動量
	bool& bJump,					// ジャンプ状況
	bool& bHit						// 衝突判定
)
{
	// 位置を取得
	D3DXVECTOR3 posMash = GetVec3Position();
	D3DXVECTOR3 rotMash = GetVec3Rotation();
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(fRadius, 0.0f, fRadius);
	D3DXVECTOR3 vtxMashMax = D3DXVECTOR3
	(
		GetModelData().vtxMax.x + sinf(rotMash.y) * GetModelData().vtxMax.x,
		fHeight,
		GetModelData().vtxMax.z + cosf(rotMash.y) * GetModelData().vtxMax.x
	);
	D3DXVECTOR3 vtxMashMin = D3DXVECTOR3
	(
		-GetModelData().vtxMin.x + sinf(rotMash.y) * -GetModelData().vtxMin.x,
		0.0f,
		-GetModelData().vtxMin.z + cosf(rotMash.y) * -GetModelData().vtxMin.x
	);

	// 向きの四方向変換
	useful::RotToFourDire(GetVec3Rotation().y);

	if (collision::Box3D
	(
		rPos,			// 判定位置
		m_posDefault,	// 判定目標位置
		vtxMax,			// 判定サイズ(右・上・後)
		vtxMin,			// 判定サイズ(左・下・前)
		vtxMashMax,		// 判定目標サイズ(右・上・後)
		vtxMashMin		// 判定目標サイズ(左・下・前)
	))
	{ // ふすまに近づいた場合

		// 開扉状態にする
		m_state = STATE_OPEN;
	}
	else
	{ // 上記以外

		// 閉扉状態にする
		m_state = STATE_CLOSE;
	}

	// アクターの当たり判定
	CActor::Collision
	(
		rPos,		// 位置
		rPosOld,	// 前回の位置
		fRadius,	// 半径
		fHeight,	// 高さ
		rMove,		// 移動量
		bJump,		// ジャンプ状況
		bHit		// 衝突判定
	);
}

//==========================================
// 状態処理
//==========================================
void CMash::State(const float fDeltaTime)
{
	switch (m_state)
	{
	case CMash::STATE_CLOSE:

		// 閉扉処理
		Close(fDeltaTime);

		break;

	case CMash::STATE_OPEN:

		// 開扉処理
		Open(fDeltaTime);

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//==========================================
// 閉扉処理
//==========================================
void CMash::Close(const float fDeltaTime)
{
	// 座標を取得
	D3DXVECTOR3 pos = GetVec3Position();
	D3DXVECTOR3 posOld = GetVec3Position();

	// 移動量を加算
	pos -= m_move * fDeltaTime;

	if ((pos.x >= m_posDefault.x && posOld.x <= m_posDefault.x) ||
		(pos.x <= m_posDefault.x && posOld.x >= m_posDefault.x))
	{ // 目標位置を超えた場合

		// 位置を設定する
		pos.x = m_posDefault.x;
	}

	if ((pos.z >= m_posDefault.z && posOld.z <= m_posDefault.z) ||
		(pos.z <= m_posDefault.z && posOld.z >= m_posDefault.z))
	{ // 目標位置を超えた場合

		// 位置を設定する
		pos.z = m_posDefault.z;
	}

	// 位置を反映
	SetVec3Position(pos);
}

//==========================================
//  移動処理
//==========================================
void CMash::Open(const float fDeltaTime)
{
	// 座標を取得
	D3DXVECTOR3 pos = GetVec3Position();
	D3DXVECTOR3 posOld = GetVec3Position();

	// 移動量を加算
	pos += m_move * fDeltaTime;

	if ((pos.x >= m_offsetMove.x && posOld.x <= m_offsetMove.x) ||
		(pos.x <= m_offsetMove.x && posOld.x >= m_offsetMove.x))
	{ // 目標位置を超えた場合

		// 位置を設定する
		pos.x = m_offsetMove.x;
	}

	if ((pos.z >= m_offsetMove.z && posOld.z <= m_offsetMove.z) ||
		(pos.z <= m_offsetMove.z && posOld.z >= m_offsetMove.z))
	{ // 目標位置を超えた場合

		// 位置を設定する
		pos.z = m_offsetMove.z;
	}

	// 位置を反映
	SetVec3Position(pos);
}
