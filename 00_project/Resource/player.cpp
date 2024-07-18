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
#include "transpoint.h"
#include "effect3D.h"
#include "actor.h"
#include "effekseerControl.h"
#include "effekseerManager.h"
#include "gimmick_action.h"
#include "enemyAttack.h"
#include "tension.h"

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
	const float	GRAVITY		= 60.0f;			// 重力
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

	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 15.0f, 0.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), XCOL_CYAN);	// オフセット情報
	const int ORBIT_PART = 15;	// 分割数

	const float	STEALTH_MOVE	= 300.0f;	// 忍び足の移動量
	const float	NORMAL_MOVE = 600.0f;	// 通常の移動量
	const float	DODGE_MOVE = 800.0f;	// 回避の移動量
	const float CLONE_MOVE		= NORMAL_MOVE * 1.1f; // 分身の移動量

	const D3DXVECTOR3 TENSION_SIZE = D3DXVECTOR3(75.0f, 75.0f, 0.0f); // 士気力ゲージのサイズ

	const int INIT_CLONE = 5; // 最初に使える分身の数
	const float DISTANCE_CLONE = 50.0f; // 分身の出現位置との距離
	const int JUST_RECOVER = 500; // ジャストアクションでの回復量
	const float GIMMICK_TIMER = 0.5f; // 直接ギミックを生成できる時間
	const float STICK_ERROR = D3DX_PI * 0.875f; // スティックの入力誤差許容範囲

	// ブラーの情報
	namespace blurInfo
	{
		const float	START_ALPHA = 0.4f;	// ブラー開始透明度
		const int	MAX_LENGTH = 15;	// 保持オブジェクト最大数
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
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER, CObject::DIM_3D, PRIORITY),
	m_pOrbit		(nullptr),		// 軌跡の情報
	m_oldPos		(VEC3_ZERO),	// 過去位置
	m_move			(VEC3_ZERO),	// 移動量
	m_destRot		(VEC3_ZERO),	// 目標向き
	m_state			(STATE_NONE),	// 状態
	m_bJump			(false),		// ジャンプ状況
	m_nCounterState	(0),			// 状態管理カウンター
	m_pCheckPoint	(nullptr),		// セーブしたチェックポイント
	m_fScalar		(0.0f),			// 移動量
	m_bClone		(true),			// 分身操作可能フラグ
	m_bGimmickClone	(false),		// ギミッククローンの生成フラグ
	m_fGimmickTimer	(0.0f),			// ギミッククローンの生成タイマー
	m_fTempStick	(0.0f),			// スティックの入力角を保存する変数
	m_bGetCamera	(false),		// カメラ取得フラグ
	m_fCameraRot	(0.0f),			// カメラの角度
	m_fStickRot		(0.0f),			// スティックの角度
	m_sFrags		({}),			// フィールドフラグ
	m_pCurField		(nullptr),		// 現在乗ってる地面
	m_pOldField		(nullptr),		// 前回乗ってた地面
	m_pEffectdata	(nullptr)		// エフェクト情報
{
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
	m_pOrbit		= nullptr;		// 軌跡の情報
	m_oldPos		= VEC3_ZERO;	// 過去位置
	m_move			= VEC3_ZERO;	// 移動量
	m_destRot		= VEC3_ZERO;	// 目標向き
	m_state			= STATE_NONE;	// 状態
	m_bJump			= true;			// ジャンプ状況
	m_nCounterState	= 0;			// 状態管理カウンター
	m_pCheckPoint	= nullptr;		// セーブしたチェックポイント
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

	// 軌跡の生成
	m_pOrbit = COrbit::Create
	( // 引数
		GetParts(MODEL_BODY)->GetPtrMtxWorld(),	// 親マトリックス
		ORBIT_OFFSET,	// オフセット情報
		ORBIT_PART		// 分割数
	);
	if (m_pOrbit == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
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

	// プレイヤーを出現させる
	SetSpawn();

	// 士気力ゲージの生成
	for (int i = 0; i < INIT_CLONE; ++i)
	{
		CTension::Create();
	}

	// 開始エフェクトを出す
	GET_EFFECT->Create("data\\EFFEKSEER\\gamestart.efkefc", GetVec3Position(), GetVec3Rotation(), VEC3_ZERO, 60.0f);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayer::Uninit(void)
{
	// 軌跡の終了
	SAFE_UNINIT(m_pOrbit);

	// エフェクトの終了
	SAFE_DELETE(m_pEffectdata);

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

	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション

	// 過去位置の更新
	UpdateOldPosition();

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

	case STATE_NORMAL:

		// 通常状態の更新
		currentMotion = UpdateNormal(fDeltaTime);
		break;

	case STATE_DODGE:

		// 回避状態の更新
		currentMotion = UpdateDodge(fDeltaTime);
		break;

	default:
		assert(false);
		break;
	}

	// 軌跡の更新
	m_pOrbit->Update(fDeltaTime);

	// モーション・オブジェクトキャラクターの更新
	UpdateMotion(currentMotion, fDeltaTime);

#ifdef _DEBUG

	// 入力情報を受け取るポインタ
	CInputKeyboard* pKeyboard = GET_INPUTKEY;

	if (pKeyboard->IsTrigger(DIK_RIGHT))
	{
		RecoverCheckPoint();
	}
	if (pKeyboard->IsTrigger(DIK_LEFT))
	{
		RecoverJust();
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
	CObject::SetEnableUpdate(bUpdate);		// 自身

}

//============================================================
//	描画状況の設定処理
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);		// 自身

}

//============================================================
//	生成処理
//============================================================
CPlayer *CPlayer::Create(CScene::EMode mode)
{
	// ポインタを宣言
	CPlayer *pPlayer = nullptr;	// プレイヤー情報

	// プレイヤーの生成
	switch (mode)
	{ // モードごとの処理
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_SELECT:
	case CScene::MODE_GAME:
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
bool CPlayer::HitKnockBack(const int nDamage, const D3DXVECTOR3& /*rVecKnock*/)
{
	if (IsDeath())				 { return false; }	// 死亡済み
	if (m_state != STATE_NORMAL) { return false; }	// 通常状態以外

	return true;
}

//============================================================
//	ヒット処理
//============================================================
bool CPlayer::Hit(const int nDamage)
{
	if (IsDeath())				 { return false; }	// 死亡済み
	if (m_state != STATE_NORMAL) { return false; }	// 通常状態以外

	// ジャンプエフェクトを出す
	GET_EFFECT->Create("data\\EFFEKSEER\\hit.efkefc", GetVec3Position() + OFFSET_JUMP, GetVec3Rotation(), VEC3_ZERO, 250.0f);

	return true;
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

	// 位置を設定
	D3DXVECTOR3 pos = D3DXVECTOR3(-1600.0f, 0.0f, 0.0f);	// 位置
	SetVec3Position(pos);

	// 向きを設定
	D3DXVECTOR3 rot = VEC3_ZERO;	// 向き
	SetVec3Rotation(rot);
	m_destRot = rot;

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

	// ジャンプエフェクトを出す
	GET_EFFECT->Create("data\\EFFEKSEER\\Highjump.efkefc", GetVec3Position() + OFFSET_JUMP, GetVec3Rotation(), VEC3_ZERO, 25.0f);

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

}

//==========================================
//  ジャストアクションでの回復処理
//==========================================
void CPlayer::RecoverJust()
{
	// 回復エフェクトを出す
	GET_EFFECT->Create("data\\EFFEKSEER\\concentration.efkefc", GetVec3Position(), GetVec3Rotation(), VEC3_ZERO, 50.0f);
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
	UpdateGravity();

	// 位置更新
	UpdatePosition(posPlayer, fDeltaTime);

	// アクターの当たり判定
	CollisionActor(posPlayer);

	// 着地判定
	UpdateLanding(posPlayer, fDeltaTime);

	// 向き更新
	UpdateRotation(rotPlayer, fDeltaTime);

	// 壁の当たり判定
	GET_STAGE->CollisionWall(posPlayer, m_oldPos, RADIUS, HEIGHT, m_move, &m_bJump);

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
		SetState(STATE_NORMAL);
	}

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
	UpdateGravity();

	// 位置更新
	UpdatePosition(posPlayer, fDeltaTime);

	// アクターの当たり判定
	CollisionActor(posPlayer);

	// 着地判定
	UpdateLanding(posPlayer, fDeltaTime);

	// 向き更新
	UpdateRotation(rotPlayer, fDeltaTime);

	// 壁の当たり判定
	GET_STAGE->CollisionWall(posPlayer, m_oldPos, RADIUS, HEIGHT, m_move, &m_bJump);

	// ステージ遷移の更新
	UpdateTrans(posPlayer);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// 分身の処理
	if(ControlClone(posPlayer, rotPlayer, fDeltaTime))
	{ currentMotion = MOTION_DODGE; }

	// 保存位置の更新
	UpdateSaveTeleport();

	// 現在のモーションを返す
	return currentMotion;
}

//===========================================
//  回避状態時の更新処理
//===========================================
CPlayer::EMotion CPlayer::UpdateDodge(const float fDeltaTime)
{
	// 回避モーション以外の場合通常状態になる
	if (GetMotion()->GetType() != MOTION_DODGE)
	{
		// エフェクトを削除する
		SAFE_DELETE(m_pEffectdata);

		// 通常状態に戻る
		m_state = STATE_NORMAL;

		// 待機モーションにする
		return MOTION_IDOL;
	}

	// 向きの取得
	float rot = GetVec3Rotation().y;

	// 移動方向の算出
	m_move.x = sinf(rot) * DODGE_MOVE;
	m_move.z = cosf(rot) * DODGE_MOVE;

	// 位置の取得
	D3DXVECTOR3 pos = GetVec3Position();

	// エフェクトの位置を設定する
	m_pEffectdata->m_pos = pos;

	// 重力の更新
	UpdateGravity();

	// 位置更新
	UpdatePosition(pos, fDeltaTime);

	// アクターの当たり判定
	CollisionActor(pos);

	// 着地判定
	UpdateLanding(pos, fDeltaTime);

	// 壁の当たり判定
	GET_STAGE->CollisionWall(pos, m_oldPos, RADIUS, HEIGHT, m_move, &m_bJump);

	// 位置を反映
	SetVec3Position(pos);

	return MOTION_DODGE;
}

//============================================================
//	過去位置の更新処理
//============================================================
void CPlayer::UpdateOldPosition(void)
{
	// 過去位置を更新
	m_oldPos = GetVec3Position();
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
		D3DXVECTOR3 fRate = pPad->GetStickRateL(pad::DEAD_RATE);
		m_move.x = -sinf(fMoveRot) * NORMAL_MOVE;
		m_move.z = -cosf(fMoveRot) * NORMAL_MOVE;

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
			m_move.x *= 3.0f;
			m_move.z *= 3.0f;
		}
#endif
		// 歩行モーションにする
		currentMotion = MOTION_DASH;

		// 移動量をスカラー値に変換する
		m_fScalar = sqrtf(m_move.x * m_move.x + m_move.z * m_move.z);

		// 移動量が一定未満の場合忍び足モーションになる
		if (m_fScalar <= STEALTH_MOVE)
		{
			// 忍び足モーションにする
			currentMotion = MOTION_STEALTHWALK;
		}
	}
	else
	{
		// フラグを折る
		m_bGetCamera = false;
	}

#ifdef _DEBUG

	// キーボードの移動操作
	DebugMoveControl();

	// ジャンプ操作
	DebugJumpControl();

#endif

	// モーションを返す
	return currentMotion;
}

//============================================================
//	重力の更新処理
//============================================================
void CPlayer::UpdateGravity(void)
{
	// 重力を加算
	m_move.y -= GRAVITY;
}

//============================================================
//	保存位置の更新処理
//============================================================
void CPlayer::UpdateSaveTeleport(void)
{
	CInputPad* pPad = GET_INPUTPAD;
	if (pPad->IsTrigger(CInputPad::KEY_BACK))
	{
		// チェックポイントに帰る
		SaveReset();
	}
}

//============================================================
//	着地状況の更新処理
//============================================================
bool CPlayer::UpdateLanding(D3DXVECTOR3& rPos, const float fDeltaTime)
{
	bool bLand = false;	// 着地フラグ
	CStage *pStage = GET_STAGE;	// ステージ情報
	D3DXVECTOR3 move = m_move * fDeltaTime;		//現在の移動速度を一時保存

	// 前回の着地地面を保存
	m_pOldField = m_pCurField;

	// 地面・制限位置の着地判定
	if (pStage->LandFieldPosition(rPos, m_oldPos, m_move, &m_pCurField)
	||  pStage->LandLimitPosition(rPos, m_move, 0.0f))
	{ // プレイヤーが着地していた場合

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		m_bJump = false;
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
				GET_EFFECT->Create("data\\EFFEKSEER\\landing_big.efkefc", GetVec3Position() + move, GetVec3Rotation(), VEC3_ZERO, 90.0f);
				// 着地音(大)の再生
				PLAY_SOUND(CSound::LABEL_SE_LAND_B);
			}
			else if (move.y < LANDING_SPEED_M)
			{
				// 着地(中)エフェクトを出す
				GET_EFFECT->Create("data\\EFFEKSEER\\landing_mid.efkefc", GetVec3Position() + move, GetVec3Rotation(), VEC3_ZERO, 60.0f);
				// 着地音(大)の再生
				PLAY_SOUND(CSound::LABEL_SE_LAND_B);
			}
			else
			{
				// 着地(小)エフェクトを出す
				GET_EFFECT->Create("data\\EFFEKSEER\\landing_small.efkefc", GetVec3Position() + move, GetVec3Rotation(), VEC3_ZERO, 90.0f);
				// 着地音(小)の再生
				PLAY_SOUND(CSound::LABEL_SE_LAND_S);
			}

			
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

#if 0
	case MOTION_DASH:	// 歩行モーション

		if (GetMotionPose() % 4 == 0 && GetMotionCounter() == 0)
		{ // 足がついたタイミングの場合

			switch (m_land)
			{ // 着地物ごとの処理
			case LAND_OBSTACLE:

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_WALK_OBS);	// 歩行音（障害物）

				break;

			default:

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_WALK_BUILD);	// 歩行音（ビル）

				break;
			}
		}

		break;

	case MOTION_STEALTHWALK:	// 忍び足モーション

		break;
#endif

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
		}

		break;
	}
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
//	ステージ遷移の更新処理
//==========================================
void CPlayer::UpdateTrans(D3DXVECTOR3& rPos)
{
	CInputPad* pPad = GET_INPUTPAD;	// パッド情報
	if (pPad->IsTrigger(CInputPad::KEY_B))
	{
		// 触れている遷移ポイントを取得
		CTransPoint *pHitTrans = CTransPoint::Collision(rPos, RADIUS);

		// 遷移ポイントに触れていない場合抜ける
		if (pHitTrans == nullptr) { return; }

		// 遷移ポイントのマップパスに遷移
		GET_STAGE->SetInitMapPass(pHitTrans->GetTransMapPass().c_str());
		GET_MANAGER->SetLoadScene(CScene::MODE_GAME);
	}
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
	DelelteClone();

	// 右スティックの入力がない場合関数を抜ける
	if (!pPad->GetTriggerRStick()) { return false; }

	// 回避処理を呼び出す
	if (Dodge(rPos, pPad))
	{
		// 移動方向の算出
		m_move.x = sinf(rRot.y) * DODGE_MOVE;
		m_move.z = cosf(rRot.y) * DODGE_MOVE;

		// エフェクトを出す
		m_pEffectdata = GET_EFFECT->Create("data\\EFFEKSEER\\concentration.efkefc", rPos, rRot, m_move * fDeltaTime, 40.0f, true);

		// 士気力を増やす
		CTension::Create();

		// 回避状態に変更
		m_state = STATE_DODGE;
		return true;
	}

	// 使用可能な士気力がなかった場合関数を抜ける
	if (CTension::GetUseNum() <= 0) { return false; }

	// ギミックの直接生成ができる場合関数を抜ける
	if (CreateGimmick(fDeltaTime)) { return false; }

	// スティック入力の方向を取得する
	float fRotStick = pPad->GetPressRStickRot();

	// カメラの向きを取得
	float fCameraRot = GET_MANAGER->GetCamera()->GetRotation().y;

	// スティック方向を3D空間に対応する
	float fTemp = -(fRotStick + fCameraRot);
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
		m_fTempStick = -fTemp;
	}

	return false;
}

//==========================================
//  直前のチェックポイントに帰る
//==========================================
void CPlayer::SaveReset()
{
	// セーブされていない場合関数を抜ける
	if (m_pCheckPoint == nullptr) { return; }

	// チェックポイントの座標に飛ぶ
	SetVec3Position(m_pCheckPoint->GetVec3Position());
}

//==========================================
//  分身を呼び戻す
//==========================================
void CPlayer::DelelteClone()
{
	// パッドの入力情報を取得する
	CInputPad* pPad = GET_INPUTPAD;

	// 右スティックの押し込みがなかった場合関数を抜ける
	if (!pPad->IsTrigger(CInputPad::KEY_RSTICKPUSH)) { return; }

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

	// スティック入力があった場合
	if (pPad->GetTriggerRStick())
	{
		// 前回入力との誤差が許容範囲外の場合関数を抜ける
		if (m_fTempStick - pPad->GetPressRStickRot() > STICK_ERROR)
		{
			m_fGimmickTimer = 0.0f;
			m_bGimmickClone = false;
			return false;
		}

		// ギミックのリストを取得
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
			float fDistance = vecToGimmick.x* vecToGimmick.x + vecToGimmick.y * vecToGimmick.y + vecToGimmick.z * vecToGimmick.z;
			if (fTempDistance < fDistance)
			{ continue; }

			// 対象ギミックを保存する
			pGimmick = gimmick;
			fTempDistance = fDistance;
		}

		// 距離が近くて使用可能な士気力が足りている場合
		if (pGimmick->CollisionPlayer() && CTension::GetUseNum() >= pGimmick->GetNumActive())
		{
			// 直接ギミックになる分身を必要分生成
			for (int i = 0; i < pGimmick->GetNumActive(); ++i)
			{
				CPlayerClone::Create(pGimmick);
			}
		}

		// フラグをリセットし関数を抜ける
		m_fGimmickTimer = 0.0f;
		m_bGimmickClone = false;
		return true;
	}

	return true;
}

//===========================================
//  回避処理
//===========================================
bool CPlayer::Dodge(D3DXVECTOR3& rPos, CInputPad* pPad)
{
	// リストがnullの場合falseを返す
	if (CEnemyAttack::GetList() == nullptr) { return false; }

	// 攻撃する敵のリストを取得
	std::list<CEnemyAttack*> list = CEnemyAttack::GetList()->GetList();

	// 攻撃範囲を取得
	D3DXVECTOR3 coliisionUp = CEnemyAttack::GetAttackUp();
	D3DXVECTOR3 coliisionDown = CEnemyAttack::GetAttackDown();

	// 全ての敵を確認する
	for (CEnemyAttack* enemy : list)
	{
		// 回避可能状態でない場合次に進む
		if (!enemy->IsDodge()) { continue; }

		// ボックスの当たり判定
		if (!collision::Box3D
		(
			rPos,				// 判定位置
			enemy->GetVec3Position(),			// 判定目標位置
			GetVec3Sizing(),	// 判定サイズ(右・上・後)
			GetVec3Sizing(),	// 判定サイズ(左・下・前)
			coliisionUp,		// 判定目標サイズ(右・上・後)
			coliisionDown		// 判定目標サイズ(左・下・前)
		))
		{
			// 当たっていない場合は次に進む
			continue;
		}

		// 回避に成功しtrueを返す
		return true;
	}

	return false;
}

//==========================================
// アクターの当たり判定
//==========================================
void CPlayer::CollisionActor(D3DXVECTOR3& pos)
{
	// アクターのリスト構造が無ければ抜ける
	if (CActor::GetList() == nullptr) { return; }

	std::list<CActor*> list = CActor::GetList()->GetList();	// リストを取得

	for (auto actor : list)
	{
		// 当たり判定処理
		actor->Collision
		(
			pos,		// 位置
			m_oldPos,	// 前回の位置
			RADIUS,		// 半径
			HEIGHT,		// 高さ
			m_move,		// 移動量
			m_bJump		// ジャンプ状況
		);
	}

	// 位置を適用
	SetVec3Position(pos);
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
