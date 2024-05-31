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

// 定数定義
namespace
{
	const D3DXVECTOR3 CLONE_RADIUS = D3DXVECTOR3(20.0f, 0.0f, 20.0f);	// 半径
	const int INIT_NUM_ACTIVE = 2;			// 発動可能人数の初期値(TODO：後でデータ書き出し化する予定)
}

//************************************************************
//	子クラス [CGimmickAction] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGimmickAction::CGimmickAction() : CGimmick(),
m_nNumClone(0),					// 範囲に入っている分身の数
m_nNumActive(INIT_NUM_ACTIVE),	// 発動可能な分身の数
m_bActive(false)				// 発動状況
{

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
	// オブジェクト3Dの初期化
	if (FAILED(CGimmick::Init()))
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
void CGimmickAction::Uninit(void)
{
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
	if (m_nNumActive <= m_nNumClone) { m_bActive = true; }

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
// クローンとの当たり判定
//============================================================
void CGimmickAction::CollisionClone(void)
{
	// 分身のリスト構造が無ければ抜ける
	if (CPlayerClone::GetList() == nullptr) { m_nNumClone = 0; return; }

	std::list<CPlayerClone*> list = CPlayerClone::GetList()->GetList();	// リストを取得
	D3DXVECTOR3 pos = GetVec3Position();	// 位置
	D3DXVECTOR3 size = GetVec3Sizing() * 0.5f;	// サイズ
	D3DXVECTOR3 posClone = VEC3_ZERO;		// 分身の位置
	D3DXVECTOR3 sizeClone = CLONE_RADIUS;	// 分身のサイズ
	int nNumClone = 0;						// 範囲内の分身の総数

	for (auto clone : list)
	{
		// 追跡する分身だった場合次の分身にする
		if (clone->GetAction() == CPlayerClone::ACTION_CHASE) { continue; }

		// 位置を取得
		posClone = clone->GetVec3Position();

		if (collision::Box2D
		(
			pos,		// 判定位置
			posClone,	// 判定目標位置
			size,		// 判定サイズ(右・上・後)
			size,		// 判定サイズ(左・下・前)
			sizeClone,	// 判定目標サイズ(右・上・後)
			sizeClone	// 判定目標サイズ(左・下・前)
		))
		{ // 四角の中に入った場合

			// 分身に自分の情報を渡す
			clone->SetGimmick(this);

			// 分身の総数を加算する
			nNumClone++;

			// 分身の総数が必要数に達したらループを抜ける
			if (nNumClone >= m_nNumActive) { break; }
		}
	}

	// 分身の総数を設定する
	m_nNumClone = nNumClone;
}

//============================================================
// プレイヤーとの当たり判定
//============================================================
bool CGimmickAction::CollisionPlayer(void)
{
	// 判定フラグ
	bool bHit = false;

	// 発動可能フラグがoffなら関数を抜ける
	if (!m_bActive) { return bHit; }

	// プレイヤーのリスト構造が無ければ抜ける
	if (CPlayer::GetList() == nullptr) { return bHit; }

	std::list<CPlayer*> list = CPlayer::GetList()->GetList();	// リストを取得
	D3DXVECTOR3 pos = GetVec3Position();		// 位置
	D3DXVECTOR3 size = GetVec3Sizing() * 0.5f;	// サイズ
	D3DXVECTOR3 posPlayer = VEC3_ZERO;			// プレイヤーの位置
	D3DXVECTOR3 sizePlayer = CLONE_RADIUS;		// プレイヤーのサイズ

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
