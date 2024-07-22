//============================================================
//
//	リザルトマネージャー処理 [resultManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "resultManager.h"
#include "retentionManager.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"
#include "string2D.h"
#include "scrollText2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// リザルトマネージャーの優先順位

	namespace fade
	{
		const float FADE_CENT		= 880.0f;	// フェード中心X座標
		const float	FADEWAIT_TIME	= 1.2f;		// フェード開始待機時間
		const float	WAIT_TIME		= 0.15f;	// フェード待機時間
		const float	ADD_MOVE		= 0.65f;	// フェード移動量
		const float	ADD_ACCEL_MOVE	= 2.1f;		// フェード加速移動量

		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(800.0f, 1770.0f, 0.0f);	// フェード大きさ
		const D3DXCOLOR	  COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);	// フェード色
		const D3DXVECTOR3 UP_POS	= D3DXVECTOR3(FADE_CENT, -900.0f, 0.0f);		// フェード上位置
		const D3DXVECTOR3 CENT_POS	= D3DXVECTOR3(FADE_CENT, SCREEN_CENT.y, 0.0f);	// フェード中央位置
		const D3DXVECTOR3 DOWN_POS	= D3DXVECTOR3(FADE_CENT, SCREEN_HEIGHT + 900.0f, 0.0f);			// フェード下位置
		const D3DXVECTOR3 UP_MIDDLE_POS		= D3DXVECTOR3(FADE_CENT, UP_POS.y + 300.0f, 0.0f);		// フェード上中央位置
		const D3DXVECTOR3 DOWN_MIDDLE_POS	= D3DXVECTOR3(FADE_CENT, DOWN_POS.y - 300.0f, 0.0f);	// フェード下中央位置
	}

	namespace text
	{
		const char *FONT = "data\\FONT\\零ゴシック.otf";	// フォントパス
		//const char *PASS = "data\\TEXT\\intro.txt";	// テキストパス
		const bool	ITALIC			= false;	// イタリック
		const float	CHAR_HEIGHT		= 45.0f;	// 文字縦幅
		const float	LINE_HEIGHT		= 62.0f;	// 行間縦幅
		const float	WAIT_TIME_NOR	= 0.105f;	// 文字表示の待機時間

		const D3DXVECTOR3 POS = D3DXVECTOR3(fade::FADE_CENT, 460.0f, 0.0f);	// テキスト位置
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// 横配置
		const CText2D::EAlignY	 ALIGN_Y = CText2D::YALIGN_TOP;		// 縦配置
	}
}

//************************************************************
//	親クラス [CResultManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CResultManager::CResultManager() :
	m_pFade		(nullptr),		// フェード情報
	m_pTitle	(nullptr),		// タイトル情報
	m_pGodItem	(nullptr),		// 神器タイトル情報
	m_pTime		(nullptr),		// タイムタイトル情報
	m_state		(STATE_NONE),	// 状態
	m_fMoveY	(0.0f),			// 縦移動量
	m_nCurTime	(0.0f)			// 現在の待機時間
{

}

//============================================================
//	デストラクタ
//============================================================
CResultManager::~CResultManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CResultManager::Init(void)
{
	// メンバ変数を初期化
	m_pFade		= nullptr;			// フェード情報
	m_pTitle	= nullptr;			// タイトル情報
	m_pGodItem	= nullptr;			// 神器タイトル情報
	m_pTime		= nullptr;			// タイムタイトル情報
	m_state		= STATE_FADEWAIT;	// 状態
	m_fMoveY	= 0.0f;				// 縦移動量
	m_nCurTime	= 0.0f;				// 現在の待機時間

	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		fade::UP_POS,	// 位置
		fade::SIZE,		// 大きさ
		VEC3_ZERO,		// 向き
		fade::COL		// 色
	);
	if (m_pFade == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを登録・割当
	m_pFade->BindTexture("data\\TEXTURE\\resultFade000.png");	// TODO

	// 優先順位を設定
	m_pFade->SetPriority(PRIORITY);

	// タイトルの生成
	m_pTitle = CScrollText2D::Create
	( // 引数
		text::FONT,				// フォントパス
		text::ITALIC,			// イタリック
		text::POS,				// 原点位置
		text::WAIT_TIME_NOR,	// 文字表示の待機時間
		text::CHAR_HEIGHT,		// 文字縦幅
		text::LINE_HEIGHT,		// 行間縦幅
		text::ALIGN_X,			// 横配置
		text::ALIGN_Y			// 縦配置
	);
	if (m_pTitle == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位をフェードより上にする
	m_pTitle->SetPriority(PRIORITY);

	// TODO：ここにタイトル
	m_pTitle->AddString(L"ここに");
	m_pTitle->AddString(L"　ステージ名");

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CResultManager::Uninit(void)
{
	// フェードの終了
	SAFE_UNINIT(m_pFade);
}

//============================================================
//	更新処理
//============================================================
void CResultManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:
		break;

	case STATE_FADEWAIT:

		// フェード待機の更新
		UpdateFadeWait(fDeltaTime);
		break;

	case STATE_FADEIN:

		// フェードインの更新
		UpdateFadeIn(fDeltaTime);
		break;

	case STATE_FADEIN_WAIT:

		// フェードイン待機の更新
		UpdateFadeInWait(fDeltaTime);
		break;

	case STATE_FADEIN_ACCEL:

		// フェードイン加速の更新
		UpdateFadeInAccel(fDeltaTime);
		break;

	case STATE_STAGE_TITLE:

		// ステージタイトルの更新
		UpdateStageTitle(fDeltaTime);
		break;

	case STATE_WAIT:

		// 待機の更新
		UpdateWait(fDeltaTime);

		// TODO
		m_state = STATE_FADEOUT;
		break;

	case STATE_FADEOUT:

		// フェードアウトの更新
		UpdateFadeOut(fDeltaTime);
		break;

	case STATE_FADEOUT_WAIT:

		// フェードアウト待機の更新
		UpdateFadeOutWait(fDeltaTime);
		break;

	case STATE_FADEOUT_ACCEL:

		// フェードアウト加速の更新
		UpdateFadeOutAccel(fDeltaTime);
		break;

	case STATE_END:

		// フェード中の場合抜ける
		if (GET_MANAGER->GetFade()->IsFade()) { break; }

		// 選択画面に遷移する
		GET_MANAGER->SetLoadScene(CScene::MODE_SELECT);
		return;

	default:	// 例外処理
		assert(false);
		break;
	}

	// フェードの更新
	m_pFade->Update(fDeltaTime);
}

//============================================================
//	生成処理
//============================================================
CResultManager *CResultManager::Create(void)
{
	// リザルトマネージャーの生成
	CResultManager *pResultManager = new CResultManager;
	if (pResultManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// リザルトマネージャーの初期化
		if (FAILED(pResultManager->Init()))
		{ // 初期化に失敗した場合

			// リザルトマネージャーの破棄
			SAFE_DELETE(pResultManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pResultManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CResultManager::Release(CResultManager *&prResultManager)
{
	// リザルトマネージャーの終了
	assert(prResultManager != nullptr);
	prResultManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prResultManager);
}

//============================================================
//	フェード待機の更新処理
//============================================================
void CResultManager::UpdateFadeWait(const float fDeltaTime)
{
	// タイマーを加算
	m_nCurTime += fDeltaTime;
	if (m_nCurTime >= fade::FADEWAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_nCurTime = 0;

		// フェードイン状態にする
		m_state = STATE_FADEIN;
	}
}

//============================================================
//	フェードインの更新処理
//============================================================
void CResultManager::UpdateFadeIn(const float fDeltaTime)
{
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// フェード位置

	// 移動量を加算
	m_fMoveY += fade::ADD_MOVE;

	// フェードの位置を加算
	posFade.y += m_fMoveY;

	if (posFade.y > fade::UP_MIDDLE_POS.y)
	{ // 現在位置が停止位置を超えた場合

		// フェードを停止位置に補正
		posFade.y = fade::UP_MIDDLE_POS.y;

		// 移動量を初期化
		m_fMoveY = 0.0f;

		// フェードイン待機状態にする
		m_state = STATE_FADEIN_WAIT;
	}

	m_pFade->SetVec3Position(posFade);	// フェード位置を反映
}

//============================================================
//	フェードイン待機の更新処理
//============================================================
void CResultManager::UpdateFadeInWait(const float fDeltaTime)
{
	// タイマーを加算
	m_nCurTime += fDeltaTime;
	if (m_nCurTime >= fade::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_nCurTime = 0;

		// フェードイン加速状態にする
		m_state = STATE_FADEIN_ACCEL;
	}
}

//============================================================
//	フェードイン加速の更新処理
//============================================================
void CResultManager::UpdateFadeInAccel(const float fDeltaTime)
{
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// フェード位置

	// 移動量を加算
	m_fMoveY += fade::ADD_ACCEL_MOVE;

	// フェードの位置を加算
	posFade.y += m_fMoveY;

	if (posFade.y > fade::CENT_POS.y)
	{ // 現在位置が停止位置を超えた場合

		// フェードを停止位置に補正
		posFade.y = fade::CENT_POS.y;

		// 移動量を初期化
		m_fMoveY = 0.0f;

		// ステージタイトルの文字送りを開始する
		m_pTitle->SetEnableScroll(true);

		// ステージタイトル状態にする
		m_state = STATE_STAGE_TITLE;
	}

	m_pFade->SetVec3Position(posFade);	// フェード位置を反映
}

//============================================================
//	ステージタイトルの更新処理
//============================================================
void CResultManager::UpdateStageTitle(const float fDeltaTime)
{
	// 文字送りが終了していない場合抜ける
	if (m_pTitle->IsScroll()) { return; }

	// 待機状態にする
	m_state = STATE_WAIT;
}

//============================================================
//	待機の更新処理
//============================================================
void CResultManager::UpdateWait(const float fDeltaTime)
{
#if 0
	D3DXCOLOR colControl = m_pControl->GetColor();	// 操作表示色

	if (colControl.a < control::MIN_COL.a)
	{ // 透明度が最低限より低い場合

		// 透明度を加算
		colControl.a += control::ADD_ALPHA;

		if (colControl.a > control::MIN_COL.a)
		{ // 透明度が超過した場合

			// 透明度を補正
			colControl.a = control::MIN_COL.a;
		}

		// 操作表示色を設定
		m_pControl->SetColor(colControl);
	}
	else
	{ // 透明度が最低限以上の場合

		// 変数を宣言
		float fAddAlpha = 0.0f;	// 透明度の加算量

		// 透明度を上げる
		m_fSinAlpha += control::ADD_SINROT;
		useful::NormalizeRot(m_fSinAlpha);	// 向き正規化

		// 透明度加算量を求める
		fAddAlpha = (control::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinAlpha) - 1.0f);

		// 操作表示色を設定
		m_pControl->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, control::BASIC_ALPHA + fAddAlpha));
	}
#endif
}

//============================================================
//	フェードアウトの更新処理
//============================================================
void CResultManager::UpdateFadeOut(const float fDeltaTime)
{
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// フェード位置

	// 移動量を加算
	m_fMoveY += fade::ADD_MOVE;

	// フェードの位置を加算
	posFade.y += m_fMoveY;

	if (posFade.y > fade::DOWN_MIDDLE_POS.y)
	{ // 現在位置が停止位置を超えた場合

		// フェードを停止位置に補正
		posFade.y = fade::DOWN_MIDDLE_POS.y;

		// 移動量を初期化
		m_fMoveY = 0.0f;

		// フェードアウト待機状態にする
		m_state = STATE_FADEOUT_WAIT;
	}

	m_pFade->SetVec3Position(posFade);	// フェード位置を反映
}

//============================================================
//	フェードアウト待機の更新処理
//============================================================
void CResultManager::UpdateFadeOutWait(const float fDeltaTime)
{
	// タイマーを加算
	m_nCurTime += fDeltaTime;
	if (m_nCurTime >= fade::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_nCurTime = 0;

		// フェードアウト加速状態にする
		m_state = STATE_FADEOUT_ACCEL;
	}
}

//============================================================
//	フェードアウト加速の更新処理
//============================================================
void CResultManager::UpdateFadeOutAccel(const float fDeltaTime)
{
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// フェード位置

	// 移動量を加算
	m_fMoveY += fade::ADD_ACCEL_MOVE;

	// フェードの位置を加算
	posFade.y += m_fMoveY;

	if (posFade.y > fade::DOWN_POS.y)
	{ // 現在位置が停止位置を超えた場合

		// フェードを停止位置に補正
		posFade.y = fade::DOWN_POS.y;

		// 移動量を初期化
		m_fMoveY = 0.0f;

		// 終了状態にする
		m_state = STATE_END;
	}

	m_pFade->SetVec3Position(posFade);	// フェード位置を反映
}
