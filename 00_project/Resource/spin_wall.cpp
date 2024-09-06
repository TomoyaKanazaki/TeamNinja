//==========================================
//
//  回転扉クラス(spin_wall.cpp)
//  Author：小原立暉
//
//==========================================
#include "spin_wall.h"
#include "collision.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float ROT_MOVE = 0.04f;	// 向きの移動量
}

//==========================================
//  コンストラクタ
//==========================================
CSpinWall::CSpinWall(const D3DXVECTOR3& rRot) :
	m_rotDefault(rRot),
	m_collMax(VEC3_ZERO),
	m_collMin(VEC3_ZERO),
	m_state(STATE_STOP),
	m_fRotMove(0.0f),
	m_fTotalMove(0.0f),
	m_bFront(true)
{
	// Do Nothing
}

//==========================================
//  デストラクタ
//==========================================
CSpinWall::~CSpinWall()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CSpinWall::Init(void)
{
	// 値を初期化
	m_state = STATE_STOP;		// 状態

	// 親クラスの初期化処理
	return CActor::Init();
}

//==========================================
//  終了処理
//==========================================
void CSpinWall::Uninit(void)
{
	// 親クラスの終了処理
	CActor::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CSpinWall::Update(const float fDeltaTime)
{
	// 状態処理
	State();

	// 親クラスの更新処理
	CActor::Update(fDeltaTime);
}

//==========================================
//  描画処理
//==========================================
void CSpinWall::Draw(CShader* pShader)
{
	// 親クラスの描画処理
	CActor::Draw(pShader);
}

//==========================================
//  拡大率設定のオーバーライド
//==========================================
void CSpinWall::SetVec3Scaling(const D3DXVECTOR3& rScale)
{
	// 拡大率の設定処理
	CActor::SetVec3Scaling(rScale);

	// 当たり判定のサイズ設定
	CollSizeSet(rScale);
}

#ifdef _DEBUG

//==========================================
// 初期向きの設定処理
//==========================================
void CSpinWall::SetDefaultRot(const D3DXVECTOR3& rRot)
{
	// 初期向きを設定する
	m_rotDefault = rRot;
}

#endif // _DEBUG


//==========================================
// 当たり判定処理
//==========================================
void CSpinWall::Collision
(
	D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight,			// 高さ
	D3DXVECTOR3& rMove,				// 移動量
	bool& bJump						// ジャンプ状況
)
{
	// 回転中の場合、抜ける
	if (m_state == STATE_SPIN) { return; }

	// 回転の当たり判定
	SpinCollision
	(
		rPos,		// 位置
		rPosOld,	// 前回の位置
		fRadius,	// 半径
		fHeight		// 高さ
	);

	// 当たり判定処理
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
void CSpinWall::Collision
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
	// 回転中の場合、抜ける
	if (m_state == STATE_SPIN) { return; }

	// 回転の当たり判定
	SpinCollision
	(
		rPos,		// 位置
		rPosOld,	// 前回の位置
		fRadius,	// 半径
		fHeight		// 高さ
	);

	// 当たり判定処理
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
void CSpinWall::State(void)
{
	switch (m_state)
	{
	case CSpinWall::STATE_STOP:

		// 特に無し

		break;

	case CSpinWall::STATE_SPIN:

		// 回転処理
		Spin();

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//==========================================
// 回転処理
//==========================================
void CSpinWall::Spin(void)
{
	// 向きを取得する
	D3DXVECTOR3 rot = GetVec3Rotation();
	D3DXVECTOR3 rotOld = GetVec3Rotation();

	// 回転させる
	rot.y += m_fRotMove;
	m_fTotalMove += fabsf(m_fRotMove);
	useful::NormalizeRot(rot.y);

	if (m_fTotalMove >= D3DX_PI)
	{ // 半回転した場合

		// 停止状態にする
		m_state = STATE_STOP;

		// 向きの合計値をリセットする
		m_fTotalMove = 0.0f;

		// 表向き状況を逆転する
		m_bFront = !m_bFront;

		// 向きを補正する
		rot.y = m_bFront == true ? m_rotDefault.y : m_rotDefault.y + D3DX_PI;
	}

	// 向きを正規化する
	SetVec3Rotation(rot);
}

//==========================================
// 回転設定処理
//==========================================
void CSpinWall::SetSpin(const float fRotMove)
{
	// 回転状態にする
	m_state = STATE_SPIN;

	// 向きの移動量を設定する
	m_fRotMove = fRotMove;
}

//==========================================
// 回転の当たり判定処理
//==========================================
void CSpinWall::SpinCollision
(
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight				// 高さ
)
{
	// 位置を取得
	D3DXVECTOR3 posWall = GetVec3Position();
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(fRadius, 0.0f, fRadius);
	EAngle angle = useful::RotToFourDire(m_rotDefault.y);

	if (angle == EAngle::ANGLE_90 ||
		angle == EAngle::ANGLE_270)
	{ // 90度、270度の場合

		if (posWall.z + m_collMax.z >= rPos.z - fRadius &&
			posWall.z <= rPos.z + fRadius)
		{ // 左側にいた場合

			if (rPos.x + fRadius > posWall.x - m_collMin.x
				&& rPosOld.x + fRadius <= posWall.x - m_collMin.x)
			{ // 左からの当たり判定

				// 回転設定処理
				SetSpin(ROT_MOVE);
			}
			else if (rPos.x - fRadius < posWall.x + m_collMax.x
				&& rPosOld.x - fRadius >= posWall.x + m_collMax.x)
			{ // 右からの当たり判定

				// 回転設定処理
				SetSpin(-ROT_MOVE);
			}
		}
		else if (posWall.z - m_collMin.z <= rPos.z + fRadius &&
			posWall.z >= rPos.z - fRadius)
		{ // 右側にいた場合

			if (rPos.x + fRadius > posWall.x - m_collMin.x
				&& rPosOld.x + fRadius <= posWall.x - m_collMin.x)
			{ // 左からの当たり判定

				// 回転設定処理
				SetSpin(-ROT_MOVE);
			}
			else if (rPos.x - fRadius < posWall.x + m_collMax.x
				&& rPosOld.x - fRadius >= posWall.x + m_collMax.x)
			{ // 右からの当たり判定

				// 回転設定処理
				SetSpin(ROT_MOVE);
			}
		}
	}
	else
	{ // 上記以外

		if (posWall.x + m_collMax.x >= rPos.x - fRadius &&
			posWall.x <= rPos.x + fRadius)
		{ // 左側にいた場合

			if (rPos.z + fRadius > posWall.z - m_collMin.z
				&& rPosOld.z + fRadius <= posWall.z - m_collMin.z)
			{ // 左からの当たり判定

				// 回転設定処理
				SetSpin(-ROT_MOVE);
			}
			else if (rPos.z - fRadius < posWall.z + m_collMax.z
				&& rPosOld.z - fRadius >= posWall.z + m_collMax.z)
			{ // 右からの当たり判定

				// 回転設定処理
				SetSpin(ROT_MOVE);
			}
		}
		else if (posWall.x - m_collMin.x <= rPos.x + fRadius &&
			posWall.x >= rPos.x - fRadius)
		{ // 右側にいた場合

			if (rPos.z + fRadius > posWall.z - m_collMin.z
				&& rPosOld.z + fRadius <= posWall.z - m_collMin.z)
			{ // 左からの当たり判定

				// 回転設定処理
				SetSpin(ROT_MOVE);
			}
			else if (rPos.z - fRadius < posWall.z + m_collMax.z
				&& rPosOld.z - fRadius >= posWall.z + m_collMax.z)
			{ // 右からの当たり判定

				// 回転設定処理
				SetSpin(-ROT_MOVE);
			}
		}
	}
}

//==========================================
// 当たり判定のサイズの設定処理
//==========================================
void CSpinWall::CollSizeSet(const D3DXVECTOR3& rScale)
{
	EAngle angle = useful::RotToFourDire(GetVec3Rotation().y);
	D3DXVECTOR3 ModelMax = GetModelData().vtxMax;
	D3DXVECTOR3 ModelMin = GetModelData().vtxMin;

	if (angle == EAngle::ANGLE_90 ||
		angle == EAngle::ANGLE_270)
	{ // 90度、270度の場合

		m_collMax = D3DXVECTOR3
		(
			ModelMax.z * rScale.z,
			ModelMax.y * rScale.y,
			ModelMax.x * rScale.x
		);
		m_collMin = D3DXVECTOR3
		(
			-ModelMin.z * rScale.z,
			-ModelMin.y * rScale.y,
			-ModelMin.x * rScale.x
		);
	}
	else
	{ // 上記以外

		m_collMax = D3DXVECTOR3
		(
			ModelMax.x * rScale.x,
			ModelMax.y * rScale.y,
			ModelMax.z * rScale.z
		);
		m_collMin = D3DXVECTOR3
		(
			-ModelMin.x * rScale.x,
			-ModelMin.y * rScale.y,
			-ModelMin.z * rScale.z
		);
	}
}