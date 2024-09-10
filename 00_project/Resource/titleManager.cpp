//============================================================
//
//	タイトルマネージャー処理 [titleManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "titleManager.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"
#include "player.h"
#include "titleLogo2D.h"
#include "blink2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 5;	// タイトルの優先順位
	const D3DXVECTOR3 ABS_INIT_OFFSET	= D3DXVECTOR3(80.0f, 0.0f, 0.0f);	// 生成位置オフセットの絶対値
	const D3DXVECTOR3 DEST_OFFSET		= D3DXVECTOR3(0.0f, 170.0f, 0.0f);	// 生成位置オフセットの絶対値
	const D3DXVECTOR3 POS_ONE	= D3DXVECTOR3(640.0f, 95.0f, 0.0f);			// 第一目標位置
	const D3DXVECTOR3 POS_TWO	= D3DXVECTOR3(260.0f, 95.0f, 0.0f);			// 第二目標位置
	const D3DXVECTOR3 SIZE_POLY	= D3DXVECTOR3(320.0f, 320.0f, 0.0f);		// 大きさ
	const POSGRID2 TEX_PART		= POSGRID2(1, 4);	// テクスチャ分割数
	const float MOVE_TIME_ONE	= 1.8f;		// 生成移動時間
	const float MOVE_TIME_TWO	= 3.0f;		// 生成移動時間
	const float WAIT_TIME		= 0.092f;	// 生成待機時間
	const float TRANS_TIME		= 3.0f;		// 遷移待機時間

	const D3DXCOLOR COL_BLUR[] =	// ブラー色
	{
		D3DCOLOR_RGBA(110, 228, 234, 255),	// 水色
		D3DCOLOR_RGBA(255, 142, 204, 255),	// 紫色
	};

	const char* TEXTURE_BLUR = "data\\TEXTURE\\title_blur000.png";	// ブラーテクスチャ
	const char* TEXTURE_LOGO = "data\\TEXTURE\\title_logo000.png";	// ロゴテクスチャ
	const char* TEXTURE_AURA = "data\\TEXTURE\\title_aura000.png";	// オーラテクスチャ

	namespace control
	{
		const char* TEXTURE		= "data\\TEXTURE\\title_start000.png";	// 操作テクスチャ
		const D3DXVECTOR3 POS	= D3DXVECTOR3(845.0f, 595.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(848.0f, 180.0f, 0.0f);	// 大きさ
		const float MIN_ALPHA	= 0.4f;	// 最低透明度
		const float MAX_ALPHA	= 1.0f;	// 最大透明度
		const float LEVEL_ALPHA	= 3.4f;	// 透明度の加減量
		const float SUB_ALPHA	= 0.5f;	// インのα値減少量
		const float ADD_ALPHA	= 1.0f;	// アウトのα値増加量
	}
}

//************************************************************
//	親クラス [CTitleManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================

CTitleManager::CTitleManager() :
	m_pStart	(nullptr),		// 開始操作情報
	m_state		(STATE_NONE),	// 状態
	m_fCurTime	(0.0f)			// 現在の経過時間
{
	// メンバ変数をクリア
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));	// タイトル情報
}

//============================================================
//	デストラクタ
//============================================================
CTitleManager::~CTitleManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTitleManager::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));	// タイトル情報
	m_pStart	= nullptr;		// 開始操作情報
	m_state		= STATE_NONE;	// 状態
	m_fCurTime	= 0.0f;			// 現在の経過時間

	//--------------------------------------------------------
	//	タイトルの生成・設定
	//--------------------------------------------------------
	for (int i = 0; i < NUM_LOGO; i++)
	{ // ロゴの文字数分繰り返す

		int nFlag = (i % 2);					// 数値フラグ
		float fSide = (1.0f - (nFlag * 2.0f));	// 生成方向係数
		float fWaitTime = WAIT_TIME * (float)i;	// 待機時間
		D3DXCOLOR colBlur = COL_BLUR[nFlag];	// 生成位置オフセット
		D3DXVECTOR3 destOffset = DEST_OFFSET * (float)i;	// 目標位置
		D3DXVECTOR3 initOffset = ABS_INIT_OFFSET * fSide;	// 生成位置オフセット

		// タイトルの生成
		m_apLogo[i] = CTitleLogo2D::Create
		( // 引数
			TEXTURE_BLUR,			// ブラーテクスチャパス
			POS_ONE + destOffset,	// 第一目標位置
			POS_TWO + destOffset,	// 第二目標位置
			initOffset,		// オフセット
			SIZE_POLY,		// 大きさ
			MOVE_TIME_ONE,	// 第一移動時間
			MOVE_TIME_TWO,	// 第二移動時間
			fWaitTime,		// 第一待機時間
			fWaitTime		// 第二待機時間
		);
		if (m_apLogo[i] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを割当
		m_apLogo[i]->BindTexture(TEXTURE_LOGO);

		// オーラテクスチャを割当
		m_apLogo[i]->BindAuraTexture(TEXTURE_AURA);

		// ブラー色を設定
		m_apLogo[i]->SetBlurColor(colBlur);

		// パターンを設定
		m_apLogo[i]->SetPattern(i);

		// テクスチャ横分割数を設定
		m_apLogo[i]->SetWidthPattern(TEX_PART.x);

		// テクスチャ縦分割数を設定
		m_apLogo[i]->SetHeightPattern(TEX_PART.y);

		// 優先順位を設定
		m_apLogo[i]->SetPriority(PRIORITY);
	}

	// 開始操作の生成
	m_pStart = CBlink2D::Create
	( // 引数
		control::POS,			// 位置
		control::SIZE,			// 大きさ
		control::MIN_ALPHA,		// 最低透明度
		control::MAX_ALPHA,		// 最大透明度
		control::LEVEL_ALPHA,	// 透明度の加減量
		control::SUB_ALPHA,		// インのα値減少量
		control::ADD_ALPHA		// アウトのα値増加量
	);
	if (m_pStart == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを割当
	m_pStart->BindTexture(control::TEXTURE);

	// 優先順位を設定
	m_pStart->SetPriority(PRIORITY);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTitleManager::Uninit(void)
{
	for (int i = 0; i < NUM_LOGO; i++)
	{ // ロゴの文字数分繰り返す

		// タイトル情報の終了
		SAFE_UNINIT(m_apLogo[i]);
	}
}

//============================================================
//	更新処理
//============================================================
void CTitleManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない
	{
		// フェード中なら抜ける
		if (GET_MANAGER->GetFade()->IsFade()) { break; }

		for (int i = 0; i < NUM_LOGO; i++)
		{ // ロゴの文字数分繰り返す

			// 演出を開始する
			m_apLogo[i]->SetStag();
		}

		// ロゴ表示状態にする
		m_state = STATE_LOGO;
		break;
	}
	case STATE_LOGO:	// ロゴ表示
	{
		int nCurIdx = NUM_LOGO - 1;	// 最後尾インデックス
		if (!m_apLogo[nCurIdx]->IsStag())
		{ // 最後尾の演出が終了した場合

			// 開始操作の点灯を開始する
			m_pStart->SetBlink(true);

			// 操作待機状態にする
			m_state = STATE_WAIT;
		}
		break;
	}
	case STATE_WAIT:	// 操作待機
	{
		if (GET_INPUTKEY->IsAnyTrigger()
		||  GET_INPUTPAD->IsAnyTrigger())
		{
			// カメラの走り状態設定
			GET_CAMERA->SetRotateRun();

			// 演出開始の設定
			GET_PLAYER->SetStart();

			// 開始操作の点灯を終了する
			m_pStart->SetBlink(false);

			// 忍者疾走状態にする
			m_state = STATE_RUN;
		}
		break;
	}
	case STATE_RUN:		// 忍者疾走
	{
		// 経過時間を加算
		m_fCurTime += fDeltaTime;
		if (m_fCurTime >= TRANS_TIME)
		{ // プレイヤーたちが草むらを超えた場合

			// セレクト画面に遷移
			GET_MANAGER->SetLoadScene(CScene::MODE_SELECT);
		}
		break;
	}
	case STATE_END:		// 終了
	{
		break;
	}
	default:	// 例外処理
		assert(false);
		break;
	}

	// TODO：デバッグ
#ifdef _DEBUG
	if (GET_INPUTKEY->IsTrigger(DIK_F5))
	{
		GET_MANAGER->SetLoadScene(CScene::MODE_SELECT);
	}
#endif
}

//============================================================
//	生成処理
//============================================================
CTitleManager *CTitleManager::Create(void)
{
	// タイトルマネージャーの生成
	CTitleManager *pTitleManager = new CTitleManager;
	if (pTitleManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// タイトルマネージャーの初期化
		if (FAILED(pTitleManager->Init()))
		{ // 初期化に失敗した場合

			// タイトルマネージャーの破棄
			SAFE_DELETE(pTitleManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pTitleManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CTitleManager::Release(CTitleManager *&prTitleManager)
{
	// タイトルマネージャーの終了
	assert(prTitleManager != nullptr);
	prTitleManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prTitleManager);
}
