//============================================================
//
//	カメラ処理 [camera.cpp]
//	Author：藤田勇一
//	Adder : 金崎朋弥
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "camera.h"
#include "manager.h"
#include "sceneGame.h"
#include "renderer.h"
#include "stage.h"
#include "player.h"
#include "multiModel.h"
#include "field.h"
#include "camera_change.h"
#include "goal.h"
#include "ZTexture.h"
#include "collision.h"
#include "retentionManager.h"
#include "gameManager.h"
#include "timerUI.h"
#include "cinemaScope.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	// カメラ基本情報
	namespace basic
	{
		const D3DXVECTOR3 INIT_VECU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 上方向ベクトルの初期値
		const D3DXVECTOR3 INIT_POSV = D3DXVECTOR3(0.0f, 300.0f, -600.0f);	// 視点の初期値
		const float VIEW_NEAR	= 10.0f;	// モデルが見えるZ軸の最小値
		const float VIEW_FAR	= 50000.0f;	// モデルが見えるZ軸の最大値
		const float MIN_DIS		= 1.0f;		// カメラの視点から注視点への距離の最小
		const float MAX_DIS		= 10000.0f;	// カメラの視点から注視点への距離の最大

		const float VIEW_ASPECT		= (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;	// アスペクト比
		const float VIEW_ANGLE		= D3DXToRadian(45.0f);	// 視野角
		const float LIMIT_ROT_HIGH	= D3DX_PI - 0.04f;		// X上回転の制限値
		const float LIMIT_ROT_LOW	= 0.04f;				// X下回転の制限値
	}

	// カメラ揺れ情報
	namespace swing
	{
		const float REV_DIS = 0.001f;	// カメラ揺れ計算時のカメラ距離の補正係数
	}

	// 回転カメラ情報
	namespace rotate
	{
		const D3DXVECTOR3 INIT_POSR	= D3DXVECTOR3(0.0f, 50.0f, 0.0f);	// 回転カメラの注視点の初期値
		const D3DXVECTOR2 INIT_ROT	= D3DXVECTOR2(1.4f, 0.0f);			// 回転カメラの向き初期値

		const float INIT_DIS	= 150.0f;	// 回転カメラの距離初期値
		const float ADD_ROTY	= 0.0025f;	// 回転カメラの向き加算量Y
	}

	// 追従カメラ情報
	namespace follow
	{
		const D3DXVECTOR3 REV_POSV = D3DXVECTOR3(0.4f, 0.45f, 0.4f);	// カメラ視点の補正係数
		const D3DXVECTOR3 REV_POSR = D3DXVECTOR3(1.0f, 0.35f, 1.0f);	// カメラ注視点の補正係数

		const float	STICK_REV	= 0.00000225f;	// カメラ操作スティックの傾き量の補正係数

		const float	ROTX_REV	= 0.5f;		// カメラピッチ回転の補正係数
		const float	REV_ROT		= 1.0f;		// カメラ向きの補正係数
		const float	INIT_DIS	= 550.0f;	// 追従カメラの距離
		const float	INIT_ROTX	= 1.8f;		// 追従カメラの向きX初期値

		const int	LOOK_BOSS_FRAME	= 18;				// 追従カメラのボス視認速度
		const float	LIMIT_ROT_HIGH	= D3DX_PI - 0.5f;	// X上回転の制限値
		const float	LIMIT_ROT_LOW	= 1.1f;				// X下回転の制限値
		const float	MAX_SUB_DIS		= 1500.0f;			// 下方向カメラの距離減算量
	}

	// 操作カメラ情報
	namespace control
	{
		const float REV_MOVE_MOUSE	= 1.0f;		// マウス操作でのカメラの移動の補正係数
		const float REV_DIS_MOUSE	= 0.2f;		// マウス操作でのカメラの距離の補正係数
		const float REV_ROT_MOUSE	= 0.0045f;	// マウス操作でのカメラの回転量の補正係数
	}

	// 三人称カメラ情報
	namespace tps
	{
		const D3DXVECTOR3 REV_POSV = D3DXVECTOR3(0.4f, 0.45f, 0.4f);	// カメラ視点の補正係数
		const D3DXVECTOR3 REV_POSR = D3DXVECTOR3(1.0f, 0.35f, 1.0f);	// カメラ注視点の補正係数

		const float	STICK_REV = 0.00000225f;	// カメラ操作スティックの傾き量の補正係数

		const float	ROTX_REV = 0.5f;		// カメラピッチ回転の補正係数
		const float	REV_ROT = 1.0f;			// カメラ向きの補正係数
		const float	INIT_DIS = 700.0f;		// 追従カメラの距離
		const float	INIT_HEIGHT = 1200.0f;	// 追従カメラの高さ
		const float	INIT_ROTX = 1.5f;		// 追従カメラの向きX初期値

		const int	LOOK_BOSS_FRAME = 18;				// 追従カメラのボス視認速度
		const float	LIMIT_ROT_HIGH = D3DX_PI - 0.5f;	// X上回転の制限値
		const float	LIMIT_ROT_LOW = 1.1f;				// X下回転の制限値
		const float	MAX_SUB_DIS = 1500.0f;				// 下方向カメラの距離減算量
	}

	// 回り込みカメラ情報
	namespace around
	{
		const D3DXVECTOR3 REV_POSV = D3DXVECTOR3(0.4f, 0.45f, 0.4f);	// カメラ視点の補正係数
		const D3DXVECTOR3 REV_POSR = D3DXVECTOR3(1.0f, 0.35f, 1.0f);	// カメラ注視点の補正係数

		const float	STICK_REV = 0.00000225f;	// カメラ操作スティックの傾き量の補正係数

		const float	ROTX_REV = 0.5f;		// カメラピッチ回転の補正係数
		const float	REV_ROT = 0.1f;			// カメラ向きの補正係数
		const float	INIT_DIS = 700.0f;		// 追従カメラの距離
		const float	INIT_HEIGHT = 1200.0f;	// 追従カメラの高さ
		const float	INIT_ROTX = 1.3f;		// 追従カメラの向きX初期値
		const float CENTER_ROTX = 1.1f;		// 中心線の時のx

		const float LIMIT_DIFF = D3DX_PI * 0.2f;		// 回転速度の上限
		const float	LIMIT_ROT_HIGH = D3DX_PI - 0.5f;	// X上回転の制限値
		const float	LIMIT_ROT_LOW = 1.1f;				// X下回転の制限値
		const float	MAX_SUB_DIS = 1500.0f;				// 下方向カメラの距離減算量
	}

	// 望遠カメラ情報
	namespace telephoto
	{
		const D3DXVECTOR3 REV_POSV = D3DXVECTOR3(0.4f, 0.45f, 0.4f);	// カメラ視点の補正係数
		const D3DXVECTOR3 REV_POSR = D3DXVECTOR3(1.0f, 0.35f, 1.0f);	// カメラ注視点の補正係数
		const float	REV_ROT = 0.1f;			// カメラ向きの補正係数
		const float	INIT_DIS = 700.0f;		// 追従カメラの距離
		const float	INIT_HEIGHT = 1200.0f;	// 追従カメラの高さ
		const float	INIT_ROTX = 1.5f;		// 追従カメラの向きX初期値
		const float	MAX_SUB_DIS = 1500.0f;	// 下方向カメラの距離減算量
		const float VIEW_ANGLE = D3DXToRadian(70.0f);	// 視野角
	}

	// 神器獲得カメラ情報
	namespace god
	{
		const float	INIT_DIS	= 300.0f;	// 追従カメラの距離
		const float	INIT_ROTX	= HALF_PI;	// 追従カメラの向きX初期値
		const float	PLUS_ROTY	= 0.0f;		// プレイヤーヨー向きに加算する量
		const float	PLUS_UP		= 80.0f;	// プレイヤー縦位置に加算する量
		const float	REV_DIFF	= 0.04f;	// 差分の補正係数
	}

	// リザルトカメラ情報
	namespace result
	{
		const float	PLUS_SIDE[] =	// プレイヤー横位置に加算する
		{
			85.0f,	// クリア不明
			115.0f,	// クリア失敗
			85.0f,	// クリア成功
		};
		const float	PLUS_UP[] =	// プレイヤー縦位置に加算する
		{
			60.0f,	// クリア不明
			40.0f,	// クリア失敗
			60.0f,	// クリア成功
		};

		const float	INIT_DIS	= 200.0f;	// 追従カメラの距離
		const float	INIT_ROTX	= HALF_PI;	// 追従カメラの向きX初期値
		const float	PLUS_ROTY	= 0.28f;	// プレイヤーヨー向きに加算する量
		const float REV_DIFF	= 0.035f;	// 差分の補正係数
	}

	// スタートカメラ情報
	namespace start
	{
		const float INIT_POSR_HEIGHT = 40.0f;							// 注視点の初期高度
		const D3DXVECTOR3 LAND_ROT = D3DXVECTOR3(1.3f, D3DX_PI * -0.15f, 0.0f);	// 着地待ち状態の向き
		const float DISTANCE[CCamera::SStart::STATE_MAX] =				// 距離
		{
			200.0f,				// 着地待ち状態
			400.0f,				// 回り込み状態
			around::INIT_DIS	// 戻り状態
		};
		const float REV_DIFF = 0.035f;		// 差分の補正係数
		const int LAND_COUNT = 50;			// 着地状態のカウント数
		const int START_MOTION_BLEND = 15;	// スタートモーションのモーションブレンド数
		const D3DXVECTOR3 ROUND_ROT = D3DXVECTOR3(1.6f, D3DX_PI * 0.4f, 0.0f);	// 回り込み状態の向き
		const D3DXVECTOR3 ROUND_ROT_MOVE = D3DXVECTOR3(0.01f, D3DX_PI * 0.004f, 0.0f);	// 回り込み状態の向きの移動量
		const D3DXVECTOR3 REV_POSV = D3DXVECTOR3(0.4f, 0.45f, 0.4f);			// カメラ視点の補正係数
		const int ROUND_COUNT = 80;			// 周り込み状態の維持カウント数
		const float BACK_REV_ROT = 0.05f;	// 戻り状態の向きの補正係数
		const int BACK_COUNT = 140;			// 戻り状態のカウント数
		const float ENTRY_GAME_SHIFT = -63.30f;		// ゲームに入る時にずらす距離
	}
}

//************************************************************
//	親クラス [CCamera] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CCamera::CCamera() :
	m_state		(STATE_NONE),	// 状態
	m_bUpdate	(false),		// 更新状況
	m_fFov		(0.0f)			// 視野角
{
	// メンバ変数をクリア
	memset(&m_aCamera[0], 0, sizeof(m_aCamera));	// カメラの情報
}

//============================================================
//	デストラクタ
//============================================================
CCamera::~CCamera()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CCamera::Init(void)
{
	//--------------------------------------------------------
	//	メンバ変数を初期化
	//--------------------------------------------------------
	memset(&m_aCamera[0], 0, sizeof(m_aCamera));	// カメラの情報
	memset(&m_startInfo, 0, sizeof(m_startInfo));	// スタートカメラの情報
	m_state		= STATE_AROUND;	// 状態
	m_bUpdate	= true;			// 更新状況
	m_fFov = basic::VIEW_ANGLE;

	//--------------------------------------------------------
	//	メインカメラの初期化
	//--------------------------------------------------------
	// カメラ情報を初期化
	m_aCamera[TYPE_MAIN].posV		= VEC3_ZERO;		// 現在の視点
	m_aCamera[TYPE_MAIN].posR		= VEC3_ZERO;		// 現在の注視点
	m_aCamera[TYPE_MAIN].destPosV	= VEC3_ZERO;		// 目標の視点
	m_aCamera[TYPE_MAIN].destPosR	= VEC3_ZERO;		// 目標の注視点
	m_aCamera[TYPE_MAIN].vecU		= basic::INIT_VECU;	// 上方向ベクトル
	m_aCamera[TYPE_MAIN].rot		= VEC3_ZERO;		// 現在の向き
	m_aCamera[TYPE_MAIN].destRot	= VEC3_ZERO;		// 目標の向き
	m_aCamera[TYPE_MAIN].fDis		= 0.0f;				// 現在の視点と注視点の距離
	m_aCamera[TYPE_MAIN].fDestDis	= 0.0f;				// 目標の視点と注視点の距離

	// カメラ揺れ情報を初期化
	m_aCamera[TYPE_MAIN].swingInfo.shiftPos		= VEC3_ZERO;	// 位置ずれ量
	m_aCamera[TYPE_MAIN].swingInfo.fShiftAngle	= 0.0f;			// 位置をずらす角度
	m_aCamera[TYPE_MAIN].swingInfo.fShiftLength	= 0.0f;			// 位置をずらす距離
	m_aCamera[TYPE_MAIN].swingInfo.fSubAngle	= 0.0f;			// ずらす角度の減算量
	m_aCamera[TYPE_MAIN].swingInfo.fSubLength	= 0.0f;			// ずらす距離の減算量

	// ビューポート情報を初期化
	m_aCamera[TYPE_MAIN].viewportInfo.X			= 0;				// 左上隅のピクセル座標X
	m_aCamera[TYPE_MAIN].viewportInfo.Y			= 0;				// 左上隅のピクセル座標Y
	m_aCamera[TYPE_MAIN].viewportInfo.Width		= SCREEN_WIDTH;		// 描画する画面の横幅
	m_aCamera[TYPE_MAIN].viewportInfo.Height	= SCREEN_HEIGHT;	// 描画する画面の縦幅
	m_aCamera[TYPE_MAIN].viewportInfo.MinZ		= 0.0f;
	m_aCamera[TYPE_MAIN].viewportInfo.MaxZ		= 1.0f;

	//--------------------------------------------------------
	//	モデルUI表示カメラの初期化
	//--------------------------------------------------------
	// カメラ情報を初期化
	m_aCamera[TYPE_MODELUI].posV		= basic::INIT_POSV;	// 現在の視点
	m_aCamera[TYPE_MODELUI].posR		= VEC3_ZERO;		// 現在の注視点
	m_aCamera[TYPE_MODELUI].destPosV	= basic::INIT_POSV;	// 目標の視点
	m_aCamera[TYPE_MODELUI].destPosR	= VEC3_ZERO;		// 目標の注視点
	m_aCamera[TYPE_MODELUI].vecU		= basic::INIT_VECU;	// 上方向ベクトル
	m_aCamera[TYPE_MODELUI].rot			= VEC3_ZERO;		// 現在の向き
	m_aCamera[TYPE_MODELUI].destRot		= VEC3_ZERO;		// 目標の向き
	m_aCamera[TYPE_MODELUI].fDis		= 0.0f;				// 現在の視点と注視点の距離
	m_aCamera[TYPE_MODELUI].fDestDis	= 0.0f;				// 目標の視点と注視点の距離

	// カメラ揺れ情報を初期化
	m_aCamera[TYPE_MODELUI].swingInfo.shiftPos		= VEC3_ZERO;	// 位置ずれ量
	m_aCamera[TYPE_MODELUI].swingInfo.fShiftAngle	= 0.0f;			// 位置をずらす角度
	m_aCamera[TYPE_MODELUI].swingInfo.fShiftLength	= 0.0f;			// 位置をずらす距離
	m_aCamera[TYPE_MODELUI].swingInfo.fSubAngle		= 0.0f;			// ずらす角度の減算量
	m_aCamera[TYPE_MODELUI].swingInfo.fSubLength	= 0.0f;			// ずらす距離の減算量

	// ビューポート情報を初期化
	m_aCamera[TYPE_MODELUI].viewportInfo.X		= 0;				// 左上隅のピクセル座標X
	m_aCamera[TYPE_MODELUI].viewportInfo.Y		= 0;				// 左上隅のピクセル座標Y
	m_aCamera[TYPE_MODELUI].viewportInfo.Width	= SCREEN_WIDTH;		// 描画する画面の横幅
	m_aCamera[TYPE_MODELUI].viewportInfo.Height	= SCREEN_HEIGHT;	// 描画する画面の縦幅
	m_aCamera[TYPE_MODELUI].viewportInfo.MinZ	= 0.0f;
	m_aCamera[TYPE_MODELUI].viewportInfo.MaxZ	= 0.5f;

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CCamera::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CCamera::Update(const float fDeltaTime)
{
	// デバッグ表示
	DebugProc::Print(DebugProc::POINT_LEFT, "[ 視点 ]：%f %f %f\n", m_aCamera[TYPE_MAIN].posV.x, m_aCamera[TYPE_MAIN].posV.y, m_aCamera[TYPE_MAIN].posV.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[注視点]：%f %f %f\n", m_aCamera[TYPE_MAIN].posR.x, m_aCamera[TYPE_MAIN].posR.y, m_aCamera[TYPE_MAIN].posR.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[ 向き ]：%f %f %f\n", m_aCamera[TYPE_MAIN].rot.x, m_aCamera[TYPE_MAIN].rot.y, m_aCamera[TYPE_MAIN].rot.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[ 距離 ]：%f\n", m_aCamera[TYPE_MAIN].fDis);
	DebugProc::Print(DebugProc::POINT_LEFT, "[ 状態 ]：%d\n", m_state);

	// 更新を止めている場合抜ける
	if (!m_bUpdate) { return; }

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME &&
		CSceneGame::GetGameManager()->GetState() == CGameManager::EState::STATE_START)
	{ // ゲームが始まった時のみ

		// カメラ処理
		StartCamera();

		// この先の処理を行わない
		return;
	}

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// なにもしない状態
		break;

	case STATE_CONTROL:	// 操作状態

		// カメラの更新 (操作)
		Control();
		break;

	case STATE_ROTATE:	// 回転状態

		// カメラの更新 (回転)
		Rotate();
		break;

	case STATE_FOLLOW:	// 追従状態

		// カメラの更新 (追従)
		Follow();
		break;

	case STATE_AROUND:	// 回り込み

		// 望遠の更新
		Around();
		break;

	case STATE_TELEPHOTO:	// 望遠

		// 望遠の更新
		Telephoto();
		break;

	case STATE_GODITEM:	// 神器獲得

		// 神器獲得の更新
		GodItem();
		break;

	case STATE_RESULT:	// リザルト

		// リザルトの更新
		Result();
		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// カメラ揺れの更新
	Swing();
}

//============================================================
//	再設定処理
//============================================================
void CCamera::Reset(void)
{
	for (int nCntCamera = 0; nCntCamera < TYPE_MAX; nCntCamera++)
	{ // カメラの総数分繰り返す

		// カメラ揺れ情報を初期化
		SSwing *pSwing = &m_aCamera[nCntCamera].swingInfo;
		pSwing->shiftPos	 = VEC3_ZERO;	// 位置ずれ量
		pSwing->fShiftAngle	 = 0.0f;		// 位置をずらす角度
		pSwing->fShiftLength = 0.0f;		// 位置をずらす距離
		pSwing->fSubAngle	 = 0.0f;		// ずらす角度の減算量
		pSwing->fSubLength	 = 0.0f;		// ずらす距離の減算量
	}
}

//============================================================
//	カメラ設定処理
//============================================================
void CCamera::SetCamera(const EType type)
{
	if (type <= NONE_IDX || type >= TYPE_MAX) { assert(false); return; }	// 種類範囲外

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// ビューポートの設定
	pDevice->SetViewport(&m_aCamera[type].viewportInfo);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_aCamera[type].mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH
	( // 引数
		&m_aCamera[type].mtxProjection,	// プロジェクションマトリックス
		m_fFov,	// 視野角
		basic::VIEW_ASPECT,	// 画面のアスペクト比
		basic::VIEW_NEAR,	// Z軸の最小値
		basic::VIEW_FAR		// Z軸の最大値
	);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_aCamera[type].mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_aCamera[type].mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH
	( // 引数
		&m_aCamera[type].mtxView,	// ビューマトリックス
		&m_aCamera[type].posV,		// 視点
		&m_aCamera[type].posR,		// 注視点
		&m_aCamera[type].vecU		// 上方向ベクトル
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_aCamera[type].mtxView);
}

//============================================================
//	カメラ目標位置の設定処理 (回転)
//============================================================
void CCamera::SetDestRotate(void)
{
	if (m_state != STATE_ROTATE) { return; }	// カメラ回転状態以外

	//--------------------------------------------------------
	//	向きの更新
	//--------------------------------------------------------
	// 現在向きの更新
	m_aCamera[TYPE_MAIN].rot.x = m_aCamera[TYPE_MAIN].destRot.x = rotate::INIT_ROT.x;
	m_aCamera[TYPE_MAIN].rot.y = m_aCamera[TYPE_MAIN].destRot.y = rotate::INIT_ROT.y;
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);		// 現在向きを正規化
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot);	// 目標向きを正規化

	//--------------------------------------------------------
	//	距離の更新
	//--------------------------------------------------------
	m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis = rotate::INIT_DIS;

	//--------------------------------------------------------
	//	位置の更新
	//--------------------------------------------------------
	// 注視点の更新
	m_aCamera[TYPE_MAIN].posR = m_aCamera[TYPE_MAIN].destPosR = rotate::INIT_POSR;

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].posR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].posR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].posR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 視野角を設定
	m_fFov = basic::VIEW_ANGLE;
}

//============================================================
//	カメラの目標位置の設定処理 (追従)
//============================================================
void CCamera::SetDestFollow(void)
{
	CListManager<CPlayer> *pList = CPlayer::GetList();	// プレイヤーリスト
	if (pList == nullptr)		 { return; }	// リスト未使用
	if (pList->GetNumAll() != 1) { return; }	// プレイヤーが1人じゃない
	if (m_state != STATE_FOLLOW) { return; }	// カメラ追従状態以外

	auto player = pList->GetList().front();	// プレイヤー情報
	D3DXVECTOR3 diffPosV = VEC3_ZERO;		// 視点の差分位置
	D3DXVECTOR3 diffPosR = VEC3_ZERO;		// 注視点の差分位置
	D3DXVECTOR3 diffRot  = VEC3_ZERO;		// 差分向き

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	// 向きを設定
	m_aCamera[TYPE_MAIN].rot.x = m_aCamera[TYPE_MAIN].destRot.x = follow::INIT_ROTX;
	m_aCamera[TYPE_MAIN].rot.y = m_aCamera[TYPE_MAIN].destRot.y = player->GetVec3Rotation().y + D3DX_PI;

	// 向きを正規化
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot);

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 目標距離を設定
	m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis = follow::INIT_DIS;
	if (m_aCamera[TYPE_MAIN].rot.x < HALF_PI)
	{ // 下から向き始めた場合

		// 地面を貫通しないよう補正
		m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis -= (follow::MAX_SUB_DIS / HALF_PI) * (HALF_PI - m_aCamera[TYPE_MAIN].rot.x);
	}

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// 注視点をプレイヤーの頭の位置にする
	m_aCamera[TYPE_MAIN].posR = m_aCamera[TYPE_MAIN].destPosR = player->GetVec3Position() + D3DXVECTOR3(0.0f, player->GetHeight(), 0.0f);

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].destPosR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].destPosR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].destPosR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 視野角を設定
	m_fFov = basic::VIEW_ANGLE;
}

//===========================================
//  カメラ目標位置設定(回り込み)
//===========================================
void CCamera::SetDestAround(void)
{
	// プレイヤー情報の取得
	CPlayer* pPlayer = GET_PLAYER;

	// プレイヤーがいない場合抜ける
	if (pPlayer == nullptr) { return; }

	// プレイヤーの座標を取得
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();

	// 目標の角度を算出
	CalcAround(posPlayer);

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	// 向きを設定
	m_aCamera[TYPE_MAIN].rot.x = m_aCamera[TYPE_MAIN].destRot.x;
	m_aCamera[TYPE_MAIN].rot.y = m_aCamera[TYPE_MAIN].destRot.y;

	// 向きを正規化
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot);

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 距離を設定
	m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis = around::INIT_DIS;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// 注視点をプレイヤーの頭の位置にする
	m_aCamera[TYPE_MAIN].posR = m_aCamera[TYPE_MAIN].destPosR = posPlayer + D3DXVECTOR3(0.0f, pPlayer->GetHeight(), 0.0f);

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].destPosR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].destPosR.y + ((around::INIT_HEIGHT * cosf(m_aCamera[TYPE_MAIN].rot.x)));	// TODO：Yの距離だけ定数はきもすぎる。後マイナスにしてないのもやばい。
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].destPosR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 視野角を設定
	m_fFov = basic::VIEW_ANGLE;
}

//===========================================
//  カメラ目標位置設定(望遠)
//===========================================
void CCamera::SetDestTelephoto(void)
{
	CListManager<CPlayer>* pList = CPlayer::GetList();	// プレイヤーリスト
	if (pList == nullptr) { return; }	// リスト未使用
	if (pList->GetNumAll() != 1) { return; }	// プレイヤーが1人じゃない
	if (m_state != STATE_AROUND) { return; }	// 回り込み状態以外

	auto player = pList->GetList().front();	// プレイヤー情報

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	// 向きを設定
	m_aCamera[TYPE_MAIN].rot.x = m_aCamera[TYPE_MAIN].destRot.x = telephoto::INIT_ROTX;
	m_aCamera[TYPE_MAIN].rot.y = m_aCamera[TYPE_MAIN].destRot.y = player->GetVec3Rotation().y;

	// 向きを正規化
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot);

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 目標距離を設定
	m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis = telephoto::INIT_DIS;
	if (m_aCamera[TYPE_MAIN].rot.x < HALF_PI)
	{ // 下から向き始めた場合

		// 地面を貫通しないよう補正
		m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis -= (telephoto::MAX_SUB_DIS / HALF_PI) * (HALF_PI - m_aCamera[TYPE_MAIN].rot.x);
	}

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// 注視点をプレイヤーの頭の位置にする
	m_aCamera[TYPE_MAIN].posR = m_aCamera[TYPE_MAIN].destPosR = player->GetVec3Position() + D3DXVECTOR3(0.0f, player->GetHeight(), 0.0f);

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].destPosR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].destPosR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].destPosR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 視野角を設定
	m_fFov = basic::VIEW_ANGLE;
}

//============================================================
//	カメラ目標位置の設定処理 (神器獲得)
//============================================================
void CCamera::SetDestGodItem(void)
{
	// カメラが神器獲得状態以外なら抜ける
	if (m_state != STATE_GODITEM) { return; }

	// プレイヤーの情報を取得
	CPlayer* pPlayer = GET_PLAYER;	// プレイヤー情報
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = pPlayer->GetDestRotation();	// プレイヤー目標向き

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	// 向きを設定
	m_aCamera[TYPE_MAIN].destRot.x = m_aCamera[TYPE_MAIN].rot.x = god::INIT_ROTX;
	m_aCamera[TYPE_MAIN].destRot.y = m_aCamera[TYPE_MAIN].rot.y = rotPlayer.y + god::PLUS_ROTY;
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);		// 現在向きを正規化
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot);	// 目標向きを正規化

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 距離を設定
	m_aCamera[TYPE_MAIN].fDestDis = m_aCamera[TYPE_MAIN].fDis = god::INIT_DIS;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// 注視点の目標位置を計算
	m_aCamera[TYPE_MAIN].destPosR = posPlayer;			// プレイヤー位置を基準点にする
	m_aCamera[TYPE_MAIN].destPosR.y += god::PLUS_UP;	// プレイヤーの縦方向にずらす

	// 目標位置をそのまま現在位置にも設定
	m_aCamera[TYPE_MAIN].posR = m_aCamera[TYPE_MAIN].destPosR;

	// 視点位置を設定
	m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].posR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].posR.y + ((m_aCamera[TYPE_MAIN]. fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));	// TODO：yだけ+なのきもすぎ
	m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].posR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 視野角を設定
	m_fFov = basic::VIEW_ANGLE;
}

//============================================================
//	カメラの目標位置の設定処理 (リザルト)
//============================================================
void CCamera::SetDestResult(void)
{
	// カメラがリザルト状態以外なら抜ける
	if (m_state != STATE_RESULT) { return; }

	// プレイヤーの情報を取得
	CPlayer* pPlayer = GET_PLAYER;	// プレイヤー情報
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = pPlayer->GetDestRotation();	// プレイヤー目標向き

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	// 向きを設定
	m_aCamera[TYPE_MAIN].destRot.x = m_aCamera[TYPE_MAIN].rot.x = result::INIT_ROTX;
	m_aCamera[TYPE_MAIN].destRot.y = m_aCamera[TYPE_MAIN].rot.y = rotPlayer.y + result::PLUS_ROTY;
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);		// 現在向きを正規化
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot);	// 目標向きを正規化

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 距離を設定
	m_aCamera[TYPE_MAIN].fDestDis = m_aCamera[TYPE_MAIN].fDis = result::INIT_DIS;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// クリア状況を取得
	CRetentionManager::EWin win = GET_RETENTION->GetWin();

	// 注視点の目標位置を計算
	m_aCamera[TYPE_MAIN].destPosR = posPlayer;	// プレイヤー位置を基準点にする
	m_aCamera[TYPE_MAIN].destPosR.y += result::PLUS_UP[win];	// プレイヤーの縦方向にずらす
	m_aCamera[TYPE_MAIN].destPosR.x += sinf(rotPlayer.y + HALF_PI) * result::PLUS_SIDE[win];	// プレイヤー向き基準の横方向にずらす
	m_aCamera[TYPE_MAIN].destPosR.z += cosf(rotPlayer.y + HALF_PI) * result::PLUS_SIDE[win];	// プレイヤー向き基準の横方向にずらす

	// 目標位置をそのまま現在位置にも設定
	m_aCamera[TYPE_MAIN].posR = m_aCamera[TYPE_MAIN].destPosR;

	// 視点位置を設定
	m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].posR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].posR.y + ((m_aCamera[TYPE_MAIN]. fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));	// TODO：yだけ+なのきもすぎ
	m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].posR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 視野角を設定
	m_fFov = basic::VIEW_ANGLE;
}

//============================================================
//	視点の設定処理
//============================================================
void CCamera::SetPositionV(const D3DXVECTOR3& rPosV)
{
	SCamera *pCamera = &m_aCamera[TYPE_MAIN];	// メインカメラ情報

	// 視点・目標視点を設定
	pCamera->posV = pCamera->destPosV = rPosV;

	// 注視点・目標注視点の更新
	pCamera->posR.x = pCamera->destPosR.x = pCamera->posV.x + ((pCamera->fDis * sinf(pCamera->rot.x)) * sinf(pCamera->rot.y));
	pCamera->posR.y = pCamera->destPosR.y = pCamera->posV.y + ((pCamera->fDis * cosf(pCamera->rot.x)));
	pCamera->posR.z = pCamera->destPosR.z = pCamera->posV.z + ((pCamera->fDis * sinf(pCamera->rot.x)) * cosf(pCamera->rot.y));
}

//============================================================
//	注視点の設定処理
//============================================================
void CCamera::SetPositionR(const D3DXVECTOR3& rPosR)
{
	SCamera *pCamera = &m_aCamera[TYPE_MAIN];	// メインカメラ情報

	// 注視点・目標注視点を設定
	pCamera->posR = pCamera->destPosR = rPosR;

	// 視点・目標視点の更新
	pCamera->posV.x = pCamera->destPosV.x = pCamera->posR.x + ((-pCamera->fDis * sinf(pCamera->rot.x)) * sinf(pCamera->rot.y));
	pCamera->posV.y = pCamera->destPosV.y = pCamera->posR.y + ((-pCamera->fDis * cosf(pCamera->rot.x)));
	pCamera->posV.z = pCamera->destPosV.z = pCamera->posR.z + ((-pCamera->fDis * sinf(pCamera->rot.x)) * cosf(pCamera->rot.y));
}

//============================================================
//	向きの設定処理
//============================================================
void CCamera::SetRotation(const D3DXVECTOR3& rRot)
{
	SCamera *pCamera = &m_aCamera[TYPE_MAIN];	// メインカメラ情報

	// 向き・目標向きを設定
	pCamera->rot = pCamera->destRot = rRot;

	// 視点・目標視点の更新
	pCamera->posV.x = pCamera->destPosV.x = pCamera->posR.x + ((-pCamera->fDis * sinf(pCamera->rot.x)) * sinf(pCamera->rot.y));
	pCamera->posV.y = pCamera->destPosV.y = pCamera->posR.y + ((-pCamera->fDis * cosf(pCamera->rot.x)));
	pCamera->posV.z = pCamera->destPosV.z = pCamera->posR.z + ((-pCamera->fDis * sinf(pCamera->rot.x)) * cosf(pCamera->rot.y));
}

//============================================================
//	距離の設定処理
//============================================================
void CCamera::SetDistance(const float fDis)
{
	SCamera *pCamera = &m_aCamera[TYPE_MAIN];	// メインカメラ情報

	// 距離・目標距離を設定
	pCamera->fDis = pCamera->fDestDis = fDis;

	// 視点・目標視点の更新
	pCamera->posV.x = pCamera->destPosV.x = pCamera->posR.x + ((-pCamera->fDis * sinf(pCamera->rot.x)) * sinf(pCamera->rot.y));
	pCamera->posV.y = pCamera->destPosV.y = pCamera->posR.y + ((-pCamera->fDis * cosf(pCamera->rot.x)));
	pCamera->posV.z = pCamera->destPosV.z = pCamera->posR.z + ((-pCamera->fDis * sinf(pCamera->rot.x)) * cosf(pCamera->rot.y));
}

//============================================================
//	カメラ取得処理
//============================================================
CCamera::SCamera CCamera::GetCamera(const EType type)
{
	// カメラの情報を返す
	return m_aCamera[type];
}

//============================================================
//	カメラ状態の設定処理
//============================================================
void CCamera::SetState(const EState state)
{
	// 状態を設定
	m_state = state;

	// 再設定
	Reset();
}

//============================================================
//	カメラ状態取得処理
//============================================================
CCamera::EState CCamera::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	カメラ揺れの設定処理
//============================================================
void CCamera::SetSwing(const EType type, const SSwing swing)
{
	if (m_state == STATE_CONTROL) { return; }	// カメラ操作状態

	if (type > NONE_IDX && type < TYPE_MAX)
	{ // 種類が範囲内の場合

		// 引数のカメラ揺れ情報を設定
		m_aCamera[type].swingInfo = swing;
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	更新状況の設定処理
//============================================================
void CCamera::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	m_bUpdate = bUpdate;
}

//==========================================
//  プレイヤー座標を二次元座標に変換する
//==========================================
D3DXVECTOR3 CCamera::CalcPlayerPos()
{
	// プレイヤー座標を取得
	D3DXVECTOR3 posPlayer = GET_PLAYER->GetCenterPos();

	//ビューポートの設定
	D3DVIEWPORT9 vp = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };

	//計算用変数宣言
	D3DXMATRIX mtxWorld; //ワールドマトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//スクリーン座標を算出
	D3DXVECTOR3 screenPos;
	D3DXVec3Project
	(
		&screenPos,
		&posPlayer,
		&vp,
		&m_aCamera[TYPE_MAIN].mtxProjection,
		&m_aCamera[TYPE_MAIN].mtxView,
		&mtxWorld
	);

	// スクリーン座標を返す
	return screenPos;
}

//===========================================
//  ワールド座標をスクリーン座標に変換する
//=========================================
D3DXVECTOR3 CCamera::CalcWorldToScreen(const D3DXVECTOR3& pos)
{
	//ビューポートの設定
	D3DVIEWPORT9 vp = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };

	//計算用変数宣言
	D3DXMATRIX mtxWorld; //ワールドマトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//スクリーン座標を算出
	D3DXVECTOR3 screenPos;
	D3DXVec3Project
	(
		&screenPos,
		&pos,
		&vp,
		&m_aCamera[TYPE_MAIN].mtxProjection,
		&m_aCamera[TYPE_MAIN].mtxView,
		&mtxWorld
	);

	// スクリーン座標を返す
	return screenPos;
}

//===========================================
//  スクリーン内判定
//===========================================
bool CCamera::OnScreen(const D3DXVECTOR3& pos)
{
	//ビューポートの設定
	D3DVIEWPORT9 vp = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };

	//計算用変数宣言
	D3DXMATRIX mtxWorld; //ワールドマトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//スクリーン座標を算出
	D3DXVECTOR3 screenPos;
	D3DXVec3Project
	(
		&screenPos,
		&pos,
		&vp,
		&m_aCamera[TYPE_MAIN].mtxProjection,
		&m_aCamera[TYPE_MAIN].mtxView,
		&mtxWorld
	);

	// カメラの裏側にいるときfalseを返す
	if (screenPos.z >= 1.0f) { return false; }

	// xの判定でfalseを返す
	if (screenPos.x < 0.0f || screenPos.x > SCREEN_WIDTH) { return false; }

	// yの判定でfalseを返す
	if (screenPos.y < 0.0f || screenPos.y > SCREEN_HEIGHT) { return false; }

	// ここまで来れたらtrueを返す
	return true;
}

//===========================================
//  スクリーン座標を返すスクリーン内判定
//===========================================
bool CCamera::OnScreen(const D3DXVECTOR3& pos, D3DXVECTOR3& posOut)
{
	//ビューポートの設定
	D3DVIEWPORT9 vp = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };

	//計算用変数宣言
	D3DXMATRIX mtxWorld; //ワールドマトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//スクリーン座標を算出
	D3DXVec3Project
	(
		&posOut,
		&pos,
		&vp,
		&m_aCamera[TYPE_MAIN].mtxProjection,
		&m_aCamera[TYPE_MAIN].mtxView,
		&mtxWorld
	);

	// カメラの裏側にいるときfalseを返す
	if (posOut.z >= 1.0f) { return false; }

	// xの判定でfalseを返す
	if (posOut.x < 0.0f || posOut.x > SCREEN_WIDTH) { return false; }

	// yの判定でfalseを返す
	if (posOut.y < 0.0f || posOut.y > SCREEN_HEIGHT) { return false; }

	// ここまで来れたらtrueを返す
	return true;
}

//===========================================
//  モデルのスクリーン内判定
//===========================================
bool CCamera::OnScreenBox(const D3DXVECTOR3& posMin, const D3DXVECTOR3& posMax)
{
	// 視点座標と方向を取得する
	D3DXVECTOR3 pos = this->GetDestPositionV();
	float fRot = this->GetRotation().y + D3DX_PI;

	// 方向を適用して視野角の向きを求める
	float fRotFov[2] =
	{
		fRot - m_fFov,
		fRot + m_fFov
	};
	useful::NormalizeRot(fRotFov[0]);
	useful::NormalizeRot(fRotFov[1]);

	// 視点から引数までのベクトルを算出
	D3DXVECTOR3 vecToPos[2] =
	{
		posMin - pos,
		posMax - pos
	};

	// 算出した向きから描画範囲を求める
	D3DXVECTOR3 vecToFar[2];
	for (int i = 0; i < 2; ++i)
	{
		vecToFar[i] = D3DXVECTOR3(basic::VIEW_FAR * cosf(fRotFov[i]), 0.0f, basic::VIEW_FAR * sinf(fRotFov[i])) 
					- D3DXVECTOR3(basic::VIEW_NEAR * cosf(fRotFov[i]), 0.0f, basic::VIEW_NEAR * sinf(fRotFov[i]));
	}

	// 外積の計算を行う
	float fCloss[4];
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			fCloss[i + j] = vecToPos[i].x * vecToFar[j].z - vecToPos[i].z * vecToFar[j].x;
		}
	}

	// 最小と最大のどちらも死角の場合falseを返す
	if (fCloss[0] * fCloss[1] > 0.0f && fCloss[2] * fCloss[3] > 0.0f) { return false; }

	return true;
}

//==========================================
//  ポリゴンのスクリーン内判定
//==========================================
bool CCamera::OnScreenPolygon(const D3DXVECTOR3& posMin, const D3DXVECTOR3& posMax)
{
	return false;
}

//===========================================
// スタート状態のリセット処理
//===========================================
void CCamera::StartReset(void)
{
	// 値をリセットする
	m_startInfo.nCount = 0;			// カウント
	m_startInfo.state = SStart::STATE_LAND;	// 状態
}

//===========================================
// スタート演出時のカメラ
//===========================================
void CCamera::StartCamera(void)
{
	CListManager<CPlayer>* pList = CPlayer::GetList();	// プレイヤーリスト
	if (pList == nullptr) { return; }	// リスト未使用
	if (pList->GetNumAll() != 1) { return; }	// プレイヤーが1人じゃない

	auto player = pList->GetList().front();	// プレイヤー情報

	// プレイヤーが NULL じゃない場合、抜ける
	if (player == nullptr) { return; }

	// 距離を設定する
	m_aCamera[TYPE_MAIN].fDestDis = start::DISTANCE[m_startInfo.state];

	// 差分距離を計算
	float fDiffDis = m_aCamera[TYPE_MAIN].fDestDis - m_aCamera[TYPE_MAIN].fDis;

	// 現在距離を計算
	m_aCamera[TYPE_MAIN].fDis += fDiffDis * start::REV_DIFF;

	if (GET_INPUTPAD->IsTrigger(CInputPad::EKey::KEY_A) ||
		GET_INPUTKEY->IsTrigger(DIK_RETURN))
	{ // スキップキーを押した場合

		// スコープアウト
		CSceneGame::GetCinemaScope()->SetScopeOut();

		// プレイヤーの位置を設定する(空中から始まらないように)
		D3DXVECTOR3 posPlayer = player->GetVec3Position();
		posPlayer.x += sinf(player->GetVec3Rotation().y) * start::ENTRY_GAME_SHIFT;
		posPlayer.y = 0.0f;
		posPlayer.z += cosf(player->GetVec3Rotation().y) * start::ENTRY_GAME_SHIFT;
		player->SetVec3Position(posPlayer);

		// カメラ目標位置設定
		SetDestAround();

		// プレイヤーを通常状態にする
		player->SetState(CPlayer::EState::STATE_NORMAL);
		player->SetAlpha(1.0f);

		// ゲームを通常状態にする
		CSceneGame::GetGameManager()->SetState(CGameManager::EState::STATE_NORMAL);

		// タイマーの計測を開始する
		CSceneGame::GetTimerUI()->Start();

		// 現在のモーションの設定
		player->SetMotion(CPlayer::MOTION_IDOL);

		// この先の処理を行わない
		return;
	}

	switch (m_startInfo.state)
	{
	case SStart::STATE_LAND:		// 着地待ち状態

		// 着地待ちカメラ処理
		StartLand(player);

		break;

	case SStart::STATE_ROUND:		// 回り込み状態

		// 回り込みカメラ処理
		StartRound(player);

		break;

	case SStart::STATE_BACK:		// 戻り状態

		// ゲーム遷移処理
		EnterGame(player);

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//============================================================
// 着地待ちカメラ
//============================================================
void CCamera::StartLand(CPlayer* pPlayer)
{
	// 向きを設定する
	m_aCamera[TYPE_MAIN].rot = start::LAND_ROT;

	// 注視点を設定する
	m_aCamera[TYPE_MAIN].posR = D3DXVECTOR3
	(
		pPlayer->GetVec3Position().x,
		start::INIT_POSR_HEIGHT,
		pPlayer->GetVec3Position().z
	);

	// 視点を設定する
	m_aCamera[TYPE_MAIN].posV = D3DXVECTOR3
	(
		m_aCamera[TYPE_MAIN].posR.x + (-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y),
		m_aCamera[TYPE_MAIN].posR.y + m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x),
		m_aCamera[TYPE_MAIN].posR.z + (-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y)
	);

	// 着地していない場合、抜ける
	if (pPlayer->IsJump()) { return; }

	// 状態カウントを加算する
	m_startInfo.nCount++;

	if (pPlayer->GetMotionType() != CPlayer::MOTION_LANDING)
	{ // モーションが大着地じゃない場合

		// スタートモーションに設定する
		pPlayer->SetMotion(CPlayer::MOTION_START, start::START_MOTION_BLEND);
	}

	if (m_startInfo.nCount % start::LAND_COUNT == 0)
	{ // 一定カウント経過した場合

		// 回り込み状態にする
		m_startInfo.state = SStart::STATE_ROUND;

		// カウントを0にする
		m_startInfo.nCount = 0;
	}
}

//============================================================
// 回り込みカメラ
//============================================================
void CCamera::StartRound(CPlayer* pPlayer)
{
	D3DXVECTOR3 diffPosV = VEC3_ZERO;		// 視点の差分
	bool aComplete[2] = { false,false };	// 向き補正完了状況

	// 目的の向きを設定する
	m_aCamera[TYPE_MAIN].destRot = start::ROUND_ROT;

	// 向きを減算する
	m_aCamera[TYPE_MAIN].rot += start::ROUND_ROT_MOVE;
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);

	// 向きを補正する
	aComplete[0] = useful::LimitMaxNum(m_aCamera[TYPE_MAIN].rot.x, start::ROUND_ROT.x);
	aComplete[1] = useful::LimitMaxNum(m_aCamera[TYPE_MAIN].rot.y, start::ROUND_ROT.y);

	if (aComplete[0] && aComplete[1])
	{ // 向きが一定値になった場合

		// カウントを加算する
		m_startInfo.nCount++;
	}

	// 視点を設定する
	m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].posR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].posR.y + ((m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].posR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 視点の差分位置を計算
	diffPosV = m_aCamera[TYPE_MAIN].destPosV - m_aCamera[TYPE_MAIN].posV;

	// 視点の現在位置を更新
	m_aCamera[TYPE_MAIN].posV.x += diffPosV.x * start::REV_POSV.x;
	m_aCamera[TYPE_MAIN].posV.y += diffPosV.y * start::REV_POSV.y;
	m_aCamera[TYPE_MAIN].posV.z += diffPosV.z * start::REV_POSV.z;

	if (m_startInfo.nCount >= start::ROUND_COUNT)
	{ // 定位置について一定時間経過した場合

		// プレイヤーの位置を設定する(空中から始まらないように)
		D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();
		posPlayer.x += sinf(pPlayer->GetVec3Rotation().y) * start::ENTRY_GAME_SHIFT;
		posPlayer.z += cosf(pPlayer->GetVec3Rotation().y) * start::ENTRY_GAME_SHIFT;

		// 周り込みの計算
		CalcAround(posPlayer);

		// 戻り状態にする
		m_startInfo.state = SStart::STATE_BACK;

		// カウントをリセットする
		m_startInfo.nCount = 0;
	}
}

//============================================================
// ゲーム遷移処理
//============================================================
void CCamera::EnterGame(CPlayer* pPlayer)
{
	// カウントを加算する
	m_startInfo.nCount++;

	// プレイヤーの座標を設定
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();
	posPlayer.x += sinf(pPlayer->GetVec3Rotation().y) * start::ENTRY_GAME_SHIFT;
	posPlayer.z += cosf(pPlayer->GetVec3Rotation().y) * start::ENTRY_GAME_SHIFT;

	// 差分向きを計算
	D3DXVECTOR3 diffRot = m_aCamera[TYPE_MAIN].destRot - m_aCamera[TYPE_MAIN].rot;
	useful::NormalizeRot(diffRot);	// 差分向きを正規化

	// 差分向きを補正
	if (diffRot.x > around::LIMIT_DIFF)
	{
		diffRot.x = around::LIMIT_DIFF;
	}
	if (diffRot.y > around::LIMIT_DIFF)
	{
		diffRot.y = around::LIMIT_DIFF;
	}

	// 現在向きの更新
	m_aCamera[TYPE_MAIN].rot += diffRot * start::BACK_REV_ROT;
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化

	// 注視点をプレイヤーの頭の位置にする
	m_aCamera[TYPE_MAIN].destPosR = posPlayer + D3DXVECTOR3(0.0f, pPlayer->GetHeight(), 0.0f);

	// 視点の更新
	m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].destPosR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].destPosR.y + ((around::INIT_HEIGHT * cosf(m_aCamera[TYPE_MAIN].rot.x)));	// TODO：Yの距離だけ定数はきもすぎる。後マイナスにしてないのもやばい。
	m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].destPosR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 注視点の差分位置を計算
	D3DXVECTOR3 diffPosR = m_aCamera[TYPE_MAIN].destPosR - m_aCamera[TYPE_MAIN].posR;

	// 視点の差分位置を計算
	D3DXVECTOR3 diffPosV = m_aCamera[TYPE_MAIN].destPosV - m_aCamera[TYPE_MAIN].posV;

	// 注視点の現在位置を更新
	m_aCamera[TYPE_MAIN].posR.x += diffPosR.x * 0.2f;
	m_aCamera[TYPE_MAIN].posR.y += diffPosR.y * 0.1f;
	m_aCamera[TYPE_MAIN].posR.z += diffPosR.z * 0.2f;

	// 視点の現在位置を更新
	m_aCamera[TYPE_MAIN].posV.x += diffPosV.x * 0.4f;
	m_aCamera[TYPE_MAIN].posV.y += diffPosV.y * 0.1f;
	m_aCamera[TYPE_MAIN].posV.z += diffPosV.z * 0.4f;

	if (m_startInfo.nCount >= start::BACK_COUNT)
	{ // カウントが一定数になった場合

		// スコープアウト
		CSceneGame::GetCinemaScope()->SetScopeOut();

		// 位置を設定する
		pPlayer->SetVec3Position(posPlayer);

		// プレイヤーを通常状態にする
		pPlayer->SetState(CPlayer::EState::STATE_NORMAL);
		pPlayer->SetAlpha(1.0f);

		// ゲームを通常状態にする
		CSceneGame::GetGameManager()->SetState(CGameManager::EState::STATE_NORMAL);

		// タイマーの計測を開始する
		CSceneGame::GetTimerUI()->Start();

		// 現在のモーションの設定
		pPlayer->SetMotion(CPlayer::MOTION_IDOL);
	}
}

//============================================================
//	生成処理
//============================================================
CCamera *CCamera::Create(void)
{
	// カメラの生成
	CCamera *pCamera = new CCamera;
	if (pCamera == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// カメラの初期化
		if (FAILED(pCamera->Init()))
		{ // 初期化に失敗した場合

			// カメラの破棄
			SAFE_DELETE(pCamera);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pCamera;
	}
}

//============================================================
//	破棄処理
//============================================================
void CCamera::Release(CCamera *&prCamera)
{
	// カメラの終了
	assert(prCamera != nullptr);
	prCamera->Uninit();

	// メモリ開放
	SAFE_DELETE(prCamera);
}

//============================================================
//	カメラの更新処理 (神器獲得)
//============================================================
void CCamera::GodItem(void)
{
	// カメラが神器獲得状態以外なら抜ける
	if (m_state != STATE_GODITEM) { return; }

	// プレイヤーの情報を取得
	CPlayer* pPlayer = GET_PLAYER;	// プレイヤー情報
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = pPlayer->GetDestRotation();	// プレイヤー目標向き

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	// 目標向きを設定
	m_aCamera[TYPE_MAIN].destRot.x = god::INIT_ROTX;
	m_aCamera[TYPE_MAIN].destRot.y = rotPlayer.y + god::PLUS_ROTY;
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot);	// 目標向きを正規化

	// 差分向きを計算
	D3DXVECTOR3 diffRot = m_aCamera[TYPE_MAIN].destRot - m_aCamera[TYPE_MAIN].rot;
	useful::NormalizeRot(diffRot);	// 差分向きを正規化

	// 現在向きを計算
	m_aCamera[TYPE_MAIN].rot += diffRot * god::REV_DIFF;
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 目標距離を設定
	m_aCamera[TYPE_MAIN].fDestDis = god::INIT_DIS;

	// 差分距離を計算
	float fDiffDis = m_aCamera[TYPE_MAIN].fDestDis - m_aCamera[TYPE_MAIN].fDis;

	// 現在距離を計算
	m_aCamera[TYPE_MAIN].fDis += fDiffDis * god::REV_DIFF;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// 注視点の目標位置を計算
	m_aCamera[TYPE_MAIN].destPosR = posPlayer;			// プレイヤー位置を基準点にする
	m_aCamera[TYPE_MAIN].destPosR.y += god::PLUS_UP;	// プレイヤーの縦方向にずらす

	// 注視点の差分位置を計算
	D3DXVECTOR3 diffPosR = m_aCamera[TYPE_MAIN].destPosR - m_aCamera[TYPE_MAIN].posR;

	// 注視点の現在位置を計算
	m_aCamera[TYPE_MAIN].posR += diffPosR * god::REV_DIFF;

	// 視点の目標位置を計算
	m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].posR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].posR.y + ((m_aCamera[TYPE_MAIN]. fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));	// TODO：yだけ+なのきもすぎ
	m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].posR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 視点の差分位置を計算
	D3DXVECTOR3 diffPosV = m_aCamera[TYPE_MAIN].destPosV - m_aCamera[TYPE_MAIN].posV;

	// 視点の現在位置を計算
	m_aCamera[TYPE_MAIN].posV += diffPosV * god::REV_DIFF;

	// 視野角を設定
	m_fFov = basic::VIEW_ANGLE;
}

//============================================================
//	カメラの更新処理 (リザルト)
//============================================================
void CCamera::Result(void)
{
	// カメラがリザルト状態以外なら抜ける
	if (m_state != STATE_RESULT) { return; }

	// プレイヤーの情報を取得
	CPlayer* pPlayer = GET_PLAYER;	// プレイヤー情報
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = pPlayer->GetDestRotation();	// プレイヤー目標向き

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	// 目標向きを設定
	m_aCamera[TYPE_MAIN].destRot.x = result::INIT_ROTX;
	m_aCamera[TYPE_MAIN].destRot.y = rotPlayer.y + result::PLUS_ROTY;
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot);	// 目標向きを正規化

	// 差分向きを計算
	D3DXVECTOR3 diffRot = m_aCamera[TYPE_MAIN].destRot - m_aCamera[TYPE_MAIN].rot;
	useful::NormalizeRot(diffRot);	// 差分向きを正規化

	// 現在向きを計算
	m_aCamera[TYPE_MAIN].rot += diffRot * result::REV_DIFF;
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 目標距離を設定
	m_aCamera[TYPE_MAIN].fDestDis = result::INIT_DIS;

	// 差分距離を計算
	float fDiffDis = m_aCamera[TYPE_MAIN].fDestDis - m_aCamera[TYPE_MAIN].fDis;

	// 現在距離を計算
	m_aCamera[TYPE_MAIN].fDis += fDiffDis * result::REV_DIFF;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// クリア状況を取得
	CRetentionManager::EWin win = GET_RETENTION->GetWin();
	
	// 注視点の目標位置を計算
	m_aCamera[TYPE_MAIN].destPosR = posPlayer;	// プレイヤー位置を基準点にする
	m_aCamera[TYPE_MAIN].destPosR.y += result::PLUS_UP[win];	// プレイヤーの縦方向にずらす
	m_aCamera[TYPE_MAIN].destPosR.x += sinf(rotPlayer.y + HALF_PI) * result::PLUS_SIDE[win];	// プレイヤー向き基準の横方向にずらす
	m_aCamera[TYPE_MAIN].destPosR.z += cosf(rotPlayer.y + HALF_PI) * result::PLUS_SIDE[win];	// プレイヤー向き基準の横方向にずらす

	// 注視点の差分位置を計算
	D3DXVECTOR3 diffPosR = m_aCamera[TYPE_MAIN].destPosR - m_aCamera[TYPE_MAIN].posR;

	// 注視点の現在位置を計算
	m_aCamera[TYPE_MAIN].posR += diffPosR * result::REV_DIFF;

	// 視点の目標位置を計算
	m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].posR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].posR.y + ((m_aCamera[TYPE_MAIN]. fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));	// TODO：yだけ+なのきもすぎ
	m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].posR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 視点の差分位置を計算
	D3DXVECTOR3 diffPosV = m_aCamera[TYPE_MAIN].destPosV - m_aCamera[TYPE_MAIN].posV;

	// 視点の現在位置を計算
	m_aCamera[TYPE_MAIN].posV += diffPosV * result::REV_DIFF;

	// 視野角を設定
	m_fFov = basic::VIEW_ANGLE;
}

//============================================================
//	カメラの更新処理 (回転)
//============================================================
void CCamera::Rotate(void)
{
	//--------------------------------------------------------
	//	向きの更新
	//--------------------------------------------------------
	// 現在向きの更新
	m_aCamera[TYPE_MAIN].rot.x = rotate::INIT_ROT.x;
	m_aCamera[TYPE_MAIN].rot.y += rotate::ADD_ROTY;
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化

	//--------------------------------------------------------
	//	距離の更新
	//--------------------------------------------------------
	m_aCamera[TYPE_MAIN].fDis = rotate::INIT_DIS;

	//--------------------------------------------------------
	//	位置の更新
	//--------------------------------------------------------
	// 注視点の更新
	m_aCamera[TYPE_MAIN].posR = rotate::INIT_POSR;

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].posR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].posR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].posR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));
}

//============================================================
//	カメラの更新処理 (追従)
//============================================================
void CCamera::Follow(void)
{
	CInputPad *pPad = GET_INPUTPAD;	// パッド情報
	CListManager<CPlayer> *pList = CPlayer::GetList();	// プレイヤーリスト
	if (pList == nullptr) 		 { return; }	// リスト未使用
	if (pList->GetNumAll() != 1) { return; }	// プレイヤーが1人じゃない
	if (m_state != STATE_FOLLOW) { return; }	// カメラ追従状態以外

	auto player = pList->GetList().front();	// プレイヤー情報
	D3DXVECTOR3 diffPosV = VEC3_ZERO;		// 視点の差分位置
	D3DXVECTOR3 diffPosR = VEC3_ZERO;		// 注視点の差分位置
	D3DXVECTOR3 diffRot  = VEC3_ZERO;		// 差分向き

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	float fRTilt = pPad->GetPressRStickTilt();	// スティックの傾き量
	if (pad::DEAD_ZONE < fRTilt)
	{ // デッドゾーン以上の場合

		// 目標向きを設定
		float fMove = fRTilt * follow::STICK_REV;	// カメラ回転量
		m_aCamera[TYPE_MAIN].destRot.x += sinf(pPad->GetPressRStickRot()) * fMove * follow::ROTX_REV;
		m_aCamera[TYPE_MAIN].destRot.y += cosf(pPad->GetPressRStickRot()) * fMove;

		// 目標向きを正規化
		useful::LimitNum(m_aCamera[TYPE_MAIN].destRot.x, follow::LIMIT_ROT_LOW, follow::LIMIT_ROT_HIGH);
		useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot.y);

		// 差分向きを計算
		diffRot = m_aCamera[TYPE_MAIN].destRot - m_aCamera[TYPE_MAIN].rot;
		useful::NormalizeRot(diffRot);	// 差分向きを正規化

		// 現在向きの更新
		m_aCamera[TYPE_MAIN].rot += diffRot * follow::REV_ROT;
		useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化
	}

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 目標距離を設定
	m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis = follow::INIT_DIS;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// 注視点をプレイヤーの頭の位置にする
	m_aCamera[TYPE_MAIN].destPosR = player->GetVec3Position() + D3DXVECTOR3(0.0f, player->GetHeight(), 0.0f);

	// 視点の更新
	m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].destPosR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].destPosR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].destPosR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 注視点の差分位置を計算
	diffPosR = m_aCamera[TYPE_MAIN].destPosR - m_aCamera[TYPE_MAIN].posR;

	// 視点の差分位置を計算
	diffPosV = m_aCamera[TYPE_MAIN].destPosV - m_aCamera[TYPE_MAIN].posV;

	// 注視点の現在位置を更新
	m_aCamera[TYPE_MAIN].posR.x += diffPosR.x * follow::REV_POSR.x;
	m_aCamera[TYPE_MAIN].posR.y += diffPosR.y * follow::REV_POSR.y;
	m_aCamera[TYPE_MAIN].posR.z += diffPosR.z * follow::REV_POSR.z;

	// 視点の現在位置を更新
	m_aCamera[TYPE_MAIN].posV.x += diffPosV.x * follow::REV_POSV.x;
	m_aCamera[TYPE_MAIN].posV.y += diffPosV.y * follow::REV_POSV.y;
	m_aCamera[TYPE_MAIN].posV.z += diffPosV.z * follow::REV_POSV.z;
}

//============================================================
//	カメラの更新処理 (操作)
//============================================================
void CCamera::Control(void)
{
	// 位置の更新
	Move();

	// 距離の更新
	Distance();

	// 向きの更新
	Rotation();
}

//============================================================
//	位置の更新処理 (操作)
//============================================================
void CCamera::Move(void)
{
	// ポインタを宣言
	CInputMouse	*pMouse = GET_INPUTMOUSE;	// マウスの取得

	// 変数を宣言
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// マウスの移動量

	// マウス操作の更新
	if (pMouse->IsPress(CInputMouse::KEY_LEFT) && pMouse->IsPress(CInputMouse::KEY_RIGHT))
	{ // 右クリックと左クリックが押されている場合

		// 視点を移動
		m_aCamera[TYPE_MAIN].posV.x -= sinf(m_aCamera[TYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posV.z -= cosf(m_aCamera[TYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posV.x += sinf(m_aCamera[TYPE_MAIN].rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posV.z += cosf(m_aCamera[TYPE_MAIN].rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;

		// 注視点を移動
		m_aCamera[TYPE_MAIN].posR.x -= sinf(m_aCamera[TYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posR.z -= cosf(m_aCamera[TYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posR.x += sinf(m_aCamera[TYPE_MAIN].rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posR.z += cosf(m_aCamera[TYPE_MAIN].rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;
	}
}

//============================================================
//	距離の更新処理 (操作)
//============================================================
void CCamera::Distance(void)
{
	// ポインタを宣言
	CInputMouse	*pMouse = GET_INPUTMOUSE;	// マウスの取得

	// 変数を宣言
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// マウスの移動量

	// マウス操作の更新
	if (mouseMove.z != 0.0f)
	{ // マウスホイールが操作された場合

		// 距離を加算
		m_aCamera[TYPE_MAIN].fDis += mouseMove.z * control::REV_DIS_MOUSE;
	}

	// 距離の補正
	useful::LimitNum(m_aCamera[TYPE_MAIN].fDis, basic::MIN_DIS, basic::MAX_DIS);
}

//============================================================
//	向きの更新処理 (操作)
//============================================================
void CCamera::Rotation(void)
{
	// ポインタを宣言
	CInputMouse	*pMouse = GET_INPUTMOUSE;	// マウスの取得

	// 変数を宣言
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// マウスの移動量

	// マウス操作の更新
	if (pMouse->IsPress(CInputMouse::KEY_LEFT) && !pMouse->IsPress(CInputMouse::KEY_RIGHT))
	{ // 左クリックだけが押されている場合

		// カメラのY軸を回転
		m_aCamera[TYPE_MAIN].rot.y += mouseMove.x * control::REV_ROT_MOUSE;

		// カメラのX軸を回転
		m_aCamera[TYPE_MAIN].rot.x += mouseMove.y * control::REV_ROT_MOUSE;
	}

	// 向きの補正
	useful::LimitNum(m_aCamera[TYPE_MAIN].rot.x, basic::LIMIT_ROT_LOW, basic::LIMIT_ROT_HIGH);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot.y);

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].posR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].posR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].posR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// マウス操作の更新
	if (pMouse->IsPress(CInputMouse::KEY_RIGHT) && !pMouse->IsPress(CInputMouse::KEY_LEFT))
	{ // 右クリックだけが押されている場合

		// カメラのY軸を回転
		m_aCamera[TYPE_MAIN].rot.y += mouseMove.x * control::REV_ROT_MOUSE;

		// カメラのX軸を回転
		m_aCamera[TYPE_MAIN].rot.x += mouseMove.y * control::REV_ROT_MOUSE;
	}

	// 向きの補正
	useful::LimitNum(m_aCamera[TYPE_MAIN].rot.x, basic::LIMIT_ROT_LOW, basic::LIMIT_ROT_HIGH);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot.y);

	// 注視点の更新
	m_aCamera[TYPE_MAIN].posR.x = m_aCamera[TYPE_MAIN].posV.x + ((m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posR.y = m_aCamera[TYPE_MAIN].posV.y + ((m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posR.z = m_aCamera[TYPE_MAIN].posV.z + ((m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));
}

//============================================================
//	カメラ揺れの更新処理
//============================================================
void CCamera::Swing(void)
{
	if (m_state == STATE_CONTROL) { return; }	// カメラ操作状態

	for (int nCntCamera = 0; nCntCamera < TYPE_MAX; nCntCamera++)
	{ // カメラの総数分繰り返す

		SCamera *pCamera = &m_aCamera[nCntCamera];				// カメラ情報
		SSwing  *pSwing  = &m_aCamera[nCntCamera].swingInfo;	// カメラ揺れ情報

		if (pSwing->fShiftLength > 0.0f)
		{ // 注視点のずらし量が設定されている場合

			// 変数を宣言
			D3DXQUATERNION quat;	// クォータニオン
			D3DXMATRIX mtxRot;		// 回転マトリックス
			D3DXVECTOR3 offset;		// 位置ずれオフセット
			float fRotY;			// 位置ずれ向き
			D3DXVECTOR3 vecAxis = pCamera->posR - pCamera->posV;	// 回転軸ベクトル

			// クォータニオンを作成
			D3DXVec3Normalize(&vecAxis, &vecAxis);	// 回転軸を正規化
			D3DXQuaternionRotationAxis(&quat, &vecAxis, pSwing->fShiftAngle);

			// 回転マトリックスを作成
			D3DXMatrixRotationQuaternion(&mtxRot, &quat);

			// 位置をずらす向きを求める
			fRotY = atan2f(-vecAxis.z, vecAxis.x);

			// 位置ずれオフセットを設定
			float fCalcTemp = pSwing->fShiftLength * (fabsf(pCamera->fDis) * swing::REV_DIS);
			offset = D3DXVECTOR3(sinf(fRotY) * fCalcTemp, 0.0f, cosf(fRotY) * fCalcTemp);

			// オフセットを反映した回転マトリックスを座標変換
			D3DXVec3TransformCoord(&pSwing->shiftPos, &offset, &mtxRot);

			// 注視点に位置のずれを加算
			pCamera->posR += pSwing->shiftPos;

			// 位置ずれ量を減算
			pSwing->fShiftAngle  -= pSwing->fSubAngle;
			pSwing->fShiftLength -= pSwing->fSubLength;

			// 角度を補正
			useful::NormalizeRot(pSwing->fShiftAngle);

			// 距離を補正
			useful::LimitMinNum(pSwing->fShiftLength, 0.0f);
		}
	}
}

//===========================================
//  回り込み
//===========================================
void CCamera::Around(void)
{
	// 回り込み状態じゃない場合関数を抜ける
	if (m_state != STATE_AROUND) { return; }

	// プレイヤー情報の取得
	CPlayer* pPlayer = GET_PLAYER;

	// プレイヤーがいない場合抜ける
	if (pPlayer == nullptr) { return; }

	// プレイヤーの座標を取得
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();

	// 目標の角度を算出
	CalcAround(posPlayer);

	// 差分向きを計算
	D3DXVECTOR3 diffRot = m_aCamera[TYPE_MAIN].destRot - m_aCamera[TYPE_MAIN].rot;
	useful::NormalizeRot(diffRot);	// 差分向きを正規化

	// 差分向きを補正
	if (diffRot.y < -around::LIMIT_DIFF)
	{
		diffRot.y = -around::LIMIT_DIFF;
	}
	if (diffRot.y > around::LIMIT_DIFF)
	{
		diffRot.y = around::LIMIT_DIFF;
	}

	// 現在向きの更新
	m_aCamera[TYPE_MAIN].rot += diffRot * around::REV_ROT;
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化

	// 目標距離を設定
	m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis = around::INIT_DIS;

	// 注視点をプレイヤーの頭の位置にする
	m_aCamera[TYPE_MAIN].destPosR = posPlayer + D3DXVECTOR3(0.0f, pPlayer->GetHeight(), 0.0f);

	// 視点の更新
	m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].destPosR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].destPosR.y + ((around::INIT_HEIGHT * cosf(m_aCamera[TYPE_MAIN].rot.x)));	// TODO：Yの距離だけ定数はきもすぎる。後マイナスにしてないのもやばい。
	m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].destPosR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 注視点の差分位置を計算
	D3DXVECTOR3 diffPosR = m_aCamera[TYPE_MAIN].destPosR - m_aCamera[TYPE_MAIN].posR;

	// 視点の差分位置を計算
	D3DXVECTOR3 diffPosV = m_aCamera[TYPE_MAIN].destPosV - m_aCamera[TYPE_MAIN].posV;

	// 注視点の現在位置を更新
	m_aCamera[TYPE_MAIN].posR.x += diffPosR.x * around::REV_POSR.x;
	m_aCamera[TYPE_MAIN].posR.y += diffPosR.y * around::REV_POSR.y;
	m_aCamera[TYPE_MAIN].posR.z += diffPosR.z * around::REV_POSR.z;

	// 視点の現在位置を更新
	m_aCamera[TYPE_MAIN].posV.x += diffPosV.x * around::REV_POSV.x;
	m_aCamera[TYPE_MAIN].posV.y += diffPosV.y * around::REV_POSV.y;
	m_aCamera[TYPE_MAIN].posV.z += diffPosV.z * around::REV_POSV.z;
}

//===========================================
//  回り込みの計算
//===========================================
void CCamera::CalcAround(const D3DXVECTOR3& posPlayer)
{
	// カメラ方向変更判定リストの取得
	if (CCameraChanger::GetList() == nullptr)
	{
		// 情報を変更
		m_aCamera[TYPE_MAIN].destRot.y = D3DX_PI * 0.5f;
		m_aCamera[TYPE_MAIN].destRot.x = around::CENTER_ROTX;
		return;
	}

	// リストの中身を確認する
	std::list<CCameraChanger*> list = CCameraChanger::GetList()->GetList();	// リストを取得
	for (CCameraChanger* changer : list)
	{
		// プレイヤーが内部にいない場合次に進む
		if (!changer->CollChange()) { continue; }

		// 情報を変更
		m_aCamera[TYPE_MAIN].destRot.y = changer->GetDirection();
		m_aCamera[TYPE_MAIN].destRot.x = changer->GetRotation();
		return;
	}

	// 情報を変更
	m_aCamera[TYPE_MAIN].destRot.y = D3DX_PI * 0.5f;
	m_aCamera[TYPE_MAIN].destRot.x = around::CENTER_ROTX;
}

//===========================================
//  望遠処理
//===========================================
void CCamera::Telephoto()
{
	// ゴール情報を取得
	CGoal* goal = CGoal::GetGoal();

	// ゴール座標を取得
	D3DXVECTOR3 posGoal = goal->GetVec3Position();

	// プレイヤー情報を取得
	CPlayer* player = GET_PLAYER;

	// プレイヤー座標を取得
	D3DXVECTOR3 posPlayer = player->GetVec3Position();

	// プレイヤーの身長を取得
	float fHeightPlayer = player->GetHeight();

	// プレイヤーの幅を取得
	float fRadiusPlayer = player->GetRadius();

	// プレイヤーとゴールの中間に注視点を設定する
	m_aCamera[TYPE_MAIN].destPosR = (posGoal + posPlayer) * 0.5f;
	 
	// 角度を設定
	m_aCamera[TYPE_MAIN].destRot.y = D3DX_PI * 0.5f;

	// 変数宣言
	D3DXVECTOR3 diffPosV = VEC3_ZERO;		// 視点の差分位置
	D3DXVECTOR3 diffPosR = VEC3_ZERO;		// 注視点の差分位置
	D3DXVECTOR3 diffRot = VEC3_ZERO;		// 差分向き

	//// 差分向きを計算
	//diffRot = m_aCamera[TYPE_MAIN].destRot - m_aCamera[TYPE_MAIN].rot;
	//useful::NormalizeRot(diffRot);	// 差分向きを正規化

	//// 現在向きの更新
	//m_aCamera[TYPE_MAIN].rot += diffRot * telephoto::REV_ROT;
	//useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化

	// 目標距離を設定
	//m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis = telephoto::INIT_DIS;

	//視点をプレイヤーの頭に設定
	m_aCamera[TYPE_MAIN].destPosV = D3DXVECTOR3
	(
		posPlayer.x + fRadiusPlayer,
		posPlayer.y + fHeightPlayer,
		posPlayer.z
	);

	// 視点の更新
	//m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].destPosR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	//m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].destPosR.y + ((telephoto::INIT_HEIGHT * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	//m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].destPosR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 注視点の差分位置を計算
	diffPosR = m_aCamera[TYPE_MAIN].destPosR - m_aCamera[TYPE_MAIN].posR;

	// 視点の差分位置を計算
	diffPosV = m_aCamera[TYPE_MAIN].destPosV - m_aCamera[TYPE_MAIN].posV;

	// 注視点の現在位置を更新
	m_aCamera[TYPE_MAIN].posR.x += diffPosR.x * telephoto::REV_POSR.x;
	m_aCamera[TYPE_MAIN].posR.y += diffPosR.y * telephoto::REV_POSR.y;
	m_aCamera[TYPE_MAIN].posR.z += diffPosR.z * telephoto::REV_POSR.z;

	// 視点の現在位置を更新
	m_aCamera[TYPE_MAIN].posV.x += diffPosV.x * telephoto::REV_POSV.x;
	m_aCamera[TYPE_MAIN].posV.y += diffPosV.y * telephoto::REV_POSV.y;
	m_aCamera[TYPE_MAIN].posV.z += diffPosV.z * telephoto::REV_POSV.z;
}
