//============================================================
//
//	ランキングマネージャー処理 [rankingManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "rankingManager.h"
#include "retentionManager.h"
#include "manager.h"
#include "camera.h"
#include "fade.h"
#include "loadtext.h"
#include "stage.h"
#include "object2D.h"
#include "player.h"
#include "string2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIO_BG = 5;	// 背景の優先順位
	const int PRIO_UI = 6;	// UIの優先順位

	namespace fade
	{
		const float	MOVE_INTIME	 = 0.2f;	// イン変動時間
		const float	MOVE_OUTTIME = 0.5f;	// アウト変動時間
		const float DEST_ALPHA	 = 0.5f;	// 目標透明度
		const float INIT_ALPHA	 = 0.0f;	// 初期透明度
		const float DIFF_ALPHA	 = DEST_ALPHA - INIT_ALPHA;	// 差分透明度
		const D3DXCOLOR INIT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, INIT_ALPHA);		// 初期色
		const D3DXVECTOR3 SIZE	 = D3DXVECTOR3(SCREEN_SIZE.x, 280.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 POS	 = D3DXVECTOR3(SCREEN_CENT.x, 548.0f, 0.0f);	// 位置
		const D3DXVECTOR3 ROT	 = VEC3_ZERO;	// 向き
	}

	namespace title
	{
		const char *TEXTURE		= "data\\TEXTURE\\get_magatama.png";	// テクスチャパス
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(632.0f, 184.0f, 0.0f);	// 大きさ
		const float	MOVE_TIME	= 0.68f;	// 移動時間
		const D3DXCOLOR DEST_COL	= XCOL_WHITE;			// 目標色
		const D3DXCOLOR INIT_COL	= XCOL_AWHITE;			// 初期色
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;	// 差分色
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(SCREEN_CENT.x, 505.0f, 0.0f);		// 目標位置
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// 初期位置
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// 差分位置
	}

	namespace line
	{
		const float	MOVE_TIME	= 0.5f;	// 移動時間
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, 590.0f, 0.0f);	// 位置
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(980.0f, 10.0f, 0.0f);		// 目標大きさ
		const D3DXVECTOR3 INIT_SIZE	= D3DXVECTOR3(0.0f, DEST_SIZE.y, 0.0f);	// 初期大きさ
		const D3DXVECTOR3 DIFF_SIZE	= DEST_SIZE - INIT_SIZE;				// 差分大きさ
	}

	namespace select
	{
		const wchar_t *STRING[] = { L"はい", L"いいえ" };	// 文字列
		const char *FONT = "data\\FONT\\玉ねぎ楷書激無料版v7改.ttf";	// フォントパス

		const bool	ITALIC		= false;	// イタリック
		const float	MOVE_TIME	= 0.68f;	// 移動時間
		const float	PLUS_TIME	= 0.25f;	// 経過の延長時間
		const float	WAIT_TIME	= 0.15f;	// 待機時間
		const float	CHAR_HEIGHT	= 80.0f;	// 文字縦幅
		const float	DEST_ALPHA	= 1.0f;		// 目標透明度
		const float	INIT_ALPHA	= 0.0f;		// 初期透明度
		const float	DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// 差分透明度

		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);		// 非選択時の色
		const D3DXCOLOR CHOICE_COL	= XCOL_YELLOW;								// 選択時の色
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, DEST_ALPHA);	// 目標色
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, INIT_ALPHA);	// 初期色
		const D3DXVECTOR3 SPACE		= D3DXVECTOR3(360.0f, 0.0f, 0.0f);			// 空白
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_CENTER;			// 横配置

		const D3DXVECTOR3 DEST_POS = D3DXVECTOR3(SCREEN_CENT.x - SPACE.x * 0.5f, 640.0f, 0.0f);	// 目標位置
		const D3DXVECTOR3 INIT_POS = DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);					// 初期位置
		const D3DXVECTOR3 DIFF_POS = DEST_POS - INIT_POS;										// 差分位置
	}

	namespace fall
	{
		const float	MOVE_TIME	= 0.5f;	// 移動時間
		const float DEST_ALPHA	= 0.0f;	// 目標透明度
		const float INIT_ALPHA	= 1.0f;	// 初期透明度
		const float DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// 差分透明度
		const D3DXCOLOR DEST_COL	 = D3DXCOLOR(1.0f, 1.0f, 1.0f, DEST_ALPHA);	// 目標色
		const D3DXCOLOR INIT_COL	 = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// 初期色
		const D3DXCOLOR DIFF_COL	 = DEST_COL - INIT_COL;				// 差分色
		const D3DXVECTOR3 OFFSET_POS = D3DXVECTOR3(0.0f, 30.0f, 0.0f);	// 初期位置オフセット
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CRankingManager::AFuncUpdateState CRankingManager::m_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,							// 何もしない更新
	&CRankingManager::UpdateFadeOut,	// フェードアウト更新
	&CRankingManager::UpdateLine,		// 下線出現更新
	&CRankingManager::UpdateTitle,		// タイトル出現更新
	&CRankingManager::UpdateSelect,		// 選択肢表示更新
	&CRankingManager::UpdateWait,		// 待機更新
	&CRankingManager::UpdateFadeIn,		// フェードイン更新
	&CRankingManager::UpdateFall,		// UI消失更新
	&CRankingManager::UpdateEnd,		// 終了更新
};

//************************************************************
//	親クラス [CRankingManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CRankingManager::CRankingManager() :
	m_pFade		 (nullptr),		// フェード情報
	m_pTitle	 (nullptr),		// タイトル情報
	m_pLine		 (nullptr),		// 下線情報
	m_state		 (STATE_NONE),	// 状態
	m_fCurTime	 (0.0f),		// 現在の待機時間
	m_nCurSelect (0),			// 現在の選択肢
	m_nOldSelect (0)			// 前回の選択肢
{
	// メンバ変数をクリア
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択肢情報

	// スタティックアサート
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CRankingManager::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	デストラクタ
//============================================================
CRankingManager::~CRankingManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CRankingManager::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択肢情報
	m_state		 = STATE_FADEOUT;	// 状態
	m_pFade		 = nullptr;	// フェード情報
	m_pTitle	 = nullptr;	// タイトル情報
	m_pLine		 = nullptr;	// 下線情報
	m_fCurTime	 = 0.0f;	// 現在の待機時間
	m_nCurSelect = 0;		// 現在の選択肢
	m_nOldSelect = 0;		// 前回の選択肢

	//--------------------------------------------------------
	//	フェードの生成 / 初期設定
	//--------------------------------------------------------
	// フェードの生成
	m_pFade = CObject2D::Create
	( // 引数
		fade::POS,		// 位置
		fade::SIZE,		// 大きさ
		fade::ROT,		// 向き
		fade::INIT_COL	// 色
	);
	if (m_pFade == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pFade->SetPriority(PRIO_BG);

	// ラベルを設定
	m_pFade->SetLabel(CObject::LABEL_UI);

	//--------------------------------------------------------
	//	タイトルの生成 / 初期設定
	//--------------------------------------------------------
	// タイトルの生成
	m_pTitle = CObject2D::Create
	( // 引数
		title::INIT_POS,	// 位置
		title::SIZE,		// 大きさ
		VEC3_ZERO,			// 向き
		title::INIT_COL		// 色
	);
	if (m_pTitle == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを割当
	m_pTitle->BindTexture(title::TEXTURE);

	// 優先順位を設定
	m_pTitle->SetPriority(PRIO_UI);

	// ラベルを設定
	m_pTitle->SetLabel(CObject::LABEL_UI);

	//--------------------------------------------------------
	//	下線の生成 / 初期設定
	//--------------------------------------------------------
	// 下線の生成
	m_pLine = CObject2D::Create
	( // 引数
		line::POS,		// 位置
		line::INIT_SIZE	// 大きさ
	);
	if (m_pLine == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pLine->SetPriority(PRIO_UI);

	// ラベルを設定
	m_pLine->SetLabel(CObject::LABEL_UI);

	//--------------------------------------------------------
	//	選択肢の生成 / 初期設定
	//--------------------------------------------------------
	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// 生成位置を計算
		D3DXVECTOR3 posIcon = select::INIT_POS + (select::SPACE * (float)i);	// 生成位置

		// 選択肢の生成
		m_apSelect[i] = CString2D::Create
		( // 引数
			select::FONT,			// フォントパス
			select::ITALIC,			// イタリック
			select::STRING[i],		// 指定文字列
			posIcon,				// 原点位置
			select::CHAR_HEIGHT,	// 文字縦幅
			select::ALIGN_X,		// 横配置
			VEC3_ZERO,				// 原点向き
			select::INIT_COL		// 色
		);
		if (m_apSelect[i] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apSelect[i]->SetPriority(PRIO_UI);

		// ラベルを設定
		m_apSelect[i]->SetLabel(CObject::LABEL_UI);

		// 自動描画をOFFにする
		m_apSelect[i]->SetEnableDraw(false);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CRankingManager::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CRankingManager::Update(const float fDeltaTime)
{
	assert(m_state > NONE_IDX && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // 更新関数が指定されている場合

		// 各状態ごとの更新
		(this->*(m_aFuncUpdateState[m_state]))(fDeltaTime);
	}

	// スキップ操作の更新
	UpdateSkip();
}

//============================================================
//	生成処理
//============================================================
CRankingManager *CRankingManager::Create(void)
{
	// ランキングマネージャーの生成
	CRankingManager *pRankingManager = new CRankingManager;
	if (pRankingManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ランキングマネージャーの初期化
		if (FAILED(pRankingManager->Init()))
		{ // 初期化に失敗した場合

			// ランキングマネージャーの破棄
			SAFE_DELETE(pRankingManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pRankingManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CRankingManager::Release(CRankingManager *&prRankingManager)
{
	// ランキングマネージャーの終了
	assert(prRankingManager != nullptr);
	prRankingManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prRankingManager);
}

//============================================================
//	フェードアウトの更新処理
//============================================================
void CRankingManager::UpdateFadeOut(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, fade::MOVE_OUTTIME);

	// フェードの透明度を反映
	m_pFade->SetAlpha(fade::INIT_ALPHA + (fade::DIFF_ALPHA * fRate));

	if (m_fCurTime >= fade::MOVE_OUTTIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// フェードの透明度を補正
		m_pFade->SetAlpha(fade::DEST_ALPHA);

		// 下線出現状態にする
		m_state = STATE_LINE;
	}
}

//============================================================
//	下線出現の更新処理
//============================================================
void CRankingManager::UpdateLine(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, line::MOVE_TIME);

	// 下線の大きさを反映
	m_pLine->SetVec3Sizing(line::INIT_SIZE + (line::DIFF_SIZE * fRate));

	if (m_fCurTime >= line::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 下線の大きさを補正
		m_pLine->SetVec3Sizing(line::DEST_SIZE);

		// タイトル出現状態にする
		m_state = STATE_TITLE;
	}
}

//============================================================
//	タイトル出現の更新処理
//============================================================
void CRankingManager::UpdateTitle(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, title::MOVE_TIME);

	// タイトルの色を反映
	m_pTitle->SetColor(title::INIT_COL + (title::DIFF_COL * fRate));

	// タイトルの位置を反映
	m_pTitle->SetVec3Position(title::INIT_POS + (title::DIFF_POS * fRate));

	if (m_fCurTime >= title::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// タイトルの色を補正
		m_pTitle->SetColor(title::DEST_COL);

		// タイトルの位置を補正
		m_pTitle->SetVec3Position(title::DEST_POS);

		for (int i = 0; i < SELECT_MAX; i++)
		{ // 選択肢の総数分繰り返す

			// 選択肢の自動描画をONにする
			m_apSelect[i]->SetEnableDraw(true);
		}

		// 選択肢表示状態にする
		m_state = STATE_SELECT;
	}
}

//============================================================
//	選択肢表示の更新処理
//============================================================
void CRankingManager::UpdateSelect(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 選択肢の移動
	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// アイコン背景それぞれの経過時間を計算
		float fRateTime = m_fCurTime - (select::PLUS_TIME * (float)i);
		useful::LimitNum(fRateTime, 0.0f, select::MOVE_TIME);	// 経過時間を補正

		// それぞれの経過時刻から割合を計算
		float fRate = easeing::InOutQuad(fRateTime, 0.0f, select::MOVE_TIME);

		// 選択肢の位置を計算
		D3DXVECTOR3 posInit = select::INIT_POS + (select::SPACE * (float)i);

		// 選択肢の色を計算
		D3DXCOLOR colCur = select::INIT_COL;
		colCur.a = select::INIT_ALPHA + (select::DIFF_ALPHA * fRate);	// 現在の透明度を設定

		// 選択肢の位置を反映
		m_apSelect[i]->SetVec3Position(posInit + (select::DIFF_POS * fRate));

		// 選択肢の色を反映
		m_apSelect[i]->SetColor(colCur);
	}

	// 選択肢の移動補正
	if (m_fCurTime >= select::MOVE_TIME + select::PLUS_TIME * (SELECT_MAX - 1))
	{ // 全選択肢の待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		for (int i = 0; i < SELECT_MAX; i++)
		{ // 選択肢の総数分繰り返す

			// 選択肢の目標生成位置を計算
			D3DXVECTOR3 posDest = select::DEST_POS + (select::SPACE * (float)i);

			// 選択肢の位置を補正
			m_apSelect[i]->SetVec3Position(posDest);

			// 選択肢の色を補正
			m_apSelect[i]->SetColor(select::DEST_COL);
		}

		// 待機状態にする
		m_state = STATE_WAIT;
	}
}

//============================================================
//	待機の更新処理
//============================================================
void CRankingManager::UpdateWait(const float fDeltaTime)
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// キーボード情報
	CInputPad*		pPad = GET_INPUTPAD;	// パッド情報

	//--------------------------------------------------------
	//	選択肢操作
	//--------------------------------------------------------
	// 前回の選択肢を保存
	m_nOldSelect = m_nCurSelect;

	// 選択肢操作
	if (pKey->IsTrigger(DIK_LEFT)
	||  pPad->IsTrigger(CInputPad::KEY_LEFT)
	||  pPad->GetTriggerLStick(CInputPad::STICK_LEFT)
	||  pPad->GetTriggerRStick(CInputPad::STICK_LEFT))
	{
		// 左に選択をずらす
		m_nCurSelect = (m_nCurSelect + 1) % SELECT_MAX;
	}
	if (pKey->IsTrigger(DIK_RIGHT)
	||  pPad->IsTrigger(CInputPad::KEY_RIGHT)
	||  pPad->GetTriggerLStick(CInputPad::STICK_RIGHT)
	||  pPad->GetTriggerRStick(CInputPad::STICK_RIGHT))
	{
		// 右に選択をずらす
		m_nCurSelect = (m_nCurSelect + (SELECT_MAX - 1)) % SELECT_MAX;
	}

	// 前回の選択要素の色を白色に設定
	m_apSelect[m_nOldSelect]->SetColor(select::DEFAULT_COL);

	// 現在の選択要素の色を黄色に設定
	m_apSelect[m_nCurSelect]->SetColor(select::CHOICE_COL);

	//--------------------------------------------------------
	//	選択肢決定
	//--------------------------------------------------------
	if (pKey->IsTrigger(DIK_RETURN)
	||  pPad->IsTrigger(CInputPad::KEY_A))
	{
		// フェードイン状態にする
		m_state = STATE_FADEIN;
	}
}

//============================================================
//	フェードインの更新処理
//============================================================
void CRankingManager::UpdateFadeIn(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::OutQuad(m_fCurTime, fade::MOVE_INTIME, 0.0f);

	// フェードの透明度を反映
	m_pFade->SetAlpha(fade::INIT_ALPHA + (fade::DIFF_ALPHA * fRate));

	if (m_fCurTime >= fade::MOVE_INTIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// フェードの透明度を補正
		m_pFade->SetAlpha(fade::INIT_ALPHA);

		// UI消失状態にする
		m_state = STATE_FALL;
	}
}

//============================================================
//	UI消失の更新処理
//============================================================
void CRankingManager::UpdateFall(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, fall::MOVE_TIME);

	// 色を反映
	m_pTitle->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// タイトル情報
	m_pLine->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// 下線情報

	// 位置を反映
	m_pTitle->SetVec3Position(title::DEST_POS + (((title::DEST_POS + fall::OFFSET_POS) - title::DEST_POS) * fRate));	// タイトル情報
	m_pLine->SetVec3Position(line::POS + (((line::POS + fall::OFFSET_POS) - line::POS) * fRate));	// 下線情報

	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		D3DXVECTOR3 posSelect = select::DEST_POS + (((select::DEST_POS + fall::OFFSET_POS) - select::DEST_POS) * fRate);	// 位置
		D3DXVECTOR3 offsetSelect = (select::SPACE * (float)i);	// オフセット

		// 位置を反映
		m_apSelect[i]->SetVec3Position(posSelect + offsetSelect);

		// 色を反映
		m_apSelect[i]->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));
	}

	if (m_fCurTime >= fall::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 色を補正
		m_pTitle->SetColor(fall::DEST_COL);	// タイトル情報
		m_pLine->SetColor(fall::DEST_COL);	// 下線情報

		switch (m_nCurSelect)
		{ // 選択肢ごとの処理
		case SELECT_YES:
		{
			// 入場の設定
			GET_PLAYER->SetEnter();
			break;
		}
		case SELECT_NO:
		{
			// 通常状態にする
			GET_PLAYER->SetState(CPlayer::STATE_SELECT_NORMAL);
			break;
		}
		default:
			assert(false);
			break;
		};

		// 終了状態にする
		m_state = STATE_END;
	}
}

//============================================================
//	終了の更新処理
//============================================================
void CRankingManager::UpdateEnd(const float fDeltaTime)
{

}

//============================================================
//	スキップ操作の更新処理
//============================================================
void CRankingManager::UpdateSkip(void)
{
	if (m_state < STATE_WAIT)
	{ // 演出スキップが可能な場合

		CInputKeyboard*	pKey = GET_INPUTKEY;	// キーボード情報
		CInputPad*		pPad = GET_INPUTPAD;	// パッド情報
		if (pKey->IsTrigger(DIK_SPACE)
		||  pKey->IsTrigger(DIK_RETURN)
		||  pPad->IsTrigger(CInputPad::KEY_A)
		||  pPad->IsTrigger(CInputPad::KEY_B)
		||  pPad->IsTrigger(CInputPad::KEY_X)
		||  pPad->IsTrigger(CInputPad::KEY_Y))
		{
			// 演出のスキップ
			SkipStaging();
		}
	}
}

//============================================================
//	演出のスキップ処理
//============================================================
void CRankingManager::SkipStaging(void)
{
	// 待機状態にする
	m_state = STATE_WAIT;

	// フェードの透明度を補正
	m_pFade->SetAlpha(fade::DEST_ALPHA);

	// 下線の大きさを補正
	m_pLine->SetVec3Sizing(line::DEST_SIZE);

	// タイトルの色を補正
	m_pTitle->SetColor(title::DEST_COL);

	// タイトルの位置を補正
	m_pTitle->SetVec3Position(title::DEST_POS);
}
