//============================================================
//
//	プレイヤー処理 [player.cpp]
//	Author：藤田勇一
//  Adder : 金崎朋弥
//  Adder : 小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "player.h"
#include "playerTitle.h"
#include "playerSelect.h"
#include "manager.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "collision.h"
#include "fade.h"
#include "multiModel.h"
#include "orbit.h"
#include "shadow.h"
#include "orbit.h"
#include "object2D.h"
#include "stage.h"
#include "field.h"
#include "cloneAngleUI.h"
#include "input.h"
#include "player_clone.h"
#include "checkpoint.h"
#include "effect3D.h"
#include "collisionCube.h"
#include "collisionCylinder.h"
#include "collisionPolygon.h"
#include "collisionSphere.h"
#include "coin.h"
#include "godItem.h"
#include "touchActor.h"
#include "mash.h"
#include "spin_wall.h"
#include "effekseerControl.h"
#include "effekseerManager.h"
#include "gimmick_action.h"
#include "enemyAttack.h"
#include "tension.h"
#include "retentionManager.h"
#include "goditemUI.h"
#include "playerbackUI.h"
#include "player_dodge.h"
#include "hitstop.h"
#include "tutorial.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\CHARACTER\\player.txt";	// セットアップテキスト相対パス

	const int	PRIORITY	= 3;				// プレイヤーの優先順位
	const float	JUMP_MOVE	= 615.0f;			// 大ジャンプ上昇量
	const float	STEP_MOVE	= JUMP_MOVE * 2.0f;	// 大ジャンプ上昇量
	const float	CANON_GRAVITY= 500.0f;			// 重力
	const float CANON_MOVE	= CANON_GRAVITY * 30.0f;	// 吹っ飛ばし上昇量
	const float REBOUND		= 500.0f;			// ジャンプの跳ね返り
	const float	GRAVITY		= 3600.0f;			// 重力
	const float	RADIUS		= 20.0f;			// 半径
	const float HEIGHT		= 80.0f;			// 身長
	const float	REV_ROTA	= 9.0f;				// 向き変更の補正係数
	const float LANDING_SPEED_M = -20.0f;		// 着地エフェクトMの切り替えライン
	const float LANDING_SPEED_L = -35.0f;		// 着地エフェクトLの切り替えライン
	const float	ADD_MOVE	= 0.08f;			// 非アクション時の速度加算量
	const float	JUMP_REV	= 0.16f;			// 通常状態時の空中の移動量の減衰係数
	const float	LAND_REV	= 0.16f;			// 通常状態時の地上の移動量の減衰係数
	const float	SPAWN_ADD_ALPHA	= 0.03f;		// スポーン状態時の透明度の加算量
	const int	BLEND_FRAME_OTHER	= 5;		// モーションの基本的なブレンドフレーム
	const int	BLEND_FRAME_LAND	= 15;		// モーション着地のブレンドフレーム
	const int	CAUTIOUS_TRANS_LOOP	= 7;		// 警戒モーションに遷移する待機ループ数
	const D3DXVECTOR3 DMG_ADDROT	= D3DXVECTOR3(0.04f, 0.0f, -0.02f);	// ダメージ状態時のプレイヤー回転量
	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// 影の大きさ
	const D3DXVECTOR3 OFFSET_JUMP	= D3DXVECTOR3(0.0f, 80.0f, 0.0f);	// 大ジャンプエフェクトの発生位置オフセット
	const float SPAWN_ADD_HEIGHT = 5000.0f;		// スポーン状態で上げる高さ

	const char* BODY_ORBIT_TEXTURE = "data\\TEXTURE\\orbit001.png";		// 体の軌跡のテクスチャパス
	const COrbit::SOffset BODYORBIT_OFFSET[CPlayer::MAX_JUMPORBIT] =	// 体の軌跡のオフセット情報
	{
		COrbit::SOffset(D3DXVECTOR3(0.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), XCOL_WHITE),
		COrbit::SOffset(D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXVECTOR3(-20.0f, 20.0f, 0.0f), XCOL_WHITE),
	};
	const COrbit::SOffset JUMPORBIT_OFFSET[CPlayer::MAX_JUMPORBIT] =	// ジャンプ軌跡のオフセット情報
	{
		COrbit::SOffset(D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, -5.0f, 0.0f), XCOL_CYAN),
		COrbit::SOffset(D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, -5.0f, 0.0f), XCOL_CYAN),
		COrbit::SOffset(D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, -5.0f, 0.0f), XCOL_CYAN),
		COrbit::SOffset(D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, -5.0f, 0.0f), XCOL_CYAN),
	};
	const int BODYORBIT_PART_NUMBER = CPlayer::MODEL_BODY;			// 体の軌跡のパーツの番号
	const int JUMPORBIT_PART_NUMBER[CPlayer::MAX_JUMPORBIT] =	// ジャンプ軌跡のパーツの番号
	{
		CPlayer::MODEL_HANDL,
		CPlayer::MODEL_HANDR,
		CPlayer::MODEL_FOOTL,
		CPlayer::MODEL_FOOTR
	};
	const int BODYORBIT_PART = 20;	// 分割数
	const int JUMPORBIT_PART = 15;	// 分割数

	const float	NORMAL_MOVE = 480.0f;	// 通常の移動量
	const float	STEALTH_MOVE = 0.3f;	// 忍び足の移動量
	const float	DODGE_MOVE = 800.0f;	// 回避の移動量
	const float	DAMAGE_MOVE = 400.0f;	// ノックバックの移動量
	const float CLONE_MOVE = NORMAL_MOVE * 1.1f; // 分身の移動量

	const int INIT_CLONE = 5; // 最初に使える分身の数
	const int TELEPORT_CLONE = 5; // 回帰したときの最低保障分身数
	const int HEAL_CHECKPOINT = 3; // チェックポイントの回復量
	const int HEAL_ITEM = 3; // アイテムの回復量
	const int IRIS_COUNT = 20; // 溺死状態でアイリスアウトするカウント数
	const int DROWN_RIPPLE_COUNT = 18; // 溺死状態で波紋が出る間隔
	const float SINK_SPEED = 2.5f; // 沈めるまでのカウント数
	const float SINK_BRANCH = 120.0f; // 沈む深さ
	const int TELEPORT_POS_COUNT = 5; // 回帰位置を設定するカウント数
	const float DISTANCE_CLONE = 50.0f; // 分身の出現位置との距離
	const float GIMMICK_TIMER = 0.5f; // 直接ギミックを生成できる時間
	const float STICK_ERROR = D3DX_PI * 0.875f; // スティックの入力誤差許容範囲

	// ブラーの情報
	namespace blurInfo
	{
		const float	START_ALPHA = 0.4f;	// ブラー開始透明度
		const int	MAX_LENGTH = 15;	// 保持オブジェクト最大数
	}

	namespace dodge
	{
		const CCamera::SSwing SWING = CCamera::SSwing(10.0f, 2.0f, 0.6f);	// カメラ揺れの値
		const float STOP_TIME = 0.2f;	// ヒットストップ時間
	}

	namespace hit
	{
		const CCamera::SSwing SWING = CCamera::SSwing(14.0f, 2.0f, 1.0f);	// カメラ揺れの値
		const float STOP_TIME = 0.3f;	// ヒットストップ時間
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CPlayer> *CPlayer::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CPlayer] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
	m_pBackUI			(nullptr),		// 回帰UIの情報
	m_pDodge			(nullptr),		// 回避エフェクトの情報
	m_oldPos			(VEC3_ZERO),	// 過去位置
	m_move				(VEC3_ZERO),	// 移動量
	m_destRot			(VEC3_ZERO),	// 目標向き
	m_posInit			(VEC3_ZERO),	// 初期位置
	m_state				(STATE_NONE),	// 状態
	m_bJump				(false),		// ジャンプ状況
	m_nCounterState		(0),			// 状態管理カウンター
	m_nWalkCount		(0),			// 歩行音カウント
	m_fScalar			(0.0f),			// 移動量
	m_bClone			(true),			// 分身操作可能フラグ
	m_bGimmickClone		(false),		// ギミッククローンの生成フラグ
	m_fGimmickTimer		(0.0f),			// ギミッククローンの生成タイマー
	m_fTempStick		(0.0f),			// スティックの入力角を保存する変数
	m_bGetCamera		(false),		// カメラ取得フラグ
	m_fCameraRot		(0.0f),			// カメラの角度
	m_fStickRot			(0.0f),			// スティックの角度
	m_sFrags			({}),			// フィールドフラグ
	m_pCurField			(nullptr),		// 現在乗ってる地面
	m_pOldField			(nullptr),		// 前回乗ってた地面
	m_posTeleport		(VEC3_ZERO),	// テレポート位置
	m_nCounterTeleport	(0)				// テレポートカウント
{
	// 軌跡のメモリセット
	memset(&m_apBodyOrbit[0], 0, sizeof(m_apBodyOrbit));
	memset(&m_apJumpOrbit[0], 0, sizeof(m_apJumpOrbit));
}

//============================================================
//	デストラクタ
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayer::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apBodyOrbit[0], 0, sizeof(m_apBodyOrbit));		// 体の軌跡の情報
	memset(&m_apJumpOrbit[0], 0, sizeof(m_apJumpOrbit));		// ジャンプ軌跡の情報
	m_oldPos		= VEC3_ZERO;	// 過去位置
	m_move			= VEC3_ZERO;	// 移動量
	m_destRot		= VEC3_ZERO;	// 目標向き
	m_state			= STATE_NONE;	// 状態
	m_bJump			= true;			// ジャンプ状況
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nWalkCount	= 0;			// 歩行音カウント
	m_fScalar		= 0.0f;			// 移動量
	m_bClone		= true;			// 分身操作可能フラグ
	m_bGimmickClone	= false;		// ギミッククローンの生成フラグ
	m_fGimmickTimer	= 0.0f;			// ギミッククローンの生成タイマー
	m_fTempStick	= 0.0f;			// スティックの入力角を保存する変数

	// オブジェクトキャラクターの初期化
	if (FAILED(CObjectChara::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キャラクター情報の割当
	BindCharaData(SETUP_TXT);

#if 0

	for (int nCntBody = 0; nCntBody < MAX_BODYORBIT; nCntBody++)
	{
		// 軌跡の生成
		m_apBodyOrbit[nCntBody] = COrbit::Create
		( // 引数
			GetParts(BODYORBIT_PART_NUMBER)->GetPtrMtxWorld(),	// 親マトリックス
			BODYORBIT_OFFSET[nCntBody],	// オフセット情報
			BODYORBIT_PART,				// 分割数
			10,							// テクスチャの分割数
			false						// 加算合成状況
		);
		if (m_apBodyOrbit[nCntBody] == nullptr)
		{ // 非使用中の場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを割り当て
		m_apBodyOrbit[nCntBody]->BindTexture(BODY_ORBIT_TEXTURE);

		// 表示する
		m_apBodyOrbit[nCntBody]->SetState(COrbit::STATE_NORMAL);
	}

#endif // 0

	for (int nCntJump = 0; nCntJump < MAX_JUMPORBIT; nCntJump++)
	{
		// 軌跡の生成
		m_apJumpOrbit[nCntJump] = COrbit::Create
		( // 引数
			GetParts(JUMPORBIT_PART_NUMBER[nCntJump])->GetPtrMtxWorld(),	// 親マトリックス
			JUMPORBIT_OFFSET[nCntJump],	// オフセット情報
			JUMPORBIT_PART			// 分割数
		);
		if (m_apJumpOrbit[nCntJump] == nullptr)
		{ // 非使用中の場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 表示しない
		m_apJumpOrbit[nCntJump]->SetState(COrbit::STATE_VANISH);
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CPlayer>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	if (m_pDodge == nullptr)
	{ // 回避エフェクト情報が NULL の場合

		// 回避エフェクトの生成処理
		m_pDodge = CPlayerDodge::Create();
		if (m_pDodge == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// 士気力ゲージの生成
	if (GET_MANAGER->GetMode() == CScene::MODE_GAME) // ゲーム画面でのみ生成する
	{
		for (int i = 0; i < INIT_CLONE; ++i)
		{
			CTension::Create();
		}

		// 神器UIの生成
		CGodItemUI::Create();

		// 回帰UIの生成処理
		m_pBackUI = CPlayerBackUI::Create();
	}

#ifndef PHOTO
	m_pEffectFirefly = GET_EFFECT->Create("data\\EFFEKSEER\\firefly.efkefc", GetCenterPos(), VEC3_ZERO, VEC3_ZERO, 50.0f, false, false);
#endif

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayer::Uninit(void)
{
	for (int nCntBody = 0; nCntBody < MAX_BODYORBIT; nCntBody++)
	{
		// 軌跡の終了
		SAFE_UNINIT(m_apBodyOrbit[nCntBody]);
	}

	for (int nCntJump = 0; nCntJump < MAX_JUMPORBIT; nCntJump++)
	{
		// 軌跡の終了
		SAFE_UNINIT(m_apJumpOrbit[nCntJump]);
	}

	// 回帰UIの削除
	if (m_pBackUI != nullptr)
	{
		SAFE_UNINIT(m_pBackUI);
		m_pBackUI = nullptr;
	}

	// 回避エフェクトの削除
	if (m_pDodge != nullptr)
	{
		SAFE_UNINIT(m_pDodge);
		m_pDodge = nullptr;
	}

#ifndef PHOTO
	// エフェクトの削除
	if (m_pEffectFirefly != nullptr)
	{
		SAFE_DELETE(m_pEffectFirefly);
		m_pEffectFirefly = nullptr;
	}
#endif

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトキャラクターの終了
	CObjectChara::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayer::Update(const float fDeltaTime)
{
	DebugProc::Print(DebugProc::POINT_CENTER, "pos : (%f, %f, %f)\n", m_posCenter.x, m_posCenter.y, m_posCenter.z);
	DebugProc::Print(DebugProc::POINT_CENTER, "move : (%f, %f, %f)\n", m_move.x * fDeltaTime, m_move.y * fDeltaTime, m_move.z * fDeltaTime);

	// 過去位置の更新
	UpdateOldPosition();

	// 状態の更新
	EMotion currentMotion = UpdateState(fDeltaTime);

	// 回避エフェクトの更新
	if (m_pDodge != nullptr)
	{
		m_pDodge->Update(GetVec3Position());
	}

	for (int nCntBody = 0; nCntBody < MAX_BODYORBIT; nCntBody++)
	{
		// 軌跡の更新
		if (m_apBodyOrbit[nCntBody] == nullptr) { continue; }

		// 更新処理
		m_apBodyOrbit[nCntBody]->Update(fDeltaTime);
	}

	{ // 軌跡の更新

		int nMotion = GetMotionType();

		for (int nCntJump = 0; nCntJump < MAX_JUMPORBIT; nCntJump++)
		{
			// 軌跡の更新
			if (m_apJumpOrbit[nCntJump] == nullptr) { continue; }

			// ハイジャンプ中の場合、表示する
			if (nMotion != MOTION_JUMP_HIGH) { m_apJumpOrbit[nCntJump]->SetState(COrbit::STATE_VANISH); }

			// 更新処理
			m_apJumpOrbit[nCntJump]->Update(fDeltaTime);
		}
	}

	// モーション・オブジェクトキャラクターの更新
	UpdateMotion(currentMotion, fDeltaTime);

	if (m_pBackUI != nullptr)
	{ // 回帰UIが NULL じゃない場合

		if (m_state == STATE_NORMAL)
		{ // 一定状態以外の場合

			// チェックポイント回帰処理
			CheckPointBack(fDeltaTime);
		}
		else
		{ // 上記以外

			// 減衰状態にする
			m_pBackUI->SetState(CPlayerBackUI::STATE_SUB);
		}

		// 更新処理
		m_pBackUI->Update(fDeltaTime);
	}

#ifdef _DEBUG

	// 入力情報を受け取るポインタ
	CInputKeyboard* pKeyboard = GET_INPUTKEY;

	if (pKeyboard->IsTrigger(DIK_RIGHT))
	{
		RecoverCheckPoint();
	}

#endif
}

//============================================================
//	描画処理
//============================================================
void CPlayer::Draw(CShader *pShader)
{
	// オブジェクトキャラクターの描画
	CObjectChara::Draw(pShader);
}

//============================================================
//	更新状況の設定処理
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);	// 自身
}

//============================================================
//	描画状況の設定処理
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);	// 自身
}

//============================================================
//	状態の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateState(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:

		// 何もしない状態時の更新
		currentMotion = UpdateNone(fDeltaTime);
		break;

	case STATE_SPAWN:

		// スポーン状態時の更新
		currentMotion = UpdateSpawn(fDeltaTime);
		break;

	case STATE_START:

		// スタート状態時の更新
		currentMotion = UpdateStart(fDeltaTime);
		break;

	case STATE_NORMAL:

		// 通常状態の更新
		currentMotion = UpdateNormal(fDeltaTime);
		break;

	case STATE_GODITEM:

		// 神器獲得状態の更新
		currentMotion = UpdateGodItem(fDeltaTime);
		break;

	case STATE_GOAL:

		// ゴール状態の更新
		currentMotion = UpdateGoal(fDeltaTime);
		break;

	case STATE_DODGE:

		// 回避状態の更新
		currentMotion = UpdateDodge(fDeltaTime);
		break;

	case STATE_DEATH:

		// 死亡状態の更新
		currentMotion = UpdateDeath(fDeltaTime);
		break;

	case STATE_DAMAGE:

		// ダメージ状態の更新
		currentMotion = UpdateDamage(fDeltaTime);
		break;
	
	case STATE_DROWN:

		// 溺死状態の更新
		currentMotion = UpdateDrown(fDeltaTime);
		break;

	case STATE_BACKWAIT:

		// チェックポイントリセット待機の更新
		currentMotion = UpdateBackWait(fDeltaTime);
		break;

	default:
		assert(false);
		break;
	}

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	生成処理
//============================================================
CPlayer *CPlayer::Create
(
	const EType type,			// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot		// 向き
)
{
	// プレイヤーの生成
	CPlayer *pPlayer = nullptr;	// プレイヤー情報
	switch (type)
	{ // 種類ごとの処理
	case TYPE_TITLE:
		pPlayer = new CPlayerTitle;
		break;

	case TYPE_SELECT:
		pPlayer = new CPlayerSelect;
		break;

	case TYPE_GAME:
		pPlayer = new CPlayer;
		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (pPlayer == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// プレイヤーの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// プレイヤーの破棄
			SAFE_DELETE(pPlayer);
			return nullptr;
		}

		// セーブ情報を取得
		const int nSave = GET_RETENTION->GetSave();

		// 位置を設定
		if (nSave == -1 || CCheckPoint::GetList() == nullptr)
		{
			// 初期位置を設定する
			pPlayer->m_posInit = rPos;
			pPlayer->SetVec3Position(rPos);

			if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
			{ // ゲームモードの場合

				// 位置を設定する
				D3DXVECTOR3 pos = pPlayer->GetVec3Position();
				pos.y += SPAWN_ADD_HEIGHT;
				pPlayer->SetVec3Position(pos);

			}
		}
		else
		{
			// チェックポイントのリストを取得
			CCheckPoint* point = *CCheckPoint::GetList()->GetIndex(nSave);

			// チェックポイントの座標を設定する
			pPlayer->SetVec3Position(point->GetVec3Position());
		}

		pPlayer->m_oldPos = rPos;	// 過去位置も同一の位置にする

		// 向きを設定
		pPlayer->SetVec3Rotation(rRot);
		pPlayer->m_destRot = rRot;	// 目標向きも同一の向きにする

		// プレイヤーを出現させる
		pPlayer->SetSpawn();

		// 確保したアドレスを返す
		return pPlayer;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CPlayer> *CPlayer::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	ノックバックヒット処理
//============================================================
bool CPlayer::HitKnockBack(const int nDamage, const D3DXVECTOR3& rVecKnock)
{
	if (IsDeath()) { return false; }	// 死亡済み
	if (m_state != STATE_NORMAL) { return false; }	// 通常状態以外

	// ダメージ状態に変更
	m_state = STATE_DAMAGE;

	// コントローラのバイブレーション
	GET_INPUTPAD->SetVibration(CInputPad::TYPE_DAMAGE);

	// ノックバック方向を向く
	D3DXVECTOR3 rot = GetVec3Rotation();
	rot.y = atan2f(rVecKnock.x, rVecKnock.z);
	SetVec3Rotation(rot);

	// 死んじゃうｶﾓ
	if (CTension::GetList() == nullptr || CTension::GetList() == nullptr)
	{
		m_state = STATE_DEATH;

		// コントローラのバイブレーション
		GET_INPUTPAD->SetVibration(CInputPad::TYPE_DEATH);
	}

	// 士気力が減少する
	for (int i = 0; i < nDamage; ++i)
	{
		CTension::Vanish();
	}

	// ヒットストップさせる
	CSceneGame::GetHitStop()->SetStop(hit::STOP_TIME);

	// カメラ揺れを与える
	GET_CAMERA->SetSwing(CCamera::TYPE_MAIN, hit::SWING);

	// ダメージモーションにする
	SetMotion(MOTION_DAMAGE);

	return true;
}

//============================================================
//	ヒット処理
//============================================================
bool CPlayer::Hit(const int nDamage)
{
	if (IsDeath())				 { return false; }	// 死亡済み
	if (m_state != STATE_NORMAL) { return false; }	// 通常状態以外

	// ヒットエフェクトを出す
	GET_EFFECT->Create("data\\EFFEKSEER\\hit.efkefc", GetVec3Position() + OFFSET_JUMP, GetVec3Rotation(), VEC3_ZERO, 250.0f);

	// ダメージ状態に変更
	m_state = STATE_DAMAGE;

	// 死んじゃうｶﾓ
	if (CTension::GetList() == nullptr || CTension::GetUseNum() == 0)
	{
		m_state = STATE_DEATH;
	}

	// 士気力が減少する
	CTension::Vanish();

	// ヒットストップさせる
	CSceneGame::GetHitStop()->SetStop(hit::STOP_TIME);

	// カメラ揺れを与える
	GET_CAMERA->SetSwing(CCamera::TYPE_MAIN, hit::SWING);

	return true;
}

//============================================================
//	タイムアップ処理
//============================================================
void CPlayer::TimeUp(void)
{
	// 絶対殺す
	m_state = STATE_DEATH;
}

//============================================================
//	出現の設定処理
//============================================================
void CPlayer::SetSpawn(void)
{
	// 情報を初期化
	SetState(STATE_SPAWN);	// スポーン状態の設定
	SetMotion(MOTION_IDOL);	// 待機モーションを設定

	// カウンターを初期化
	m_nCounterState = 0;	// 状態管理カウンター

	// 移動量を初期化
	m_move = VEC3_ZERO;

	// マテリアルを再設定
	ResetMaterial();

	// 透明度を透明に再設定
	SetAlpha(0.0f);

	// 描画を再開
	SetEnableDraw(true);
}

//============================================================
//	神器獲得の設定処理
//============================================================
void CPlayer::SetEnableGodItem(const bool bGet)
{
	if (bGet)
	{ // 取得状態の設定

		// プレイヤー向きを設定
		D3DXVECTOR3 rotDest = VEC3_ZERO;	// 目標向き
		rotDest.y = GET_MANAGER->GetCamera()->GetDestRotation().y;	// ヨーはカメラ向きに

		// 向きをカメラ目線に
		SetDestRotation(rotDest);

		// 移動量を初期化
		SetMove(VEC3_ZERO);

		// 神器獲得モーションにする
		SetMotion(MOTION_GET);

		// 神器獲得状態にする
		m_state = STATE_GODITEM;
	}
	else
	{ // 取得状態の解除

		// 待機モーションにする
		SetMotion(MOTION_IDOL);

		// 通常状態にする
		m_state = STATE_NORMAL;	// TODO：ここわんちゃん過去状態の方がいいか
	}
}

//============================================================
//	リザルトの設定処理
//============================================================
void CPlayer::SetResult()
{
	// プレイヤー向きを設定
	D3DXVECTOR3 rotDest = VEC3_ZERO;	// 目標向き
	rotDest.y = GET_MANAGER->GetCamera()->GetDestRotation().y;	// ヨーはカメラ向きに

	// 操作を停止させる
	SetState(CPlayer::STATE_GOAL);

	// 向きをカメラ目線に
	SetDestRotation(rotDest);

	// 移動量を初期化
	SetMove(VEC3_ZERO);

	// 待機モーションを設定
	switch (GET_RETENTION->GetWin())
	{
	case CRetentionManager::WIN_FAIL:
		SetMotion(MOTION_DEATH);
		break;
	case CRetentionManager::WIN_SUCCESS:
		SetMotion(MOTION_GOAL);
		break;
	}
}

//============================================================
//	状態の設定処理
//============================================================
void CPlayer::SetState(const EState state)
{
	if (state > NONE_IDX && state < STATE_MAX)
	{ // 範囲内の場合

		// 引数の状態を設定
		m_state = state;
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	状態取得処理
//============================================================
CPlayer::EState CPlayer::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	半径取得処理
//============================================================
float CPlayer::GetRadius(void) const
{
	// 半径を返す
	return RADIUS;
}

//============================================================
//	縦幅取得処理
//============================================================
float CPlayer::GetHeight(void) const
{
	// 縦幅を返す
	return HEIGHT;
}

//============================================================
//	ギミックのハイジャンプ処理
//============================================================
bool CPlayer::GimmickHighJump(const int nNumClone)
{
	// ジャンプ中は飛ばない
	if (m_bJump) { return false; }

	// 上移動量を与える
	m_move.y = JUMP_MOVE * nNumClone;

	// ジャンプ中にする
	m_bJump = true;

	// モーションの設定
	SetMotion(MOTION_JUMP_HIGH, BLEND_FRAME_OTHER);

	for (int nCnt = 0; nCnt < MAX_JUMPORBIT; nCnt++)
	{
		// 表示する
		m_apJumpOrbit[nCnt]->SetState(COrbit::STATE_NORMAL);
	}

	// ジャンプエフェクトを出す
	GET_EFFECT->Create("data\\EFFEKSEER\\Highjump.efkefc", GetVec3Position() + OFFSET_JUMP, GetVec3Rotation(), VEC3_ZERO, 25.0f);

	// コントローラのバイブレーション
	GET_INPUTPAD->SetVibration(CInputPad::TYPE_JUMP);

	// ジャンプ音を鳴らす
	PLAY_SOUND(CSound::LABEL_SE_GIMMICKJUMP);

	return true;
}

//============================================================
//	ギミックのハイジャンプ処理
//============================================================
void CPlayer::GimmickLowJump(void)
{
	// 上移動量を与える
	m_move.y = STEP_MOVE;

	// ジャンプ中にする
	m_bJump = true;

	// モーションの設定
	SetMotion(MOTION_JUMP_HIGH, BLEND_FRAME_OTHER);
}

//============================================================
// ギミックの飛び降り着地
//============================================================
bool CPlayer::GimmickLand(void)
{
	// 上昇量を与える
	m_move.y = REBOUND;

	// ジャンプモーションを設定
	SetMotion(MOTION_JUMP_MINI);

	// 着地音の再生
	PLAY_SOUND(CSound::LABEL_SE_LAND_S);

	return true;
}

//==========================================
//  チェックポイントでの回復処理
//==========================================
void CPlayer::RecoverCheckPoint()
{
	// 士気力を回復する
	for (int i = 0; i < HEAL_CHECKPOINT; ++i)
	{
		CTension::Create();
	}
}

//==========================================
//  アイテムでの回復
//==========================================
void CPlayer::RecoverItem()
{
	// 士気力を回復する
	for (int i = 0; i < HEAL_ITEM; ++i)
	{
		CTension::Create();
	}
}

//===========================================
//  文字列(フラグ)の追加
//===========================================
void CPlayer::AddFrags(const char cFrag)
{
	// 文字列内を検索に同じ文字が存在したら関数を抜ける
	if (m_sFrags.find(cFrag) != std::string::npos) { return; }

	// 文字列に受け取ったフラグを追加する
	m_sFrags += cFrag;
}

//=========================================
//  文字列(フラグ)の削除
//===========================================
void CPlayer::SabFrags(const char cFrag)
{
	// 文字列内を検索し番号を取得する
	size_t nIdx = m_sFrags.find(cFrag);

	// 文字列内にフラグが存在しなかった場合関数を抜ける
	if (nIdx == std::string::npos) { return; }

	// 文字列からフラグを削除する
	m_sFrags.erase(nIdx);
}

//============================================================
//	何もしない状態時の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateNone(const float fDeltaTime)
{
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 位置更新
	UpdatePosition(posPlayer, fDeltaTime);

	// 敵の当たり判定
	CollisionEnemy(posPlayer);

	// 着地判定
	UpdateLanding(posPlayer, fDeltaTime);

	// 向き更新
	UpdateRotation(rotPlayer, fDeltaTime);

	// 壁の当たり判定
	CollisionWall(posPlayer);

	// 大人の壁の判定
	GET_STAGE->LimitPosition(posPlayer, RADIUS);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// 現在のモーションを返す
	return MOTION_IDOL;
}

//============================================================
//	スポーン状態時の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateSpawn(const float fDeltaTime)
{
	// 変数を宣言
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション

	// フェードアウト状態時の更新
	if (UpdateFadeOut(SPAWN_ADD_ALPHA))
	{ // 不透明になり切った場合

		// 状態を設定
		SetState(STATE_START);
	}

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
// スタート状態時の更新
//============================================================
CPlayer::EMotion CPlayer::UpdateStart(const float fDeltaTime)
{
	// 変数を宣言
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション
	D3DXVECTOR3 pos = GetVec3Position();	// 位置を取得

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 位置更新
	UpdatePosition(pos, fDeltaTime);

	// 敵の当たり判定
	CollisionEnemy(pos);

	// 着地判定
	UpdateLanding(pos, fDeltaTime);

	// 壁の当たり判定
	CollisionWall(pos);

	// 大人の壁の判定
	GET_STAGE->LimitPosition(pos, RADIUS);

	// 位置を反映
	SetVec3Position(pos);

	// 現在のモーションを返す
	return currentMotion;
}

//============================================================
//	通常状態時の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateNormal(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;		// 現在のモーション
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// 移動操作
	currentMotion = UpdateMove();

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 位置更新
	UpdatePosition(posPlayer, fDeltaTime);

	// 敵の当たり判定
	CollisionEnemy(posPlayer);

	// 着地判定
	UpdateLanding(posPlayer, fDeltaTime);

	// 向き更新
	UpdateRotation(rotPlayer, fDeltaTime);

	// 壁の当たり判定
	CollisionWall(posPlayer);

	// コインとの当たり判定処理
	CollisionCoin(posPlayer);

	// 神器との当たり判定処理
	CollisionGodItem(posPlayer);

	// 大人の壁の判定
	GET_STAGE->LimitPosition(posPlayer, RADIUS);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// 分身の処理
	if(ControlClone(posPlayer, rotPlayer, fDeltaTime))
	{ currentMotion = MOTION_DODGE; }

	// 現在のモーションを返す
	return currentMotion;
}

//===========================================
//	神器獲得状態時の更新処理
//===========================================
CPlayer::EMotion CPlayer::UpdateGodItem(const float fDeltaTime)
{
	D3DXVECTOR3 pos = GetVec3Position();	// 位置を取得

	// 向き反映
	SetVec3Rotation(m_destRot);

	// 壁の当たり判定
	CollisionWall(pos);

	// 大人の壁の判定
	GET_STAGE->LimitPosition(pos, RADIUS);

	// 位置を反映
	SetVec3Position(pos);

	// 神器獲得モーションを返す
	return MOTION_GET;
}

//===========================================
//	ゴール状態時の更新処理
//===========================================
CPlayer::EMotion CPlayer::UpdateGoal(const float fDeltaTime)
{
	D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 位置更新
	UpdatePosition(posPlayer, fDeltaTime);

	// 敵の当たり判定
	CollisionEnemy(posPlayer);

	// 着地判定
	UpdateLanding(posPlayer, fDeltaTime);

	// 向き更新
	UpdateRotation(rotPlayer, fDeltaTime);

	// 壁の当たり判定
	CollisionWall(posPlayer);

	// 大人の壁の判定
	GET_STAGE->LimitPosition(posPlayer, RADIUS);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	if (m_bJump)
	{ // 空中にいる場合

		// 待機モーションを返す
		return MOTION_IDOL;
	}

	switch (GET_RETENTION->GetWin())
	{
	case CRetentionManager::WIN_FAIL:

		// 死亡モーションを返す
		return MOTION_DEATH;

	case CRetentionManager::WIN_SUCCESS:

		// ゴールモーションを返す
		return MOTION_GOAL;

	default:
		assert(false);
		return MOTION_IDOL;
	}
}

//===========================================
//  回避状態時の更新処理
//===========================================
CPlayer::EMotion CPlayer::UpdateDodge(const float fDeltaTime)
{
	// 向きの取得
	float rot = GetVec3Rotation().y;

	// 移動方向の算出
	m_move.x = sinf(rot) * DODGE_MOVE;
	m_move.z = cosf(rot) * DODGE_MOVE;

	// 位置の取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 位置更新
	UpdatePosition(pos, fDeltaTime);

	// 敵の当たり判定
	CollisionEnemy(pos);

	// 着地判定
	UpdateLanding(pos, fDeltaTime);

	// 壁の当たり判定
	CollisionWall(pos);

	// 大人の壁の判定
	GET_STAGE->LimitPosition(pos, RADIUS);

	// 位置を反映
	SetVec3Position(pos);

	return MOTION_DODGE;
}

//===========================================
//  死亡状態の更新処理
//===========================================
CPlayer::EMotion CPlayer::UpdateDeath(const float fDeltaTime)
{
	// リザルトを呼び出す
	GET_GAMEMANAGER->TransitionResult(CRetentionManager::WIN_FAIL);

	// 位置の取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 位置更新
	UpdatePosition(pos, fDeltaTime);

	// 敵の当たり判定
	CollisionEnemy(pos);

	// 着地判定
	UpdateLanding(pos, fDeltaTime);

	// 壁の当たり判定
	CollisionWall(pos);

	// 大人の壁の判定
	GET_STAGE->LimitPosition(pos, RADIUS);

	// 位置を反映
	SetVec3Position(pos);

	// 死亡モーション
	return MOTION_DEATH;
}

//===========================================
//  ダメージ状態の更新処理
//===========================================
CPlayer::EMotion CPlayer::UpdateDamage(const float fDeltaTime)
{
	// 向きの取得
	float rot = GetVec3Rotation().y;

	// 移動方向の算出
	m_move.x = sinf(rot) * DAMAGE_MOVE;
	m_move.z = cosf(rot) * DAMAGE_MOVE;

	// 位置の取得
	D3DXVECTOR3 pos = GetVec3Position();

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 位置更新
	UpdatePosition(pos, fDeltaTime);

	// 敵の当たり判定
	CollisionEnemy(pos);

	// 着地判定
	UpdateLanding(pos, fDeltaTime);

	// 壁の当たり判定
	CollisionWall(pos);

	// 大人の壁の判定
	GET_STAGE->LimitPosition(pos, RADIUS);

	// 位置を反映
	SetVec3Position(pos);

	// ダメージモーション
	return MOTION_DAMAGE;
}

//===========================================
//  溺死状態の更新処理
//===========================================
CPlayer::EMotion CPlayer::UpdateDrown(const float fDeltaTime)
{
	D3DXVECTOR3 posField = m_pCurField->GetVec3Position();	// フィールドの位置
	m_move = VEC3_ZERO;		// 移動量

	// 状態カウントを加算する
	m_nCounterState++;

	if (m_nCounterState % DROWN_RIPPLE_COUNT == 0 &&
		m_pCurField != nullptr)
	{ // 一定カウントごとに

		// 位置を設定する
		D3DXVECTOR3 pos = GetVec3Position();
		pos.y = posField.y;

		// 着地(小)エフェクトを出す
		GET_EFFECT->Create("data\\EFFEKSEER\\landing_small.efkefc", pos, GetVec3Rotation(), VEC3_ZERO, 20.0f);
	}

	{ // 沈む処理

		D3DXVECTOR3 pos = GetVec3Position();
		pos.y -= SINK_SPEED;
		useful::LimitMinNum(pos.y, posField.y - SINK_BRANCH);
		SetVec3Position(pos);
	}

	// フェードが終わった時、ゲームを再開する
	if (m_nCounterState > IRIS_COUNT && 
		CManager::GetInstance()->GetFade()->IsFadeIn()) 
	{
		// スタックのリセット
		ResetStack();

		// 待機モーション
		return MOTION_IDOL;
	}

	if (m_nCounterState != IRIS_COUNT)
	{ // 状態カウントが一定値になった場合

		// この関数を抜ける
		return MOTION_DROWNING;
	}

	// アイリスアウトでフェードする
	CManager::GetInstance()->GetFade()->SetIrisFade(nullptr, 0.4f, 1.2f);

	// 溺死モーション
	return MOTION_DROWNING;
}

//============================================================
// チェックポイントリセット待機時の更新
//============================================================
CPlayer::EMotion CPlayer::UpdateBackWait(const float fDeltaTime)
{
	// 変数を宣言
	D3DXVECTOR3 pos = GetVec3Position();	// 位置を取得

	// 状態カウントを加算する
	m_nCounterState++;

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 位置更新
	UpdatePosition(pos, fDeltaTime);

	// 敵の当たり判定
	CollisionEnemy(pos);

	// 着地判定
	UpdateLanding(pos, fDeltaTime);

	// 壁の当たり判定
	CollisionWall(pos);

	// 大人の壁の判定
	GET_STAGE->LimitPosition(pos, RADIUS);

	if (m_pBackUI != nullptr)
	{ // 回帰UIが NULL じゃない場合

		// 通常状態にする
		m_pBackUI->SetState(CPlayerBackUI::STATE_NONE);
	}

	// 位置を反映
	SetVec3Position(pos);

	if (CManager::GetInstance()->GetFade()->IsFadeIn())
	{ // カウントが一定数に達した場合

		// 待機状態に戻す
		m_state = STATE_NORMAL;

		for (int nCnt = 0; nCnt < MAX_JUMPORBIT; nCnt++)
		{
			// 軌跡の更新
			if (m_apJumpOrbit[nCnt] == nullptr) { continue; }

			// 軌跡を消す
			m_apJumpOrbit[nCnt]->SetState(COrbit::STATE_NONE);
		}

		while (CTension::GetUseNum() < TELEPORT_CLONE)
		{ // 最低保証以下の場合

			// 分身数を上げる
			CTension::Create();
		}

		// セーブ情報を取得
		const int nSave = GET_GAMEMANAGER->GetSave();

		// 位置を設定
		if (nSave != -1 && CCheckPoint::GetList() != nullptr)
		{
			// チェックポイントのリストを取得
			CCheckPoint* point = *CCheckPoint::GetList()->GetIndex(nSave);

			// チェックポイントの座標を設定する
			SetVec3Position(point->GetVec3Position());
		}
		else
		{ // 上記以外

			// 初期位置に座標を移す
			SetVec3Position(m_posInit);
		}

		// カメラ目標位置設定
		CManager::GetInstance()->GetCamera()->SetDestAround();

		// モーションの設定処理
		SetMotion(MOTION_IDOL);
	}

	// 待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
//	過去位置の更新処理
//============================================================
void CPlayer::UpdateOldPosition(void)
{
	// 過去位置を更新
	m_oldPos = GetVec3Position();
}

//==========================================
//  スタック状態のリセット
//==========================================
void CPlayer::ResetStack()
{
	while (CTension::GetUseNum() < TELEPORT_CLONE)
	{ // 最低保証以下の場合

		// 分身数を上げる
		CTension::Create();
	}

	// 前の地面に座標を移す
	SetVec3Position(m_posTeleport);

	// モーションの設定処理
	SetMotion(MOTION_IDOL);

	// カメラ目標位置設定
	CManager::GetInstance()->GetCamera()->SetDestAround();

	// 待機状態に戻す
	m_state = STATE_NORMAL;

	// 状態カウントを0にする
	m_nCounterState = 0;
}

//============================================================
//	移動量・目標向きの更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateMove(void)
{
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション

	// 入力情報の取得
	CInputPad* pPad = GET_INPUTPAD;
	D3DXVECTOR3 CameraRot = GET_MANAGER->GetCamera()->GetRotation();

	// スティックの傾きから移動量を設定
	float fSpeed = pPad->GetPressLStickTilt();	// スティックの傾き量
	if (pad::DEAD_ZONE < fSpeed)
	{
		// スティック向きを取得
		float fStickRot = pPad->GetPressLStickRot() - (D3DX_PI * 0.5f);

		// スティックの角度を比較
		if(fabsf(m_fStickRot - fStickRot) > 0.1f)
		{
			m_bGetCamera = false;
		}

		// スティックの角度を保存
		m_fStickRot = fStickRot;

		// カメラの向きを取得
		if (!m_bGetCamera)
		{
			m_fCameraRot = GET_MANAGER->GetCamera()->GetRotation().y;
			m_bGetCamera = true;
		}

		// 移動の向きを算出
		float fMoveRot = fStickRot + m_fCameraRot;

		// 目標向きを設定
		m_destRot.y = fMoveRot;

		// 移動量を設定する
		m_move.x = -sinf(fMoveRot) * NORMAL_MOVE * (fSpeed / SHRT_MAX);
		m_move.z = -cosf(fMoveRot) * NORMAL_MOVE * (fSpeed / SHRT_MAX);
		D3DXVECTOR3 move = m_move;
		move.y = 0.0f;
		D3DXVec3Normalize(&move, &move);
		m_move.x = move.x * NORMAL_MOVE;
		m_move.z = move.z * NORMAL_MOVE;


		// 橋に乗っている場合移動量を消す
		if (m_sFrags.find(CField::GetFlag(CField::TYPE_XBRIDGE)) != std::string::npos)
		{
			m_move.z = 0.0f;
		}
		if (m_sFrags.find(CField::GetFlag(CField::TYPE_ZBRIDGE)) != std::string::npos)
		{
			m_move.x = 0.0f;
		}

#ifdef _DEBUG
		if (pPad->IsPress(CInputPad::KEY_Y))
		{
			m_move.x *= 5.0f;
			m_move.z *= 5.0f;
		}
#endif
		// 歩行モーションにする
		currentMotion = MOTION_DASH;

		// ガード中であれば移動を遅くする
		if (pPad->IsPress(CInputPad::KEY_RB))
		{
			// 移動量を小さくする
			m_move.x *= STEALTH_MOVE;
			m_move.z *= STEALTH_MOVE;

			// 忍び足モーションにする
			currentMotion = MOTION_STEALTHWALK;
		}

		// 移動量をスカラー値に変換する
		m_fScalar = sqrtf(m_move.x * m_move.x + m_move.z * m_move.z);
	}
	else
	{
		// 回避待機
		if (GET_INPUTPAD->IsPress(CInputPad::KEY_RB)){currentMotion = MOTION_SNEAK;}

		// フラグを折る
		m_bGetCamera = false;
	}

#ifdef _DEBUG

	// キーボードの移動操作
	DebugMoveControl();

	// ジャンプ操作
	DebugJumpControl();

#endif

#ifndef PHOTO
	if (m_pEffectFirefly != nullptr)
	{
		m_pEffectFirefly->m_pos = GetVec3Position();
	}
#endif

	// モーションを返す
	return currentMotion;
}

//============================================================
//	重力の更新処理
//============================================================
void CPlayer::UpdateGravity(const float fDeltaTime)
{
	// 重力を加算
	m_move.y -= GRAVITY * fDeltaTime;
}

//============================================================
//	着地状況の更新処理
//============================================================
bool CPlayer::UpdateLanding(D3DXVECTOR3& rPos, const float fDeltaTime)
{
	bool bLand = false;	// 着地フラグ
	CStage *pStage = GET_STAGE;	// ステージ情報
	D3DXVECTOR3 move = m_move * fDeltaTime;		//現在の移動速度を一時保存

	// ジャンプ状態を保存
	bool bJumpTemp = m_bJump;

	// アクターとの当たり判定
	CollisionActor(rPos, bLand);

	// タッチアクターとの当たり判定
	CollisionTouchActor(rPos);

	// 前回の着地地面を保存
	m_pOldField = m_pCurField;

	// 地面・制限位置・アクターの着地判定
	if (pStage->LandFieldPosition(rPos, m_oldPos, m_move, &m_pCurField)
	||  pStage->LandLimitPosition(rPos, m_move, 0.0f))
	{ // プレイヤーが着地していた場合

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		m_bJump = false;

		// テレポートカウントを加算
		m_nCounterTeleport++;

		// 水でなければ回帰する位置として設定する
		if (m_nCounterTeleport % TELEPORT_POS_COUNT == 0 &&
			m_pCurField != nullptr &&
			m_pCurField->GetFlag() != m_pCurField->GetFlag(CField::TYPE_WATER) &&
			m_pCurField->GetFlag() != m_pCurField->GetFlag(CField::TYPE_XBRIDGE) &&
			m_pCurField->GetFlag() != m_pCurField->GetFlag(CField::TYPE_ZBRIDGE))
		{
			m_posTeleport = rPos;
			m_nCounterTeleport = 0;
		}
	}

	if (m_pCurField != nullptr)
	{ // 現在地面に着地している場合

		// 当たっている状態にする
		m_pCurField->Hit(this);
	}

	if (m_pCurField != m_pOldField)
	{ // 前回と違う地面の場合

		if (m_pOldField != nullptr)
		{ // 前回地面に着地している場合

			// 当たっていない状態にする
			m_pOldField->Miss(this);
		}

		// 床が水の場合殺す
		if (m_pCurField != nullptr && m_pCurField->GetFlag() == m_pCurField->GetFlag(CField::TYPE_WATER))
		{
			// 溺死状態にする
			m_state = STATE_DROWN;

			// 状態カウントを0にする
			m_nCounterState = 0;

			// 落水音の再生
			PLAY_SOUND(CSound::LABEL_SE_WATERDEATH_000);

			// 着水っぽいエフェクトを出す
			GET_EFFECT->Create("data\\EFFEKSEER\\landing_mid.efkefc", rPos, GetVec3Rotation(), VEC3_ZERO, 45.0f);

			// 関数を抜ける
			return bLand;
		}
	}

	// 現在のモーション種類を取得
	int nCurMotion = GetMotionType();

	// ジャンプモーションのフラグを設定
	bool bTypeJump = nCurMotion == MOTION_JUMP_HIGH
				  || nCurMotion == MOTION_JUMP_MINI;

	// 落下モーションのフラグを設定
	bool bTypeFall = nCurMotion == MOTION_FALL;

	if (!m_bJump)
	{ // 空中にいない場合

		if (bTypeJump || bTypeFall)
		{ // モーションがジャンプ中、または落下中の場合

			// 着地モーションを指定
			SetMotion(MOTION_LANDING);

			if (move.y < LANDING_SPEED_L)
			{
				// 着地(大)エフェクトを出す
				GET_EFFECT->Create("data\\EFFEKSEER\\landing_big.efkefc", rPos, GetVec3Rotation(), VEC3_ZERO, 60.0f);
				// 着地音(大)の再生
				PLAY_SOUND(CSound::LABEL_SE_LAND_B);
			}
			else if (move.y < LANDING_SPEED_M)
			{
				// 着地(中)エフェクトを出す
				GET_EFFECT->Create("data\\EFFEKSEER\\landing_mid.efkefc", rPos, GetVec3Rotation(), VEC3_ZERO, 45.0f);
				// 着地音(大)の再生
				PLAY_SOUND(CSound::LABEL_SE_LAND_B);
			}
			else
			{
				// 着地(小)エフェクトを出す
				GET_EFFECT->Create("data\\EFFEKSEER\\landing_small.efkefc", rPos, GetVec3Rotation(), VEC3_ZERO, 30.0f);
				// 着地音(小)の再生
				PLAY_SOUND(CSound::LABEL_SE_LAND_S);
			}

			// 回避状態なら待機状態に変更する
			if (m_state == STATE_DODGE || m_state == STATE_DAMAGE) { m_state = STATE_NONE; }
		}
	}
	else
	{ // 空中にいる場合

		if (!bTypeJump)
		{ // モーションがジャンプ中ではない場合

			// 落下モーションを指定
			SetMotion(MOTION_FALL);
		}
	}

	// 床際のジャンプを呼び出す
	if (!bLand && !bJumpTemp)
	{
		FloorEdgeJump();
	}

	// 着地フラグを返す
	return bLand;
}

//============================================================
//	位置の更新処理
//============================================================
void CPlayer::UpdatePosition(D3DXVECTOR3& rPos, const float fDeltaTime)
{
	// 移動量を加算
	rPos += m_move * fDeltaTime;

	// 移動量を減衰
	if (m_bJump)
	{ // 空中の場合

		m_move.x += (0.0f - m_move.x) * JUMP_REV;
		m_move.z += (0.0f - m_move.z) * JUMP_REV;
	}
	else
	{ // 地上の場合

		m_move.x += (0.0f - m_move.x) * LAND_REV;
		m_move.z += (0.0f - m_move.z) * LAND_REV;
	}

	// 中心座標の更新
	m_posCenter = rPos;
	m_posCenter.y += HEIGHT * 0.5f;
}

//============================================================
//	向きの更新処理
//============================================================
void CPlayer::UpdateRotation(D3DXVECTOR3& rRot, const float fDeltaTime)
{
	// 変数を宣言
	float fDiffRot = 0.0f;	// 差分向き

	// 目標向きの正規化
	useful::NormalizeRot(m_destRot.y);

	// 目標向きまでの差分を計算
	fDiffRot = m_destRot.y - rRot.y;

	// 差分向きの正規化
	useful::NormalizeRot(fDiffRot);

	// 向きの更新
	rRot.y += fDiffRot * fDeltaTime * REV_ROTA;

	// 向きの正規化
	useful::NormalizeRot(rRot.y);
}

//============================================================
//	モーション・キャラクターの更新処理
//============================================================
void CPlayer::UpdateMotion(int nMotion, const float fDeltaTime)
{
	// 死んでたら抜ける
	if (IsDeath()) { return; }

	int nAnimMotion = GetMotionType();	// 現在再生中のモーション
	if (nMotion != NONE_IDX)
	{ // モーションが設定されている場合

		if (IsMotionLoop())
		{ // ループするモーション中の場合

			if (nAnimMotion != nMotion)
			{ // 現在のモーションが再生中のモーションと一致しない場合

				// 現在のモーションの設定
				SetMotion(nMotion, BLEND_FRAME_OTHER);
			}
		}
		else
		{ // ループしないモーション中の場合

			switch (GetMotionType())
			{ // モーションごとの処理
			case MOTION_CAUTIOUS:	// 警戒モーション
			case MOTION_LANDING:	// 着地モーション

				if (nMotion != MOTION_IDOL)
				{ // 待機モーション以外の場合

					// 現在のモーションの設定
					SetMotion(nMotion, BLEND_FRAME_OTHER);
				}

				break;
			}
		}
	}

	// オブジェクトキャラクターの更新
	CObjectChara::Update(fDeltaTime);

	switch (GetMotionType())
	{ // モーションの種類ごとの処理
	case MOTION_IDOL:	// 待機モーション

		if (GetMotionNumLoop() >= CAUTIOUS_TRANS_LOOP)
		{ // 待機モーションでしばらくいた場合

			// 警戒モーションの設定
			SetMotion(MOTION_CAUTIOUS, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_CAUTIOUS:	// 警戒モーション

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_LAND);
		}

		break;

	case MOTION_DASH:	// 歩行モーション

		// ブレンド中の場合抜ける
		if (GetMotionBlendFrame() != 0) { break; }

		if (GetMotionKey() % 2 == 0 && GetMotionKeyCounter() == 0)
		{ // 足がついたタイミングの場合

			// 歩行音を鳴らす
			PLAY_SOUND(CSound::LABEL_SE_PLAYERWALK_000);

			// TODO：歩行エフェクト
#if 0
			// エフェクトを出す
			GET_EFFECT->Create("data\\EFFEKSEER\\walk.efkefc", GetVec3Position(), VEC3_ZERO, VEC3_ZERO, 250.0f);
#endif
		}

		break;

	case MOTION_STEALTHWALK:	// 忍び足モーション

		// ブレンド中の場合抜ける
		if (GetMotionBlendFrame() != 0) { break; }

		if (GetMotionKey() % 2 == 0 && GetMotionKeyCounter() == 0)
		{ // 足がついたタイミングの場合

			// 歩行音を鳴らす
			PLAY_SOUND(CSound::LABEL_SE_PLAYERWALK_001);

			// TODO：歩行エフェクト
#if 0
			// エフェクトを出す
			GET_EFFECT->Create("data\\EFFEKSEER\\walk.efkefc", GetVec3Position(), VEC3_ZERO, VEC3_ZERO, 250.0f);
#endif
		}

		break;

	case MOTION_JUMP_MINI:	// 小ジャンプモーション

		if (!m_bJump)
		{ // ジャンプ中ではない場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_JUMP_HIGH:	// 大ジャンプモーション

		if (!m_bJump)
		{ // ジャンプ中ではない場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_FALL:	// 落下モーション

		if (!m_bJump)
		{ // ジャンプ中ではない場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_LANDING:	// 着地モーション

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(nMotion, BLEND_FRAME_LAND);
		}

		break;

	case MOTION_DODGE:	// 回避モーション

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);

			if (m_state == STATE_DODGE)
			{ // 回避状態の場合

				// 状態の更新
				m_state = STATE_NORMAL;
			}
		}

		break;

	case MOTION_DEATH:	// 死亡モーション

		break;

	case MOTION_DAMAGE:	// ダメージモーション

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);

			if (m_state == STATE_DAMAGE)
			{ // ダメージ状態の場合

				// 状態の更新
				m_state = STATE_NORMAL;
			}
		}

		break;

	case MOTION_SAVE:	// チェックポイントモーション

		if (IsMotionFinish())
		{ // モーションが再生終了した場合

			// 現在のモーションの設定
			SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);

			// 状態の更新
			m_state = STATE_NORMAL;
		}

		break;

	case MOTION_GET:	// 神器取得モーション
		break;

	case MOTION_START:	// スタートモーション

		if (GetMotionKey() % 2 == 0 && GetMotionKeyCounter() == 0 && GetMotionKey() != 0)
		{ // 足がついたタイミングの場合

			// 歩行音を鳴らす
			PLAY_SOUND(CSound::LABEL_SE_PLAYERWALK_000);

			// TODO：歩行エフェクト
#if 0
			// エフェクトを出す
			GET_EFFECT->Create("data\\EFFEKSEER\\walk.efkefc", GetVec3Position(), VEC3_ZERO, VEC3_ZERO, 250.0f);
#endif
		}

		break;

	case MOTION_STAND:	// 仁王立ちモーション
		break;

	case MOTION_GOAL:	// ゴールモーション

		if (GetMotionWholeCounter() == 1)
		{
			D3DXVECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
			D3DXVECTOR3 rotPlayer = GetDestRotation();	// プレイヤー向き
			float fRotSide = rotPlayer.y + HALF_PI;		// プレイヤー横方向
			useful::NormalizeRot(fRotSide);				// 横方向の正規化

			// プレイヤー位置にオフセットを与える
			posPlayer += D3DXVECTOR3(sinf(rotPlayer.y), 0.0f, cosf(rotPlayer.y)) * -18.0f;	// 前にオフセットを与える
			posPlayer += D3DXVECTOR3(sinf(fRotSide), 0.0f, cosf(fRotSide)) * 9.0f;			// 横にオフセットを与える
			posPlayer.y += 49.0f;	// 縦にオフセットを与える

			// プレイヤー向きにオフセットを与える
			rotPlayer.x = D3DX_PI * 0.75f;

			// 巻物エフェクトを出す
			GET_EFFECT->Create("data\\EFFEKSEER\\gole.efkefc", posPlayer, rotPlayer, VEC3_ZERO, 25.0f);
		}
		break;

	case MOTION_SELECT_IN:	// セレクト開始モーション

		if (GetMotionWholeCounter() == 8)
		{
			// 着地音の再生
			PLAY_SOUND(CSound::LABEL_SE_LAND_S);
		}
		break;

	case MOTION_SELECT_OUT:	// セレクト終了モーション

		if (GetMotionWholeCounter() == 8)
		{
			// イン結び音の再生
			PLAY_SOUND(CSound::LABEL_SE_IN);
		}
		break;

	case MOTION_DROWNING:	// 水没モーション
		break;
	}
}

//============================================================
// チェックポイント回帰処理
//============================================================
void CPlayer::CheckPointBack(const float fDeltaTime)
{
	// ボタンを押されてない場合関数を抜ける
	if (!GET_INPUTPAD->IsPress(CInputPad::KEY_LB)) { m_pBackUI->SetState(CPlayerBackUI::STATE_SUB); return; }

	if (m_pBackUI->GetState() != CPlayerBackUI::STATE_ADD)
	{ // 加算状態じゃない場合

		// 加算状態にする
		m_pBackUI->SetState(CPlayerBackUI::STATE_ADD);
	}

	// 回帰時間が一定数以下の場合、抜ける
	if (m_pBackUI->GetAlpha() < 1.0f) { return; }

	if (m_pBackUI != nullptr)
	{ // 回帰UIが NULL じゃない場合

		// 通常状態にする
		m_pBackUI->SetState(CPlayerBackUI::STATE_NONE);
	}

	// フェードを開始する
	GET_MANAGER->GetFade()->SetIrisFade(nullptr, 0.6f, 0.4f, 7);

	// テレポート音を鳴らす
	PLAY_SOUND(CSound::LABEL_SE_TELEPORT);

	// チェックポイントリセット待機状態にする
	SetState(STATE_BACKWAIT);

	// 状態カウントをリセットする
	m_nCounterState = 0;
}

//============================================================
//	フェードアウト状態時の更新処理
//============================================================
bool CPlayer::UpdateFadeOut(const float fAdd)
{
	// 変数を宣言
	bool bAlpha = false;		// 透明状況
	float fAlpha = GetAlpha();	// 透明度

	// プレイヤー自身の描画を再開
	CObject::SetEnableDraw(true);

	// 透明度を上げる
	fAlpha += fAdd;

	if (fAlpha >= GetMaxAlpha())
	{ // 透明度が上がり切った場合

		// 透明度を補正
		fAlpha = GetMaxAlpha();

		// 不透明になり切った状態にする
		bAlpha = true;
	}

	// 透明度を設定
	SetAlpha(fAlpha);

	// 透明状況を返す
	return bAlpha;
}

//============================================================
//	フェードイン状態時の更新処理
//============================================================
bool CPlayer::UpdateFadeIn(const float fSub)
{
	// 変数を宣言
	bool bAlpha = false;		// 透明状況
	float fAlpha = GetAlpha();	// 透明度

	// 透明度を下げる
	fAlpha -= fSub;

	if (fAlpha <= 0.0f)
	{ // 透明度が下がり切った場合

		// 透明度を補正
		fAlpha = 0.0f;

		// 透明になり切った状態にする
		bAlpha = true;

		// プレイヤー自身の描画を停止
		CObject::SetEnableDraw(false);
	}

	// 透明度を設定
	SetAlpha(fAlpha);

	// 透明状況を返す
	return bAlpha;
}

//==========================================
//	壁との当たり判定
//==========================================
bool CPlayer::CollisionWall(D3DXVECTOR3& rPos)
{
	// 壁との当たり判定
	return GET_STAGE->CollisionWall(rPos, m_oldPos, RADIUS, HEIGHT, m_move, &m_bJump);
}

//==========================================
//  分身の処理
//==========================================
bool CPlayer::ControlClone(D3DXVECTOR3& rPos, D3DXVECTOR3& rRot, const float fDeltaTime)
{
	// 操作可能フラグを確認
	if (!m_bClone) { return false; }

	// 入力情報の受け取り
	CInputPad* pPad = GET_INPUTPAD;

#ifdef _DEBUG

	// 移動分身の削除
	if (pPad->IsTrigger(CInputPad::KEY_LB))
	{
		// リストが存在しない場合に削除しない
		if (CPlayerClone::GetList() != nullptr)
		{
			CPlayerClone::Delete(CPlayerClone::ACTION_MOVE);
		}
	}

#endif

	// 分身を削除
	DeleteClone();

	// 右スティックの入力がない場合関数を抜ける
	if (!pPad->GetTriggerRStick()) { return false; }

	// 回避処理を呼び出す
	if (Dodge(rPos, pPad))
	{
		// 移動方向の算出
		m_move.x = sinf(rRot.y) * DODGE_MOVE;
		m_move.z = cosf(rRot.y) * DODGE_MOVE;

		if (m_pDodge != nullptr)
		{
			// エフェクトを登録する
			m_pDodge->Regist(GET_EFFECT->Create("data\\EFFEKSEER\\dodge.efkefc", GetCenterPos(), rRot, VEC3_ZERO, 25.0f, false, false));
		}

		// ヒットストップさせる
		CSceneGame::GetHitStop()->SetStop(dodge::STOP_TIME);

		// カメラ揺れを与える
		GET_CAMERA->SetSwing(CCamera::TYPE_MAIN, dodge::SWING);

		// 回避音を鳴らす
		PLAY_SOUND(CSound::LABEL_SE_PLAYERSTEP_000);

		// 士気力を増やす
		CTension::Create();

		// 回避モーションにする
		SetMotion(MOTION_DODGE);

		// 回避状態に変更
		m_state = STATE_DODGE;
		return true;
	}

	// ボタンが押されていたら関数を抜ける
	if (GET_INPUTPAD->IsPress(CInputPad::KEY_RB)) { return false; }

	// 使用可能な士気力がなかった場合関数を抜ける
	if (CTension::GetUseNum() <= 0) { PLAY_SOUND(CSound::LABEL_SE_CLONEFAIL_000); return false; }

	// ギミックの直接生成ができる場合関数を抜ける
	if (CreateGimmick(fDeltaTime)) { return false; }

	// スティック入力の方向を取得する
	float fRotStick = pPad->GetPressRStickRot();

	// カメラの向きを取得
	float fCameraRot = GET_MANAGER->GetCamera()->GetRotation().y;

	// スティック方向を3D空間に対応する
	float fTemp = -(fCameraRot + fRotStick);
	useful::NormalizeRot(fTemp);

	// 分身の位置を算出
	D3DXVECTOR3 pos = rPos + D3DXVECTOR3
	(
		DISTANCE_CLONE * cosf(fTemp),
		0.0f,
		DISTANCE_CLONE * sinf(fTemp)
	);

	// 分身の移動量を算出する
	D3DXVECTOR3 move = D3DXVECTOR3
	(
		CLONE_MOVE * cosf(fTemp),
		0.0f,
		CLONE_MOVE * sinf(fTemp)
	);

	// 歩く分身を出す
	if(CPlayerClone::Create(pos, move) == nullptr)
	{
		m_bGimmickClone = true;
		m_fGimmickTimer = 0.0f;
		m_fTempStick = fRotStick;
	}

	// 分身生み出し音を鳴らす
	PLAY_SOUND(CSound::LABEL_SE_CLONEPOP_000);

	return false;
}

//==========================================
//  分身を呼び戻す
//==========================================
void CPlayer::DeleteClone()
{
	// パッドの入力情報を取得する
	CInputPad* pPad = GET_INPUTPAD;

	// 右スティックの押し込みがなかった場合関数を抜ける
	if (!pPad->IsTrigger(CInputPad::KEY_RSTICKPUSH)) { return; }

	if (CPlayerClone::GetList() != nullptr)
	{ // リストが NULL じゃない場合

		// 分身消失音を鳴らす
		PLAY_SOUND(CSound::LABEL_SE_CLONELOST_000);
	}

	// 分身を削除する
	CPlayerClone::Delete();
}

//===========================================
//  直接ギミックを生成する処理
//===========================================
bool CPlayer::CreateGimmick(const float fDeltaTime)
{
	// 生成フラグがoffの場合関数を抜ける
	if (!m_bGimmickClone) { return false; }

	// タイマーが上限を超えていた場合関数を抜ける
	if (m_fGimmickTimer >= GIMMICK_TIMER)
	{
		m_fGimmickTimer = 0.0f;
		m_bGimmickClone = false;
		return false;
	}

	// タイマーを加算する
	m_fGimmickTimer += fDeltaTime;

	// 入力情報の取得
	CInputPad* pPad = GET_INPUTPAD;

	// スティック入力がない場合関数を抜ける
	if (!pPad->GetTriggerRStick()) { return false; }

	// 前回入力との誤差が許容範囲外の場合関数を抜ける
	if (m_fTempStick - pPad->GetPressRStickRot() > STICK_ERROR)
	{
		m_fGimmickTimer = 0.0f;
		m_bGimmickClone = false;
		return false;
	}

	// ギミックのリストを取得
	if (CGimmickAction::GetList() == nullptr) { return false; }
	std::list<CGimmickAction*> list = CGimmickAction::GetList()->GetList();

	// プレイヤーから最も近いギミックを取得する変数
	CGimmickAction* pGimmick = nullptr;
	float fTempDistance = 0.0f; // 最も近いギミックまでの距離の2乗

	// 最も近いギミックを走査する
	for (auto gimmick : list)
	{
		// 保存してるギミックがnullの場合保存して次に進む
		if (pGimmick == nullptr)
		{
			pGimmick = gimmick;
			D3DXVECTOR3 vecToGimmick = GetVec3Position() - pGimmick->GetVec3Position();
			fTempDistance = vecToGimmick.x * vecToGimmick.x + vecToGimmick.y * vecToGimmick.y + vecToGimmick.z * vecToGimmick.z;
			continue;
		}

		// プレイヤーと対象ギミックを結ぶベクトルの算出
		D3DXVECTOR3 vecToGimmick = GetVec3Position() - gimmick->GetVec3Position();

		// 距離の2乗が保存された数値よりも大きい場合次に進む
		float fDistance = vecToGimmick.x * vecToGimmick.x + vecToGimmick.y * vecToGimmick.y + vecToGimmick.z * vecToGimmick.z;
		if (fTempDistance < fDistance) { continue; }

		// 対象ギミックを保存する
		pGimmick = gimmick;
		fTempDistance = fDistance;
	}

	// 距離が近くて使用可能な士気力が足りていない場合関数を抜ける
	int nNumNeed = pGimmick->GetNumActive() - pGimmick->GetNumClone();
	if (!pGimmick->CollisionPlayer() || CTension::GetUseNum() < nNumNeed) { return false; }

	// 直接ギミックになる分身を必要分生成
	for (int i = 0; i < nNumNeed; ++i)
	{
		CPlayerClone::Create(pGimmick);
	}

	// フラグをリセットし関数を抜ける
	m_fGimmickTimer = 0.0f;
	m_bGimmickClone = false;

	return true;
}

//===========================================
//  回避処理
//===========================================
bool CPlayer::Dodge(D3DXVECTOR3& rPos, CInputPad* pPad)
{
	// ボタン入力がなかった場合falseを返す
	if (!GET_INPUTPAD->IsPress(CInputPad::KEY_RB)) { return false; }

	// リストがnullの場合falseを返す
	if (CEnemyAttack::GetList() == nullptr) { return false; }

	// 攻撃する敵のリストを取得
	std::list<CEnemyAttack*> list = CEnemyAttack::GetList()->GetList();

	// 攻撃範囲を取得
	D3DXVECTOR3 collisionUp = CEnemyAttack::GetDodgeUp();
	D3DXVECTOR3 collisionDown = CEnemyAttack::GetDodgeDown();

	// 全ての敵を確認する
	for (CEnemyAttack* enemy : list)
	{
		// 回避可能状態でない場合次に進む
		if (!enemy->IsDodge()) { continue; }

		// ボックスの当たり判定
		if (!collision::Box3D
		(
			rPos,						// 判定位置
			enemy->GetVec3Position(),	// 判定目標位置
			GetVec3Sizing(),			// 判定サイズ(右・上・後)
			GetVec3Sizing(),			// 判定サイズ(左・下・前)
			collisionUp,				// 判定目標サイズ(右・上・後)
			collisionDown				// 判定目標サイズ(左・下・前)
		))
		{
			// 当たっていない場合は次に進む
			continue;
		}

		// スティック入力の方向を取得する
		float fRotStick = pPad->GetPressRStickRot() + D3DX_PI * 0.5f;

		// カメラの向きを取得
		float fCameraRot = GET_MANAGER->GetCamera()->GetRotation().y;

		// スティック方向を3D空間に対応する
		float fTemp = fRotStick + fCameraRot;
		useful::NormalizeRot(fTemp);

		// スティック方向を向く
		D3DXVECTOR3 rot = GetVec3Rotation();
		rot.y = fTemp;
		SetVec3Rotation(rot);

		// 回避に成功しtrueを返す
		return true;
	}

	return false;
}

//===========================================
//  床際のジャンプ処理
//===========================================
void CPlayer::FloorEdgeJump()
{
	// 回避中もしくはノックバック中の場合関数を抜ける
	if (m_state == STATE_DAMAGE || m_state == STATE_DODGE) { return; }

	// 上移動量を与える
	m_move.y = JUMP_MOVE;

	// ジャンプ中にする
	m_bJump = true;

	// モーションの設定
	SetMotion(MOTION_JUMP_MINI, BLEND_FRAME_OTHER);

	// 小ジャンプ音を鳴らす
	PLAY_SOUND(CSound::LABEL_SE_PLAYERJUMP_S);
}

//==========================================
// アクターの当たり判定
//==========================================
void CPlayer::CollisionActor(D3DXVECTOR3& pos, bool& rLand)
{
	bool bJump = true;

	// スフィアの当たり判定
	if (CCollisionSphere::GetList() != nullptr) 
	{
		std::list<CCollisionSphere*> list = CCollisionSphere::GetList()->GetList();	// リストを取得

		for (auto sphere : list)
		{
			// 当たり判定処理
			sphere->Hit
			(
				pos,		// 位置
				m_oldPos,	// 前回の位置
				RADIUS,		// 半径
				HEIGHT,		// 高さ
				m_move,		// 移動量
				bJump		// ジャンプ状況
			);
		}
	}

	// シリンダーの当たり判定
	if (CCollisionCylinder::GetList() != nullptr)
	{
		std::list<CCollisionCylinder*> list = CCollisionCylinder::GetList()->GetList();	// リストを取得

		for (auto cylinder : list)
		{
			// 当たり判定処理
			cylinder->Hit
			(
				pos,		// 位置
				m_oldPos,	// 前回の位置
				RADIUS,		// 半径
				HEIGHT,		// 高さ
				m_move,		// 移動量
				bJump		// ジャンプ状況
			);
		}
	}

	// キューブの当たり判定
	if (CCollisionCube::GetList() != nullptr)
	{
		std::list<CCollisionCube*> list = CCollisionCube::GetList()->GetList();	// リストを取得

		for (auto cube : list)
		{
			// 当たり判定処理
			cube->Hit
			(
				pos,		// 位置
				m_oldPos,	// 前回の位置
				RADIUS,		// 半径
				HEIGHT,		// 高さ
				m_move,		// 移動量
				bJump		// ジャンプ状況
			);
		}
	}

	// ポリゴンの当たり判定
	if (CCollisionPolygon::GetList() != nullptr)
	{
		std::list<CCollisionPolygon*> list = CCollisionPolygon::GetList()->GetList();	// リストを取得

		for (auto polygon : list)
		{
			// 当たり判定処理
			if (!polygon->Hit(pos, m_oldPos, RADIUS, HEIGHT, m_move, bJump)) { continue; }

			// 下に重力をかける
			m_move.y = -50.0f;
		}
	}

	// ふすまの当たり判定
	if (CMash::GetList() != nullptr)
	{
		std::list<CMash*> list = CMash::GetList()->GetList();	// リストを取得

		for (auto mash : list)
		{
			// 当たり判定処理
			mash->Collision
			(
				pos,
				m_oldPos,
				RADIUS,
				HEIGHT,
				m_move,
				bJump
			);
		}
	}

	// 回転扉の当たり判定
	if (CSpinWall::GetList() != nullptr)
	{
		std::list<CSpinWall*> list = CSpinWall::GetList()->GetList();	// リストを取得

		for (auto wall : list)
		{
			// 当たり判定処理
			wall->Collision
			(
				pos,
				m_oldPos,
				RADIUS,
				HEIGHT,
				m_move,
				bJump
			);
		}
	}

	// 位置を適用
	SetVec3Position(pos);

	if (!rLand &&
		bJump == false)
	{ // 着地状況が false かつ、ジャンプ状況が false の場合

		// 着地判定を true にする
		rLand = true;

		// ジャンプ状況を false にする
		m_bJump = false;
	}
}

//==========================================
// 敵の当たり判定
//==========================================
void CPlayer::CollisionEnemy(D3DXVECTOR3& pos)
{
	// 敵のリスト構造が無ければ、抜ける
	if (CEnemy::GetList() == nullptr) { return; }

	std::list<CEnemy*> list = CEnemy::GetList()->GetList();	// リストを取得

	for (auto enemy : list)
	{
		// 当たり判定処理
		enemy->CollisionToPlayer
		(
			pos,		// 位置
			RADIUS,		// 半径
			HEIGHT		// 高さ
		);
	}
}

//==========================================
// コインとの当たり判定
//==========================================
void CPlayer::CollisionCoin(const D3DXVECTOR3& pos)
{
	// コインのリスト構造が無ければ抜ける
	if (CCoin::GetList() == nullptr) { return; }

	// リストを取得
	std::list<CCoin*> list = CCoin::GetList()->GetList();

	for (auto coin : list)
	{
		// 当たり判定処理
		coin->Collision
		(
			pos,		// 位置
			RADIUS		// 半径
		);
	}
}

//==========================================
// 神器との当たり判定
//==========================================
void CPlayer::CollisionGodItem(const D3DXVECTOR3& pos)
{
	// 神器のリスト構造が無ければ抜ける
	if (CGodItem::GetList() == nullptr) { return; }

	// リストを取得
	std::list<CGodItem*> list = CGodItem::GetList()->GetList();

	for (auto godItem : list)
	{
		// 当たり判定処理
		if (godItem->Collision
		(
			pos,		// 位置
			RADIUS		// 半径
		))
		{
			// 回復
			RecoverItem();
		}
	}
}

//============================================================
// タッチアクターとの当たり判定
//============================================================
void CPlayer::CollisionTouchActor(const D3DXVECTOR3& rPos)
{
	// タッチアクターのリスト構造が無ければ抜ける
	if (CTouchActor::GetList() == nullptr) { return; }

	// リストを取得
	std::list<CTouchActor*> list = CTouchActor::GetList()->GetList();

	for (auto actor : list)
	{
		// 当たり判定処理
		actor->Collision
		(
			rPos,				// 位置
			GetOldPosition(),	// 前回の位置
			RADIUS,				// 半径
			HEIGHT				// 高さ
		);
	}
}

//============================================================
//	セットアップ処理
//============================================================
HRESULT CPlayer::LoadSetup(const char* pPass)
{
	EType type = TYPE_SELECT;		// 種類の代入用
	D3DXVECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向きの代入用

	// ファイルを開く
	std::ifstream file(pPass);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "プレイヤーセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// ファイルを読込
	std::string str;	// 読込文字列
	while (file >> str)
	{ // ファイルの終端ではない場合ループ

		if (str.front() == '#')
		{ // コメントアウトされている場合

			// 一行全て読み込む
			std::getline(file, str);
		}
		else if (str == "STAGE_PLAYERSET")
		{
			do
			{ // END_STAGE_PLAYERSETを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str == "TYPE")
				{
					file >> str;	// ＝を読込
					file >> str;	// 種類を読込

					// 文字列を列挙に変換
					if		(str == "TITLE")	{ type = TYPE_TITLE; }
					else if	(str == "SELECT")	{ type = TYPE_SELECT; }
					else if	(str == "GAME")		{ type = TYPE_GAME; }
				}
				else if (str == "POS")
				{
					file >> str;	// ＝を読込

					// 位置を読込
					file >> pos.x;
					file >> pos.y;
					file >> pos.z;
				}
				else if (str == "ROT")
				{
					file >> str;	// ＝を読込

					// 向きを読込
					file >> rot.x;
					file >> rot.y;
					file >> rot.z;
				}
			} while (str != "END_STAGE_PLAYERSET");	// END_STAGE_CHECKSETを読み込むまでループ

			// プレイヤーの生成
			if (CPlayer::Create(type, pos, D3DXToRadian(rot)) == nullptr)
			{ // 確保に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}

#ifdef _DEBUG

//==========================================
// ジャンプ操作処理
//==========================================
void CPlayer::DebugJumpControl(void)
{
	if (GET_INPUTPAD->IsTrigger(CInputPad::KEY_X)
	||  GET_INPUTKEY->IsTrigger(DIK_SPACE))
	{
		// 上昇量を与えるよ
		m_move.y = JUMP_MOVE;

		// ジャンプ中にするよ
		m_bJump = true;

		// ジャンプモーションを設定
		SetMotion(MOTION_JUMP_MINI);
	}
}

//==========================================
// キーボードの移動操作処理
//==========================================
void CPlayer::DebugMoveControl(void)
{
	CInputKeyboard* pKey = GET_INPUTKEY;
	float fMoveRot = 0.0f;	// 移動方向
	const float MOVE = 60.0f;

	if (pKey->IsPress(DIK_W))
	{
		if (pKey->IsPress(DIK_A))
		{
			// 移動方向を設定
			fMoveRot = (D3DX_PI * 0.75f);

			// 位置を設定
			m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
			m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;
		}
		else if (pKey->IsPress(DIK_D))
		{
			// 移動方向を設定
			fMoveRot = (D3DX_PI * -0.75f);

			// 位置を設定
			m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
			m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;
		}
		else
		{
			// 移動方向を設定
			fMoveRot = D3DX_PI;

			// 位置を設定
			m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
			m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;
		}

		// 向きを設定
		m_destRot.y = fMoveRot;
	}
	else if (pKey->IsPress(DIK_S))
	{
		if (pKey->IsPress(DIK_A))
		{
			// 移動方向を設定
			fMoveRot = (D3DX_PI * 0.25f);

			// 位置を設定
			m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
			m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;
		}
		else if (pKey->IsPress(DIK_D))
		{
			// 移動方向を設定
			fMoveRot = (D3DX_PI * -0.25f);

			// 位置を設定
			m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
			m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;
		}
		else
		{
			// 移動方向を設定
			fMoveRot = 0.0f;

			// 位置を設定
			m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
			m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;
		}

		// 向きを設定
		m_destRot.y = fMoveRot;
	}
	else if (pKey->IsPress(DIK_A))
	{
		// 移動方向を設定
		fMoveRot = (D3DX_PI * 0.5f);

		// 位置を設定
		m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
		m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;

		// 向きを設定
		m_destRot.y = fMoveRot;
	}
	else if (pKey->IsPress(DIK_D))
	{
		// 移動方向を設定
		fMoveRot = (D3DX_PI * -0.5f);

		// 位置を設定
		m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
		m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;

		// 向きを設定
		m_destRot.y = fMoveRot;
	}

	// 向きの正規化
	useful::NormalizeRot(m_destRot.y);
}

//==========================================
// キーボードの分身操作処理
//==========================================
void CPlayer::DebugCloneControl(void)
{
	CInputKeyboard* pKey = GET_INPUTKEY;
	float fStickRot = 0.0f;
	D3DXVECTOR3 move = VEC3_ZERO;
	D3DXVECTOR3 pos = GetVec3Position();

	if (pKey->IsTrigger(DIK_I))
	{ // 前関係移動
		if (pKey->IsTrigger(DIK_J))
		{
			fStickRot = (D3DX_PI * 0.75f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// 歩く分身を出す
			CPlayerClone::Create(pos, move);
		}
		else if (pKey->IsTrigger(DIK_L))
		{
			fStickRot = (D3DX_PI * -0.75f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// 歩く分身を出す
			CPlayerClone::Create(pos, move);
		}
		else
		{
			fStickRot = D3DX_PI;

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// 歩く分身を出す
			CPlayerClone::Create(pos, move);
		}
	}
	else if (pKey->IsTrigger(DIK_K))
	{ // 後ろ関係移動
		if (pKey->IsTrigger(DIK_J))
		{
			fStickRot = (D3DX_PI * 0.25f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// 歩く分身を出す
			CPlayerClone::Create(pos, move);
		}
		else if (pKey->IsTrigger(DIK_L))
		{
			fStickRot = (D3DX_PI * -0.25f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// 歩く分身を出す
			CPlayerClone::Create(pos, move);
		}
		else
		{
			fStickRot = 0.0f;

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// 歩く分身を出す
			CPlayerClone::Create(pos, move);
		}
	}
	else if (pKey->IsTrigger(DIK_J))
	{ // 左関係移動
		fStickRot = (D3DX_PI * 0.5f);

		move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
		move.y = 0.0f;
		move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

		// 歩く分身を出す
		CPlayerClone::Create(pos, move);
	}
	else if (pKey->IsTrigger(DIK_L))
	{ // 右関係移動
		fStickRot = (D3DX_PI * -0.5f);

		move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
		move.y = 0.0f;
		move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

		// 歩く分身を出す
		CPlayerClone::Create(pos, move);
	}
}

#endif // _DEBUG
