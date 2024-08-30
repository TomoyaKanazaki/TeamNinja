//===========================================
//
//  橋のギミック(gimmick_bridge.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "gimmick_bridge.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"
#include "field.h"
#include "multi_plant.h"

#include "camera.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const float DISTANCE = 30.0f; // 待機位置との距離
	const float ACTIVE_UP = 10.0f; // 橋がかけられた際のY位置上昇量
	const float FIELD_SIZE = 55.0f; // 橋の幅
	const float PLANT_RANGE = 50.0f; // 花の咲く範囲
	const CCamera::SSwing SWING = CCamera::SSwing(10.0f, 2.0f, 0.6f);		// カメラ揺れの値
}

//===========================================
//  コンストラクタ
//===========================================
CGimmickBridge::CGimmickBridge() : CGimmickAction(),
m_ConectPoint(),
m_vecToWait(VEC3_ZERO),
m_nIdxWait(0),
m_pField(nullptr),
m_fRot(0.0f)
{
}

//===========================================
//  デストラクタ
//===========================================
CGimmickBridge::~CGimmickBridge()
{
}

//===========================================
//  初期化処理
//===========================================
HRESULT CGimmickBridge::Init(void)
{
	// 親クラスの初期化
	if (FAILED(CGimmickAction::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
//  終了処理
//===========================================
void CGimmickBridge::Uninit(void)
{
	// 親クラスの終了
	CGimmickAction::Uninit();
}

//===========================================
//  更新処理
//===========================================
void CGimmickBridge::Update(const float fDeltaTime)
{
	// 遠距離判定
	bool bFar = useful::IsNearPosR(GetVec3Position());
	if (!bFar) { return; }

	// 橋を架ける
	if (IsActive()) { Active(); }
	else { SAFE_UNINIT(m_pField); }

	// 親クラスの更新
	CGimmickAction::Update(fDeltaTime);
}

//===========================================
//  描画処理
//===========================================
void CGimmickBridge::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmickAction::Draw(pShader);
}

//===========================================
//  各分身毎の待機位置を算出
//===========================================
D3DXVECTOR3 CGimmickBridge::CalcWaitPoint(const int Idx, const CPlayerClone* pClone)
{
	// 受け取ったインデックスが最大値を超えている場合警告
	if (Idx > GetNumActive()) { assert(false); }
	
	// 方向の取得
	EAngle angle = GetAngle();

	// インデックス番号が0の場合2点のうちプレイヤーに近い方を待機中心とする
	if (Idx == 0 && !IsActive())
	{
		// プレイヤー座標を取得
		D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();

		// プレイヤーと2点を結ぶベクトルを算出
		D3DXVECTOR3 vecToPlayer[2] = { posPlayer - m_ConectPoint[0], posPlayer - m_ConectPoint[1] };

		// 2点を結ぶベクトル
		D3DXVECTOR3 vecToWait = VEC3_ZERO;

		// xzの片軸を分身の位置にする
		switch (angle)
		{
		case ANGLE_90:
		case ANGLE_270:
			m_ConectPoint[0].z = pClone->GetVec3Position().z;
			m_ConectPoint[1].z = pClone->GetVec3Position().z;
			break;

		case ANGLE_0:
		case ANGLE_180:
			m_ConectPoint[0].x = pClone->GetVec3Position().x;
			m_ConectPoint[1].x = pClone->GetVec3Position().x;
			break;

		default:
			assert(false);
			break;
		}

		// 距離の2乗が小さい方の配列番号を保存
		if (vecToPlayer[0].x * vecToPlayer[0].x + vecToPlayer[0].z * vecToPlayer[0].z <=
			vecToPlayer[1].x * vecToPlayer[1].x + vecToPlayer[1].z * vecToPlayer[1].z)
		{
			m_nIdxWait = 0;
			vecToWait = m_ConectPoint[0] - m_ConectPoint[1];
		}
		else
		{
			m_nIdxWait = 1;
			vecToWait = m_ConectPoint[1] - m_ConectPoint[0];
		}

		// 待機中心同士を結ぶベクトルを算出し正規化する
		D3DXVec3Normalize(&m_vecToWait, &vecToWait);
	}

	// 待機位置を返す
	return m_ConectPoint[m_nIdxWait] + (m_vecToWait * DISTANCE * (float)Idx) + D3DXVECTOR3(0.0f, ACTIVE_UP * (float)IsActive(), 0.0f);	// ギミック発動中なら少し上にずらす
}

//===========================================
//  各分身毎の待機向きを算出
//===========================================
D3DXVECTOR3 CGimmickBridge::CalcWaitRotation(const int Idx, const CPlayerClone* pClone)
{
	// 受け取ったインデックスが最大値を超えている場合警告
	if (Idx > GetNumActive()) { assert(false); }

	if (IsActive())
	{ // ギミック発動中の場合
		// 方向を取得
		EAngle angle = GetAngle();
		float fTemp = 0.0f;

		// y軸を設定
		switch (angle)
		{
		case ANGLE_90:
		case ANGLE_270:

			fTemp = D3DX_PI * 0.5f;
			break;

		case ANGLE_0:
		case ANGLE_180:

			fTemp = 0.0f;
			break;

		default:
			assert(false);
			break;
		}

		// 分身の向きを取得
		float fRot = pClone->GetVec3Rotation().x;

		// 向きを補正する
		fRot += (-HALF_PI - fRot) * 0.07f;
		if (fRot < -HALF_PI)
		{
			fRot = -HALF_PI;
		}

		// 向きを保存する
		m_fRot = fRot;

		// 向きを返す
		return D3DXVECTOR3(fRot, fTemp + (D3DX_PI * (float)m_nIdxWait), 0.0f);
	}

	// 向きを求める
	D3DXVECTOR3 rot = VEC3_ZERO;
	rot.y = atan2f(m_vecToWait.x, m_vecToWait.z);

	// 算出した向きを返す
	return rot;
}

//===========================================
//   サイズの設定
//===========================================
void CGimmickBridge::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// サイズの設定
	CObject3D::SetVec3Sizing(rSize);
	
	// 花の生成位置を設定する
	D3DXVECTOR3 posPlant[2] = { GetVec3Position(), GetVec3Position() };

	// 方向の取得
	EAngle angle = GetAngle();
	float fAngle = ANGLE_PI(angle) + D3DX_PI * 0.5f;

	// 方向に合わせて生成位置をずらす
	posPlant[0] += D3DXVECTOR3((rSize.x + PLANT_RANGE) * 0.5f * cosf(fAngle), 0.0f, (rSize.z + PLANT_RANGE) * 0.5f * sinf(fAngle));
	posPlant[1] += D3DXVECTOR3((rSize.x + PLANT_RANGE) * 0.5f * cosf(fAngle + D3DX_PI), 0.0f, (rSize.z + PLANT_RANGE) * 0.5f * sinf(fAngle + D3DX_PI));

	// サイズの設定
	D3DXVECTOR3 size = GetVec3Sizing();
	switch (angle)
	{
	case ANGLE_0:
	case ANGLE_180:
		size.z = PLANT_RANGE;
		break;

	case ANGLE_90:
	case ANGLE_270:
		size.x = PLANT_RANGE;
		break;

	default:
		assert(false);
		break;
	}

	// 花を生成
	for (int i = 0; i < 2; ++i)
	{
		CMultiPlant::Create(posPlant[i], size, GetType(), GetNumActive());
	}

	// 橋の端を設定
	CalcConectPoint();
}

//===========================================
//  橋の端の計算処理
//===========================================
void CGimmickBridge::CalcConectPoint()
{
	// 自身の位置を取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 自身のサイズを取得
	D3DXVECTOR3 size = GetVec3Sizing();

	// 自身の方向を取得
	EAngle angle = GetAngle();

	// 計算を行う
	switch (angle)
	{
	// x軸方向に架ける
	case ANGLE_90:
	case ANGLE_270:

		m_ConectPoint[0] = pos + D3DXVECTOR3(size.x * 0.5f, 0.0f, 0.0f);
		m_ConectPoint[1] = pos - D3DXVECTOR3(size.x * 0.5f, 0.0f, 0.0f);
		break;

	// z軸方向に架ける
	case ANGLE_0:
	case ANGLE_180:

		m_ConectPoint[0] = pos + D3DXVECTOR3(0.0f, 0.0f, size.z * 0.5f);
		m_ConectPoint[1] = pos - D3DXVECTOR3(0.0f, 0.0f, size.z * 0.5f);
		break;

	default:
		assert(false);
		break;
	}
}

//===========================================
//  アクティブ状態の処理
//===========================================
void CGimmickBridge::Active()
{
	// nullチェック
	if (m_pField != nullptr) { return; }

	// 向きが一定の値を上回っている場合関数を抜ける
	if (m_fRot - -HALF_PI > 0.01f)
	{
		// カメラが揺れる
		CManager::GetInstance()->GetCamera()->SetSwing(CCamera::TYPE_MAIN, SWING);

		return;
	}

	// 足場の座標を設定
	D3DXVECTOR3 posField = (m_ConectPoint[0] + m_ConectPoint[1]) * 0.5f;

	// 足場のサイズ
	D3DXVECTOR2 sizeField = VEC2_ZERO;

	// 自身の方向を取得
	EAngle angle = GetAngle();

	// 橋の方向を決める
	switch (angle)
	{
	// x軸方向に架ける
	case ANGLE_90:
	case ANGLE_270:

		sizeField.x = fabsf(m_ConectPoint[0].x - m_ConectPoint[1].x);
		sizeField.y = FIELD_SIZE;

		// 生成
		m_pField = CField::Create(CField::TYPE_XBRIDGE, posField, VEC3_ZERO, sizeField, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), POSGRID2(1, 1), POSGRID2(1, 1));

		break;

	// z軸方向に架ける
	case ANGLE_0:
	case ANGLE_180:

		sizeField.x = FIELD_SIZE;
		sizeField.y = fabsf(m_ConectPoint[0].z - m_ConectPoint[1].z);

		// 生成
		m_pField = CField::Create(CField::TYPE_ZBRIDGE, posField, VEC3_ZERO, sizeField, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), POSGRID2(1, 1), POSGRID2(1, 1));

		break;

	default:
		assert(false);
		break;
	}
}
