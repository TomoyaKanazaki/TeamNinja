//============================================================
//
//	アクションギミック処理 [gimmick_action.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "gimmick_action.h"

#include "player_clone.h"
#include "player.h"
#include "collision.h"

#include "effekseerControl.h"
#include "effekseerManager.h"

#include "camera.h"
#include "multi_plant.h"

#include "object3D.h"

// 定数定義
namespace
{
	const float RADIUS = 70.0f;	// 半径
	const bool ACTION_SET_FRAG[] = // アクションポイントの設定可能フラグ
	{
		true, // ジャンプ台
		true, // 踏み台
		false, // 設置
		true, // 飛び降り
		false, // 重い扉
		false, // 橋
	};
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CGimmickAction>* CGimmickAction::m_pList = nullptr;	// オブジェクトリスト

//===========================================
//  静的警告処理
//===========================================
static_assert(NUM_ARRAY(ACTION_SET_FRAG) == CGimmick::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	子クラス [CGimmickAction] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGimmickAction::CGimmickAction() : CGimmick(),
m_nNumClone(0),					// 範囲に入っている分身の数
m_bActive(false),				// 発動状況
m_bMoment(false),				// 発動中
m_posAction(VEC3_ZERO)			// アクションポイント(待機座標)
{
	memset(&m_pEffect[0], 0, sizeof(m_pEffect));
}

//============================================================
//	デストラクタ
//============================================================
CGimmickAction::~CGimmickAction()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGimmickAction::Init(void)
{
	// メンバ変数の初期化
	m_posAction = GetVec3Position();

	// オブジェクト3Dの初期化
	if (FAILED(CGimmick::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CGimmickAction>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGimmickAction::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// エフェクトの終了
	for (int i = 0; i < 4; ++i)
	{
		m_pEffect[i] = nullptr;
	}

	// オブジェクト3Dの終了
	CGimmick::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGimmickAction::Update(const float fDeltaTime)
{
	// 分身との当たり判定
	CollisionClone();

	// 発動可能条件を false にする
	m_bActive = false;

	// 必要な分身が揃っていればフラグをon
	if (GetNumActive() <= m_nNumClone) { m_bActive = true; }

	// エフェクトの生成
	//DispEffect();

	// 親クラスの更新
	CGimmick::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CGimmickAction::Draw(CShader* pShader)
{
	// オブジェクト3Dの描画
	CGimmick::Draw(pShader);
}

//============================================================
// プレイヤーとの当たり判定(矩形)
//============================================================
bool CGimmickAction::CollisionPlayer(void)
{
	// 判定フラグ
	bool bHit = false;

	// プレイヤーのリスト構造が無ければ抜ける
	if (CPlayer::GetList() == nullptr) { return bHit; }

	std::list<CPlayer*> list = CPlayer::GetList()->GetList();	// リストを取得
	D3DXVECTOR3 pos = GetVec3Position();		// 位置
	D3DXVECTOR3 size = GetVec3Sizing() * 0.5f;	// サイズ
	D3DXVECTOR3 posPlayer = VEC3_ZERO;			// プレイヤーの位置
	D3DXVECTOR3 sizePlayer = D3DXVECTOR3(RADIUS, 0.0f, RADIUS);		// プレイヤーのサイズ

	for (auto player : list)
	{
		// 位置を取得
		posPlayer = player->GetVec3Position();

		if (collision::Box2D
		(
			pos,		// 判定位置
			posPlayer,	// 判定目標位置
			size,		// 判定サイズ(右・上・後)
			size,		// 判定サイズ(左・下・前)
			sizePlayer,	// 判定目標サイズ(右・上・後)
			sizePlayer	// 判定目標サイズ(左・下・前)
		))
		{ // 四角の中に入った場合
			bHit = true;
		}
	}

	return bHit;
}

//===========================================
//  プレイヤーとの当たり判定(円形)
//===========================================
bool CGimmickAction::DistancePlayer()
{
	// 判定フラグ
	bool bHit = false;

	// 発動可能フラグがoffなら関数を抜ける
	if (!m_bActive) { return bHit; }

	// プレイヤー情報の取得
	CPlayer* player = GET_PLAYER;

	// プレイヤーの座標を取得
	D3DXVECTOR3 posPlayer = player->GetVec3Position();

	// ギミックの中心座標を取得
	D3DXVECTOR3 posActive = GetActionPoint();

	// 中心座標とプレイヤーの距離の2乗を算出
	D3DXVECTOR3 vecToPlayer = posPlayer - posActive;
	float fDistance = vecToPlayer.x * vecToPlayer.x + vecToPlayer.z * vecToPlayer.z;

	// 距離の2乗がプレイヤーの半径2乗未満の場合フラグを立てる
	if (RADIUS * RADIUS > fDistance)
	{
		bHit = true;
	}

	return bHit;
}

//===========================================
//  分身の総数を加算
//===========================================
void CGimmickAction::AddNumClone()
{
	// 総数が必要数に達していたら関数を抜ける
	if (m_nNumClone >= GetNumActive()) { return; }
	
	// 加算
	++m_nNumClone;

	if (m_nNumClone >= GetNumActive())
	{ // 分身の数が必要数に達した場合

		// 抜け出す
		return;
	}


}

//===========================================
//  分身の総数を減算
//===========================================
void CGimmickAction::SabNumClone()
{
	// 総数が0に以下の場合関数を抜ける
	if (m_nNumClone <= 0) { return; }

	// 減算
	--m_nNumClone;
}

//==========================================
//	リスト取得処理
//==========================================
CListManager<CGimmickAction>* CGimmickAction::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//===========================================
//  範囲表示処理
//===========================================
void CGimmickAction::DispEffect()
{
	// 自身の座標を取得
	D3DXVECTOR3 posThis = GetVec3Position();

	D3DXVECTOR3 posVtx[4] = {};
	for (int i = 0; i < 4; ++i)
	{
		posVtx[i] = GetVertexPosition(i) + posThis;
	}

	// エフェクトの表示フラグ
	bool bDisp = false;

	// 全頂点を走査する
	for (int i = 0; i < 4; ++i)
	{
		// 頂点座標を取得する
		posVtx[i] = GetVertexPosition(i) + posThis;

		// 1点でもスクリーン内の場合次に進む
		if (bDisp) { continue; }

		// スクリーン内判定
		bDisp = SCREEN_IN(posVtx[i]);
	}

	//スクリーン外の場合エフェクトを削除して関数を抜ける
	if (!bDisp)
	{
		// エフェクトの終了
		for (int i = 0; i < 4; ++i)
		{
			if (m_pEffect[i] != nullptr) { SAFE_DELETE(m_pEffect[i]); }
		}

		return;
	}
	
	// エフェクトの生成
	for (int i = 0; i < 4; ++i)
	{
		// 既に生成されていた場合次に進む
		if (m_pEffect[i] != nullptr) { continue; }

		m_pEffect[i] = GET_EFFECT->Create
		(
			"data\\EFFEKSEER\\smoke.efkefc",
			posVtx[i],
			VEC3_ZERO,
			VEC3_ZERO,
			5.0f,
			true
		);
	}
}

//===========================================
//  発動判定位置の設定
//===========================================
void CGimmickAction::SetActionPoint(const D3DXVECTOR3& pos)
{
	// 設定可能フラグがoffの場合中心座標を設定し関数を抜ける
	if (!ACTION_SET_FRAG[GetType()])
	{ m_posAction = GetVec3Position(); return; }

	// 待機位置に引数を設定する
	m_posAction = pos;
}

//===========================================
//  サイズの設定
//===========================================
void CGimmickAction::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// サイズの設定
	CObject3D::SetVec3Sizing(rSize);

	// 植物の生成
	CMultiPlant::Create(GetVec3Position(), rSize, GetType(), GetNumActive());
}
