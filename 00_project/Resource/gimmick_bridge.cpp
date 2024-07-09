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

//===========================================
//  定数定義
//===========================================
namespace
{
	const float DISTANCE = 30.0f;	// 待機位置との距離
	const float ACTIVE_UP = 10.0f;	// 橋がかけられた際のY位置上昇量
}

//===========================================
//  コンストラクタ
//===========================================
CGimmickBridge::CGimmickBridge() : CGimmickAction(),
m_bSet(false),
m_ConectPoint(),
m_vecToWait(VEC3_ZERO),
m_nIdxWait(0),
m_pField(nullptr)
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
	// 橋の端の設定
	if (!m_bSet) { CalcConectPoint(); }

	// 橋を架ける
	if (IsActive())
	{
		Active();
		SetEnableDraw(false);
	}
	else
	{
		SAFE_UNINIT(m_pField);
		SetEnableDraw(true);
	}

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
D3DXVECTOR3 CGimmickBridge::CalcWaitPoint(const int Idx)
{
	// 受け取ったインデックスが最大値を超えている場合警告
	if (Idx > GetNumActive()) { assert(false); }

	// インデックス番号が0の場合2点のうちプレイヤーに近い方を待機中心とする
	if (Idx == 0)
	{
		// プレイヤー座標を取得
		D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();

		// プレイヤーと2点を結ぶベクトルを算出
		D3DXVECTOR3 vecToPlayer[2] = { posPlayer - m_ConectPoint[0], posPlayer - m_ConectPoint[1] };

		// 距離の2乗が小さい方の配列番号を保存
		if (vecToPlayer[0].x * vecToPlayer[0].x + vecToPlayer[0].z * vecToPlayer[0].z <=
			vecToPlayer[1].x * vecToPlayer[1].x + vecToPlayer[1].z * vecToPlayer[1].z)
		{
			m_nIdxWait = 0;
		}
		else
		{
			m_nIdxWait = 1;
		}

		// 中心から待機中心へのベクトルを算出し正規化する
		D3DXVECTOR3 vecToWait = m_ConectPoint[m_nIdxWait] - GetVec3Position();
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

		// 向きを寝そべる形にする
		return D3DXVECTOR3(-HALF_PI, fTemp + (D3DX_PI * (float)m_nIdxWait), 0.0f);
	}

	// 待機中心との差分を求める
	D3DXVECTOR3 vecCenter = GetActionPoint() - pClone->GetVec3Position();

	// 向きを求める
	D3DXVECTOR3 rot = VEC3_ZERO;
	rot.y = -atan2f(vecCenter.x, -vecCenter.z);

	// 算出した向きを返す
	return rot;
}

//===========================================
//  橋の端の計算処理
//===========================================
void CGimmickBridge::CalcConectPoint()
{
	// 設定済みフラグを立てる
	m_bSet = true;

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
	// 足場を生成する
	if (m_pField == nullptr)
	{
		// sizeを2次元に変換
		D3DXVECTOR2 size = D3DXVECTOR2(GetVec3Sizing().x, GetVec3Sizing().z);

		// TODO：位置上にあげる
		m_pField = CField::Create(CField::TYPE_BRIDGE, GetVec3Position() + D3DXVECTOR3(0.0f, 25.0f, 0.0f), GetVec3Rotation(), size, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), POSGRID2(10, 10), POSGRID2(10, 10));
		m_pField->SetEnableDraw(false);
	}

	// TODO : 分身の配置を変更？
}
