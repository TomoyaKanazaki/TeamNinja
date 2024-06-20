//============================================================
//
//	犬敵処理 [enemyDog.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "enemyWolf.h"
#include "renderer.h"
#include "deltaTime.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\wolf.txt";	// セットアップテキスト相対パス
	const int	BLEND_FRAME_OTHER = 5;		// モーションの基本的なブレンドフレーム
	const int	BLEND_FRAME_LAND = 15;		// モーション着地のブレンドフレーム
	const int	CAUTIOUS_TRANS_LOOP = 7;	// 警戒モーションに遷移する待機ループ数
}

//************************************************************
//	子クラス [CEnemyWolf] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CEnemyWolf::CEnemyWolf() : CEnemy()
{

}

//============================================================
//	デストラクタ
//============================================================
CEnemyWolf::~CEnemyWolf()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CEnemyWolf::Init(void)
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
void CEnemyWolf::Uninit(void)
{
	// 敵の終了
	CEnemy::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CEnemyWolf::Update(const float fDeltaTime)
{
	// 過去位置更新
	UpdateOldPosition();

	// 敵の更新
	CEnemy::Update(fDeltaTime);

	// モーションの更新処理
	UpdateMotion(MOTION_IDOL, fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CEnemyWolf::Draw(CShader* pShader)
{
	// 敵の描画
	CEnemy::Draw(pShader);
}

//============================================================
// 情報の設定処理
//============================================================
void CEnemyWolf::SetData(void)
{

}

//============================================================
//	モーション・キャラクターの更新処理
//============================================================
void CEnemyWolf::UpdateMotion(int nMotion, const float fDeltaTime)
{
//	// 死んでたら抜ける
//	if (IsDeath()) { return; }
//
//	int nAnimMotion = GetMotionType();	// 現在再生中のモーション
//
//	if (nMotion != NONE_IDX)
//	{ // モーションが設定されている場合
//
//		if (IsMotionLoop())
//		{ // ループするモーション中の場合
//
//			if (nAnimMotion != nMotion)
//			{ // 現在のモーションが再生中のモーションと一致しない場合
//
//				// 現在のモーションの設定
//				SetMotion(nMotion, BLEND_FRAME_OTHER);
//			}
//		}
//		else
//		{ // ループしないモーション中の場合
//
//			switch (GetMotionType())
//			{
//			case CEnemyWolf::MOTION_FOUND:		// 発見モーション
//			case CEnemyWolf::MOTION_TURN:		// 振り向きモーション
//			case CEnemyWolf::MOTION_BITE:
//
//
//
//				break;
//			}
//
//			//switch (GetMotionType())
//			//{ // モーションごとの処理
//			//case MOTION_CAUTIOUS:	// 警戒モーション
//			//case MOTION_LANDING:	// 着地モーション
//
//			//	if (nMotion != MOTION_IDOL)
//			//	{ // 待機モーション以外の場合
//
//			//		// 現在のモーションの設定
//			//		SetMotion(nMotion, BLEND_FRAME_OTHER);
//			//	}
//
//			//	break;
//			//}
//		}
//	}
//
//	// オブジェクトキャラクターの更新
//	CObjectChara::Update(fDeltaTime);
//
//	switch (GetMotionType())
//	{ // モーションの種類ごとの処理
//	case MOTION_IDOL:	// 待機モーション
//
//		if (GetMotionNumLoop() >= CAUTIOUS_TRANS_LOOP)
//		{ // 待機モーションでしばらくいた場合
//
//			// 警戒モーションの設定
//			SetMotion(MOTION_CAUTIOUS, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case MOTION_CAUTIOUS:	// 警戒モーション
//
//		if (IsMotionFinish())
//		{ // モーションが再生終了した場合
//
//			// 現在のモーションの設定
//			SetMotion(nMotion, BLEND_FRAME_LAND);
//		}
//
//		break;
//
//#if 0
//	case MOTION_DASH:	// 歩行モーション
//
//		if (GetMotionPose() % 4 == 0 && GetMotionCounter() == 0)
//		{ // 足がついたタイミングの場合
//
//			switch (m_land)
//			{ // 着地物ごとの処理
//			case LAND_OBSTACLE:
//
//				// サウンドの再生
//				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_WALK_OBS);	// 歩行音（障害物）
//
//				break;
//
//			default:
//
//				// サウンドの再生
//				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_WALK_BUILD);	// 歩行音（ビル）
//
//				break;
//			}
//		}
//
//		break;
//
//	case MOTION_STEALTHWALK:	// 忍び足モーション
//
//		break;
//#endif
//
//	case MOTION_JUMP_MINI:	// 小ジャンプモーション
//
//		if (!m_bJump)
//		{ // ジャンプ中ではない場合
//
//			// 現在のモーションの設定
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case MOTION_JUMP_HIGH:	// 大ジャンプモーション
//
//		if (!m_bJump)
//		{ // ジャンプ中ではない場合
//
//			// 現在のモーションの設定
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case MOTION_FALL:	// 落下モーション
//
//		if (!m_bJump)
//		{ // ジャンプ中ではない場合
//
//			// 現在のモーションの設定
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case MOTION_LANDING:	// 着地モーション
//
//		if (IsMotionFinish())
//		{ // モーションが再生終了した場合
//
//			// 現在のモーションの設定
//			SetMotion(nMotion, BLEND_FRAME_LAND);
//		}
//
//		break;
//	}
}