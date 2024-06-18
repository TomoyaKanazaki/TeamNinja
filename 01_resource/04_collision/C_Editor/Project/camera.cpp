//============================================================
//
//	カメラ処理 [camera.cpp]
//	Author：藤田勇一
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

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	// カメラ基本情報
	namespace basic
	{
		const D3DXVECTOR3 INIT_VECU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 上方向ベクトルの初期値
		const D3DXVECTOR3 INIT_POSV = D3DXVECTOR3(0.0f, 0.0f, -600.0f);	// 視点の初期値
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

	// 追従カメラ情報
	namespace follow
	{
		const D3DXVECTOR3 REV_POSV = D3DXVECTOR3(0.4f, 0.45f, 0.4f);	// カメラ視点の補正係数
		const D3DXVECTOR3 REV_POSR = D3DXVECTOR3(1.0f, 0.35f, 1.0f);	// カメラ注視点の補正係数

		const float	STICK_REV	= 0.00000225f;	// カメラ操作スティックの傾き量の補正係数

		const float	ROTX_REV	= 0.5f;		// カメラピッチ回転の補正係数
		const float REV_ROT		= 1.0f;		// カメラ向きの補正係数
		const float INIT_DIS	= 400.0f;	// 追従カメラの距離
		const float INIT_ROTX	= 1.8f;		// 追従カメラの向きX初期値

		const int	LOOK_BOSS_FRAME	= 25;				// 追従カメラのボス視認速度
		const float LIMIT_ROT_HIGH	= D3DX_PI - 1.0f;	// X上回転の制限値
		const float LIMIT_ROT_LOW	= 1.0f;				// X下回転の制限値
	}

	// 操作カメラ情報
	namespace control
	{
		const float REV_MOVE_MOUSE	= 1.0f;		// マウス操作でのカメラの移動の補正係数
		const float REV_DIS_MOUSE	= 0.2f;		// マウス操作でのカメラの距離の補正係数
		const float REV_ROT_MOUSE	= 0.0045f;	// マウス操作でのカメラの回転量の補正係数
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
	m_bUpdate	(false)			// 更新状況
{
	// メンバ変数をクリア
	memset(&m_aCamera[0],	0, sizeof(m_aCamera));	// カメラの情報
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
	m_state		= STATE_NONE;	// 状態
	m_bUpdate	= true;			// 更新状況

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
void CCamera::Update(void)
{
	if (m_bUpdate)
	{ // 更新する状況の場合

		switch (m_state)
		{ // 状態ごとの処理
		case STATE_NONE:	// なにもしない状態

			// 無し

			break;

		case STATE_CONTROL:	// 操作状態

			// カメラの更新 (操作)
			Control();

			break;

		case STATE_FOLLOW:	// 追従状態

			// カメラの更新 (追従)
			Follow();

			break;

		default:	// 例外処理
			assert(false);
			break;
		}

		// カメラ揺れの更新
		Swing();
	}

	// デバッグ表示
	GET_MANAGER->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[ 視点 ]：%f %f %f\n", m_aCamera[TYPE_MAIN].posV.x, m_aCamera[TYPE_MAIN].posV.y, m_aCamera[TYPE_MAIN].posV.z);
	GET_MANAGER->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[注視点]：%f %f %f\n", m_aCamera[TYPE_MAIN].posR.x, m_aCamera[TYPE_MAIN].posR.y, m_aCamera[TYPE_MAIN].posR.z);
	GET_MANAGER->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[ 向き ]：%f %f %f\n", m_aCamera[TYPE_MAIN].rot.x, m_aCamera[TYPE_MAIN].rot.y, m_aCamera[TYPE_MAIN].rot.z);
	GET_MANAGER->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[ 距離 ]：%f\n", m_aCamera[TYPE_MAIN].fDis);
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
		basic::VIEW_ANGLE,	// 視野角
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
//	カメラの目標位置の設定処理 (追従)
//============================================================
void CCamera::SetDestFollow(void)
{
	CPlayer *pPlayer = CScene::GetPlayer();		// プレイヤー情報
	if (pPlayer == nullptr)		 { return; }	// プレイヤー未使用
	if (m_state != STATE_FOLLOW) { return; }	// カメラ追従状態以外

	D3DXVECTOR3 diffPosV = VEC3_ZERO;	// 視点の差分位置
	D3DXVECTOR3 diffPosR = VEC3_ZERO;	// 注視点の差分位置
	D3DXVECTOR3 diffRot  = VEC3_ZERO;	// 差分向き

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	// 向きを設定
	m_aCamera[TYPE_MAIN].rot.x = m_aCamera[TYPE_MAIN].destRot.x = follow::INIT_ROTX;
	m_aCamera[TYPE_MAIN].rot.y = m_aCamera[TYPE_MAIN].destRot.y = pPlayer->GetVec3Rotation().y + D3DX_PI;

	// 向きを正規化
	useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].rot);
	useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].destRot);

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 目標距離を設定
	m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis = follow::INIT_DIS;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// 注視点をプレイヤーの頭の位置にする
	m_aCamera[TYPE_MAIN].posR = m_aCamera[TYPE_MAIN].destPosR = pPlayer->GetVec3Position() + D3DXVECTOR3(0.0f, pPlayer->GetHeight(), 0.0f);

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].destPosR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].destPosR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].destPosR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));
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

//============================================================
//	向きの設定処理
//============================================================
void CCamera::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 引数のカメラの向きを設定
	m_aCamera[TYPE_MAIN].rot = rRot;

	// 向きを正規化
	useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].rot);
}

//============================================================
//	目標向きの設定処理
//============================================================
void CCamera::SetVec3DestRotation(const D3DXVECTOR3& rRot)
{
	// 引数のカメラの目標向きを設定
	m_aCamera[TYPE_MAIN].destRot = rRot;

	// 向きを正規化
	useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].destRot);
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
//	向き取得処理
//============================================================
D3DXVECTOR3 CCamera::GetVec3Rotation(void) const
{
	// カメラの向きを返す
	return m_aCamera[TYPE_MAIN].rot;
}

//============================================================
//	目標向き取得処理
//============================================================
D3DXVECTOR3 CCamera::GetVec3DestRotation(void) const
{
	// カメラの目標向きを返す
	return m_aCamera[TYPE_MAIN].destRot;
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
//	カメラの更新処理 (追従)
//============================================================
void CCamera::Follow(void)
{
	CInputPad *pPad = GET_INPUTPAD;				// パッド情報
	CPlayer *pPlayer = CScene::GetPlayer();		// プレイヤー情報
	if (pPlayer == nullptr)		 { return; }	// プレイヤー未使用
	if (m_state != STATE_FOLLOW) { return; }	// カメラ追従状態以外

	D3DXVECTOR3 diffPosV = VEC3_ZERO;	// 視点の差分位置
	D3DXVECTOR3 diffPosR = VEC3_ZERO;	// 注視点の差分位置
	D3DXVECTOR3 diffRot  = VEC3_ZERO;	// 差分向き

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
		useful::Vec3NormalizeRot(diffRot);	// 差分向きを正規化

		// 現在向きの更新
		m_aCamera[TYPE_MAIN].rot += diffRot * follow::REV_ROT;
		useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化
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
	m_aCamera[TYPE_MAIN].destPosR = pPlayer->GetVec3Position() + D3DXVECTOR3(0.0f, pPlayer->GetHeight(), 0.0f);

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
