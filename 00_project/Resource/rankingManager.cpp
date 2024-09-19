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
#include "renderer.h"
#include "loadtext.h"
#include "stage.h"
#include "object2D.h"
#include "player.h"
#include "string2D.h"
#include "timeUI.h"
#include "anim2D.h"
#include "timer.h"
#include "transpoint.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIO_BG = 5;	// 背景の優先順位
	const int PRIO_UI = 6;	// UIの優先順位
	const float RANK_TIME[CRankingManager::MAX_RANK] = { 150.0f, 180.0f, 225.0f };	// 初期ランキングタイム

	namespace fade
	{
		const float	MOVE_INTIME	 = 0.15f;	// イン変動時間
		const float	MOVE_OUTTIME = 0.15f;	// アウト変動時間
		const float DEST_ALPHA	 = 0.75f;	// 目標透明度
		const float INIT_ALPHA	 = 0.0f;	// 初期透明度
		const float DIFF_ALPHA	 = DEST_ALPHA - INIT_ALPHA;	// 差分透明度
		const D3DXCOLOR INIT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, INIT_ALPHA);		// 初期色
		const D3DXVECTOR3 SIZE	 = D3DXVECTOR3(SCREEN_SIZE.x, 520.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 POS	 = SCREEN_CENT;	// 位置
		const D3DXVECTOR3 ROT	 = VEC3_ZERO;	// 向き
	}

	namespace title
	{
		const char *TEXTURE		= "data\\TEXTURE\\get_magatama.png";	// テクスチャパス
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(632.0f, 184.0f, 0.0f) * 0.8f;	// 大きさ
		const float	MOVE_TIME	= 0.68f;	// 移動時間
		const D3DXCOLOR DEST_COL	= XCOL_WHITE;			// 目標色
		const D3DXCOLOR INIT_COL	= XCOL_AWHITE;			// 初期色
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;	// 差分色
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(SCREEN_CENT.x, 180.0f, 0.0f);		// 目標位置
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// 初期位置
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// 差分位置
	}

	namespace balloon
	{
		const char *TEXTURE		= "data\\TEXTURE\\get_magatama.png";	// テクスチャパス
		const D3DXVECTOR3 SIZE	= SCREEN_SIZE * 0.42f;		// 大きさ
		const float	MOVE_TIME	= 0.68f;					// 移動時間
		const D3DXCOLOR DEST_COL	= XCOL_WHITE;			// 目標色
		const D3DXCOLOR INIT_COL	= XCOL_AWHITE;			// 初期色
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;	// 差分色
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(345.0f, 385.0f, 0.0f);			// 目標位置
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// 初期位置
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// 差分位置
	}

	namespace name
	{
		const char		*FONT	= "data\\FONT\\玉ねぎ楷書激無料版v7改.ttf";	// フォントパス
		const wchar_t	*STRING = L"遂行順位";	// 表示文字列
		const bool		ITALIC	= true;			// イタリック

		const float	CHAR_HEIGHT	 = 120.0f * 0.42f;	// 文字縦幅
		const D3DXVECTOR3 POS	 = D3DXVECTOR3(635.0f, 275.0f, 0.0f);		// 位置
		const D3DXVECTOR3 OFFSET = D3DXVECTOR3(8.0f, 8.0f, 0.0f) * 0.42f;	// 影オフセット
		const D3DXVECTOR3 ROT	 = VEC3_ZERO;								// 向き

		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// 横配置
		const D3DXCOLOR COL_SHADOW	= XCOL_ABLUE;	// 影の色
		const D3DXCOLOR COL_NAME	= XCOL_AWHITE;	// 名前の色
	}

	namespace rank
	{
		const char *TEXTURE		= "data\\TEXTURE\\ranking000.png";		// フォントパス
		const POSGRID2 PART		= POSGRID2(1, 3);						// テクスチャ分割数
		const D3DXVECTOR3 POS	= D3DXVECTOR3(725.0f, 338.5f, 0.0f);	// 位置
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(0.0f, 66.5f, 0.0f);		// 空白
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(125.4f, 74.25f, 0.0f);	// 数字大きさ
	}

	namespace time
	{
		const D3DXVECTOR3 POS			= D3DXVECTOR3(979.0f, rank::POS.y, 0.0f);		// 位置
		const D3DXVECTOR3 SPACE			= rank::SPACE;									// 空白
		const D3DXVECTOR3 VAL_SIZE		= D3DXVECTOR3(62.82f, 74.25f, 0.0f) * 0.87f;	// 数字大きさ
		const D3DXVECTOR3 PART_SIZE		= D3DXVECTOR3(27.85f, 63.65f, 0.0f) * 0.87f;	// 区切り大きさ
		const D3DXVECTOR3 VAL_SPACE		= D3DXVECTOR3(VAL_SIZE.x * 0.85f, 0.0f, 0.0f);	// 数字空白
		const D3DXVECTOR3 PART_SPACE	= D3DXVECTOR3(PART_SIZE.x * 0.85f, 0.0f, 0.0f);	// 区切り空白
		const CValue::EType TYPE		= CValue::TYPE_NORMAL;		// 数字種類
		const CTimeUI::EAlignX ALIGN_X	= CTimeUI::XALIGN_CENTER;	// 横配置
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

		const D3DXVECTOR3 DEST_POS = D3DXVECTOR3(SCREEN_CENT.x - SPACE.x * 0.5f, 555.0f, 0.0f);	// 目標位置
		const D3DXVECTOR3 INIT_POS = DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);					// 初期位置
		const D3DXVECTOR3 DIFF_POS = DEST_POS - INIT_POS;										// 差分位置
	}

	namespace fall
	{
		const float	MOVE_TIME	= 0.25f;	// 移動時間
		const float DEST_ALPHA	= 0.0f;		// 目標透明度
		const float INIT_ALPHA	= 1.0f;		// 初期透明度
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
	&CRankingManager::UpdateSpawn,		// UI出現更新
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
CRankingManager::CRankingManager(CTransPoint* pParent) :
	m_pFade		 (nullptr),		// フェード情報
	m_pTitle	 (nullptr),		// タイトル情報
	m_pBalloon	 (nullptr),		// 吹き出し情報
	m_pShadow	 (nullptr),		// ランキングタイトルの影情報
	m_pName		 (nullptr),		// ランキングタイトル情報
	m_pParent	 (pParent),		// 遷移ポイント情報
	m_state		 (STATE_NONE),	// 状態
	m_fCurTime	 (0.0f),		// 現在の待機時間
	m_nCurSelect (0),			// 現在の選択肢
	m_nOldSelect (0)			// 前回の選択肢
{
	// メンバ変数をクリア
	memset(&m_apRankValue[0], 0, sizeof(m_apRankValue));	// ランキング順位情報
	memset(&m_apRankTime[0], 0, sizeof(m_apRankTime));		// ランキング時間情報
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));			// 選択肢情報

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
	memset(&m_apRankValue[0], 0, sizeof(m_apRankValue));	// ランキング順位情報
	memset(&m_apRankTime[0], 0, sizeof(m_apRankTime));		// ランキング時間情報
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));			// 選択肢情報
	m_state		 = STATE_FADEOUT;	// 状態
	m_pFade		 = nullptr;	// フェード情報
	m_pTitle	 = nullptr;	// タイトル情報
	m_pBalloon	 = nullptr;	// 吹き出し情報
	m_pShadow	 = nullptr;	// ランキングタイトルの影情報
	m_pName		 = nullptr;	// ランキングタイトル情報
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
	//	吹き出しの生成 / 初期設定
	//--------------------------------------------------------
	// 吹き出しの生成
	m_pBalloon = CObject2D::Create
	( // 引数
		balloon::INIT_POS,	// 位置
		balloon::SIZE,		// 大きさ
		VEC3_ZERO,			// 向き
		balloon::INIT_COL	// 色
	);
	if (m_pBalloon == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ビルボードシーンのレンダーテクスチャを割当
	m_pBalloon->BindTexture(GET_RENDERER->GetRenderTextureIndex(CObject::SCENE_BILLBOARD));

	// 優先順位を設定
	m_pBalloon->SetPriority(PRIO_UI);

	// ラベルを設定
	m_pBalloon->SetLabel(CObject::LABEL_UI);

	//--------------------------------------------------------
	//	ランキングタイトル影の生成 / 初期設定
	//--------------------------------------------------------
	// ランキングタイトル影の生成
	m_pShadow = CString2D::Create
	( // 引数
		name::FONT,					// フォントパス
		name::ITALIC,				// イタリック
		name::STRING,				// 表示文字列
		name::POS + name::OFFSET,	// 原点位置
		name::CHAR_HEIGHT,			// 文字縦幅
		name::ALIGN_X,				// 横配置
		name::ROT,					// 原点向き
		name::COL_SHADOW			// 色
	);
	if (m_pShadow == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pShadow->SetPriority(PRIO_UI);

	//--------------------------------------------------------
	//	ランキングタイトルの生成 / 初期設定
	//--------------------------------------------------------
	// ランキングタイトルの生成
	m_pName = CString2D::Create
	( // 引数
		name::FONT,			// フォントパス
		name::ITALIC,		// イタリック
		name::STRING,		// 表示文字列
		name::POS,			// 原点位置
		name::CHAR_HEIGHT,	// 文字縦幅
		name::ALIGN_X,		// 横配置
		name::ROT,			// 原点向き
		name::COL_NAME		// 色
	);
	if (m_pName == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pName->SetPriority(PRIO_UI);

	//--------------------------------------------------------
	//	ランキングの生成 / 初期設定
	//--------------------------------------------------------
	float aRank[MAX_RANK] = {};	// ランキング配列

	// ランキングパスを作成
	std::filesystem::path path = m_pParent->GetTransMapPass();	// マップディレクトリ
	path = path.parent_path();	// マップディレクトリにする
	path.append("ranking.bin");	// ランキングパスを追加

	// ランキング読込
	if (FAILED(LoadRank(path.string().c_str(), &aRank[0])))
	{ // 読込に失敗した場合

		// ランキングの再読込
		if (FAILED(LoadRank(path.string().c_str(), &aRank[0])))
		{ // 再読込に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	for (int i = 0; i < MAX_RANK; i++)
	{ // ランキング表示数分繰り返す

		//----------------------------------------------------
		//	ランキング順位の生成 / 初期設定
		//----------------------------------------------------
		// 生成位置を計算
		D3DXVECTOR3 posRank = rank::POS + (rank::SPACE * (float)i);	// 生成位置

		// ランキング時間の生成
		m_apRankValue[i] = CAnim2D::Create
		( // 引数
			rank::PART.x,	// テクスチャ横分割数
			rank::PART.y,	// テクスチャ縦分割数
			posRank,		// 位置
			rank::SIZE,		// 大きさ
			VEC3_ZERO,		// 向き
			XCOL_AWHITE		// 色
		);
		if (m_apRankValue[i] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを割当
		m_apRankValue[i]->BindTexture(rank::TEXTURE);

		// パターンを設定
		m_apRankValue[i]->SetPattern(i);

		// 優先順位を設定
		m_apRankValue[i]->SetPriority(PRIO_UI);

		// ラベルを設定
		m_apRankValue[i]->SetLabel(CObject::LABEL_UI);

		//----------------------------------------------------
		//	ランキング時間の生成 / 初期設定
		//----------------------------------------------------
		// 生成位置を計算
		D3DXVECTOR3 posTime = time::POS + (time::SPACE * (float)i);	// 生成位置

		// ランキング時間の生成
		m_apRankTime[i] = CTimeUI::Create
		( // 引数
			aRank[i],				// 表示時間
			posTime,				// 位置
			time::VAL_SIZE,			// 数字の大きさ
			time::PART_SIZE,		// 区切りの大きさ
			time::VAL_SPACE,		// 数字の空白
			time::PART_SPACE,		// 区切りの空白
			time::TYPE,				// 数字種類
			time::ALIGN_X,			// 横配置
			CTimeUI::YALIGN_CENTER,	// 縦配置
			VEC3_ZERO,				// 向き
			XCOL_AWHITE				// 色
		);
		if (m_apRankTime[i] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apRankTime[i]->SetPriority(PRIO_UI);
	}

	//--------------------------------------------------------
	//	選択肢の生成 / 初期設定
	//--------------------------------------------------------
	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// 生成位置を計算
		D3DXVECTOR3 posSelect = select::INIT_POS + (select::SPACE * (float)i);	// 生成位置

		// 選択肢の生成
		m_apSelect[i] = CString2D::Create
		( // 引数
			select::FONT,			// フォントパス
			select::ITALIC,			// イタリック
			select::STRING[i],		// 指定文字列
			posSelect,				// 原点位置
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
CRankingManager *CRankingManager::Create(CTransPoint* pParent)
{
	// ランキングマネージャーの生成
	CRankingManager *pRankingManager = new CRankingManager(pParent);
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
//	ランキングの設定処理
//============================================================
int CRankingManager::SetRank(const float fNewTime)
{
	int nLowIdx = MAX_RANK - 1;	// 最下位インデックス
	int nUpdateIdx = NONE_IDX;	// 更新順位インデックス
	float aRank[MAX_RANK];		// ランキング時間

	// ランキングの読込
	if (FAILED(LoadRank(GET_STAGE->GetCurMapSaveRankPass().c_str(), &aRank[0])))
	{ // 読込に失敗した場合

		// ランキングの再読込
		if (FAILED(LoadRank(GET_STAGE->GetCurMapSaveRankPass().c_str(), &aRank[0])))
		{ // 再読込に失敗した場合

			// 失敗を返す
			assert(false);
			return NONE_IDX;
		}
	}

	if (fNewTime < aRank[nLowIdx])
	{ // 最下位の時間より速い場合

		// ランキングのソート
		nUpdateIdx = SortRank(fNewTime, &aRank[0]);

		// ランキングの保存
		SaveRank(GET_STAGE->GetCurMapSaveRankPass().c_str(), &aRank[0]);
	}

	// 順位が更新されたインデックスを返す
	return nUpdateIdx;
}

//============================================================
//	ランキングのソート処理
//============================================================
int CRankingManager::SortRank(const float fNewTime, float* pRankArray)
{
	int nLowIdx = MAX_RANK - 1;	// 最下位インデックス
	int nUpdateIdx = NONE_IDX;	// 更新順位インデックス
	int	nCurMinIdx;				// 最小値のインデックス

	// 現在の最下位の情報と書き換え
	pRankArray[nLowIdx] = fNewTime;

	for (int nCntKeep = 0; nCntKeep < nLowIdx; nCntKeep++)
	{ // 入れ替える値の総数 -1回分繰り返す

		// 現在の繰り返し数を代入 (要素1とする)
		nCurMinIdx = nCntKeep;

		for (int nCntSort = nCntKeep + 1; nCntSort < MAX_RANK; nCntSort++)
		{ // 入れ替える値の総数 -nCntKeep分繰り返す

			if (pRankArray[nCurMinIdx] > pRankArray[nCntSort])
			{ // 最小値に設定されている値より、現在の値のほうが小さい場合

				// 現在の要素番号を最小値に設定
				nCurMinIdx = nCntSort;
			}
		}

		if (nCntKeep != nCurMinIdx)
		{ // 最小値の要素番号に変動があった場合

			// 要素の入れ替え
			float fKeepNum			= pRankArray[nCntKeep];
			pRankArray[nCntKeep]	= pRankArray[nCurMinIdx];
			pRankArray[nCurMinIdx]	= fKeepNum;

			// 更新されたインデックスを保存
			nUpdateIdx = nCntKeep;
		}
	}

	// 順位が更新されたインデックスを返す
	return nUpdateIdx;
}

//============================================================
//	ランキングの読込処理
//============================================================
HRESULT CRankingManager::LoadRank(const char* pPath, float* pRankArray)
{
	// ファイルを開く
	std::ifstream file(pPath, std::ios_base::binary);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		float aRank[MAX_RANK];	// ランキング生成用

		// エラーメッセージボックス
		MessageBox(nullptr, "ランキングの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 値をすべて最大タイムにする
		for (int i = 0; i < MAX_RANK; i++)
		{ aRank[i] = RANK_TIME[i]; }

		// クリアタイムの保存
		SaveRank(pPath, &aRank[0]);

		// 失敗を返す
		return E_FAIL;
	}

	// 引数の獲得状況を読み込み
	file.read((char*)pRankArray, sizeof(float) * MAX_RANK);

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}

//============================================================
//	ランキングの保存処理
//============================================================
HRESULT CRankingManager::SaveRank(const char* pPath, float* pRankArray)
{
	// ファイルを開く
	std::ofstream file(pPath, std::ios_base::binary);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ランキングの書き出しに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 引数の獲得状況を書き出し
	file.write((char*)pRankArray, sizeof(float) * MAX_RANK);

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
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

		// UI出現状態にする
		m_state = STATE_SPAWN;
	}
}

//============================================================
//	UI出現の更新処理
//============================================================
void CRankingManager::UpdateSpawn(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InOutQuad(m_fCurTime, fall::MOVE_TIME, 0.0f);

	// タイトル情報の反映
	m_pTitle->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));		// 色を反映
	m_pTitle->SetVec3Position(title::DEST_POS + (((title::DEST_POS + fall::OFFSET_POS) - title::DEST_POS) * fRate));	// 位置を反映

	// 吹き出し情報の反映
	m_pBalloon->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// 色を反映
	m_pBalloon->SetVec3Position(balloon::DEST_POS + (((balloon::DEST_POS + fall::OFFSET_POS) - balloon::DEST_POS) * fRate));	// 位置を反映

	// ランキングタイトル影情報の反映
	const D3DXVECTOR3 posShadow = name::POS + name::OFFSET;				// 影文字位置
	m_pShadow->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));	// 透明度を反映
	m_pShadow->SetVec3Position(posShadow + (((posShadow + fall::OFFSET_POS) - posShadow) * fRate));	// 位置を反映

	// ランキングタイトル情報の反映
	m_pName->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));	// 透明度を反映
	m_pName->SetVec3Position(name::POS + (((name::POS + fall::OFFSET_POS) - name::POS) * fRate));	// 位置を反映

	for (int i = 0; i < MAX_RANK; i++)
	{ // ランキング表示数分繰り返す

		// ランキング時間情報の反映
		const D3DXVECTOR3 posRank = rank::POS + (rank::SPACE * (float)i);		// 生成位置
		m_apRankValue[i]->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// 色を反映
		m_apRankValue[i]->SetVec3Position(posRank + (((posRank + fall::OFFSET_POS) - posRank) * fRate));	// 位置を反映

		// ランキング時間情報の反映
		const D3DXVECTOR3 posTime = time::POS + (time::SPACE * (float)i);		// 生成位置
		m_apRankTime[i]->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// 色を反映
		m_apRankTime[i]->SetVec3Position(posTime + (((posTime + fall::OFFSET_POS) - posTime) * fRate));		// 位置を反映
	}

	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		D3DXVECTOR3 posSelect = select::DEST_POS + (((select::DEST_POS + fall::OFFSET_POS) - select::DEST_POS) * fRate);	// 位置
		D3DXVECTOR3 offsetSelect = (select::SPACE * (float)i);	// オフセット

		// 選択肢の反映
		m_apSelect[i]->SetVec3Position(posSelect + offsetSelect);	// 位置を反映
		m_apSelect[i]->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));	// 透明度を反映
	}

	if (m_fCurTime >= fall::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// タイトル情報の色を補正
		m_pTitle->SetColor(fall::INIT_COL);

		// 吹き出し情報の色を補正
		m_pBalloon->SetColor(fall::INIT_COL);

		// ランキングタイトル影情報の透明度を補正
		m_pShadow->SetAlpha(fall::INIT_ALPHA);

		// ランキングタイトル情報の透明度を補正
		m_pName->SetAlpha(fall::INIT_ALPHA);

		for (int i = 0; i < MAX_RANK; i++)
		{ // ランキング表示数分繰り返す

			// ランキング時間情報の色を補正
			m_apRankValue[i]->SetColor(fall::INIT_COL);

			// ランキング時間情報の色を補正
			m_apRankTime[i]->SetColor(fall::INIT_COL);
		}

		for (int i = 0; i < SELECT_MAX; i++)
		{ // 選択肢の総数分繰り返す

			// 選択肢の透明度を補正
			m_apSelect[i]->SetAlpha(fall::INIT_ALPHA);
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

	// タイトル情報の反映
	m_pTitle->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));		// 色を反映
	m_pTitle->SetVec3Position(title::DEST_POS + (((title::DEST_POS + fall::OFFSET_POS) - title::DEST_POS) * fRate));	// 位置を反映

	// 吹き出し情報の反映
	m_pBalloon->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// 色を反映
	m_pBalloon->SetVec3Position(balloon::DEST_POS + (((balloon::DEST_POS + fall::OFFSET_POS) - balloon::DEST_POS) * fRate));	// 位置を反映

	// ランキングタイトル影情報の反映
	const D3DXVECTOR3 posShadow = name::POS + name::OFFSET;				// 影文字位置
	m_pShadow->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));	// 透明度を反映
	m_pShadow->SetVec3Position(posShadow + (((posShadow + fall::OFFSET_POS) - posShadow) * fRate));	// 位置を反映

	// ランキングタイトル情報の反映
	m_pName->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));	// 透明度を反映
	m_pName->SetVec3Position(name::POS + (((name::POS + fall::OFFSET_POS) - name::POS) * fRate));	// 位置を反映

	for (int i = 0; i < MAX_RANK; i++)
	{ // ランキング表示数分繰り返す

		// ランキング時間情報の反映
		const D3DXVECTOR3 posRank = rank::POS + (rank::SPACE * (float)i);		// 生成位置
		m_apRankValue[i]->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// 色を反映
		m_apRankValue[i]->SetVec3Position(posRank + (((posRank + fall::OFFSET_POS) - posRank) * fRate));	// 位置を反映

		// ランキング時間情報の反映
		const D3DXVECTOR3 posTime = time::POS + (time::SPACE * (float)i);		// 生成位置
		m_apRankTime[i]->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// 色を反映
		m_apRankTime[i]->SetVec3Position(posTime + (((posTime + fall::OFFSET_POS) - posTime) * fRate));		// 位置を反映
	}

	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		D3DXVECTOR3 posSelect = select::DEST_POS + (((select::DEST_POS + fall::OFFSET_POS) - select::DEST_POS) * fRate);	// 位置
		D3DXVECTOR3 offsetSelect = (select::SPACE * (float)i);	// オフセット

		// 選択肢の反映
		m_apSelect[i]->SetVec3Position(posSelect + offsetSelect);	// 位置を反映
		m_apSelect[i]->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));	// 透明度を反映
	}

	if (m_fCurTime >= fall::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// タイトル情報の色を補正
		m_pTitle->SetColor(fall::DEST_COL);

		// 吹き出し情報の色を補正
		m_pBalloon->SetColor(fall::DEST_COL);

		// ランキングタイトル影情報の透明度を補正
		m_pShadow->SetAlpha(fall::DEST_ALPHA);

		// ランキングタイトル情報の透明度を補正
		m_pName->SetAlpha(fall::DEST_ALPHA);

		for (int i = 0; i < MAX_RANK; i++)
		{ // ランキング表示数分繰り返す

			// ランキング時間情報の色を補正
			m_apRankValue[i]->SetColor(fall::DEST_COL);

			// ランキング時間情報の色を補正
			m_apRankTime[i]->SetColor(fall::DEST_COL);
		}

		for (int i = 0; i < SELECT_MAX; i++)
		{ // 選択肢の総数分繰り返す

			// 選択肢の透明度を補正
			m_apSelect[i]->SetAlpha(fall::DEST_ALPHA);
		}

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

	// タイトルの色を補正
	m_pTitle->SetColor(title::DEST_COL);

	// タイトルの位置を補正
	m_pTitle->SetVec3Position(title::DEST_POS);

	// 吹き出し情報の色を補正
	m_pBalloon->SetColor(balloon::DEST_COL);

	// ランキングタイトル影情報の透明度を補正
	m_pShadow->SetAlpha(1.0f);

	// ランキングタイトル情報の透明度を補正
	m_pName->SetAlpha(1.0f);

	for (int i = 0; i < MAX_RANK; i++)
	{ // ランキング表示数分繰り返す

		// ランキング時間情報の色を補正
		m_apRankValue[i]->SetColor(XCOL_WHITE);

		// ランキング時間情報の色を補正
		m_apRankTime[i]->SetColor(XCOL_WHITE);
	}

	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// 選択肢の色を補正
		m_apSelect[i]->SetColor(select::DEST_COL);

		// 選択肢の位置を補正
		m_apSelect[i]->SetVec3Position(select::DEST_POS + (select::SPACE * (float)i));
	}
}
