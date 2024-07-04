//===========================================
//
//  大砲のギミック(gimmick_canon.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "gimmick_canon.h"
#include "manager.h"
#include "player.h"
#include "field.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const float FEILD_LINE = 2600.0f; // フィールドの基準Z線
	const int ACTIVE_NUM = 5; // 発動に必要な人数
}

//===========================================
//  コンストラクタ
//===========================================
CGimmickCanon::CGimmickCanon() : CGimmickAction(),
m_fTarget(0.0f),
m_bShoot(false)
{
}

//===========================================
//  デストラクタ
//===========================================
CGimmickCanon::~CGimmickCanon()
{
}

//===========================================
//  初期化処理
//===========================================
HRESULT CGimmickCanon::Init(void)
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
void CGimmickCanon::Uninit(void)
{
	// 親クラスの終了
	CGimmickAction::Uninit();
}

//===========================================
//  更新処理
//===========================================
void CGimmickCanon::Update(const float fDeltaTime)
{
	// 発射処理
	if (IsActive() && DistancePlayer() && !m_bShoot)
	{
		GET_PLAYER->SetShoot(m_fTarget);
		m_bShoot = true;
	}

	// 親クラスの更新
	CGimmickAction::Update(fDeltaTime);
}

//===========================================
//  描画処理
//===========================================
void CGimmickCanon::Draw(CShader* pShader)
{
	// 親クラスの描画
	CGimmickAction::Draw(pShader);
}

//===========================================
//  待機位置の計算処理
//===========================================
D3DXVECTOR3 CGimmickCanon::CalcWaitPoint(const int Idx)
{
	// TODO モーションに合わせていい感じの処理を作る
	return GetVec3Position();
}

//===========================================
//  待機位置の向きの計算
//===========================================
D3DXVECTOR3 CGimmickCanon::CalcWaitRotation(const int Idx, const CPlayerClone* pClone)
{
	// TODO モーションに合わせていい感じの処理を作る
	return GetVec3Rotation();
}

//===========================================
//  生成処理
//===========================================
CGimmickCanon* CGimmickCanon::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, ETarget eTarget)
{
	// ギミックの生成
	CGimmickCanon* pGimmick = new CGimmickCanon;

	// メモリの確保に失敗した場合関数を抜ける
	if (pGimmick == nullptr) { assert(false); return nullptr; }

	// 初期化
	if (FAILED(pGimmick->Init())) { assert(false); SAFE_DELETE(pGimmick); return nullptr; }

	// 位置を設定
	pGimmick->SetVec3Position(rPos);

	// サイズの設定
	pGimmick->SetVec3Sizing(rRot);

	// 目標地点の設定
	pGimmick->SetTarget(FEILD_LINE * eTarget);

	// 必要人数の設定
	pGimmick->SetNumActive(ACTIVE_NUM);

	// 確保したアドレスを返す
	return pGimmick;
}
