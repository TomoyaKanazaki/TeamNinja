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
#include "texture.h"
#include "model.h"
#include "object2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *TEX_LOGO_FILE[] =	// ロゴテクスチャファイル
	{
		"data\\TEXTURE\\npn_title.png",	// タイトルテクスチャ
	};

	const char *TEX_START_FILE[] =	// スタートテクスチャファイル
	{
		"data\\TEXTURE\\title003.png",	// STARTテクスチャ
	};

	const int PRIORITY = 5;	// タイトルの優先順位

	namespace fade
	{
		const D3DXCOLOR COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);	// タイトルフェードの色

		const float SUB_ALPHA = 0.008f;	// α値の減算量
	}

	namespace logo
	{
		const D3DXVECTOR3 POS = D3DXVECTOR3(1100.0f,  290.0f, 0.0f);	// タイトルロゴの位置
		const D3DXVECTOR3 STOP_POS = D3DXVECTOR3(130.0f, 290.0f, 0.0f);	// タイトルロゴの位置

		const D3DXVECTOR3 SIZE = D3DXVECTOR3(450.0f, 600.0f, 0.0f);	// タイトルロゴの大きさ
		const D3DXVECTOR3 INIT_SIZE = SIZE * 0.0f;	// タイトルロゴの初期の大きさ

		const D3DXCOLOR COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// タイトルロゴの色

		const float		  MOVE  = 10.0f;	// タイトルロゴの移動量
		const float	INIT_SCALE	= 0.0f;		// タイトルロゴの初期拡大率
		const float	SUB_SCALE	= 0.1f;		// タイトルロゴ拡大率の加算量
		const int WAIT_TIME		= 40;		// タイトルロゴ移動までの待機時間
	}

	namespace select
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(350.0f, 560.0f, 0.0f);	// 選択の位置
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(560.0f, 0.0f, 0.0f);		// 選択の空間
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(420.0f, 140.0f, 0.0f);	// 選択の大きさ

		const D3DXCOLOR CHOICE_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 選択中カラー
		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// 非選択中カラー
	}

	namespace start
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(640.0f, 600.0f, 0.0f);	// スタートの位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(500.0f, 120.0f, 0.0f);	// スタートの大きさ

		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// スタートの初期色
		const D3DXCOLOR COL      = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// スタートの色

		const float INIT_ALPHA = 0.0f;	// 透明度の初期値
		const float SUB_ALPHA = 0.016f;	// 透明度の加算量

		const float	SUB_SCALE = 0.2f;	// スタートの拡大率の加算量
	}
}

//************************************************************
//	スタティックアサート
//************************************************************
//static_assert(NUM_ARRAY(TEX_LOGO_FILE)   == CTitleManager::LOGO_MAX,   "ERROR : LogoTexture Count Mismatch");
//static_assert(NUM_ARRAY(TEX_START_FILE)	 == CTitleManager::SELECT_MAX, "ERROR : SelectTexture Count Mismatch");

//************************************************************
//	親クラス [CTitleManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTitleManager::CTitleManager() :
	m_pLogo			(nullptr),		// タイトル表示の情報
	m_pFade			(nullptr),		// フェードの情報
	m_pStart		(nullptr),		// スタートの情報
	m_state			(STATE_NONE),	// 状態
	m_startState	(STARTSTATE_NONE),	// スタート状態
	m_fScale		(0.0f),			// タイトル拡大率
	m_nSelect		(0),			// 現在の選択
	m_nOldSelect	(0),			// 前回の選択
	m_nWaitCounter	(0)				// タイトル移動までの待機時間
{
	// メンバ変数をクリア
	//memset(&m_apLogo[0],	0, sizeof(m_apLogo));	// タイトル表示の情報
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
	// ポインタを宣言
	CTexture *pTexture = GET_MANAGER->GetTexture();	// テクスチャへのポインタ

	// メンバ変数を初期化
	m_pLogo			= nullptr;			// タイトル表示の情報
	m_pFade			= nullptr;			// フェードの情報
	m_pStart		= nullptr;			// スタートの情報
	m_state			= STATE_FADEOUT;	// 状態
	m_startState    = STARTSTATE_APPEAR;// スタート状態
	m_fScale		= logo::INIT_SCALE;	// タイトル拡大率
	m_fMove			= logo::MOVE;		// タイトルロゴの移動量
	m_nSelect		= 0;				// 現在の選択
	m_nOldSelect	= 0;				// 前回の選択
	m_nWaitCounter  = 0;				// タイトル移動までの待機時間

	//--------------------------------------------------------
	//	タイトルロゴの生成・設定
	//--------------------------------------------------------
	// タイトルロゴの生成
	m_pLogo = CObject2D::Create
	( // 引数
		logo::POS,			// 位置
		logo::INIT_SIZE		// 初期の大きさ
	);
	if (m_pLogo == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pLogo->BindTexture(pTexture->Regist(TEX_LOGO_FILE[0]));

	// 優先順位を設定
	m_pLogo->SetPriority(PRIORITY);

	// 描画をしない設定にする
	m_pLogo->SetEnableDraw(false);

	//--------------------------------------------------------
	//	スタートの生成・設定
	//--------------------------------------------------------
	// スタートの生成
	m_pStart = CObject2D::Create
	( // 引数
		start::POS,		// 位置
		start::SIZE,	// 大きさ
		VEC3_ZERO,		// 向き
		start::INIT_COL	// 色
	);
	if (m_pStart == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pStart->BindTexture(pTexture->Regist(TEX_START_FILE[0]));

	// 優先順位を設定
	m_pStart->SetPriority(PRIORITY);

	// 描画をしない設定にする
	m_pStart->SetEnableDraw(false);

	//--------------------------------------------------------
	//	フェードの生成・設定
	//--------------------------------------------------------
	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SCREEN_SIZE,	// 大きさ
		VEC3_ZERO,		// 向き
		fade::COL		// 色
	);
	if (m_pFade == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pFade->SetPriority(PRIORITY);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTitleManager::Uninit(void)
{
	// タイトルロゴの終了
	SAFE_UNINIT(m_pLogo);

	// フェードの終了
	SAFE_UNINIT(m_pFade);

	// スタートの終了
	SAFE_UNINIT(m_pStart);
}

//============================================================
//	更新処理
//============================================================
void CTitleManager::Update(const float fDeltaTime)
{
	// 遷移決定の更新
	UpdateStart();

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない状態

		// 無し

		break;

	case STATE_FADEOUT:	// フェードアウト状態

		// フェードアウトの更新
		UpdateFade();

		break;

	case STATE_SCALE:	// タイトル縮小状態

		// タイトル移動の更新
		UpdateScale();

		break;

	case STATE_MOVE:	// タイトル移動状態

		// タイトル移動の更新
		UpdateMove();

		break;

	case STATE_WAIT:	// 遷移待機状態

		// スタート点滅
		UpdateBlink();

		break;

	case STATE_TRANS:	// 遷移状態

		// 遷移
		UpdateTrans();

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// タイトルロゴの更新
	m_pLogo->Update(fDeltaTime);

	// フェードの更新
	m_pFade->Update(fDeltaTime);

	// スタートの更新
	m_pStart->Update(fDeltaTime);
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

//============================================================
//	フェードアウトの更新処理
//============================================================
void CTitleManager::UpdateFade(void)
{
	// 変数を宣言
	D3DXCOLOR colFade = m_pFade->GetColor();	// フェードの色

	if (colFade.a > 0.0f)
	{ // 透明になっていない場合

		// 透明度を減算
		colFade.a -= fade::SUB_ALPHA;
	}
	else
	{ // 透明になった場合

		// 透明度を補正
		colFade.a = 0.0f;

		// 描画をする設定にする
		m_pLogo->SetEnableDraw(true);

		// スタートを描画する設定にする
		m_pStart->SetEnableDraw(true);

		// 状態を変更
		m_state = STATE_SCALE;	// タイトル縮小状態
	}

	// 透明度を反映
	m_pFade->SetColor(colFade);
}

//============================================================
// タイトル拡大
//============================================================
void CTitleManager::UpdateScale(void)
{
	if (m_fScale < 1.0f)
	{ // 拡大率が最大値より小さい場合

		// 拡大率を加算
		m_fScale += logo::SUB_SCALE;

		// タイトルロゴの大きさを設定
		m_pLogo->SetVec3Sizing(logo::SIZE * m_fScale);
	}
	else
	{ // 拡大率が最大値以上の場合

		if (m_nWaitCounter >= logo::WAIT_TIME)
		{ // 一定時間経った場合

			// タイトル移動の待機時間を初期化
			m_nWaitCounter = 0;

			// 拡大率を補正
			m_fScale = 1.0f;

			// タイトルロゴの大きさを設定
			m_pLogo->SetVec3Sizing(logo::SIZE);

			// 状態を変更
			m_state = STATE_MOVE;	// タイトル移動状態

			// カメラの更新を再開
			GET_MANAGER->GetCamera()->SetEnableUpdate(true);

			// サウンドの再生
			GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
		}
		else
		{ // 一定時間経ってない場合

			// タイトル移動の待機時間加算
			m_nWaitCounter++;
		}
	}
}

//============================================================
//	タイトル移動の更新処理
//============================================================
void CTitleManager::UpdateMove(void)
{
	D3DXVECTOR3 pos;		// 位置

	// 位置取得
	pos = m_pLogo->GetVec3Position();

	if (pos.x >= 130.0f)
	{ // 特定の位置に到達するまで

		// 位置更新
		pos.x -= m_fMove;

		// 位置を設定
		m_pLogo->SetVec3Position(pos);
	}
	else
	{
		// 状態を変更
		m_state = STATE_WAIT;	// 遷移待機状態

		// カメラの更新を再開
		GET_MANAGER->GetCamera()->SetEnableUpdate(true);

		// サウンドの再生
		GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
	}
}

//============================================================
//	遷移決定の更新処理
//============================================================
void CTitleManager::UpdateStart(void)
{
	// ポインタを宣言
	CInputKeyboard	*pKeyboard	= GET_INPUTKEY;	// キーボード
	CInputPad		*pPad		= GET_INPUTPAD;	// パッド

	if (pKeyboard->IsTrigger(DIK_RETURN)
	||  pKeyboard->IsTrigger(DIK_SPACE)
	||  pPad->IsTrigger(CInputPad::KEY_A)
	||  pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X)
	||  pPad->IsTrigger(CInputPad::KEY_Y)
	||  pPad->IsTrigger(CInputPad::KEY_START))
	{
		if (m_state != STATE_WAIT && m_state != STATE_TRANS)
		{ // 遷移待機 && 遷移状態ではない場合

			// 演出スキップ
			SkipStaging();

			// サウンドの再生
			GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// 決定音01
		}
		else if(m_state != STATE_TRANS)
		{ // 遷移待機状態の場合

			// 遷移状態にする
			m_state = STATE_TRANS;
		}
	}
}

//============================================================
// スタート点滅
//============================================================
void CTitleManager::UpdateBlink(void)
{
	D3DXCOLOR col;		// 色

	if (GET_MANAGER->GetFade()->GetState() == CFade::FADE_NONE)
	{ // フェード中ではない場合

		// 色取得
		col = m_pStart->GetColor();

		if (m_startState == STARTSTATE_APPEAR)
		{ // 出現状態の場合

			// 透明度加算
			col.a += start::SUB_ALPHA;

			if (col.a >= 1.0f)
			{ // 完全に不透明の場合

				// 透明度補正
				col.a = 1.0f;

				// 消滅状態にする
				m_startState = STARTSTATE_DISAPPEAR;
			}
		}
		else if (m_startState == STARTSTATE_DISAPPEAR)
		{ // 消滅状態の場合

			// 透明度加算
			col.a -= start::SUB_ALPHA;

			if (col.a <= 0.0f)
			{ // 完全に透明の場合

				// 透明度補正
				col.a = 0.0f;

				// 消滅状態にする
				m_startState = STARTSTATE_APPEAR;
			}
		}
		
		// 色設定
		m_pStart->SetColor(col);
	}
}

//============================================================
// 遷移
//============================================================
void CTitleManager::UpdateTrans(void)
{
	if (GET_MANAGER->GetFade()->GetState() == CFade::FADE_NONE)
	{ // フェード中ではない場合

		D3DXCOLOR col;	// 色

		// スタートの色を取得
		col = m_pStart->GetColor();

		if (col.a >= 0.0f)
		{ // 透明じゃない場合

			// 拡大率を加算
			m_fScale += start::SUB_SCALE;
			col.a -= start::SUB_ALPHA;

			// スタートの大きさを設定
			m_pStart->SetVec3Sizing(start::SIZE * m_fScale);

			// スタートの色を設定
			m_pStart->SetColor(col);
		}
		else
		{ // 透明の場合

			// 状態を変更
			m_state = STATE_NONE;	// 何もしない状態

			// シーンの設定
			GET_MANAGER->SetScene(CScene::MODE_GAME);	// ゲーム画面

			// サウンドの再生
			GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// 決定音00
		}
	}
}

//============================================================
//	演出スキップ処理
//============================================================
void CTitleManager::SkipStaging(void)
{
	// タイトルロゴの大きさを設定
	m_pLogo->SetVec3Sizing(logo::SIZE);

	// タイトルロゴの位置を設定
	m_pLogo->SetVec3Position(logo::STOP_POS);

	// 描画をする設定にする
	m_pLogo->SetEnableDraw(true);

	// フェードを透明にする
	m_pFade->SetColor(XCOL_ABLACK);

	// スタートを描画する設定にする
	m_pStart->SetEnableDraw(true);

	// カメラの更新を再開
	GET_MANAGER->GetCamera()->SetEnableUpdate(true);

	// 状態を変更
	m_state = STATE_WAIT;	// 遷移待機状態
}

