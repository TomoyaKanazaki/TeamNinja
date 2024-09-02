//============================================================
//
//	クリア失敗マネージャー処理 [clearFailManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "clearFailManager.h"
#include "manager.h"
#include "string2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY	= 6;		// クリア失敗マネージャーの優先順位
	const float	FADE_CENT	= 880.0f;	// フェード中心X座標

	namespace cont
	{
		const char		*FONT	= "data\\FONT\\零ゴシック.otf";	// フォントパス
		const wchar_t	*STRING	= L"任務をやり直す";			// 文字列

		const bool	ITALIC		= false;	// イタリック
		const float	MOVE_TIME	= 0.2f;		// 移動時間
		const float	WAIT_TIME	= 0.5f;		// タイトル待機時間
		const float	DEST_HEIGHT	= 130.0f;	// 文字目標縦幅
		const float	INIT_HEIGHT	= DEST_HEIGHT * 14.0f;			// 文字初期縦幅
		const float	DIFF_HEIGHT	= DEST_HEIGHT - INIT_HEIGHT;	// 文字差分縦幅

		const D3DXVECTOR3 POS = D3DXVECTOR3(FADE_CENT, 360.0f, 0.0f);	// 位置
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_CENTER;	// 横配置
	}

	namespace select
	{
		const wchar_t *STRING[] = { L"はい", L"いいえ" };	// 文字列
		const char *FONT = "data\\FONT\\零ゴシック.otf";	// フォントパス

		const bool	ITALIC		= false;	// イタリック
		const float	MOVE_TIME	= 0.68f;	// 移動時間
		const float	PLUS_TIME	= 0.25f;	// 経過の延長時間
		const float	WAIT_TIME	= 0.15f;	// 待機時間
		const float	CHAR_HEIGHT	= 100.0f;	// 文字縦幅
		const float	DEST_ALPHA	= 1.0f;		// 目標透明度
		const float	INIT_ALPHA	= 0.0f;		// 初期透明度
		const float	DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// 差分透明度

		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);		// 非選択時の色
		const D3DXCOLOR CHOICE_COL	= XCOL_YELLOW;								// 選択時の色
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, DEST_ALPHA);	// 目標色
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, INIT_ALPHA);	// 初期色
		const D3DXVECTOR3 SPACE		= D3DXVECTOR3(360.0f, 0.0f, 0.0f);			// 空白
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_CENTER;			// 横配置

		const D3DXVECTOR3 DEST_POS = D3DXVECTOR3(FADE_CENT - SPACE.x * 0.5f, 540.0f, 0.0f);	// 目標位置
		const D3DXVECTOR3 INIT_POS = DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);				// 初期位置
		const D3DXVECTOR3 DIFF_POS = DEST_POS - INIT_POS;									// 差分位置
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CClearFailManager::AFuncUpdateState CClearFailManager::m_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,										// 何もしない更新
	&CClearFailManager::UpdateContinueTitleWait,	// コンテニュータイトル待機更新
	&CClearFailManager::UpdateContinueTitle,		// コンテニュータイトル表示更新
	&CClearFailManager::UpdateSelectWait,			// 選択肢待機更新
	&CClearFailManager::UpdateSelect,				// 選択肢表示更新
	&CClearFailManager::UpdateWait,					// 待機更新
	nullptr,										// 終了更新
};

//************************************************************
//	親クラス [CClearFailManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CClearFailManager::CClearFailManager() :
	m_pContinue	 (nullptr),		// コンテニュー情報
	m_state		 (STATE_NONE),	// 状態
	m_fCurTime	 (0.0f),		// 現在の待機時間
	m_nCurSelect (0),			// 現在の選択肢
	m_nOldSelect (0)			// 前回の選択肢
{
	// メンバ変数をクリア
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択肢情報

	// スタティックアサート
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CClearFailManager::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	デストラクタ
//============================================================
CClearFailManager::~CClearFailManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CClearFailManager::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択肢情報
	m_state		 = STATE_CONTINUE_TITLE_WAIT;		// 状態
	m_pContinue	 = nullptr;	// コンテニュー情報
	m_fCurTime	 = 0.0f;	// 現在の待機時間
	m_nCurSelect = 0;		// 現在の選択肢
	m_nOldSelect = 0;		// 前回の選択肢

	//--------------------------------------------------------
	//	コンテニューの生成 / 初期設定
	//--------------------------------------------------------
	// コンテニューの生成
	m_pContinue = CString2D::Create
	( // 引数
		cont::FONT,			// フォントパス
		cont::ITALIC,		// イタリック
		cont::STRING,		// 指定文字列
		cont::POS,			// 原点位置
		cont::INIT_HEIGHT,	// 文字縦幅
		cont::ALIGN_X		// 横配置
	);
	if (m_pContinue == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pContinue->SetPriority(PRIORITY);

	// 自動描画をOFFにする
	m_pContinue->SetEnableDraw(false);

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
		m_apSelect[i]->SetPriority(PRIORITY);

		// 自動描画をOFFにする
		m_apSelect[i]->SetEnableDraw(false);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CClearFailManager::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CClearFailManager::Update(const float fDeltaTime)
{
	assert(m_state > NONE_IDX && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // 更新関数が指定されている場合

		// 各状態ごとの更新
		(this->*(m_aFuncUpdateState[m_state]))(fDeltaTime);
	}
}

//============================================================
//	演出のスキップ処理
//============================================================
void CClearFailManager::SkipStaging(void)
{
	// 待機状態にする
	m_state = STATE_WAIT;

	// コンテニューを演出後の見た目にする
	m_pContinue->SetEnableDraw(true);				// 自動描画をONにする
	m_pContinue->SetCharHeight(cont::DEST_HEIGHT);	// 目標サイズに設定

	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// 選択肢の目標生成位置を計算
		D3DXVECTOR3 posDest = select::DEST_POS + (select::SPACE * (float)i);

		// 選択肢を演出後の見た目にする
		m_apSelect[i]->SetEnableDraw(true);			// 自動描画をONにする
		m_apSelect[i]->SetVec3Position(posDest);	// 目標位置に設定
		m_apSelect[i]->SetColor(select::DEST_COL);	// 目標色に設定
	}
}

//============================================================
//	全UIオブジェクトの移動処理
//============================================================
void CClearFailManager::SetAllMove(const D3DXVECTOR3& rMove)
{
	// コンテニューの位置を移動
	m_pContinue->SetVec3Position(m_pContinue->GetVec3Position() + rMove);

	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// 選択肢の位置を移動
		m_apSelect[i]->SetVec3Position(m_apSelect[i]->GetVec3Position() + rMove);
	}
}

//============================================================
//	コンテニュータイトル待機の更新処理
//============================================================
void CClearFailManager::UpdateContinueTitleWait(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= cont::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// コンテニュータイトルの自動描画をONにする
		m_pContinue->SetEnableDraw(true);

		// コンテニュータイトル表示状態にする
		m_state = STATE_CONTINUE_TITLE;
	}
}

//============================================================
//	コンテニュータイトル表示の更新処理
//============================================================
void CClearFailManager::UpdateContinueTitle(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, cont::MOVE_TIME);

	// コンテニュータイトルの大きさを反映
	m_pContinue->SetCharHeight(cont::INIT_HEIGHT + (cont::DIFF_HEIGHT * fRate));

	if (m_fCurTime >= cont::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// コンテニュータイトルの大きさを補正
		m_pContinue->SetCharHeight(cont::DEST_HEIGHT);

		// 選択肢待機状態にする
		m_state = STATE_SELECT_WAIT;
	}
}

//============================================================
//	選択肢待機の更新処理
//============================================================
void CClearFailManager::UpdateSelectWait(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= select::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

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
void CClearFailManager::UpdateSelect(const float fDeltaTime)
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
void CClearFailManager::UpdateWait(const float fDeltaTime)
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
	|| pPad->IsTrigger(CInputPad::KEY_A)
	|| pPad->IsTrigger(CInputPad::KEY_B))
	{
		switch (m_nCurSelect)
		{ // 選択肢ごとの処理
		case SELECT_YES:

			// 同じマップを次のマップに指定
			SetNext(CScene::MODE_GAME);
			break;

		case SELECT_NO:

			// セレクトマップを次のマップに指定
			SetNext(CScene::MODE_SELECT);
			break;

		default:
			assert(false);
			break;
		}

		// 終了状態にする
		m_state = STATE_END;
	}
}
