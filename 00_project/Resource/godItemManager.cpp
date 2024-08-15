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

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// 神器獲得演出マネージャーの優先順位

	namespace title
	{
		const char *TEXTURE		= "data\\TEXTURE\\get_magatama.png";	// テクスチャパス
		const float	MOVE_TIME	= 0.68f;	// 移動時間
		const float	WAIT_TIME	= 0.15f;	// 数値待機時間
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(632.0f, 184.0f, 0.0f);	// 大きさ
		const D3DXCOLOR DEST_COL	= XCOL_WHITE;			// 目標色
		const D3DXCOLOR INIT_COL	= XCOL_AWHITE;			// 初期色
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;	// 差分色
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(SCREEN_CENT.x, 445.0f, 0.0f);		// 目標位置
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// 初期位置
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// 差分位置
	}

	namespace line
	{
		const float	MOVE_TIME	= 0.5f;	// 移動時間
		const float	WAIT_TIME	= 0.5f;	// 待機時間
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, 530.0f, 0.0f);	// 位置
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(980.0f, 20.0f, 0.0f);		// 目標大きさ
		const D3DXVECTOR3 INIT_SIZE	= D3DXVECTOR3(0.0f, DEST_SIZE.y, 0.0f);	// 初期大きさ
		const D3DXVECTOR3 DIFF_SIZE	= DEST_SIZE - INIT_SIZE;				// 差分大きさ
	}

	namespace name
	{
		const char *FONT	= "data\\FONT\\零ゴシック.otf";	// フォントパス
		const bool	ITALIC	= false;			// イタリック
		const float	WAIT_TIME		= 0.5f;		// 待機時間
		const float	CHAR_HEIGHT		= 70.0f;	// 文字縦幅
		const float	WAIT_TIME_NOR	= 0.012f;	// 文字表示の待機時間
		const D3DXVECTOR3 POS = D3DXVECTOR3(SCREEN_CENT.x, 580.0f, 0.0f);	// 位置
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_CENTER;		// 横配置
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CGodItemManager::AFuncUpdateState CGodItemManager::m_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,						// 何もしない更新
	&CGodItemManager::UpdateLine,	// 下線出現更新
	&CGodItemManager::UpdateTitle,	// タイトル出現更新
	&CGodItemManager::UpdateName,	// 名前出現更新
	&CGodItemManager::UpdateWait,	// 待機更新
	&CGodItemManager::UpdateEnd,	// 終了更新
};

//************************************************************
//	親クラス [CGodItemManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGodItemManager::CGodItemManager() :
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
	m_state		= STATE_LINE;	// 状態	// TODO：あとで変わる
	m_pTitle	= nullptr;	// タイトル情報
	m_pLine		= nullptr;	// 下線情報
	m_pName		= nullptr;	// 名前情報
	m_fCurTime	= 0.0f;		// 現在の待機時間

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
	m_pTitle->SetPriority(PRIORITY);

	// ラベルを設定
	m_pTitle->SetLabel(CObject::LABEL_UI);	// 自動破棄/更新をするラベル

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
	m_pLine->SetPriority(PRIORITY);

	// ラベルを設定
	m_pLine->SetLabel(CObject::LABEL_UI);	// 自動破棄/更新をするラベル

	//--------------------------------------------------------
	//	名前の生成 / 初期設定
	//--------------------------------------------------------
	// 名前の生成
	m_pName = CScrollString2D::Create
	( // 引数
		name::FONT,				// フォントパス
		name::ITALIC,			// イタリック
		L"",					// 表示文字列
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
	m_pName->SetPriority(PRIORITY);

	// TODO
#if 0
	// テキストを割当
	loadtext::BindText(m_pName, loadtext::LoadText(GET_STAGE->GetCurMapStagePass().c_str(), 0));
#else
	m_pName->SetString(L"仮テキスト　ここに情景を文章化");
#endif

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGodItemManager::Uninit(void)
{
	// タイトルの終了
	SAFE_UNINIT(m_pTitle);

	// 下線の終了
	SAFE_UNINIT(m_pLine);

	// 名前の終了
	SAFE_UNINIT(m_pName);
}

//============================================================
//	更新処理
//============================================================
void CGodItemManager::Update(const float fDeltaTime)
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
CGodItemManager *CGodItemManager::Create(void)
{
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

		// 確保したアドレスを返す
		return pGodItemManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CGodItemManager::Release(CGodItemManager *&prGodItemManager)
{
	// 神器獲得演出マネージャーの終了
	assert(prGodItemManager != nullptr);
	prGodItemManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prGodItemManager);
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

	// 遂行時間の色を反映
	m_pTitle->SetColor(title::INIT_COL + (title::DIFF_COL * fRate));

	// 遂行時間の位置を反映
	m_pTitle->SetVec3Position(title::INIT_POS + (title::DIFF_POS * fRate));

	if (m_fCurTime >= title::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 遂行時間の色を補正
		m_pTitle->SetColor(title::DEST_COL);

		// 遂行時間の位置を補正
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

}

//============================================================
//	終了の更新処理
//============================================================
void CGodItemManager::UpdateEnd(const float fDeltaTime)
{

}

//============================================================
//	スキップ操作の更新処理
//============================================================
void CGodItemManager::UpdateSkip(void)
{
	//if (m_state >= STATE_WAIT)	// TODO：まだないけどWAITは作らないとね
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
	// TODO：演出スキップもつくろうね
#if 0
	CRetentionManager::EWin win = GET_RETENTION->GetWin();	// クリア状況

	// クリアマネージャー更新状態にする
	m_state = STATE_CLEAR;

	// 神器獲得演出カメラを目標位置に設定
	GET_MANAGER->GetCamera()->SetDestGodItem();

	// フェードを目標位置に設定
	m_pFade->SetVec3Position(fade::CENT_POS);

	// 全文字を表示させる
	m_pTitle->SetEnableDraw(true);

	// クリアごとのハンコテクスチャを割当
	m_pStamp->BindTexture(stamp::TEXTURE[win]);

	// ハンコを演出後の見た目にする
	m_pStamp->SetEnableDraw(true);				// 自動描画をONにする
	m_pStamp->SetVec3Sizing(stamp::DEST_SIZE);	// 目標サイズに設定

	if (m_pClear == nullptr)
	{ // クリアマネージャーが生成されていない場合

		// クリアマネージャーの生成
		m_pClear = CClearManager::Create(win);
		assert(m_pClear != nullptr);
	}

	// クリアマネージャーの演出スキップ
	assert(m_pClear != nullptr);
	m_pClear->SkipStaging();
#endif
}
