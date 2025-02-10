//==========================================
//
//  目標に向かって移動するエフェクトのクラス(effect_move.cpp)
//  Author : Tomoya Kanazaki 
//
//==========================================
#include "effect_move.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float EFFECT_SCALE = 50.0f; // エフェクトの大きさ
	const float TARGET_HEIGHT = 50.0f; // 目標座標の高さ補正値
}

//==========================================
//  コンストラクタ
//==========================================
CMoveEffect::CMoveEffect() : CObject(LABEL_EFFECT),
	m_pos(VEC3_ZERO),
	m_fDeleteDistance(0.0f),
	m_fSpeed(0.0f),
	m_pTargetObj(nullptr),
	m_posTarget(VEC3_ZERO),
	m_pEffect(nullptr)
{
	// Do Nothing
}

//==========================================
//  デストラクタ
//==========================================
CMoveEffect::~CMoveEffect()
{
	// Do Nothing
}

//==========================================
//  初期化処理
//==========================================
HRESULT CMoveEffect::Init()
{
	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void CMoveEffect::Uninit()
{
	// エフェクト情報の破棄
	SAFE_DELETE(m_pEffect);

	// 自身を破棄
	Release();
}

//==========================================
//  更新処理
//==========================================
void CMoveEffect::Update(const float fDeltaTime)
{
	// 移動処理
	Move(fDeltaTime);
}

//==========================================
//  終了処理
//==========================================
void CMoveEffect::Draw(CShader* pShader)
{
	// 見た目はエフェクシア側で描画される
}

//==========================================
//  生成処理
//==========================================
CMoveEffect* CMoveEffect::Create(std::string path, const D3DXVECTOR3& posDef, float fSpeed, const D3DXVECTOR3& posTarget, float fDistanceDelete)
{
	// ポインタを宣言
	CMoveEffect* pEffect = new CMoveEffect;	// プレイヤー情報

	// 生成に失敗した場合nullを返す
	if (pEffect == nullptr) { assert(false); return nullptr; }

	// プレイヤーの初期化
	if (FAILED(pEffect->Init()))
	{ // 初期化に失敗した場合
		assert(false);
		// プレイヤーの破棄
		SAFE_DELETE(pEffect);
		return nullptr;
	}

	// 各種情報を設定する
	pEffect->m_pos = posDef; // 初期位置
	pEffect->m_fSpeed = fSpeed; // 移動速度
	pEffect->m_posTarget = posTarget; // 目標座標
	pEffect->m_fDeleteDistance = fDistanceDelete; // 削除範囲

	// 確保したアドレスを返す
	return pEffect;

}

//==========================================
//  生成処理
//==========================================
CMoveEffect* CMoveEffect::Create(std::string path, const D3DXVECTOR3& posDef, float fSpeed, CObject* pObj, float fDistanceDelete)
{
	// ポインタを宣言
	CMoveEffect* pEffect = new CMoveEffect;	// プレイヤー情報

	// 生成に失敗した場合nullを返す
	if (pEffect == nullptr) { return nullptr; }

	// プレイヤーの初期化
	if (FAILED(pEffect->Init()))
	{ // 初期化に失敗した場合

		// プレイヤーの破棄
		SAFE_DELETE(pEffect);
		return nullptr;
	}

	// 各種情報を設定する
	pEffect->m_pos = posDef; // 初期位置
	pEffect->m_fSpeed = fSpeed; // 移動速度
	pEffect->m_pTargetObj = pObj; // 目標オブジェクト
	pEffect->m_fDeleteDistance = fDistanceDelete; // 削除範囲

	// エフェクトを生成する
	pEffect->CreateEffect(path);

	// 確保したアドレスを返す
	return pEffect;
}

//==========================================
//  エフェクトの生成
//==========================================
void CMoveEffect::CreateEffect(std::string path)
{
	m_pEffect = GET_EFFECT->Create(path, m_pos, VEC3_ZERO, VEC3_ZERO, EFFECT_SCALE, true, false);
}

//==========================================
//  移動処理
//==========================================
void CMoveEffect::Move(const float fDeltaTime)
{
	DebugProc::Print(DebugProc::POINT_CENTER, "Effectpos : (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);

	// 目標オブジェクトが設定されている場合
	if (m_pTargetObj != nullptr)
	{
		// 目標の位置を取得
		m_posTarget = m_pTargetObj->GetVec3Position();
		m_posTarget.y += TARGET_HEIGHT;
	}

	// 目標地点と自身の座標を結ぶベクトルを算出
	D3DXVECTOR3 vec = m_posTarget - m_pos;

	// 算出したベクトルの大きさが削除範囲内の場合
	if (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z <= m_fDeleteDistance * m_fDeleteDistance)
	{
		// 自身を終了し関数を抜ける
		Uninit();
		return;
	}

	// 算出した値を正規化する
	D3DXVec3Normalize(&vec, &vec);

	// 正規化した値に移動量をかけ合わせる
	vec *= m_fSpeed * fDeltaTime;

	// 現在の座標に移動量を加算する
	m_pos += vec;

	// 座標を見た目に適用する
	m_pEffect->m_pos = Effekseer::Vector3D(m_pos.x, m_pos.y, m_pos.z);
}
