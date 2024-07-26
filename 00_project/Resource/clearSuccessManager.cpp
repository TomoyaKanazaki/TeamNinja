//============================================================
//
//	クリア成功マネージャー処理 [clearSuccessManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "clearSuccessManager.h"
#include "retentionManager.h"
#include "manager.h"
#include "sceneGame.h"
#include "anim2D.h"
#include "string2D.h"
#include "scrollText2D.h"
#include "timeUI.h"
#include "blink2D.h"
#include "godItem.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// クリア成功マネージャーの優先順位

	namespace time
	{
		const char		*FONT	= "data\\FONT\\零ゴシック.otf";	// フォントパス
		const wchar_t	*STRING	= L"任務遂行時間";	// 文字列
		const bool	ITALIC		= false;	// イタリック
		const float	MOVE_TIME	= 0.2f;		// 移動時間
		const float	WAIT_TIME	= 0.5f;		// タイトル待機時間
		const float DEST_HEIGHT	= 100.0f;	// 文字目標縦幅
		const float INIT_HEIGHT	= DEST_HEIGHT * 14.0f;	// 文字初期縦幅
		const float DIFF_HEIGHT	= DEST_HEIGHT - INIT_HEIGHT;	// 文字差分縦幅
		const D3DXVECTOR3 POS	= D3DXVECTOR3(540.0f, 292.0f, 0.0f);	// 位置
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;		// 横配置
	}

	namespace val_time
	{
		const float	MOVE_TIME = 0.68f;	// 移動時間
		const float	WAIT_TIME = 0.15f;	// 数値待機時間
		const D3DXCOLOR DEST_COL		= XCOL_WHITE;			// 目標色
		const D3DXCOLOR INIT_COL		= XCOL_AWHITE;			// 初期色
		const D3DXCOLOR DIFF_COL		= DEST_COL - INIT_COL;	// 差分色
		const D3DXVECTOR3 DEST_POS		= D3DXVECTOR3(770.0f, 375.0f, 0.0f);			// 目標位置
		const D3DXVECTOR3 INIT_POS		= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// 初期位置
		const D3DXVECTOR3 DIFF_POS		= DEST_POS - INIT_POS;							// 差分位置
		const D3DXVECTOR3 VAL_SIZE		= D3DXVECTOR3(65.5f, 87.0f, 0.0f);				// 数字大きさ
		const D3DXVECTOR3 PART_SIZE		= D3DXVECTOR3(38.0f, 87.0f, 0.0f);				// 区切り大きさ
		const D3DXVECTOR3 VAL_SPACE		= D3DXVECTOR3(VAL_SIZE.x * 0.85f, 0.0f, 0.0f);	// 数字空白
		const D3DXVECTOR3 PART_SPACE	= D3DXVECTOR3(PART_SIZE.x * 0.85f, 0.0f, 0.0f);	// 区切り空白
		const CValue::EType		TYPE	= CValue::TYPE_NORMAL;		// 数字種類
		const CTimeUI::EAlignX	ALIGN_X	= CTimeUI::XALIGN_LEFT;		// 横配置
		const CTimeUI::EAlignY	ALIGN_Y	= CTimeUI::YALIGN_CENTER;	// 縦配置
	}

	namespace item
	{
		const char		*FONT	= "data\\FONT\\零ゴシック.otf";	// フォントパス
		const wchar_t	*STRING	= L"獲得した勾玉";		// 文字列
		const bool		ITALIC	= false;				// イタリック
		const float	MOVE_TIME	= time::MOVE_TIME;		// 移動時間
		const float	WAIT_TIME	= time::WAIT_TIME;		// タイトル待機時間
		const float	DEST_HEIGHT	= time::DEST_HEIGHT;	// 文字目標縦幅
		const float	INIT_HEIGHT	= time::INIT_HEIGHT;	// 文字初期縦幅
		const float	DIFF_HEIGHT	= time::DIFF_HEIGHT;	// 文字差分縦幅
		const D3DXVECTOR3 POS	= D3DXVECTOR3(540.0f, 490.0f, 0.0f);	// 位置
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;		// 横配置
	}

	namespace icon_item
	{
		const char* TEXTURE = "data\\TEXTURE\\itemGod000.png";				// 神器テクスチャ
		const POSGRID2 TEX_PART	= POSGRID2(3, 1);							// テクスチャ分割
		const D3DXVECTOR3 POS	= D3DXVECTOR3(865.0f, 585.0f, 0.0f);		// 位置
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(140.0f, 0.0f, 0.0f);			// 空白
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(140.0f, 140.0f, 0.0f);	// 目標大きさ
		const D3DXVECTOR3 INIT_SIZE	= DEST_SIZE * 10.0f;					// 初期大きさ
		const D3DXVECTOR3 DIFF_SIZE	= DEST_SIZE - INIT_SIZE;				// 差分大きさ

		const float	MOVE_TIME	= 0.4f;		// 移動時間
		const float	PLUS_TIME	= 0.45f;	// 経過の延長時間
		const float	WAIT_TIME	= 0.5f;		// アイコン待機時間
		const float	DEST_ALPHA	= 1.0f;		// 目標透明度
		const float	INIT_ALPHA	= 0.0f;		// 初期透明度
		const float	DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// 差分透明度
		const D3DXCOLOR DEST_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, DEST_ALPHA);	// 目標色
		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// 初期色
	}

	namespace icon_bg
	{
		const float	MOVE_TIME	= 0.68f;	// 移動時間
		const float	PLUS_TIME	= 0.1f;		// 経過の延長時間
		const float	WAIT_TIME	= 0.15f;	// アイコン背景待機時間
		const float	DEST_ALPHA	= 1.0f;		// 目標透明度
		const float	INIT_ALPHA	= 0.0f;		// 初期透明度
		const float	DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// 差分透明度
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(0.35f, 0.35f, 0.35f, DEST_ALPHA);	// 目標色
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(0.35f, 0.35f, 0.35f, INIT_ALPHA);	// 初期色
		const D3DXVECTOR3 DEST_POS	= icon_item::POS;								// 目標位置
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// 初期位置
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// 差分位置
	}

	namespace control
	{
		const char* TEXTURE		= "data\\TEXTURE\\resultControl000.png";	// 操作テクスチャ
		const D3DXVECTOR3 POS	= D3DXVECTOR3(620.0f, 640.0f, 0.0f);		// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(284.0f, 136.0f, 0.0f);		// 大きさ
		const float MIN_ALPHA	= 0.4f;	// 最低透明度
		const float MAX_ALPHA	= 1.0f;	// 最大透明度
		const float LEVEL_ALPHA	= 3.4f;	// 透明度の加減量
		const float SUB_ALPHA	= 1.2f;	// インのα値減少量
		const float ADD_ALPHA	= 1.2f;	// アウトのα値増加量
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CClearSuccessManager::AFuncUpdateState CClearSuccessManager::m_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,										// 何もしない更新
	&CClearSuccessManager::UpdateTimeTitleWait,		// 遂行時間タイトル待機更新
	&CClearSuccessManager::UpdateTimeTitle,			// 遂行時間タイトル表示更新
	&CClearSuccessManager::UpdateTimeValueWait,		// 遂行時間待機更新
	&CClearSuccessManager::UpdateTimeValue,			// 遂行時間表示更新
	&CClearSuccessManager::UpdateItemTitleWait,		// 神器タイトル待機更新
	&CClearSuccessManager::UpdateItemTitle,			// 神器タイトル表示更新
	&CClearSuccessManager::UpdateItemIconBgWait,	// 神器アイコン背景待機更新
	&CClearSuccessManager::UpdateItemIconBg,		// 神器アイコン背景表示更新
	&CClearSuccessManager::UpdateItemIconWait,		// 神器アイコン待機更新
	&CClearSuccessManager::UpdateItemIcon,			// 神器アイコン表示更新
	&CClearSuccessManager::UpdateWait,				// 待機更新
	nullptr,										// 終了更新
};

//************************************************************
//	親クラス [CClearSuccessManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CClearSuccessManager::CClearSuccessManager() :
	m_pTime		(nullptr),		// 遂行時間タイトル情報
	m_pTimeVal	(nullptr),		// 遂行時間情報
	m_pGodItem	(nullptr),		// 神器タイトル情報
	m_pControl	(nullptr),		// 操作情報
	m_state		(STATE_NONE),	// 状態
	m_fCurTime	(0.0f)			// 現在の待機時間
{
	// メンバ変数をクリア
	memset(&m_apGodItemBG[0],   0, sizeof(m_apGodItemBG));		// 神器アイコン背景情報
	memset(&m_apGodItemIcon[0], 0, sizeof(m_apGodItemIcon));	// 神器アイコン情報

	// スタティックアサート
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CClearSuccessManager::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	デストラクタ
//============================================================
CClearSuccessManager::~CClearSuccessManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CClearSuccessManager::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apGodItemBG[0], 0, sizeof(m_apGodItemBG));		// 神器アイコン背景情報
	memset(&m_apGodItemIcon[0], 0, sizeof(m_apGodItemIcon));	// 神器アイコン情報
	m_state = STATE_TIME_TITLE_WAIT;	// 状態
	m_pTime		= nullptr;	// 遂行時間タイトル情報
	m_pTimeVal	= nullptr;	// 遂行時間情報
	m_pGodItem	= nullptr;	// 神器タイトル情報
	m_pControl	= nullptr;	// 操作情報
	m_fCurTime	= 0.0f;		// 現在の待機時間

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
		CSceneGame::TIME_LIMIT - GET_RETENTION->GetTime(),	// 表示時間
		val_time::INIT_POS,		// 位置
		val_time::VAL_SIZE,		// 数字の大きさ
		val_time::PART_SIZE,	// 区切りの大きさ
		val_time::VAL_SPACE,	// 数字の空白
		val_time::PART_SPACE,	// 区切りの空白
		val_time::TYPE,			// 数字種類
		val_time::ALIGN_X,		// 横配置
		val_time::ALIGN_Y,		// 縦配置
		VEC3_ZERO,				// 向き
		val_time::INIT_COL		// 色
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
		item::FONT,			// フォントパス
		item::ITALIC,		// イタリック
		item::STRING,		// 指定文字列
		item::POS,			// 原点位置
		item::INIT_HEIGHT,	// 文字縦幅
		item::ALIGN_X		// 横配置
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
	//	神器アイコン背景の生成 / 初期設定
	//--------------------------------------------------------
	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // 神器の総数分繰り返す

		// アイコン背景生成位置を計算
		D3DXVECTOR3 posBG = icon_bg::INIT_POS + (icon_item::SPACE * (float)i);	// アイコン背景生成位置

		// 神器アイコン背景の生成
		m_apGodItemBG[i] = CAnim2D::Create(icon_item::TEX_PART.x, icon_item::TEX_PART.y, posBG, icon_item::DEST_SIZE, VEC3_ZERO, icon_bg::INIT_COL);
		if (m_apGodItemBG[i] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを割当
		m_apGodItemBG[i]->BindTexture(icon_item::TEXTURE);

		// テクスチャパターンを設定
		m_apGodItemBG[i]->SetPattern(i);

		// 優先順位を設定
		m_apGodItemBG[i]->SetPriority(PRIORITY);

		// ラベルを設定
		m_apGodItemBG[i]->SetLabel(CObject::LABEL_UI);	// 自動破棄/更新をするラベル

		// 自動描画をOFFにする
		m_apGodItemBG[i]->SetEnableDraw(false);
	}

	//--------------------------------------------------------
	//	神器アイコンの生成 / 初期設定
	//--------------------------------------------------------
	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // 神器の総数分繰り返す

		// アイコン生成位置を計算
		D3DXVECTOR3 posIcon = icon_item::POS + (icon_item::SPACE * (float)i);	// アイコン生成位置

		// 神器アイコンの生成
		m_apGodItemIcon[i] = CAnim2D::Create(icon_item::TEX_PART.x, icon_item::TEX_PART.y, posIcon, icon_item::INIT_SIZE, VEC3_ZERO, icon_item::INIT_COL);
		if (m_apGodItemIcon[i] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを割当
		m_apGodItemIcon[i]->BindTexture(icon_item::TEXTURE);

		// テクスチャパターンを設定
		m_apGodItemIcon[i]->SetPattern(i);

		// 優先順位を設定
		m_apGodItemIcon[i]->SetPriority(PRIORITY);

		// ラベルを設定
		m_apGodItemIcon[i]->SetLabel(CObject::LABEL_UI);	// 自動破棄/更新をするラベル

		// 自動描画をOFFにする
		m_apGodItemIcon[i]->SetEnableDraw(false);
	}

	//--------------------------------------------------------
	//	操作の生成 / 初期設定
	//--------------------------------------------------------
	// 操作の生成
	m_pControl = CBlink2D::Create
	( // 引数
		control::POS,			// 位置
		control::SIZE,			// 大きさ
		control::MIN_ALPHA,		// 最低透明度
		control::MAX_ALPHA,		// 最大透明度
		control::LEVEL_ALPHA,	// 透明度の加減量
		control::SUB_ALPHA,		// インのα値減少量
		control::ADD_ALPHA		// アウトのα値増加量
	);
	if (m_pControl == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを割当
	m_pControl->BindTexture(control::TEXTURE);

	// 優先順位を設定
	m_pControl->SetPriority(PRIORITY);

	// 自動描画をOFFにする
	m_pControl->SetEnableDraw(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CClearSuccessManager::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CClearSuccessManager::Update(const float fDeltaTime)
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
void CClearSuccessManager::SkipStaging(void)
{
	// 待機状態にする
	m_state = STATE_WAIT;

	// 遂行時間タイトルを演出後の見た目にする
	m_pTime->SetEnableDraw(true);				// 自動描画をONにする
	m_pTime->SetCharHeight(time::DEST_HEIGHT);	// 目標サイズに設定

	// 遂行時間を演出後の見た目にする
	m_pTimeVal->SetEnableDraw(true);					// 自動描画をONにする
	m_pTimeVal->SetVec3Position(val_time::DEST_POS);	// 目標位置に設定
	m_pTimeVal->SetColor(val_time::DEST_COL);			// 目標色に設定

	// 神器タイトルを演出後の見た目にする
	m_pGodItem->SetEnableDraw(true);				// 自動描画をONにする
	m_pGodItem->SetCharHeight(item::DEST_HEIGHT);	// 目標サイズに設定

	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // 神器の総数分繰り返す

		// 神器アイコン背景の目標生成位置を計算
		D3DXVECTOR3 posDest = icon_bg::DEST_POS + (icon_item::SPACE * (float)i);

		// 神器アイコン背景を演出後の見た目にする
		m_apGodItemBG[i]->SetEnableDraw(true);			// 自動描画をONにする
		m_apGodItemBG[i]->SetVec3Position(posDest);		// 目標位置に設定
		m_apGodItemBG[i]->SetColor(icon_bg::DEST_COL);	// 目標色に設定

		// 神器アイコンを演出後の見た目にする
		m_apGodItemIcon[i]->SetEnableDraw(CGodItem::IsGet((CGodItem::EType)i));	// 自動描画を獲得している場合ONにする
		m_apGodItemIcon[i]->SetVec3Sizing(icon_item::DEST_SIZE);	// 目標サイズに設定
		m_apGodItemIcon[i]->SetColor(icon_item::DEST_COL);			// 目標色に設定
	}

	// 操作を演出後の見た目にする
	m_pControl->SetEnableDraw(true);	// 自動描画をONにする
	m_pControl->SetBlink(true);			// 点滅を開始する
}

//============================================================
//	全UIオブジェクトの移動処理
//============================================================
void CClearSuccessManager::SetAllMove(const D3DXVECTOR3& rMove)
{
	// 遂行時間タイトルの位置を移動
	m_pTime->SetVec3Position(m_pTime->GetVec3Position() + rMove);

	// 遂行時間の位置を移動
	m_pTimeVal->SetVec3Position(m_pTimeVal->GetVec3Position() + rMove);

	// 神器タイトルの位置を移動
	m_pGodItem->SetVec3Position(m_pGodItem->GetVec3Position() + rMove);

	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // 神器の総数分繰り返す

		// 神器アイコン背景の位置を移動
		m_apGodItemBG[i]->SetVec3Position(m_apGodItemBG[i]->GetVec3Position() + rMove);

		// 神器アイコンの位置を移動
		m_apGodItemIcon[i]->SetVec3Position(m_apGodItemIcon[i]->GetVec3Position() + rMove);
	}

	// 操作の位置を移動
	m_pControl->SetVec3Position(m_pControl->GetVec3Position() + rMove);
}

//============================================================
//	遂行時間タイトル待機の更新処理
//============================================================
void CClearSuccessManager::UpdateTimeTitleWait(const float fDeltaTime)
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
void CClearSuccessManager::UpdateTimeTitle(const float fDeltaTime)
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

		// 遂行時間待機状態にする
		m_state = STATE_TIME_VALUE_WAIT;
	}
}

//============================================================
//	遂行時間タイトル待機の更新処理
//============================================================
void CClearSuccessManager::UpdateTimeValueWait(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= val_time::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 遂行時間の自動描画をONにする
		m_pTimeVal->SetEnableDraw(true);

		// 遂行時間表示状態にする
		m_state = STATE_TIME_VALUE;
	}
}

//============================================================
//	遂行時間表示の更新処理
//============================================================
void CClearSuccessManager::UpdateTimeValue(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, val_time::MOVE_TIME);

	// 遂行時間の色を反映
	m_pTimeVal->SetColor(val_time::INIT_COL + (val_time::DIFF_COL * fRate));

	// 遂行時間の位置を反映
	m_pTimeVal->SetVec3Position(val_time::INIT_POS + (val_time::DIFF_POS * fRate));

	if (m_fCurTime >= val_time::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 遂行時間の色を補正
		m_pTimeVal->SetColor(val_time::DEST_COL);

		// 遂行時間の位置を補正
		m_pTimeVal->SetVec3Position(val_time::DEST_POS);

		// 神器タイトル待機状態にする
		m_state = STATE_ITEM_TITLE_WAIT;
	}
}

//============================================================
//	神器タイトル待機の更新処理
//============================================================
void CClearSuccessManager::UpdateItemTitleWait(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= item::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 神器タイトルの自動描画をONにする
		m_pGodItem->SetEnableDraw(true);

		// 神器タイトル表示状態にする
		m_state = STATE_ITEM_TITLE;
	}
}

//============================================================
//	神器タイトル表示の更新処理
//============================================================
void CClearSuccessManager::UpdateItemTitle(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, item::MOVE_TIME);

	// 神器タイトルの大きさを反映
	m_pGodItem->SetCharHeight(item::INIT_HEIGHT + (item::DIFF_HEIGHT * fRate));

	if (m_fCurTime >= item::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 神器タイトルの大きさを補正
		m_pGodItem->SetCharHeight(item::DEST_HEIGHT);

		// 神器アイコン背景待機状態にする
		m_state = STATE_ITEM_BG_WAIT;
	}
}

//============================================================
//	神器アイコン背景待機の更新処理
//============================================================
void CClearSuccessManager::UpdateItemIconBgWait(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= icon_bg::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		for (int i = 0; i < CStage::GOD_MAX; i++)
		{ // 神器の総数分繰り返す

			// 神器アイコン背景の自動描画をONにする
			m_apGodItemBG[i]->SetEnableDraw(true);
		}

		// 神器アイコン背景表示状態にする
		m_state = STATE_ITEM_BG;
	}
}

//============================================================
//	神器アイコン背景表示の更新処理
//============================================================
void CClearSuccessManager::UpdateItemIconBg(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// アイコン背景の移動
	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // 神器の総数分繰り返す

		// アイコン背景それぞれの経過時間を計算
		float fRateTime = m_fCurTime - (icon_bg::PLUS_TIME * (float)i);
		useful::LimitNum(fRateTime, 0.0f, icon_bg::MOVE_TIME);	// 経過時間を補正

		// それぞれの経過時刻から割合を計算
		float fRate = easeing::InOutQuad(fRateTime, 0.0f, icon_bg::MOVE_TIME);

		// アイコン背景の位置を計算
		D3DXVECTOR3 posInit = icon_bg::INIT_POS + (icon_item::SPACE * (float)i);

		// アイコン背景の色を計算
		D3DXCOLOR colCur = icon_bg::INIT_COL;
		colCur.a = icon_bg::INIT_ALPHA + (icon_bg::DIFF_ALPHA * fRate);	// 現在の透明度を設定

		// 神器アイコン背景の位置を反映
		m_apGodItemBG[i]->SetVec3Position(posInit + (icon_bg::DIFF_POS * fRate));

		// 神器アイコン背景の色を反映
		m_apGodItemBG[i]->SetColor(colCur);
	}

	// アイコン背景の移動補正
	if (m_fCurTime >= icon_bg::MOVE_TIME + icon_bg::PLUS_TIME * (CStage::GOD_MAX - 1))
	{ // 全アイコン背景の待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		for (int i = 0; i < CStage::GOD_MAX; i++)
		{ // 神器の総数分繰り返す

			// アイコン背景の目標生成位置を計算
			D3DXVECTOR3 posDest = icon_bg::DEST_POS + (icon_item::SPACE * (float)i);

			// 神器アイコン背景の位置を補正
			m_apGodItemBG[i]->SetVec3Position(posDest);

			// 神器アイコン背景の色を補正
			m_apGodItemBG[i]->SetColor(icon_bg::DEST_COL);
		}

		// 神器アイコン待機状態にする
		m_state = STATE_ITEM_ICON_WAIT;
	}
}

//============================================================
//	神器アイコン待機の更新処理
//============================================================
void CClearSuccessManager::UpdateItemIconWait(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= icon_item::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		for (int i = 0; i < CStage::GOD_MAX; i++)
		{ // 神器の総数分繰り返す

			// 獲得した神器アイコンの自動描画をONにする
			m_apGodItemIcon[i]->SetEnableDraw(CGodItem::IsGet((CGodItem::EType)i));
		}

		// 神器アイコン表示状態にする
		m_state = STATE_ITEM_ICON;
	}
}

//============================================================
//	神器アイコン表示の更新処理
//============================================================
void CClearSuccessManager::UpdateItemIcon(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// アイコンのサイズ変更
	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // 神器の総数分繰り返す

		// アイコン背景それぞれの経過時間を計算
		float fRateTime = m_fCurTime - (icon_item::PLUS_TIME * (float)i);
		useful::LimitNum(fRateTime, 0.0f, icon_item::MOVE_TIME);	// 経過時間を補正

		// それぞれの経過時刻から割合を計算
		float fRate = easeing::InQuad(fRateTime, 0.0f, icon_item::MOVE_TIME);

		// アイコン背景の色を計算
		D3DXCOLOR colCur = icon_item::INIT_COL;
		colCur.a = icon_item::INIT_ALPHA + (icon_item::DIFF_ALPHA * fRate);	// 現在の透明度を設定

		// 神器アイコンの大きさを反映
		m_apGodItemIcon[i]->SetVec3Sizing(icon_item::INIT_SIZE + (icon_item::DIFF_SIZE * fRate));

		// 神器アイコンの色を反映
		m_apGodItemIcon[i]->SetColor(colCur);
	}

	// アイコンのサイズ変更補正
	if (m_fCurTime >= icon_item::MOVE_TIME + icon_item::PLUS_TIME * (CStage::GOD_MAX - 1))
	{ // 全アイコンの待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		for (int i = 0; i < CStage::GOD_MAX; i++)
		{ // 神器の総数分繰り返す

			// 神器アイコンの大きさを補正
			m_apGodItemIcon[i]->SetVec3Sizing(icon_item::DEST_SIZE);

			// 神器アイコンの色を補正
			m_apGodItemIcon[i]->SetColor(icon_item::DEST_COL);
		}

		// 操作の自動描画をONにする
		m_pControl->SetEnableDraw(true);

		// 操作の点滅を開始する
		m_pControl->SetBlink(true);

		// 待機状態にする
		m_state = STATE_WAIT;
	}
}

//============================================================
//	待機の更新処理
//============================================================
void CClearSuccessManager::UpdateWait(const float fDeltaTime)
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
		// 終了状態にする
		m_state = STATE_END;
	}
}
