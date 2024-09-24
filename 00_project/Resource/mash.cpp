//==========================================
//
//  ふすまクラス(mash.cpp)
//  Author : Tomoya Kanazaki
//  Adder：Ritsuki Obara
//
//==========================================
#include "mash.h"
#include "manager.h"
#include "collision.h"
#include "sound.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float MOVE_SPEED = 120.0f;	// 移動速度
	const float SENSOR_RANGE = 40.0f;	// センサーが反応する範囲
}

//************************************************************
// 静的メンバ変数宣言
//************************************************************
CListManager<CMash>* CMash::m_pList = nullptr;		// リスト構造

//==========================================
//  コンストラクタ
//==========================================
CMash::CMash(const D3DXVECTOR3& rPos) :
	m_posDefault(rPos),
	m_offsetMove(VEC3_ZERO),
	m_move(VEC3_ZERO),
	m_collMax(VEC3_ZERO),
	m_collMin(VEC3_ZERO),
	m_state(STATE_CLOSE),
	m_stateOld(m_state)
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
	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CMash>::Create();

		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 親クラスの初期化処理
	return CActor::Init();
}

//==========================================
//  終了処理
//==========================================
void CMash::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 親クラスの終了処理
	CActor::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CMash::Update(const float fDeltaTime)
{
	if (m_state != m_stateOld)
	{ // 状態が前回と違う場合

		switch (m_state)
		{
		case CMash::STATE_CLOSE:

			// ふすまが閉じたときの音を鳴らす
			PLAY_SOUND(CSound::LABEL_SE_MASHCLOSE);

			break;

		case CMash::STATE_OPEN:

			// ふすまの音を鳴らす
			PLAY_SOUND(CSound::LABEL_SE_MASH);

			break;

		default:

			// 停止
			assert(false);

			break;
		}
	}

	// 状態を設定する
	m_stateOld = m_state;

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
//  拡大率設定のオーバーライド
//==========================================
void CMash::SetVec3Scaling(const D3DXVECTOR3& rScale)
{
	// 拡大率の設定処理
	CActor::SetVec3Scaling(rScale);

	// 移動する距離を取得する
	float offsetMax = (GetModelData().vtxMax.x * rScale.x) * 2.0f;

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
		(MOVE_SPEED * rScale.x) * sinf(fRot),
		0.0f,
		(MOVE_SPEED * rScale.x) * cosf(fRot)
	);

	// 当たり判定のサイズ設定
	CollSizeSet(rScale);
}

//==========================================
// リスト構造の取得処理
//==========================================
CListManager<CMash>* CMash::GetList(void)
{
	// リスト構造を返す
	return m_pList;
}

#ifdef _DEBUG

//==========================================
// 初期位置の設定処理
//==========================================
void CMash::SetDefaultPos(const D3DXVECTOR3& rPos)
{
	// 初期位置を設定する
	m_posDefault = rPos;
}

#endif // _DEBUG


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
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(fRadius, 0.0f, fRadius);

	if (collision::Box3D
	(
		rPos,			// 判定位置
		m_posDefault,	// 判定目標位置
		vtxMax,			// 判定サイズ(右・上・後)
		vtxMin,			// 判定サイズ(左・下・前)
		m_collMax,		// 判定目標サイズ(右・上・後)
		m_collMin		// 判定目標サイズ(左・下・前)
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
}

//==========================================
// 当たり判定処理(判定を返すオーバーライド)
//==========================================
bool CMash::Collision
(
	D3DXVECTOR3& rPos,				// 位置
	const D3DXVECTOR3& rPosOld,		// 前回の位置
	const float fRadius,			// 半径
	const float fHeight,			// 高さ
	D3DXVECTOR3& rMove,				// 移動量
	bool& bJump,					// ジャンプ状況
	bool& bHit,						// 衝突判定
	const bool bDelete				// 消去状態
)
{
	// 位置を取得
	D3DXVECTOR3 posMash = GetVec3Position();
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(fRadius, 0.0f, fRadius);

	if (collision::Box3D
	(
		rPos,			// 判定位置
		m_posDefault,	// 判定目標位置
		vtxMax,			// 判定サイズ(右・上・後)
		vtxMin,			// 判定サイズ(左・下・前)
		m_collMax,		// 判定目標サイズ(右・上・後)
		m_collMin		// 判定目標サイズ(左・下・前)
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
	return CActor::Collision
	(
		rPos,		// 位置
		rPosOld,	// 前回の位置
		fRadius,	// 半径
		fHeight,	// 高さ
		rMove,		// 移動量
		bJump,		// ジャンプ状況
		bHit,		// 衝突判定
		bDelete		// 消去状態
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

//==========================================
// 当たり判定のサイズの設定処理
//==========================================
void CMash::CollSizeSet(const D3DXVECTOR3& rScale)
{
	EAngle angle = useful::RotToFourDire(GetVec3Rotation().y);
	D3DXVECTOR3 ModelMax = GetModelData().vtxMax;
	D3DXVECTOR3 ModelMin = GetModelData().vtxMin;

	if (angle == EAngle::ANGLE_90 ||
		angle == EAngle::ANGLE_270)
	{ // 90度、270度の場合

		m_collMax = D3DXVECTOR3
		(
			ModelMax.z * rScale.z + SENSOR_RANGE,
			ModelMax.y * rScale.y,
			ModelMax.x * rScale.x
		);
		m_collMin = D3DXVECTOR3
		(
			-ModelMin.z * rScale.z + SENSOR_RANGE,
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
			ModelMax.z * rScale.z + SENSOR_RANGE
		);
		m_collMin = D3DXVECTOR3
		(
			-ModelMin.x * rScale.x,
			-ModelMin.y * rScale.y,
			-ModelMin.z * rScale.z + SENSOR_RANGE
		);
	}
}
