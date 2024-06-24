//===========================================
//
//  橋のギミック(gimmick_bridge.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "gimmick_bridge.h"
#include "manager.h"
#include "player.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const float DISTANCE = 30.0f; // 待機位置との距離
}

//===========================================
//  コンストラクタ
//===========================================
CGimmickBridge::CGimmickBridge() : CGimmickAction(),
m_bSet(false),
m_ConectPoint(),
m_vecToWait(VEC3_ZERO),
m_nIdxWait(0)
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
	return m_ConectPoint[m_nIdxWait] + (m_vecToWait * DISTANCE * (float)Idx);
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

	// 計算を行う
	if (size.x < size.z) // z方向に架かる場合
	{
		// 中心座標にサイズ * 0.5を加算する
		m_ConectPoint[0] = pos + D3DXVECTOR3(0.0f, 0.0f, size.z * 0.5f);
		m_ConectPoint[1] = pos - D3DXVECTOR3(0.0f, 0.0f, size.z * 0.5f);
	}
	else if (size.x > size.z) // x方向に架かる場合
	{
		// 中心座標にサイズ * 0.5を加算する
		m_ConectPoint[0] = pos + D3DXVECTOR3(size.x * 0.5f, 0.0f, 0.0f);
		m_ConectPoint[1] = pos - D3DXVECTOR3(size.x * 0.5f, 0.0f, 0.0f);
	}
	else // xzのサイズが一致している場合
	{
		// 本当はやめてほしい。
		assert(false);
		
		// 中心座標にサイズ * 0.5を加算する
		m_ConectPoint[0] = pos + D3DXVECTOR3(size.x * 0.5f, 0.0f, size.z * 0.5f);
		m_ConectPoint[1] = pos - D3DXVECTOR3(size.x * 0.5f, 0.0f, size.z * 0.5f);
	}
}
