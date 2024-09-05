//============================================================
//
//	デバッグ処理 [debug.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "debug.h"
#include "manager.h"
#include "object.h"
#include "renderer.h"
#include "scene.h"
#include "sceneSelect.h"
#include "sceneGame.h"
#include "editManager.h"
#include "debugproc.h"
#include "pause.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int MEASURE_FPS	= 500;	// FPSの計測タイミング
	const int FILL_MAX		= 3;	// 塗りつぶしモードの総数
}

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_DEBUG_DISP		(DIK_F1)	// デバッグ表示ON/OFFキー
#define NAME_DEBUG_DISP		("F1")		// デバッグ表示ON/OFF表示
#define KEY_CAMERA_CONTROL	(DIK_F2)	// カメラ操作ON/OFFキー
#define NAME_CAMERA_CONTROL	("F2")		// カメラ操作ON/OFF表示
#define KEY_FILLMODE		(DIK_F3)	// 塗りつぶしモード変更キー
#define NAME_FILLMODE		("F3")		// 塗りつぶしモード変更表示
#define KEY_2D_DISP			(DIK_F4)	// 2Dオブジェクト表示ON/OFFキー
#define NAME_2D_DISP		("F4")		// 2Dオブジェクト表示ON/OFF表示
#define KEY_EDITMODE		(DIK_F5)	// エディターモードON/OFFキー
#define NAME_EDITMODE		("F5")		// エディターモードON/OFF表示
#define KEY_ACTOR_DISP		(DIK_F6)	// アクターの当たり判定表示ON/OFFキー
#define NAME_ACTOR_TRANS	("F6")		// アクターの当たり判定表示ON/OFF表示
#define KEY_PAUSE_DISP		(DIK_F7)	// ポーズ表示ON/OFFキー
#define NAME_PAUSE_DISP		("F7")		// ポーズ表示ON/OFF表示
#define KEY_RESULT_TRANS	(DIK_F8)	// リザルト遷移キー
#define NAME_RESULT_TRANS	("F8")		// リザルト遷移表示
#define KEY_SHADER_ENABLE	(DIK_F9)	// シェーダー切り替えキー
#define NAME_SHADER_ENABLE	("F9")		// シェーダー切り替え表示
//************************************************************
//	親クラス [CDebug] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
#ifdef _DEBUG

CDebug::CDebug() :
	m_nFps				(0),					// FPSカウンタ
	m_bDisp2D			(false),				// 2D表示状況
	m_bDisp3D			(false),				// 3D表示状況
	m_bDispActor		(false),				// アクターの当たり判定の表示状況
	m_bEnableShader		(false),				// シェーダーの使用状態
	m_dwFrameCount		(0),					// フレームカウント
	m_dwFPSLastTime		(0),					// 最後にFPSを計測した時刻
	m_fillMode			(D3DFILL_SOLID),		// 塗りつぶしモード
	m_oldCameraState	(CCamera::STATE_NONE)	// カメラの過去状態
{

}

#else	// NDEBUG

CDebug::CDebug() {}

#endif	// _DEBUG

//============================================================
//	デストラクタ
//============================================================
#ifdef _DEBUG

CDebug::~CDebug()
{

}

#else	// NDEBUG

CDebug::~CDebug() {}

#endif	// _DEBUG

//============================================================
//	初期化処理
//============================================================
#ifdef _DEBUG

HRESULT CDebug::Init(void)
{
	// メンバ変数を初期化
	m_nFps				= 0;					// FPSカウンタ
	m_bDisp2D			= true;					// 2D表示状況
	m_bDisp3D			= true;					// 3D表示状況
	m_bDispActor		= false;				// アクターの当たり判定の表示状況
	m_dwFrameCount		= 0;					// フレームカウント
	m_dwFPSLastTime		= timeGetTime();		// 現在時刻を取得
	m_fillMode			= D3DFILL_SOLID;		// 塗りつぶしモード
	m_oldCameraState	= CCamera::STATE_NONE;	// カメラの過去状態

	// オブジェクトの表示状況を設定
	CObject::SetEnableDebugDispAll(m_bDisp2D, m_bDisp3D);

	// 成功を返す
	return S_OK;
}

#else	// NDEBUG

HRESULT CDebug::Init(void) { return S_OK; }

#endif	// _DEBUG

//============================================================
//	終了処理
//============================================================
#ifdef _DEBUG

void CDebug::Uninit(void)
{

}

#else	// NDEBUG

void CDebug::Uninit(void) {}

#endif	// _DEBUG

//============================================================
//	FPS測定処理
//============================================================
#ifdef _DEBUG

void CDebug::MeasureFps(const DWORD dwCurrentTime)
{
	if ((dwCurrentTime - m_dwFPSLastTime) >= MEASURE_FPS)
	{ // 0.5秒経過

		// FPSを計測
		m_nFps = (m_dwFrameCount * 1000) / (dwCurrentTime - m_dwFPSLastTime);

		// FPSを測定した時刻を保存
		m_dwFPSLastTime = dwCurrentTime;

		// フレームカウントをクリア
		m_dwFrameCount = 0;
	}
}

#else	// NDEBUG

void CDebug::MeasureFps(const DWORD /*dwCurrentTime*/) {}

#endif	// _DEBUG

//============================================================
//	フレームカウント加算処理
//============================================================
#ifdef _DEBUG

void CDebug::AddFrameCount(void)
{
	// フレームカウントを加算
	m_dwFrameCount++;
}

#else	// NDEBUG

void CDebug::AddFrameCount(void) {}

#endif	// _DEBUG

//============================================================
//	FPS取得処理
//============================================================
#ifdef _DEBUG

int CDebug::GetFps(void) const
{
	// FPSを返す
	return m_nFps;
}

#else	// NDEBUG

int CDebug::GetFps(void) const { return 0; }

#endif	// _DEBUG

//============================================================
//	シェーダー状態取得処理
//============================================================
#ifdef _DEBUG

bool CDebug::GetEnableShader(void) const
{
	// FPSを返す
	return m_bEnableShader;
}

#else	// NDEBUG

bool CDebug::GetEnableShader(void) const { return true; }

#endif	// _DEBUG

//============================================================
//	塗りつぶしモード設定処理
//============================================================
#ifdef _DEBUG

void CDebug::SetFillMode(void)
{
	// 塗りつぶしモードを現在のモードに設定
	GET_DEVICE->SetRenderState(D3DRS_FILLMODE, m_fillMode);
}

#else	// NDEBUG

void CDebug::SetFillMode(void) {}

#endif	// _DEBUG

//============================================================
//	デバッグ操作更新処理
//============================================================
#ifdef _DEBUG

void CDebug::UpdateDebugControl(void)
{
	// デバッグ表示変更
	ChangeDispDebug();

	// カメラ操作変更
	ChangeControlCamera();

	// 塗りつぶしモード変更
	ChangeFillMode();

	// 2Dオブジェクト表示変更
	ChangeDisp2D();

	switch (GET_MANAGER->GetMode())
	{ // モードごとの処理
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_SELECT:

		// エディターモード変更
		ChangeEditMode();

		// アクター当たり判定表示変更
		ChangeActorDisp();
		break;

	case CScene::MODE_GAME:

		// エディターモード変更
		ChangeEditMode();

		// アクター当たり判定表示変更
		ChangeActorDisp();

		// ポーズ表示変更
		ChangeDispPause();
		break;

	default:
		assert(false);
		break;
	}
	// シェーダー描画切り替え
	ChangeEnableShader();
}

#else	// NDEBUG

void CDebug::UpdateDebugControl(void) {}

#endif	// _DEBUG

//============================================================
//	デバッグ操作表示処理
//============================================================
#ifdef _DEBUG

void CDebug::DrawDebugControl(void)
{
	DebugProc::Print(DebugProc::POINT_LEFT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "　[デバッグ操作]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[%s]：デバッグ表示のON/OFF\n", NAME_DEBUG_DISP);
	DebugProc::Print(DebugProc::POINT_LEFT, "[%s]：カメラ操作のON/OFF\n", NAME_CAMERA_CONTROL);
	DebugProc::Print(DebugProc::POINT_LEFT, "[%s]：塗りつぶしモードの変更\n", NAME_FILLMODE);
	DebugProc::Print(DebugProc::POINT_LEFT, "[%s]：2Dオブジェクト表示のON/OFF\n", NAME_2D_DISP);

	switch (GET_MANAGER->GetMode())
	{ // モードごとの処理
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_SELECT:

		DebugProc::Print(DebugProc::POINT_LEFT, "[%s]：エディットモードのON/OFF\n", NAME_EDITMODE);
		DebugProc::Print(DebugProc::POINT_LEFT, "[%s]：アクターの当たり判定表示変更\n", NAME_ACTOR_TRANS);
		break;

	case CScene::MODE_GAME:

		DebugProc::Print(DebugProc::POINT_LEFT, "[%s]：エディットモードのON/OFF\n", NAME_EDITMODE);
		DebugProc::Print(DebugProc::POINT_LEFT, "[%s]：アクターの当たり判定表示変更\n", NAME_ACTOR_TRANS);
		DebugProc::Print(DebugProc::POINT_LEFT, "[%s]：ポーズ描画のON/OFF\n", NAME_PAUSE_DISP);
		break;

	default:
		assert(false);
		break;
	}
	DebugProc::Print(DebugProc::POINT_LEFT, "[%s]：シェーダーの有効化/無効化\n", NAME_SHADER_ENABLE);
}

#else	// NDEBUG

void CDebug::DrawDebugControl(void) {}

#endif	// _DEBUG

//============================================================
//	デバッグ情報表示処理
//============================================================
#ifdef _DEBUG

void CDebug::DrawDebugData(void)
{
	DebugProc::Print(DebugProc::POINT_LEFT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "　[デバッグ情報]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "======================================\n");

	DebugProc::Print(DebugProc::POINT_CENTER, "[FPS]：%d\n", m_nFps);
}

#else	// NDEBUG

void CDebug::DrawDebugData(void) {}

#endif	// _DEBUG

//============================================================
//	生成処理
//============================================================
CDebug *CDebug::Create(void)
{
	// デバッグの生成
	CDebug *pDebug = new CDebug;
	if (pDebug == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// デバッグの初期化
		if (FAILED(pDebug->Init()))
		{ // 初期化に失敗した場合

			// デバッグの破棄
			SAFE_DELETE(pDebug);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pDebug;
	}
}

//============================================================
//	破棄処理
//============================================================
void CDebug::Release(CDebug *&prDebug)
{
	// デバッグの終了
	assert(prDebug != nullptr);
	prDebug->Uninit();

	// メモリ開放
	SAFE_DELETE(prDebug);
}

//************************************************************
//	private メンバ関数 (デバッグ時以外存在しない)
//************************************************************
#ifdef _DEBUG

//============================================================
//	デバッグ表示変更処理
//============================================================
void CDebug::ChangeDispDebug(void)
{
}

//============================================================
//	カメラ操作変更処理
//============================================================
void CDebug::ChangeControlCamera(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_CAMERA_CONTROL))
	{
		if (GET_MANAGER->GetCamera()->GetState() != CCamera::STATE_CONTROL)
		{ // カメラ操作がOFFの場合

			// 現在の状態を保存
			m_oldCameraState = GET_MANAGER->GetCamera()->GetState();

			// 操作カメラを設定
			GET_MANAGER->GetCamera()->SetState(CCamera::STATE_CONTROL);
		}
		else
		{ // カメラ操作がONの場合

			// 過去の状態を再設定
			GET_MANAGER->GetCamera()->SetState(m_oldCameraState);

			// 過去の状態を初期化
			m_oldCameraState = CCamera::STATE_NONE;
		}
	}
}

//============================================================
//	塗りつぶしモード変更処理
//============================================================
void CDebug::ChangeFillMode(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_FILLMODE))
	{
		// 塗りつぶしモードを変更する
		m_fillMode = (D3DFILLMODE)((m_fillMode + 1) % FILL_MAX);
	}
}

//============================================================
//	2Dオブジェクト表示変更処理
//============================================================
void CDebug::ChangeDisp2D(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_2D_DISP))
	{
		// 2Dの表示フラグを反転
		m_bDisp2D = !m_bDisp2D;

		// オブジェクトの表示状況を設定
		CObject::SetEnableDebugDispAll(m_bDisp2D, m_bDisp3D);
	}
}

//============================================================
//	エディターモード変更処理
//============================================================
void CDebug::ChangeEditMode(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_EDITMODE))
	{
		// エディットマネージャー取得
		CEditManager *pEdit = nullptr;
		switch (GET_MANAGER->GetMode())
		{
		case CScene::MODE_SELECT:
			pEdit = CSceneSelect::GetEditManager();
			break;

		case CScene::MODE_GAME:
			pEdit = CSceneGame::GetEditManager();
			break;

		default:
			assert(false);
			break;
		}

		// エディット状況のフラグを反転
		assert(pEdit != nullptr);
		pEdit->SetEnableEdit(!pEdit->IsEdit());
	}
}

//============================================================
//	ポーズ表示変更処理
//============================================================
void CDebug::ChangeDispPause(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_PAUSE_DISP))
	{
		// ポーズの表示状況を設定
		CSceneGame::GetPause()->SetEnableDebugDisp(!CSceneGame::GetPause()->IsDebugDisp());
	}
}
//============================================================
//	シェーダー描画変更処理
//============================================================
void CDebug::ChangeEnableShader(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_SHADER_ENABLE))
	{
		m_bEnableShader = !m_bEnableShader;
	}
}

//============================================================
// アクター当たり判定表示変更
//============================================================
void CDebug::ChangeActorDisp(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_ACTOR_DISP) || GET_INPUTPAD->IsTrigger(CInputPad::KEY_B))
	{
		// アクターの当たり判定の表示状況を切り替える
		m_bDispActor = !m_bDispActor;

		for (int nCntScene = 0; nCntScene < CObject::SCENE_MAX; nCntScene++)
		{ // シーンの総数分繰り返す

			for (int nCntDim = 0; nCntDim < CObject::DIM_MAX; nCntDim++)
			{ // 次元の総数分繰り返す

				for (int nCntPri = 0; nCntPri < object::MAX_PRIO; nCntPri++)
				{ // 優先順位の総数分繰り返す

					// オブジェクトの先頭を代入
					CObject* pObject = CObject::GetTop
					( // 引数
						static_cast<CObject::EScene>(nCntScene),	// シーン
						static_cast<CObject::EDim>(nCntDim),		// 次元
						nCntPri										// 優先順位
					);

					while (pObject != nullptr)
					{ // オブジェクトが使用されている場合繰り返す

						// 次のオブジェクトを代入
						CObject* pObjectNext = pObject->GetNext();

						if (pObject->IsDeath())
						{ // 死亡している場合

							// 次のオブジェクトへのポインタを代入
							pObject = pObjectNext;
							continue;
						}

						if (pObject->GetLabel() == CObject::LABEL_COLLISION)
						{ // 当たり判定ラベル

							// オブジェクトを見えなくする
							pObject->SetEnableDebugDisp(!m_bDispActor);
						}

						// 次のオブジェクトへのポインタを代入
						pObject = pObjectNext;
					}
				}
			}
		}
	}
}

#endif	// _DEBUG
