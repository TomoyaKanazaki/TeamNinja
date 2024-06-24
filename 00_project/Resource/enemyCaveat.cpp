//============================================================
//
//	警告敵処理 [enemy_Caveat.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "enemyCaveat.h"
#include "renderer.h"
#include "deltaTime.h"

#include "player.h"
#include "player_clone.h"
#include "multiModel.h"
#include "enemy_item.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\enemy.txt";	// セットアップテキスト相対パス
	const float MOVE = -300.0f;								// 移動量
	const float ROT_REV = 0.5f;								// 向きの補正係数

	const D3DXVECTOR3 OFFSET = D3DXVECTOR3(-3.0f, -15.0f, 0.0f);	// オフセット座標
}

//************************************************************
//	子クラス [CEnemyCaveat] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyCaveat::CEnemyCaveat() : CEnemy(),
m_state(STATE_CRAWL)		// 状態
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyCaveat::~CEnemyCaveat()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyCaveat::Init(void)
{
	// 敵の初期化
	if (FAILED(CEnemy::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キャラクター情報の割当
	BindCharaData(SETUP_TXT);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CEnemyCaveat::Uninit(void)
{
	// 敵の終了
	CEnemy::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyCaveat::Update(const float fDeltaTime)
{
	// 敵の更新
	CEnemy::Update(fDeltaTime);

	if (GetItem() != nullptr)
	{ // アイテムを持っている場合

		// アイテムのオフセット処理
		GetItem()->Offset(GetParts(8)->GetMtxWorld());
	}
}

//============================================================
//	描画処理
//============================================================
void CEnemyCaveat::Draw(CShader* pShader)
{
	// 敵の描画
	CEnemy::Draw(pShader);
}

//============================================================
// 情報の設定処理
//============================================================
void CEnemyCaveat::SetData(void)
{
	// アイテムを設定する
	SetItem(CEnemyItem::Create
	(
		CEnemyItem::TYPE_KATANA,
		OFFSET,
		GetParts(8)->GetMtxWorld()
	));
}

//============================================================
// 状態の更新処理
//============================================================
int CEnemyCaveat::UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// 現在のモーション
	switch (m_state)
	{
	case CEnemyCaveat::STATE_CRAWL:

		if (SearchPlayer() ||
			SearchClone())
		{ // 視界内にプレイヤーか分身が入った場合

			// 警告状態にする
			m_state = STATE_CAVEAT;
		}

		break;

	case CEnemyCaveat::STATE_CAVEAT:

		// 警告処理
		Caveat();

		break;

	default:		// 例外処理
		assert(false);
		break;
	}

	// 現在のモーションを返す
	return nCurMotion;
}

//============================================================
// モーションの更新処理
//============================================================
void CEnemyCaveat::UpdateMotion(int nMotion, const float fDeltaTime)
{

}

//============================================================
// 着地の更新処理
//============================================================
void CEnemyCaveat::UpdateLanding(D3DXVECTOR3* pPos)
{
	// 親クラスの着地更新
	CEnemy::UpdateLanding(pPos);

	// 現在のモーション種類を取得
	int nCurMotion = GetMotionType();

	// 落下モーションのフラグを設定
	bool bTypeFall = nCurMotion == MOTION_FALL;

	if (!IsJump())
	{ // 空中にいない場合

		if (bTypeFall)
		{ // モーションが落下中の場合

			// 着地モーションを指定
			SetMotion(MOTION_LANDING);
		}
	}
	else
	{ // 空中にいる場合

		// 落下モーションを指定
		SetMotion(MOTION_FALL);
	}
}

//============================================================
// 警告処理
//============================================================
void CEnemyCaveat::Caveat(void)
{

}
