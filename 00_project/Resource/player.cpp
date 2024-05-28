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
#include "rankingManager.h"
#include "stage.h"
#include "field.h"
#include "cloneAngleUI.h"
#include "input.h"
#include "player_clone.h"
#include "checkpoint.h"
#include "gauge2D.h"
#include "effect3D.h"
#include "actor.h"
#include "effekseerControl.h"
#include "effekseerManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\CHARACTER\\player.txt";	// セットアップテキスト相対パス

	const int	PRIORITY	= 3;			// プレイヤーの優先順位
	const int	BLEND_FRAME	= 5;			// モーションのブレンドフレーム
	const float	JUMP		= 1260.0f;		// ジャンプ上昇量
	const float	GRAVITY		= 60.0f;		// 重力
	const float	RADIUS		= 20.0f;		// 半径
	const float	REV_ROTA	= 0.15f;		// 向き変更の補正係数
	const float	ADD_MOVE	= 0.08f;		// 非アクション時の速度加算量
	const float	JUMP_REV	= 0.16f;		// 通常状態時の空中の移動量の減衰係数
	const float	LAND_REV	= 0.16f;		// 通常状態時の地上の移動量の減衰係数
	const float	SPAWN_ADD_ALPHA	= 0.03f;	// スポーン状態時の透明度の加算量

	const D3DXVECTOR3 DMG_ADDROT	= D3DXVECTOR3(0.04f, 0.0f, -0.02f);	// ダメージ状態時のプレイヤー回転量
	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// 影の大きさ

	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 15.0f, 0.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), XCOL_CYAN);	// オフセット情報
	const int ORBIT_PART = 15;	// 分割数

	const float STEALTH_BORDER	= 16000.0f;	// 忍び足になる基準のスピード
	const float	STEALTH_MOVE	= 100.0f;	// 忍び足の移動量
	const float	NORMAL_MOVE		= 600.0f;	// 通常の移動量

	const char* PARAM_FILE = "data\\TXT\\PlayerParameter.txt";

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
	m_pShadow			(nullptr),		// 影の情報
	m_pOrbit			(nullptr),		// 軌跡の情報
	m_oldPos			(VEC3_ZERO),	// 過去位置
	m_move				(VEC3_ZERO),	// 移動量
	m_destRot			(VEC3_ZERO),	// 目標向き
	m_state				(STATE_NONE),	// 状態
	m_bJump				(false),		// ジャンプ状況
	m_nCounterState		(0),			// 状態管理カウンター
	m_pTensionGauge		(nullptr),		// 士気力ゲージのポインタ
	m_nMaxTension		(0),			// 最大士気力
	m_nInitTension		(0),			// 初期士気力
	m_nSpeedTension		(0),			// 士気力ゲージの増減速度
	m_nMaxClone			(0),			// 一度に分身できる上限
	m_nRecover			(0),			// ジャストアクションでの回復量
	m_pCheckPoint		(nullptr),		// セーブしたチェックポイント
	m_fHeght			(0.0f),			// 立幅
	m_fInertial			(0.0f),			// 慣性力
	m_fScalar			(0.0f),			// 移動量
	m_fChargeTime		(0.0f)			// ため時間
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
	m_pShadow			= nullptr;		// 影の情報
	m_pOrbit			= nullptr;		// 軌跡の情報
	m_oldPos			= VEC3_ZERO;	// 過去位置
	m_move				= VEC3_ZERO;	// 移動量
	m_destRot			= VEC3_ZERO;	// 目標向き
	m_state				= STATE_NONE;	// 状態
	m_bJump				= true;			// ジャンプ状況
	m_nCounterState		= 0;			// 状態管理カウンター
	m_pTensionGauge		= nullptr;		// 士気力ゲージのポインタ
	m_pCheckPoint		= nullptr;		// セーブしたチェックポイント
	m_fScalar			= 0.0f;			// 移動量

	// 定数パラメータの読み込み
	LoadParameter();

	// オブジェクトキャラクターの初期化
	if (FAILED(CObjectChara::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キャラクター情報の割当
	BindCharaData(SETUP_TXT);

	// 影の生成
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this);
	if (m_pShadow == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

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

	// 士気力ゲージを生成
	m_pTensionGauge = CGauge2D::Create
	(
		m_nMaxTension, m_nSpeedTension, D3DXVECTOR3(300.0f, 30.0f, 0.0f),
		D3DXVECTOR3(300.0f, 30.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.56f, 0.87f, 1.0f),
		D3DXCOLOR(0.31f, 0.89f, 0.97f, 1.0f)
	);
	m_pTensionGauge->SetNum(m_nInitTension);
	m_pTensionGauge->SetLabel(LABEL_UI);

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

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayer::Uninit(void)
{
	// 士気力ゲージの終了
	SAFE_UNINIT(m_pTensionGauge);
	
	// 影の終了
	m_pShadow->DeleteObjectParent();	// 親オブジェクトを削除
	SAFE_UNINIT(m_pShadow);

	// 軌跡の終了
	SAFE_UNINIT(m_pOrbit);

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
	// 変数を宣言
	EMotion currentMotion = MOTION_IDOL;	// 現在のモーション

	// 過去位置の更新
	UpdateOldPosition();

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:
		break;

	case STATE_SPAWN:

		// スポーン状態時の更新
		currentMotion = UpdateSpawn(fDeltaTime);
		break;

	case STATE_NORMAL:

		// 通常状態の更新
		currentMotion = UpdateNormal(fDeltaTime);
		break;

	default:
		assert(false);
		break;
	}

	// アクターの当たり判定
	CollisionActor();

	// 影の更新
	m_pShadow->Update(fDeltaTime);

	// 軌跡の更新
	m_pOrbit->Update(fDeltaTime);

	// モーション・オブジェクトキャラクターの更新
	UpdateMotion(currentMotion, fDeltaTime);

	// デバッグ表示
	DebugProc::Print(DebugProc::POINT_LEFT, "士気力 : %d\n", m_pTensionGauge->GetNum());

#ifdef _DEBUG

	// 入力情報を受け取るポインタ
	CInputKeyboard* pKeyboard = GET_INPUTKEY;

	// 士気力の変更
	if (pKeyboard->IsTrigger(DIK_UP))
	{
		m_pTensionGauge->AddNum(100);
	}
	if (pKeyboard->IsTrigger(DIK_DOWN))
	{
		m_pTensionGauge->AddNum(-100);
	}
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
	CToonShader	*pToonShader = CToonShader::GetInstance();	// トゥーンシェーダー情報
	if (pToonShader->IsEffectOK())
	{ // エフェクトが使用可能な場合

		// オブジェクトキャラクターの描画
		CObjectChara::Draw(pToonShader);
	}
	else
	{ // エフェクトが使用不可能な場合

		// エフェクトエラー
		assert(false);

		// オブジェクトキャラクターの描画
		CObjectChara::Draw(pShader);
	}
}

//============================================================
//	更新状況の設定処理
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);		// 自身
	m_pShadow->SetEnableUpdate(bUpdate);	// 影
}

//============================================================
//	描画状況の設定処理
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);		// 自身
	m_pShadow->SetEnableDraw(bDraw);	// 影
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
	case CScene::MODE_RESULT:
	case CScene::MODE_RANKING:
		break;

	case CScene::MODE_TUTORIAL:
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

	// 士気力を減少
	m_pTensionGauge->AddNum(-nDamage);

	return true;
}

//============================================================
//	ヒット処理
//============================================================
bool CPlayer::Hit(const int nDamage)
{
	if (IsDeath())				 { return false; }	// 死亡済み
	if (m_state != STATE_NORMAL) { return false; }	// 通常状態以外

	// 士気力を減少
	m_pTensionGauge->AddNum(-nDamage);

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
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 500.0f, 0.0f);	// 位置
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

	// TPSカメラの目標位置の設定
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_TPS);
	GET_MANAGER->GetCamera()->SetDestTps();
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
	return m_fHeght;
}

//==========================================
//  士気力の値を取得
//==========================================
int CPlayer::GetTension() const
{
	// 士気力ゲージが存在しない場合
	if (m_pTensionGauge == nullptr) { return -1; }

	// 士気力の値を返す
	return m_pTensionGauge->GetNum();
}

//==========================================
//  チェックポイントでの回復処理
//==========================================
void CPlayer::RecoverCheckPoint()
{
	// 現在の士気力を取得する
	unsigned int nTension = GetTension();

	// 士気力ゲージが存在しなかった場合関数を抜ける
	if (nTension == -1) { return; }

	// 最大値と現在値の差を求める
	float fDiff = (float)(m_nMaxTension - nTension);

	// 差分の半分の値で士気力を回復する
	m_pTensionGauge->AddNum((int)(fDiff *= 0.5f));
}

//==========================================
//  ジャストアクションでの回復処理
//==========================================
void CPlayer::RecoverJust()
{
	// 士気力ゲージが存在しない場合
	if (m_pTensionGauge == nullptr) { return; }

	// 固定値で士気力を回復する
	m_pTensionGauge->AddNum(m_nRecover);
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
	CStage *pStage = CScene::GetStage();		// ステージ情報

	// 移動操作
	currentMotion = UpdateMove();

	// 重力の更新
	UpdateGravity();

	// 位置更新
	UpdatePosition(posPlayer, fDeltaTime);

	// 着地判定
	UpdateLanding(posPlayer);

	// 向き更新
	UpdateRotation(rotPlayer);

	// ステージ範囲外の補正
	pStage->LimitPosition(posPlayer, RADIUS);

	// 分身の処理
	ControlClone(posPlayer, rotPlayer);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// 保存位置の更新
	UpdateSaveTeleport();

	// TODO：エフェクト
	CEffect3D::Create(posPlayer, 10.0f);

	// 現在のモーションを返す
	return currentMotion;
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
	
	// KANAZAKI：忍び足とダッシュの判定書き換えて

	// スティックの傾きから移動量を設定
	float fSpeed = pPad->GetPressLStickTilt();	// スティックの傾き量
	if (pad::DEAD_ZONE < fSpeed)
	{ // デッドゾーン以上の場合

		if (fSpeed >= STEALTH_BORDER)
		{ // 通常速度の場合

			// 速度を通常にする
			fSpeed = NORMAL_MOVE;

			// 歩行モーションにする
			currentMotion = MOTION_DASH;
		}
		else
		{ // 忍び足の場合

			// 速度を忍び足にする
			fSpeed = STEALTH_MOVE;

			// 忍び足モーションにする
			currentMotion = MOTION_STEALTHWALK;
		}

		// スティック向きを取得
		float fStickRot = pPad->GetPressLStickRot() - (D3DX_PI * 0.5f);

		// 目標向きを設定
		m_destRot.y = fStickRot;
		useful::NormalizeRot(m_destRot.y);	// 向きの正規化

		// 移動量を設定する
		m_move.x = sinf(fStickRot + D3DX_PI) * fSpeed;
		m_move.z = cosf(fStickRot + D3DX_PI) * fSpeed;
	}

#ifdef _DEBUG

	// キーボードの移動操作
	DebugMoveControl();

	// ジャンプ操作
	DebugJumpControl();

#endif

	// 移動量をスカラー値に変換する
	m_fScalar = sqrtf(m_move.x * m_move.x + m_move.z * m_move.z);

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
	CInputKeyboard* pKey = GET_INPUTKEY;
	if (pKey->IsTrigger(DIK_Q))
	{
		// チェックポイントに帰る
		SaveReset();
	}
}

//============================================================
//	着地状況の更新処理
//============================================================
bool CPlayer::UpdateLanding(D3DXVECTOR3& rPos)
{
	bool bLand = false;	// 着地状況
	CStage *pStage = CScene::GetStage();	// ステージ情報

	// ジャンプしている状態にする
	m_bJump = true;

	// TODO：なんか変！

	// 地面・制限位置の着地判定
	if (pStage->LandFieldPosition(rPos, m_move)
	||  pStage->LandLimitPosition(rPos, m_move, 0.0f))
	{ // プレイヤーが着地していた場合

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		m_bJump = false;
	}

	// 着地状況を返す
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
	m_posCenter.y += m_fHeght * 0.5f;
}

//============================================================
//	向きの更新処理
//============================================================
void CPlayer::UpdateRotation(D3DXVECTOR3& rRot)
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
	rRot.y += fDiffRot * REV_ROTA;

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

	// 変数を宣言
	int nAnimMotion = GetMotionType();	// 現在再生中のモーション

	if (nMotion != NONE_IDX)
	{ // モーションが設定されている場合

		if (IsMotionLoop())
		{ // ループするモーションだった場合

			if (nAnimMotion != nMotion)
			{ // 現在のモーションが再生中のモーションと一致しない場合

				// 現在のモーションの設定
				SetMotion(nMotion, BLEND_FRAME);
			}
		}
	}

	// オブジェクトキャラクターの更新
	CObjectChara::Update(fDeltaTime);
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
//  分身の処理
//==========================================
void CPlayer::ControlClone(D3DXVECTOR3& rPos, D3DXVECTOR3& rRot)
{
	// TODO：ここも確認！

	// 入力情報の受け取り
	CInputPad* pPad = GET_INPUTPAD;

	// 追従分身の削除
	if (pPad->IsTrigger(CInputPad::KEY_RB))
	{
		// リストが存在しない場合に削除しない
		if (CPlayerClone::GetList() != nullptr)
		{
			CPlayerClone::Delete();
		}
	}

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

	// 分身の数が上限だった場合関数を抜ける
	if (CPlayerClone::GetList() != nullptr && CPlayerClone::GetList()->GetNumAll() >= m_nMaxClone) { return; }

	// 右スティックの入力がない場合関数を抜ける
	if (!pPad->GetTriggerRStick()) { return; }

#ifndef _DEBUG

	// 士気力が減少する
	m_pTensionGauge->AddNum(-500);

#endif

	// プレイヤーの方向を取得
	float fRotPlayer = GetVec3Rotation().y;

	// スティック入力の方向を取得する
	float fRotStick = pPad->GetPressRStickRot();

	// スティック方向を3D空間に対応する
	float fTemp = fRotStick - (D3DX_PI * 0.5f);
	useful::NormalizeRot(fTemp);

	// プレイヤー方向からスティックの方向を減算
	float fRot = fRotPlayer - fTemp;
	useful::NormalizeRot(fRot);

	// 求めた値とπの誤差が小さい場合ついてくる分身を出して関数を抜ける
	if (fabsf(fRot) >= D3DX_PI * 0.875f)
	{
		CPlayerClone::Create();
		GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_zitu_2.efkefc", rPos, rRot, VEC3_ZERO, 15.0f);
		return;
	}

	// 分身の移動量を算出する
	D3DXVECTOR3 move = D3DXVECTOR3
	(
		m_fScalar * cosf(-fRotStick),
		0.0f,
		m_fScalar * sinf(-fRotStick)
	);

	// 歩く分身を出す
	CPlayerClone::Create(move);
	GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_zitu_2.efkefc", rPos, rRot, VEC3_ZERO, 15.0f);
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

	// セーブした時点での士気力にする
	m_pTensionGauge->SetNum(m_pCheckPoint->GetSaveTension());
}

//==========================================
//  分身を呼び戻す
//==========================================
void CPlayer::CallClone()
{
	// パッドの入力情報を取得する
	CInputPad* pPad = GET_INPUTPAD;

	// 右スティックの押し込みがなかった場合関数を抜ける
	if (pPad->IsTrigger(CInputPad::KEY_RSTICKPUSH)) { return; }

	// 分身を追従する
	CPlayerClone::CallBack();
}

//==========================================
// アクターの当たり判定
//==========================================
void CPlayer::CollisionActor()
{
	// アクターのリスト構造が無ければ抜ける
	if (CActor::GetList() == nullptr) { return; }

	std::list<CActor*> list = CActor::GetList()->GetList();	// リストを取得
	D3DXVECTOR3 pos = GetVec3Position();	// 位置

	for (auto actor : list)
	{
		// 当たり判定処理
		actor->Collision
		(
			pos,		// 位置
			m_oldPos,	// 前回の位置
			RADIUS,		// 半径
			RADIUS,		// 高さ
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
		m_move.y = JUMP;

		// ジャンプ中にするよ
		m_bJump = true;
	}
}

//==========================================
// キーボードの移動操作処理
//==========================================
void CPlayer::DebugMoveControl(void)
{
	CInputKeyboard* pKey = GET_INPUTKEY;
	float fMoveRot = 0.0f;	// 移動方向

	if (pKey->IsPress(DIK_W))
	{
		if (pKey->IsPress(DIK_A))
		{
			// 移動方向を設定
			fMoveRot = (D3DX_PI * 0.75f);

			// 位置を設定
			m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
			m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;
		}
		else if (pKey->IsPress(DIK_D))
		{
			// 移動方向を設定
			fMoveRot = (D3DX_PI * -0.75f);

			// 位置を設定
			m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
			m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;
		}
		else
		{
			// 移動方向を設定
			fMoveRot = D3DX_PI;

			// 位置を設定
			m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
			m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;
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
			m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
			m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;
		}
		else if (pKey->IsPress(DIK_D))
		{
			// 移動方向を設定
			fMoveRot = (D3DX_PI * -0.25f);

			// 位置を設定
			m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
			m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;
		}
		else
		{
			// 移動方向を設定
			fMoveRot = 0.0f;

			// 位置を設定
			m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
			m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;
		}

		// 向きを設定
		m_destRot.y = fMoveRot;
	}
	else if (pKey->IsPress(DIK_A))
	{
		// 移動方向を設定
		fMoveRot = (D3DX_PI * 0.5f);

		// 位置を設定
		m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
		m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;

		// 向きを設定
		m_destRot.y = fMoveRot;
	}
	else if (pKey->IsPress(DIK_D))
	{
		// 移動方向を設定
		fMoveRot = (D3DX_PI * -0.5f);

		// 位置を設定
		m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
		m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;

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

	if (pKey->IsTrigger(DIK_I))
	{ // 前関係移動
		if (pKey->IsTrigger(DIK_J))
		{
			fStickRot = (D3DX_PI * 0.75f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// 歩く分身を出す
			CPlayerClone::Create(move);
		}
		else if (pKey->IsTrigger(DIK_L))
		{
			fStickRot = (D3DX_PI * -0.75f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// 歩く分身を出す
			CPlayerClone::Create(move);
		}
		else
		{
			fStickRot = D3DX_PI;

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// 歩く分身を出す
			CPlayerClone::Create(move);
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
			CPlayerClone::Create(move);
		}
		else if (pKey->IsTrigger(DIK_L))
		{
			fStickRot = (D3DX_PI * -0.25f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// 歩く分身を出す
			CPlayerClone::Create(move);
		}
		else
		{
			fStickRot = 0.0f;

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// 歩く分身を出す
			CPlayerClone::Create(move);
		}
	}
	else if (pKey->IsTrigger(DIK_J))
	{ // 左関係移動
		fStickRot = (D3DX_PI * 0.5f);

		move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
		move.y = 0.0f;
		move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

		// 歩く分身を出す
		CPlayerClone::Create(move);
	}
	else if (pKey->IsTrigger(DIK_L))
	{ // 右関係移動
		fStickRot = (D3DX_PI * -0.5f);

		move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
		move.y = 0.0f;
		move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

		// 歩く分身を出す
		CPlayerClone::Create(move);
	}
}

//==========================================
//  定数読み込み
//==========================================
void CPlayer::LoadParameter()
{
	//ローカル変数宣言
	FILE* pFile; // ファイルポインタ

	//ファイルを読み取り専用で開く
	pFile = fopen(PARAM_FILE, "r");

	// ファイルが開けなかった場合
	if (pFile == NULL) { assert(false); return; }

	// 情報の読み込み
	while (1)
	{
		// 文字列の記録用
		char aStr[256];

		// 文字列読み込み
		fscanf(pFile, "%s", &aStr[0]);

		// 条件分岐処理
		if (strcmp(&aStr[0], "TENSION_MAX") == 0) // 士気ゲージの最大値
		{
			// データを格納
			fscanf(pFile, "%d", &m_nMaxTension);
		}
		if (strcmp(&aStr[0], "TENSION_INIT") == 0) // 士気ゲージの初期値
		{
			// データを格納
			fscanf(pFile, "%d", &m_nInitTension);
		}
		if (strcmp(&aStr[0], "GAUGE_SPEED") == 0) // 士気ゲージの増減速度
		{
			// データを格納
			fscanf(pFile, "%d", &m_nSpeedTension);
		}
		if (strcmp(&aStr[0], "MAX_CLONE") == 0) // 一度に分身できる上限
		{
			// データを格納
			fscanf(pFile, "%d", &m_nMaxClone);
		}
		if (strcmp(&aStr[0], "JUST_RECOVER") == 0) // ジャストアクションでの回復量
		{
			// データを格納
			fscanf(pFile, "%d", &m_nRecover);
		}
		if (strcmp(&aStr[0], "HEIGHT") == 0) // 立幅の取得
		{
			// データを格納
			fscanf(pFile, "%f", &m_fHeght);
		}
		if (strcmp(&aStr[0], "INERTIAL") == 0) // 立幅の取得
		{
			// データを格納
			fscanf(pFile, "%f", &m_fInertial);
		}
		if (strcmp(&aStr[0], "CHARGE_TIME") == 0) // ため時間の取得
		{
			// データを格納
			fscanf(pFile, "%f", &m_fChargeTime);
		}
		if (strcmp(&aStr[0], "END_OF_FILE") == 0) // 読み込み終了
		{
			break;
		}
	}
}

#endif // _DEBUG
