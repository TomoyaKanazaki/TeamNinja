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
#include "sound.h"

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
	const float ROTATE_SPEED = 0.05f; // 橋がかかるまでにかかる時間
}

//===========================================
//  コンストラクタ
//===========================================
CGimmickBridge::CGimmickBridge() : CGimmickAction(),
m_ConectPoint(),
m_vecToWait(VEC3_ZERO),
m_nIdxWait(0),
m_pField(nullptr),
m_fRot(0.0f),
m_fMoveTime(0.0f),
m_bMove(false)
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
	if (IsActive()) { Active(fDeltaTime); }
	else { SAFE_UNINIT(m_pField); m_fRot = 0.0f; }

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

	// 待機位置を算出する
	D3DXVECTOR3 pos = m_ConectPoint[m_nIdxWait] + D3DXVECTOR3(0.0f, ACTIVE_UP * (float)IsActive(), 0.0f);

	// 待機位置にエフェクトを移動する
	CEffekseer::CEffectData* pEffect = GetEffect();
	if (pEffect != nullptr) { pEffect->m_pos = pos; }

	// 分身のインデックスに合わせて座標をずらす
	pos += m_vecToWait * DISTANCE * (float)Idx;

	return pos;	// ギミック発動中なら少し上にずらす
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
		D3DXVECTOR3 rot = pClone->GetVec3Rotation();

		// 移動が完了している場合補正せずに値を返す
		if (m_bMove)
		{
			return rot;
		}

		// 向きを補正する
		rot.x += -HALF_PI * m_fMoveTime;
		if (rot.x <= -HALF_PI)
		{
			rot.x = -HALF_PI;
			m_bMove = true;

			// カメラが揺れる
			CManager::GetInstance()->GetCamera()->SetSwing(CCamera::TYPE_MAIN, SWING);

			// コントローラのバイブレーション
			GET_INPUTPAD->SetVibration(CInputPad::TYPE_BRIDGE);

			// 橋完成音を鳴らす
			PLAY_SOUND(CSound::LABEL_SE_GIMMICKBRIDGE);
		}

		// 向きを保存する
		m_fRot = rot.x;

		// 向きを返す
		return D3DXVECTOR3(rot.x, fTemp + (D3DX_PI * (float)m_nIdxWait), 0.0f);
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
void CGimmickBridge::Active(const float fDeltaTime)
{
	// 移動時間の加算
	m_fMoveTime += fDeltaTime * ROTATE_SPEED;

	// アクティブの瞬間の処理
	Moment();

	// アクティブ中の処理
	Movement();
}

//==========================================
//  アクティブ状態になった瞬間の処理
//==========================================
void CGimmickBridge::Moment()
{
	// 足場がすでに存在している場合関数を抜ける
	if (m_pField != nullptr) { return; }

	// 移動完了フラグをfalseに設定
	m_bMove = false;

	// 移動時間をリセット
	m_fMoveTime = 0.0f;

	// 自身の向きを取得
	EAngle angle = GetAngle();

	// 足場の座標を設定
	D3DXVECTOR3 posField = m_ConectPoint[m_nIdxWait];

	// 足場のサイズ
	D3DXVECTOR2 sizeField = VEC2_ZERO;

	// 足場の向き
	D3DXVECTOR3 rotField = VEC3_ZERO;
	float fRot = m_fRot + HALF_PI - (m_nIdxWait + 1.0f) * -1.0f * HALF_PI;

	// 足場の向きを決める
	switch (angle)
	{
	// x軸方向に架ける
	case ANGLE_90:
	case ANGLE_270:

		// 大きさを設定
		sizeField.x = fabsf(m_ConectPoint[0].x - m_ConectPoint[1].x);
		sizeField.y = FIELD_SIZE;

		// 位置を設定
		posField.x += sinf(fRot) * sizeField.x * 0.5f;
		posField.y += sinf(m_fRot + HALF_PI) * sizeField.x * 0.5f;

		// 向きを設定
		rotField.z = -fRot + m_nIdxWait * -1.0f * D3DX_PI;

		// 生成
		m_pField = CField::Create(CField::TYPE_XBRIDGE, posField, rotField, sizeField, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), POSGRID2(1, 1), POSGRID2(1, 1));

		break;

		// z軸方向に架ける
	case ANGLE_0:
	case ANGLE_180:

		// 大きさを設定
		sizeField.x = FIELD_SIZE;
		sizeField.y = fabsf(m_ConectPoint[0].z - m_ConectPoint[1].z);

		// 位置を設定
		posField.z += sinf(fRot) * sizeField.y * 0.5f;
		posField.y += sinf(m_fRot + HALF_PI) * sizeField.y * 0.5f;

		// 向きを設定
		rotField.x = fRot - m_nIdxWait * -1.0f * D3DX_PI;

		// 生成
		m_pField = CField::Create(CField::TYPE_ZBRIDGE, posField, rotField, sizeField, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), POSGRID2(1, 1), POSGRID2(1, 1));

		break;

	default:
		assert(false);
		break;
	}

	m_pField->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
}

//==========================================
//  アクティブ状態中の処理
//==========================================
void CGimmickBridge::Movement()
{
	// 足場が存在していない場合関数を抜ける
	if (m_pField == nullptr) { return; }

	// 移動が完了していた場合関数を抜ける
	if (m_bMove)
	{
		m_pField->SetVec3Rotation(VEC3_ZERO);
		return;
	}

	// 自身の向きを取得
	EAngle angle = GetAngle();

	// 足場の座標を設定
	D3DXVECTOR3 posField = m_ConectPoint[m_nIdxWait];

	// 足場の大きさを取得
	float fSizeField = 0.5f;
	switch (angle)
	{
	case ANGLE_90:
	case ANGLE_270:
		fSizeField *= m_pField->GetVec2Sizing().x;
		break;

	case ANGLE_0:
	case ANGLE_180:
		fSizeField *= m_pField->GetVec2Sizing().y;
		break;

	default:
		assert(false);
		break;
	}

	// 足場の向き
	D3DXVECTOR3 rotField = VEC3_ZERO;

	// 足場の向きからy座標を算出
	posField.y += sinf(m_fRot + HALF_PI) * fSizeField;

	// x,z座標を算出
	switch (angle)
	{
	case ANGLE_0:
	case ANGLE_180:
		posField.z += sinf(m_fRot) * fSizeField * (1.0f - m_nIdxWait * 2.0f);
		rotField.x = HALF_PI + m_fRot * (1.0f - m_nIdxWait * 2.0f);
		if (m_nIdxWait) { rotField.z -= D3DX_PI; }
		break;

	case ANGLE_90:
	case ANGLE_270:
		posField.x += sinf(m_fRot) * fSizeField * (1.0f - m_nIdxWait * 2.0f);
		rotField.z = HALF_PI - m_fRot * (1.0f - m_nIdxWait * 2.0f);
		if (!m_nIdxWait) { rotField.z -= D3DX_PI; }
		break;

	default:
		assert(false);
		break;
	}
	
	// 向きと座標を適用
	m_pField->SetVec3Position(posField);
	m_pField->SetVec3Rotation(rotField);
}
