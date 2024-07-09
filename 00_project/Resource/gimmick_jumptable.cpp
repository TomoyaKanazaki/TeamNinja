//============================================================
//
//	ジャンプ台処理 [gimmick_jumptable.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "gimmick_jumptable.h"
#include "player.h"
#include "player_clone.h"

//===========================================
//  定数定義
//===========================================
namespace
{
	const float DISTANCE_CENTER = 50.0f; // 待機中心との距離
}

//************************************************************
//	子クラス [CGimmickJumpTable] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGimmickJumpTable::CGimmickJumpTable() : CGimmickAction()
{

}

//============================================================
//	デストラクタ
//============================================================
CGimmickJumpTable::~CGimmickJumpTable()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGimmickJumpTable::Init(void)
{
	// オブジェクト3Dの初期化
	if (FAILED(CGimmickAction::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGimmickJumpTable::Uninit(void)
{
	// オブジェクト3Dの終了
	CGimmickAction::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGimmickJumpTable::Update(const float fDeltaTime)
{
	// プレイヤーとの当たり判定
	if (HitPlayer())
	{
		// プレイヤーを大ジャンプ！
		SetMoment(GET_PLAYER->GimmickHighJump(GetNumActive()));
	}
	else
	{
		// ジャンプしていない場合false
		SetMoment(false);
	}

	// 親クラスの更新
	CGimmickAction::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CGimmickJumpTable::Draw(CShader* pShader)
{
	// オブジェクト3Dの描画
	CGimmickAction::Draw(pShader);
}

//===========================================
//  各分身毎の待機位置を算出
//===========================================
D3DXVECTOR3 CGimmickJumpTable::CalcWaitPoint(const int Idx, const CPlayerClone* pClone)
{
	// 受け取ったインデックスが最大値を超えている場合警告
	if (Idx > GetNumActive()) { assert(false); }

	// TODO：ここに打ち上げモーション中なら抜ける処理記述
	//if ()

	D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 posAction = GetActionPoint();				// 発動判定位置

	// 目標方向との差分を求める
	D3DXVECTOR3 vecTarget = posPlayer - posAction;

	// 差分ベクトルの向きを求める
	float fRot = atan2f(vecTarget.x, -vecTarget.z) + (((D3DX_PI * 2.0f) / GetNumActive()) * Idx);
	useful::NormalizeRot(fRot);

	// 差分ベクトル方向に傾けて座標を設定
	D3DXVECTOR3 posWait = D3DXVECTOR3
	(
		posAction.x + cosf(fRot) * DISTANCE_CENTER,
		posAction.y,
		posAction.z + sinf(fRot) * DISTANCE_CENTER
	);

	// 算出した座標を返す
	return posWait;
}

//===========================================
//  各分身毎の待機向きを算出
//===========================================
D3DXVECTOR3 CGimmickJumpTable::CalcWaitRotation(const int Idx, const CPlayerClone* pClone)
{
	// 受け取ったインデックスが最大値を超えている場合警告
	if (Idx > GetNumActive()) { assert(false); }

	// 待機中心との差分を求める
	D3DXVECTOR3 vecCenter = GetActionPoint() - pClone->GetVec3Position();

	// 向きを求める
	D3DXVECTOR3 rot = VEC3_ZERO;
	rot.y = -atan2f(vecCenter.x, -vecCenter.z);

	// 算出した向きを返す
	return rot;
}

//===========================================
//  プレイヤーとの判定
//===========================================
bool CGimmickJumpTable::HitPlayer()
{
	// プレイヤーのy座標を取得
	float fPosYPlayer = GET_PLAYER->GetVec3Position().y;

	// 分身の身長を取得
	float fHeightClone = CPlayerClone::GetHeight();

	// 自身のy座標を取得
	float fPosYThis = GetVec3Position().y;

	// 自身の高さ + 分身の身長 がプレイヤーより低い位置の場合falseを返す
	if (fPosYThis + fHeightClone < fPosYPlayer) { return false; }
	
	return DistancePlayer();
}
