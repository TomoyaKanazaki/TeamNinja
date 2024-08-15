//============================================================
//
//	神器獲得演出マネージャー処理 [godItemManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "godItemManager.h"
#include "retentionManager.h"
#include "manager.h"
#include "camera.h"
#include "fade.h"
#include "loadtext.h"
#include "stage.h"
#include "object2D.h"
#include "scrollString2D.h"

#include "sceneGame.h"
#include "gameManager.h"
#include "timerUI.h"
#include "player.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIO_BG = 5;	// 背景の優先順位
	const int PRIO_UI = 6;	// UIの優先順位

	namespace fade
	{
		const float	MOVE_TIME	 = 0.5f;	// 変動時間
		const float DEST_ALPHA	 = 0.5f;	// 目標透明度
		const float INIT_ALPHA	 = 0.0f;	// 初期透明度
		const float DIFF_ALPHA	 = DEST_ALPHA - INIT_ALPHA;	// 差分透明度
		const D3DXCOLOR INIT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, INIT_ALPHA);		// 初期色
		const D3DXVECTOR3 SIZE	 = D3DXVECTOR3(SCREEN_SIZE.x, 260.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 POS	 = D3DXVECTOR3(SCREEN_CENT.x, 555.0f, 0.0f);	// 位置
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
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(980.0f, 20.0f, 0.0f);		// 目標大きさ
		const D3DXVECTOR3 INIT_SIZE	= D3DXVECTOR3(0.0f, DEST_SIZE.y, 0.0f);	// 初期大きさ
		const D3DXVECTOR3 DIFF_SIZE	= DEST_SIZE - INIT_SIZE;				// 差分大きさ
	}

	namespace name
	{
		const char *FONT	= "data\\FONT\\零ゴシック.otf";	// フォントパス
		const bool	ITALIC	= false;			// イタリック
		const float	CHAR_HEIGHT		= 70.0f;	// 文字縦幅
		const float	WAIT_TIME_NOR	= 0.012f;	// 文字表示の待機時間
		const D3DXVECTOR3 POS = D3DXVECTOR3(SCREEN_CENT.x, 640.0f, 0.0f);	// 位置
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_CENTER;		// 横配置
	}

	namespace fall
	{
		const float	MOVE_TIME = 1.5f;	// 移動時間
		const D3DXCOLOR DEST_COL	 = XCOL_AWHITE;			// 目標色
		const D3DXCOLOR INIT_COL	 = XCOL_WHITE;			// 初期色
		const D3DXCOLOR DIFF_COL	 = DEST_COL - INIT_COL;	// 差分色
		const D3DXVECTOR3 OFFSET_POS = D3DXVECTOR3(0.0f, 30.0f, 0.0f);	// 初期位置オフセット
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CGodItemManager::AFuncUpdateState CGodItemManager::m_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,							// 何もしない更新
	&CGodItemManager::UpdateFadeOut,	// フェードアウト更新
	&CGodItemManager::UpdateLine,		// 下線出現更新
	&CGodItemManager::UpdateTitle,		// タイトル出現更新
	&CGodItemManager::UpdateName,		// 名前出現更新
	&CGodItemManager::UpdateWait,		// 待機更新
	&CGodItemManager::UpdateFadeIn,		// フェードイン更新
	&CGodItemManager::UpdateFall,		// UI消失更新
	&CGodItemManager::UpdateEnd,		// 終了更新
};
CGodItemManager* CGodItemManager::m_pThisClass = nullptr;	// 自身のインスタンス

//************************************************************
//	親クラス [CGodItemManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGodItemManager::CGodItemManager() : CObject(CObject::LABEL_MANAGER, CObject::SCENE_MAIN, CObject::DIM_2D, PRIO_UI),
	m_pFade		(nullptr),		// フェード情報
	m_pTitle	(nullptr),		// タイトル情報
	m_pLine		(nullptr),		// 下線情報
	m_pName		(nullptr),		// 名前情報
	m_state		(STATE_NONE),	// 状態
	m_fCurTime	(0.0f)			// 現在の待機時間
{
	// スタティックアサート
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CGodItemManager::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	デストラクタ
//============================================================
CGodItemManager::~CGodItemManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGodItemManager::Init(void)
{
	// メンバ変数を初期化
	m_state		= STATE_FADEOUT;	// 状態
	m_pFade		= nullptr;			// フェード情報
	m_pTitle	= nullptr;			// タイトル情報
	m_pLine		= nullptr;			// 下線情報
	m_pName		= nullptr;			// 名前情報
	m_fCurTime	= 0.0f;				// 現在の待機時間

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

	//--------------------------------------------------------
	//	名前の生成 / 初期設定
	//--------------------------------------------------------
	// 名前の生成
	m_pName = CScrollString2D::Create
	( // 引数
		name::FONT,				// フォントパス
		name::ITALIC,			// イタリック
		L"テキスト読込エラー",	// 表示文字列
		name::POS,				// 原点位置
		name::WAIT_TIME_NOR,	// 文字表示の待機時間
		name::CHAR_HEIGHT,		// 文字縦幅
		name::ALIGN_X			// 横配置
	);
	if (m_pName == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pName->SetPriority(PRIO_UI);

	// ラベルを設定
	m_pName->SetLabel(CObject::LABEL_NONE);	// 自動破棄/更新をしないラベル

	// カメラを神器獲得状態にする
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_GODITEM);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGodItemManager::Uninit(void)
{
	// フェードの終了
	SAFE_UNINIT(m_pFade);

	// タイトルの終了
	SAFE_UNINIT(m_pTitle);

	// 下線の終了
	SAFE_UNINIT(m_pLine);

	// 名前の終了
	SAFE_UNINIT(m_pName);

	// オブジェクトを破棄
	Release();

	// 自身の保存していたインスタンスを初期化
	m_pThisClass = nullptr;
}

//============================================================
//	更新処理
//============================================================
void CGodItemManager::Update(const float fDeltaTime)
{
	// フェードの更新
	m_pFade->Update(fDeltaTime);

	// タイトルの更新
	m_pTitle->Update(fDeltaTime);

	// 下線の更新
	m_pLine->Update(fDeltaTime);

	// 名前の更新
	m_pName->Update(fDeltaTime);

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
//	描画処理
//============================================================
void CGodItemManager::Draw(CShader * /*pShader*/)
{

}

//============================================================
//	生成処理
//============================================================
CGodItemManager *CGodItemManager::Create(const CGodItem::EType typeID)
{
	if (m_pThisClass != nullptr)
	{ // 自クラスの他インスタンスがある場合

		// インスタンスを破棄
		SAFE_UNINIT(m_pThisClass);
	}

	// 神器獲得演出マネージャーの生成
	CGodItemManager *pGodItemManager = new CGodItemManager;
	if (pGodItemManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 神器獲得演出マネージャーの初期化
		if (FAILED(pGodItemManager->Init()))
		{ // 初期化に失敗した場合

			// 神器獲得演出マネージャーの破棄
			SAFE_DELETE(pGodItemManager);
			return nullptr;
		}

		// 名前の文字列を設定
		pGodItemManager->SetName(typeID);

		// インスタンスを保存
		assert(m_pThisClass == nullptr);
		m_pThisClass = pGodItemManager;

		// 確保したアドレスを返す
		return pGodItemManager;
	}
}

//============================================================
//	フェードアウトの更新処理
//============================================================
void CGodItemManager::UpdateFadeOut(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, fade::MOVE_TIME);

	// フェードの透明度を反映
	m_pFade->SetAlpha(fade::INIT_ALPHA + (fade::DIFF_ALPHA * fRate));

	if (m_fCurTime >= fade::MOVE_TIME)
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
void CGodItemManager::UpdateLine(const float fDeltaTime)
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
void CGodItemManager::UpdateTitle(const float fDeltaTime)
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

		// 名前の文字送りを開始する
		m_pName->SetEnableScroll(true);

		// 待機状態にする
		m_state = STATE_WAIT;
	}
}

//============================================================
//	名前出現の更新処理
//============================================================
void CGodItemManager::UpdateName(const float fDeltaTime)
{
	if (!m_pName->IsScroll())
	{ // 文字送りが終了した場合

		// 待機状態にする
		m_state = STATE_WAIT;
	}
}

//============================================================
//	待機の更新処理
//============================================================
void CGodItemManager::UpdateWait(const float fDeltaTime)
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// キーボード情報
	CInputPad*		pPad = GET_INPUTPAD;	// パッド情報
	if (pKey->IsTrigger(DIK_SPACE)
	||  pKey->IsTrigger(DIK_RETURN)
	||  pPad->IsTrigger(CInputPad::KEY_A)
	||  pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X)
	||  pPad->IsTrigger(CInputPad::KEY_Y))
	{
		// タイマーの計測再開
		CSceneGame::GetTimerUI()->EnableStop(false);

		// プレイヤーの状態を元に戻す
		GET_PLAYER->SetEnableGodItem(false);

		// ゲームマネージャーを通常状態に戻す
		CSceneGame::GetGameManager()->SetState(CGameManager::STATE_NORMAL);

		// カメラを回り込み状態にする
		GET_MANAGER->GetCamera()->SetState(CCamera::STATE_AROUND);
		GET_MANAGER->GetCamera()->SetDestAround();

		// フェードイン状態にする
		m_state = STATE_FADEIN;
	}
}

//============================================================
//	フェードインの更新処理
//============================================================
void CGodItemManager::UpdateFadeIn(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::OutQuad(m_fCurTime, fade::MOVE_TIME, 0.0f);

	// フェードの透明度を反映
	m_pFade->SetAlpha(fade::INIT_ALPHA + (fade::DIFF_ALPHA * fRate));

	if (m_fCurTime >= fade::MOVE_TIME)
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
void CGodItemManager::UpdateFall(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, fall::MOVE_TIME);

	// 色を反映
	m_pTitle->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// タイトル情報
	m_pLine->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// 下線情報
	m_pName->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// 名前情報

	// 位置を反映
	m_pTitle->SetVec3Position(title::DEST_POS + (((title::DEST_POS + fall::OFFSET_POS) - title::DEST_POS) * fRate));	// タイトル情報
	m_pLine->SetVec3Position(line::POS + (((line::POS + fall::OFFSET_POS) - line::POS) * fRate));	// 下線情報
	m_pName->SetVec3Position(name::POS + (((name::POS + fall::OFFSET_POS) - name::POS) * fRate));	// 名前情報

	if (m_fCurTime >= fall::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 色を補正
		m_pTitle->SetColor(fall::DEST_COL);	// タイトル情報
		m_pLine->SetColor(fall::DEST_COL);	// 下線情報
		m_pName->SetColor(fall::DEST_COL);	// 名前情報

		// 終了状態にする
		m_state = STATE_END;
	}
}

//============================================================
//	終了の更新処理
//============================================================
void CGodItemManager::UpdateEnd(const float fDeltaTime)
{
	// 自身の終了
	Uninit();
}

//============================================================
//	名前の文字列設定処理
//============================================================
void CGodItemManager::SetName(const CGodItem::EType typeID)
{
	// テキストを割当
	loadtext::BindString(m_pName, loadtext::LoadText(GET_STAGE->GetCurMapGodItemPass().c_str(), (int)typeID));
}

//============================================================
//	スキップ操作の更新処理
//============================================================
void CGodItemManager::UpdateSkip(void)
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
void CGodItemManager::SkipStaging(void)
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

	// 名前の文字をすべて表示
	m_pName->SetEnableDraw(true);

	// 神器獲得カメラの目標位置にする
	GET_MANAGER->GetCamera()->SetDestGodItem();
}
