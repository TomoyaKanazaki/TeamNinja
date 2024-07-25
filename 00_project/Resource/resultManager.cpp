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
#include "camera.h"
#include "fade.h"
#include "clearManager.h"
#include "object2D.h"
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

		const char* TEXTURE		= "data\\TEXTURE\\resultFade000.png";	// フェードテクスチャ
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(800.0f, 1770.0f, 0.0f);	// フェード大きさ
		const D3DXCOLOR	  COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);	// フェード色
		const D3DXVECTOR3 UP_POS	= D3DXVECTOR3(FADE_CENT, -900.0f, 0.0f);		// フェード上位置
		const D3DXVECTOR3 CENT_POS	= D3DXVECTOR3(FADE_CENT, SCREEN_CENT.y, 0.0f);	// フェード中央位置
		const D3DXVECTOR3 DOWN_POS	= D3DXVECTOR3(FADE_CENT, SCREEN_HEIGHT + 900.0f, 0.0f);			// フェード下位置
		const D3DXVECTOR3 UP_MIDDLE_POS		= D3DXVECTOR3(FADE_CENT, UP_POS.y + 300.0f, 0.0f);		// フェード上中央位置
		const D3DXVECTOR3 DOWN_MIDDLE_POS	= D3DXVECTOR3(FADE_CENT, DOWN_POS.y - 300.0f, 0.0f);	// フェード下中央位置
	}

	namespace title
	{
		const char *FONT	= "data\\FONT\\零ゴシック.otf";	// フォントパス
		const bool	ITALIC	= false;			// イタリック
		const float	WAIT_TIME		= 0.5f;		// タイトル待機時間
		const float	CHAR_HEIGHT		= 100.0f;	// 文字縦幅
		const float	LINE_HEIGHT		= 80.0f;	// 行間縦幅
		const float	WAIT_TIME_NOR	= 0.08f;	// 文字表示の待機時間
		const D3DXVECTOR3 POS = D3DXVECTOR3(540.0f, 40.0f, 0.0f);	// テキスト位置
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// 横配置
		const CText2D::EAlignY	 ALIGN_Y = CText2D::YALIGN_TOP;		// 縦配置
	}

	namespace stamp
	{
		const char* TEXTURE[] =	// ハンコテクスチャ
		{
			nullptr,								// クリア判定無し
			"data\\TEXTURE\\resultStamp001.png",	// クリア失敗テクスチャ
			"data\\TEXTURE\\resultStamp000.png",	// クリア成功テクスチャ
		};

		const float	MOVE_TIME	= 0.3f;	// 移動時間
		const float	WAIT_TIME	= 0.5f;	// ハンコ待機時間
		const D3DXVECTOR3 POS	= D3DXVECTOR3(1020.0f, 140.0f, 0.0f);		// ハンコ位置
		const D3DXVECTOR3 ROT	= D3DXVECTOR3(0.0f, 0.0f, -0.16f);			// ハンコ向き
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(454.0f, 147.0f, 0.0f);	// ハンコ目標大きさ
		const D3DXVECTOR3 INIT_SIZE	= DEST_SIZE * 10.0f;					// ハンコ初期大きさ
		const D3DXVECTOR3 DIFF_SIZE = DEST_SIZE - INIT_SIZE;				// ハンコ差分大きさ
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CResultManager::AFuncUpdateState CResultManager::m_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,								// 何もしない更新
	&CResultManager::UpdateFadeWait,		// フェード待機更新
	&CResultManager::UpdateFadeIn,			// フェードイン更新
	&CResultManager::UpdateFadeInWait,		// フェードイン待機更新
	&CResultManager::UpdateFadeInAccel,		// フェードイン加速更新
	&CResultManager::UpdateStageWait,		// ステージ表示待機更新
	&CResultManager::UpdateStageTitle,		// ステージタイトル更新
	&CResultManager::UpdateStamp,			// ハンコ押し更新
	&CResultManager::UpdateClear,			// クリア更新
	&CResultManager::UpdateFadeOut,			// フェードアウト更新
	&CResultManager::UpdateFadeOutWait,		// フェードアウト待機更新
	&CResultManager::UpdateFadeOutAccel,	// フェードアウト加速更新
	&CResultManager::UpdateEnd,				// 終了更新
};

//************************************************************
//	親クラス [CResultManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CResultManager::CResultManager() :
	m_pClear	(nullptr),		// クリアマネージャー情報
	m_pFade		(nullptr),		// フェード情報
	m_pTitle	(nullptr),		// タイトル情報
	m_pStamp	(nullptr),		// ハンコ情報
	m_state		(STATE_NONE),	// 状態
	m_fMoveY	(0.0f),			// 縦移動量
	m_fCurTime	(0.0f)			// 現在の待機時間
{
	// スタティックアサート
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CResultManager::STATE_MAX, "ERROR : State Count Mismatch");
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
	m_state		= STATE_FADE_WAIT;	// 状態
	m_pClear	= nullptr;	// クリアマネージャー情報
	m_pFade		= nullptr;	// フェード情報
	m_pTitle	= nullptr;	// タイトル情報
	m_pStamp	= nullptr;	// ハンコ情報
	m_fMoveY	= 0.0f;		// 縦移動量
	m_fCurTime	= 0.0f;		// 現在の待機時間

	//--------------------------------------------------------
	//	フェードの生成 / 初期設定
	//--------------------------------------------------------
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

	// テクスチャを割当
	m_pFade->BindTexture(fade::TEXTURE);

	// 優先順位を設定
	m_pFade->SetPriority(PRIORITY);

	// ラベルを設定
	m_pFade->SetLabel(CObject::LABEL_UI);	// 自動破棄/更新をするラベル

	//--------------------------------------------------------
	//	タイトルの生成 / 初期設定
	//--------------------------------------------------------
	// タイトルの生成
	m_pTitle = CScrollText2D::Create
	( // 引数
		title::FONT,			// フォントパス
		title::ITALIC,			// イタリック
		title::POS,				// 原点位置
		title::WAIT_TIME_NOR,	// 文字表示の待機時間
		title::CHAR_HEIGHT,		// 文字縦幅
		title::LINE_HEIGHT,		// 行間縦幅
		title::ALIGN_X,			// 横配置
		title::ALIGN_Y			// 縦配置
	);
	if (m_pTitle == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pTitle->SetPriority(PRIORITY);

	// TODO：ここにタイトル
	m_pTitle->AddString(L"ここに");
	m_pTitle->AddString(L"　ステージ名");

	//--------------------------------------------------------
	//	ハンコの生成 / 初期設定
	//--------------------------------------------------------
	// ハンコの生成
	m_pStamp = CObject2D::Create
	( // 引数
		stamp::POS,			// 位置
		stamp::INIT_SIZE,	// 大きさ
		stamp::ROT			// 向き
	);
	if (m_pStamp == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pStamp->SetPriority(PRIORITY);

	// ラベルを設定
	m_pStamp->SetLabel(CObject::LABEL_UI);	// 自動破棄/更新をするラベル

	// 自動描画をOFFにする
	m_pStamp->SetEnableDraw(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CResultManager::Uninit(void)
{
	// クリアマネージャーの破棄
	SAFE_REF_RELEASE(m_pClear);
}

//============================================================
//	更新処理
//============================================================
void CResultManager::Update(const float fDeltaTime)
{
	assert(m_state > NONE_IDX && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // 更新関数が指定されている場合

		// 各状態ごとの更新
		(this->*(m_aFuncUpdateState[m_state]))(fDeltaTime);
	}

	// TODO
	if (GET_INPUTKEY->IsTrigger(DIK_0))
	{
		// 演出のスキップ
		SkipStaging();
	}
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
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= fade::FADEWAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

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
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= fade::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

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

		// ステージ表示待機状態にする
		m_state = STATE_STAGE_WAIT;
	}

	m_pFade->SetVec3Position(posFade);	// フェード位置を反映
}

//============================================================
//	ステージ表示待機の更新処理
//============================================================
void CResultManager::UpdateStageWait(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= title::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// ステージタイトルの文字送りを開始する
		m_pTitle->SetEnableScroll(true);

		// ステージタイトル状態にする
		m_state = STATE_STAGE_TITLE;
	}
}

//============================================================
//	ステージタイトルの更新処理
//============================================================
void CResultManager::UpdateStageTitle(const float fDeltaTime)
{
	// 文字送りが終了していない場合抜ける
	if (m_pTitle->IsScroll()) { return; }

	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= stamp::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// ハンコの自動描画をONにする
		m_pStamp->SetEnableDraw(true);

		// クリアごとのハンコテクスチャを割当
		m_pStamp->BindTexture(stamp::TEXTURE[GET_RETENTION->GetWin()]);

		// ハンコ押し状態にする
		m_state = STATE_STAMP;
	}
}

//============================================================
//	ハンコ押しの更新処理
//============================================================
void CResultManager::UpdateStamp(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, stamp::MOVE_TIME);

	// ハンコの大きさを反映
	m_pStamp->SetVec3Sizing(stamp::INIT_SIZE + (stamp::DIFF_SIZE * fRate));

	if (m_fCurTime >= stamp::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// ハンコの大きさを補正
		m_pStamp->SetVec3Sizing(stamp::DEST_SIZE);

		// クリアマネージャーの生成
		m_pClear = CClearManager::Create(GET_RETENTION->GetWin());
		assert(m_pClear != nullptr);

		// クリアマネージャー更新状態にする
		m_state = STATE_CLEAR;
	}
}

//============================================================
//	クリアマネージャーの更新処理
//============================================================
void CResultManager::UpdateClear(const float fDeltaTime)
{
	// クリアマネージャーの更新
	assert(m_pClear != nullptr);
	m_pClear->Update(fDeltaTime);

	if (m_pClear->IsEndState())
	{ // クリアごとの状態更新が終了した場合

		// フェードアウト状態にする
		m_state = STATE_FADEOUT;
	}
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

		// 全UIオブジェクトの移動
		SetAllMove(D3DXVECTOR3(0.0f, m_fMoveY - (posFade.y - fade::DOWN_MIDDLE_POS.y), 0.0f));

		// フェードを停止位置に補正
		posFade.y = fade::DOWN_MIDDLE_POS.y;

		// 移動量を初期化
		m_fMoveY = 0.0f;

		// フェードアウト待機状態にする
		m_state = STATE_FADEOUT_WAIT;
	}

	// 全UIオブジェクトの移動
	SetAllMove(D3DXVECTOR3(0.0f, m_fMoveY, 0.0f));

	m_pFade->SetVec3Position(posFade);	// フェード位置を反映
}

//============================================================
//	フェードアウト待機の更新処理
//============================================================
void CResultManager::UpdateFadeOutWait(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= fade::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

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

		// 全UIオブジェクトの移動
		SetAllMove(D3DXVECTOR3(0.0f, m_fMoveY - (posFade.y - fade::DOWN_POS.y), 0.0f));

		// フェードを停止位置に補正
		posFade.y = fade::DOWN_POS.y;

		// 移動量を初期化
		m_fMoveY = 0.0f;

		// 終了状態にする
		m_state = STATE_END;
	}

	// 全UIオブジェクトの移動
	SetAllMove(D3DXVECTOR3(0.0f, m_fMoveY, 0.0f));

	m_pFade->SetVec3Position(posFade);	// フェード位置を反映
}

//============================================================
//	終了の更新処理
//============================================================
void CResultManager::UpdateEnd(const float fDeltaTime)
{
	// フェード中の場合抜ける
	if (GET_MANAGER->GetFade()->IsFade()) { return; }

	// クリアマネージャーで選択されたシーンに遷移する
	GET_MANAGER->SetLoadScene(m_pClear->GetNext());
}

//============================================================
//	全UIオブジェクトの移動処理
//============================================================
void CResultManager::SetAllMove(const D3DXVECTOR3& rMove)
{
	// タイトルの位置を移動
	m_pTitle->SetVec3Position(m_pTitle->GetVec3Position() + rMove);

	// ハンコの位置を移動
	m_pStamp->SetVec3Position(m_pStamp->GetVec3Position() + rMove);

	// クリアマネージャーの全UIオブジェクト移動
	assert(m_pClear != nullptr);
	m_pClear->SetAllMove(rMove);
}

//============================================================
//	演出のスキップ処理
//============================================================
void CResultManager::SkipStaging(void)
{
	CRetentionManager::EWin win = GET_RETENTION->GetWin();	// クリア状況

	// クリアマネージャー更新状態にする
	m_state = STATE_CLEAR;

	// リザルトカメラを目標位置に設定
	GET_MANAGER->GetCamera()->SetDestResult();

	// フェードを目標位置に設定
	m_pFade->SetVec3Position(fade::CENT_POS);

	// 全文字を表示させる
	m_pTitle->SetEnableDraw(true);

	// クリアごとのハンコテクスチャを割当
	m_pStamp->BindTexture(stamp::TEXTURE[win]);

	// ハンコを演出後の見た目にする
	m_pStamp->SetEnableDraw(true);				// 自動描画をONにする
	m_pStamp->SetVec3Sizing(stamp::DEST_SIZE);	// 目標サイズに設定

	// クリアマネージャーの生成
	m_pClear = CClearManager::Create(win);
	assert(m_pClear != nullptr);

	// クリアマネージャーの演出スキップ
	assert(m_pClear != nullptr);
	m_pClear->SkipStaging();
}
