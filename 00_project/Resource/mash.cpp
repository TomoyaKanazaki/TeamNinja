//==========================================
//
//  ふすまクラス(mash.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "mash.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float MOVE_SPEED = 100.0f; // 移動速度
}

//==========================================
//  コンストラクタ
//==========================================
CMash::CMash() :
	m_posDefault(VEC3_ZERO),
	m_offsetMove(VEC3_ZERO),
	m_move(VEC3_ZERO)
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
	// 初期位置を保存する
	m_posDefault = rPos;

	// 移動する距離を取得する
	float offsetMax = GetModelData().vtxMax.z * 2.0f;

	// 向きから移動先オフセットを算出する
	float fRot = GetVec3Rotation().y + (D3DX_PI * 0.5f);
	m_offsetMove = D3DXVECTOR3
	(
		offsetMax * cosf(fRot),
		0.0f,
		offsetMax * sinf(fRot)
	);

	// 向きから移動量を算出する
	m_move = D3DXVECTOR3
	(
		MOVE_SPEED * cosf(fRot),
		0.0f,
		MOVE_SPEED * sinf(fRot)
	);

	// 親クラスの位置設定
	CObject::SetVec3Position(rPos);
}

//==========================================
//  移動処理
//==========================================
void CMash::Move(const float fDeltaTime)
{
	// 座標を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 移動量を加算
	pos += m_move * fDeltaTime;
}
