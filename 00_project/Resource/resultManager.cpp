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
#include "timeUI.h"

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
		const float	LINE_HEIGHT		= 100.0f;	// 行間縦幅
		const float	WAIT_TIME_NOR	= 0.08f;	// 文字表示の待機時間

		const D3DXVECTOR3 POS = D3DXVECTOR3(540.0f, 35.0f, 0.0f);	// テキスト位置
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// 横配置
		const CText2D::EAlignY	 ALIGN_Y = CText2D::YALIGN_TOP;		// 縦配置
	}

	namespace stamp
	{
		const float	MOVE_TIME	= 0.3f;	// 移動時間
		const float	WAIT_TIME	= 0.5f;	// ハンコ待機時間
		const char* TEXTURE		= "data\\TEXTURE\\resultStamp000.png";		// ハンコテクスチャ
		const D3DXVECTOR3 POS	= D3DXVECTOR3(1020.0f, 145.0f, 0.0f);		// ハンコ位置
		const D3DXVECTOR3 ROT	= D3DXVECTOR3(0.0f, 0.0f, -0.16f);			// ハンコ向き
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(454.0f, 147.0f, 0.05f);	// ハンコ目標大きさ
		const D3DXVECTOR3 INIT_SIZE	= DEST_SIZE * 10.0f;					// ハンコ初期大きさ
		const D3DXVECTOR3 DIFF_SIZE = stamp::DEST_SIZE - stamp::INIT_SIZE;	// ハンコ差分大きさ
	}

	namespace time
	{
		const char	  *FONT		= "data\\FONT\\零ゴシック.otf";	// フォントパス
		const wchar_t *STRING	= L"任務遂行時間";	// 文字列
		const bool	  ITALIC	= false;	// イタリック
		const float	  MOVE_TIME	= 0.15f;	// 移動時間
		const float	  WAIT_TIME	= 0.5f;		// タイトル待機時間
		const float DEST_HEIGHT = 100.0f;	// 文字目標縦幅
		const float INIT_HEIGHT = DEST_HEIGHT * 10.0f;	// 文字初期縦幅
		const float DIFF_HEIGHT = time::DEST_HEIGHT - time::INIT_HEIGHT;	// 文字差分縦幅

		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// 横配置
		const D3DXVECTOR3 POS = D3DXVECTOR3(540.0f, 322.0f, 0.0f);	// 位置
	}

	namespace val_time
	{
		const D3DXVECTOR3 POS			= D3DXVECTOR3(770.0f, 405.0f, 0.0f);	// 位置
		const D3DXVECTOR3 VAL_SIZE		= D3DXVECTOR3(46.8f * 1.4f, 62.4f * 1.4f, 0.0f);		// 数字大きさ
		const D3DXVECTOR3 PART_SIZE		= D3DXVECTOR3(27.3f * 1.4f, 62.4f * 1.4f, 0.0f);		// 区切り大きさ
		const D3DXVECTOR3 VAL_SPACE		= D3DXVECTOR3(VAL_SIZE.x * 0.85f, 0.0f, 0.0f);	// 数字空白
		const D3DXVECTOR3 PART_SPACE	= D3DXVECTOR3(PART_SIZE.x * 0.85f, 0.0f, 0.0f);	// 区切り空白
		const CValue::EType		TYPE	= CValue::TYPE_NORMAL;		// 数字種類
		const CTimeUI::EAlignX	ALIGN_X	= CTimeUI::XALIGN_LEFT;		// 横配置
		const CTimeUI::EAlignY	ALIGN_Y	= CTimeUI::YALIGN_CENTER;	// 縦配置
	}

	namespace god
	{
		const char	  *FONT		= "data\\FONT\\零ゴシック.otf";	// フォントパス
		const wchar_t *STRING	= L"獲得した神器";	// 文字列
		const bool	  ITALIC	= false;	// イタリック
		const float	  HEIGHT	= 100.0f;	// 文字縦幅
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// 横配置
		const D3DXVECTOR3 POS = D3DXVECTOR3(540.0f, 525.0f, 0.0f);	// 位置
	}

	namespace icon_god
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(865.0f, 620.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(140.0f, 0.0f, 0.0f);		// 空白
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
	m_pStamp	(nullptr),		// ハンコ情報
	m_pTime		(nullptr),		// 遂行時間タイトル情報
	m_pTimeVal	(nullptr),		// 遂行時間情報
	m_pGodItem	(nullptr),		// 神器タイトル情報
	m_state		(STATE_NONE),	// 状態
	m_fMoveY	(0.0f),			// 縦移動量
	m_fCurTime	(0.0f)			// 現在の待機時間
{
	// メンバ変数をクリア
	memset(&m_apGodItemIcon[0], 0, sizeof(m_apGodItemIcon));	// 神器アイコン情報
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
	memset(&m_apGodItemIcon[0], 0, sizeof(m_apGodItemIcon));	// 神器アイコン情報
	m_pFade		= nullptr;			// フェード情報
	m_pTitle	= nullptr;			// タイトル情報
	m_pStamp	= nullptr;			// ハンコ情報
	m_pTime		= nullptr;			// 遂行時間タイトル情報
	m_pTimeVal	= nullptr;			// 遂行時間情報
	m_pGodItem	= nullptr;			// 神器タイトル情報
	m_state		= STATE_FADE_WAIT;	// 状態
	m_fMoveY	= 0.0f;				// 縦移動量
	m_fCurTime	= 0.0f;				// 現在の待機時間

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

	// テクスチャを割当
	m_pStamp->BindTexture(stamp::TEXTURE);

	// 優先順位を設定
	m_pStamp->SetPriority(PRIORITY);

	// ラベルを設定
	m_pStamp->SetLabel(CObject::LABEL_UI);	// 自動破棄/更新をするラベル

	// 自動描画をOFFにする
	m_pStamp->SetEnableDraw(false);

	//--------------------------------------------------------
	//	遂行時間タイトルの生成 / 初期設定
	//--------------------------------------------------------
	// 遂行時間タイトルの生成
	m_pTime = CString2D::Create
	( // 引数
		time::FONT,			// フォントパス
		time::ITALIC,		// イタリック
		time::STRING,		// 指定文字列
		time::POS,			// 原点位置
		time::INIT_HEIGHT,	// 文字縦幅
		time::ALIGN_X		// 横配置
	);
	if (m_pTime == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pTime->SetPriority(PRIORITY);

	// 自動描画をOFFにする
	m_pTime->SetEnableDraw(false);

	//--------------------------------------------------------
	//	遂行時間の生成 / 初期設定
	//--------------------------------------------------------
	// 遂行時間の生成
	m_pTimeVal = CTimeUI::Create
	( // 引数
		0.0f,					// 表示時間	// TODO：経過時間を設定
		val_time::POS,			// 位置
		val_time::VAL_SIZE,		// 数字の大きさ
		val_time::PART_SIZE,	// 区切りの大きさ
		val_time::VAL_SPACE,	// 数字の空白
		val_time::PART_SPACE,	// 区切りの空白
		val_time::TYPE,			// 数字種類
		val_time::ALIGN_X,		// 横配置
		val_time::ALIGN_Y		// 縦配置
	);
	if (m_pTimeVal == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pTimeVal->SetPriority(PRIORITY);

	// 自動描画をOFFにする
	m_pTimeVal->SetEnableDraw(false);

	//--------------------------------------------------------
	//	神器タイトルの生成 / 初期設定
	//--------------------------------------------------------
	// 神器タイトルの生成
	m_pGodItem = CString2D::Create
	( // 引数
		god::FONT,		// フォントパス
		god::ITALIC,	// イタリック
		god::STRING,	// 指定文字列
		god::POS,		// 原点位置
		god::HEIGHT,	// 文字縦幅
		god::ALIGN_X	// 横配置
	);
	if (m_pGodItem == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pGodItem->SetPriority(PRIORITY);

	// 自動描画をOFFにする
	m_pGodItem->SetEnableDraw(false);

	//--------------------------------------------------------
	//	神器アイコンの生成 / 初期設定
	//--------------------------------------------------------
	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // 神器の総数分繰り返す

		// アイコン生成位置を計算
		D3DXVECTOR3 pos = icon_god::POS + (icon_god::SPACE * (float)i);	// 生成位置

		// 神器アイコンの生成
		m_apGodItemIcon[i] = CObject2D::Create(pos, icon_god::SIZE);
		if (m_apGodItemIcon[i] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apGodItemIcon[i]->SetPriority(PRIORITY);

		// ラベルを設定
		m_apGodItemIcon[i]->SetLabel(CObject::LABEL_UI);	// 自動破棄/更新をするラベル

		// 自動描画をOFFにする
		m_apGodItemIcon[i]->SetEnableDraw(false);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CResultManager::Uninit(void)
{

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

	case STATE_FADE_WAIT:

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

	case STATE_STAGE_WAIT:

		// ステージ表示待機の更新
		UpdateStageWait(fDeltaTime);
		break;

	case STATE_STAGE_TITLE:

		// ステージタイトルの更新
		UpdateStageTitle(fDeltaTime);
		break;

	case STATE_CLEAR_STAMP:

		// クリアハンコ押しの更新
		UpdateStamp(fDeltaTime);
		break;

	case STATE_TIME_WAIT:

		// 遂行時間表示待機の更新
		UpdateTimeWait(fDeltaTime);
		break;

	case STATE_TIME_TITLE:

		// 遂行時間タイトル表示の更新
		UpdateTitleTime(fDeltaTime);
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

		// クリアハンコ押し状態にする
		m_state = STATE_CLEAR_STAMP;
	}
}

//============================================================
//	クリアハンコ押しの更新処理
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

		// 遂行時間表示待機状態にする
		m_state = STATE_TIME_WAIT;
	}
}

//============================================================
//	遂行時間表示待機の更新処理
//============================================================
void CResultManager::UpdateTimeWait(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= time::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 遂行時間タイトルの自動描画をONにする
		m_pTime->SetEnableDraw(true);

		// 遂行時間タイトル表示状態にする
		m_state = STATE_TIME_TITLE;
	}
}

//============================================================
//	遂行時間タイトル表示の更新処理
//============================================================
void CResultManager::UpdateTitleTime(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, time::MOVE_TIME);

	// 遂行時間タイトルの大きさを反映
	m_pTime->SetCharHeight(time::INIT_HEIGHT + (time::DIFF_HEIGHT * fRate));

	if (m_fCurTime >= time::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 遂行時間タイトルの大きさを補正
		m_pTime->SetCharHeight(time::DEST_HEIGHT);

		// 待機状態にする
		m_state = STATE_WAIT;
	}
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

		// フェードを停止位置に補正
		posFade.y = fade::DOWN_POS.y;

		// 移動量を初期化
		m_fMoveY = 0.0f;

		// 終了状態にする
		m_state = STATE_END;
	}

	m_pFade->SetVec3Position(posFade);	// フェード位置を反映
}
