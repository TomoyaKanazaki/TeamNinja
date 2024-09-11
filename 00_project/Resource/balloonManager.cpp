//============================================================
//
//	吹き出しマネージャー処理 [balloonManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "balloonManager.h"
#include "manager.h"
#include "stage.h"
#include "loadtext.h"
#include "transpoint.h"
#include "object2D.h"
#include "roll2D.h"
#include "scale2D.h"
#include "scrollText2D.h"
#include "anim2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXCOLOR COL_OPEN	= XCOL_WHITE;							// 入場可能色
	const D3DXCOLOR COL_UNOPEN	= D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f);	// 入場不可能色

	namespace stage
	{
		const D3DXVECTOR3 POS	= SCREEN_CENT;			// 位置
		const D3DXVECTOR3 SIZE	= SCREEN_SIZE * 0.66f;	// 大きさ
	}

	namespace frame
	{
		const char* TEXTURE		= "data\\TEXTURE\\stageFrame000.png";	// フレームテクスチャパス
		const D3DXVECTOR3 POS	= SCREEN_CENT;							// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(1227.0f, 720.0f, 0.0f);	// 大きさ
	}

	namespace star
	{
		const char*	TEXTURE	= "data\\TEXTURE\\star000.png";	// 手裏剣テクスチャパス
		const D3DXVECTOR3 POS[CBalloonManager::NUM_STAR]	= { D3DXVECTOR3(140.0f, 230.0f, 0.0f),	D3DXVECTOR3(240.0f, 140.0f, 0.0f) };	// 位置
		const D3DXVECTOR3 ROT[CBalloonManager::NUM_STAR]	= { D3DXVECTOR3(0.0f, 0.0f, 0.8f),		D3DXVECTOR3(0.0f, 0.0f, 0.2f) };		// 向き
		const D3DXVECTOR3 SIZE[CBalloonManager::NUM_STAR]	= { D3DXVECTOR3(250.0f, 250.0f, 0.0f),	D3DXVECTOR3(320.0f, 320.0f, 0.0f) };	// 大きさ
		const D3DXCOLOR COL[CBalloonManager::NUM_STAR]		= { XCOL_WHITE, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f) };	// 色
		const float	ADD_ROT[CBalloonManager::NUM_STAR]		= { 0.2f, 0.3f };	// 回転量
	}

	namespace cont
	{
		const char* TEXTURE		= "data\\TEXTURE\\button_a.png";		// 操作方法テクスチャパス
		const D3DXVECTOR3 POS	= D3DXVECTOR3(975.0f, 500.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(160.0f, 160.0f, 0.0f);	// 大きさ
		const float MIN_SCALE	= 0.95f;	// 最低拡大率
		const float MAX_SCALE	= 1.45f;	// 最大拡大率
		const float INIT_SCALE	= 0.95f;	// 初期拡大率
		const float CALC_SCALE	= 7.0f;		// 拡大率の加減量
	}

	namespace item
	{
		const D3DXCOLOR COL[] =
		{
			D3DXCOLOR(0.35f, 0.35f, 0.35f, 1.0f),	// 未獲得色
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// 獲得済み色
		};

		const char* TEXTURE		= "data\\TEXTURE\\itemGod000.png";		// 神器テクスチャ
		const POSGRID2 TEX_PART	= POSGRID2(3, 1);						// テクスチャ分割
		const D3DXVECTOR3 POS	= D3DXVECTOR3(180.0f, 570.0f, 0.0f);	// 位置
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(165.0f, 0.0f, 0.0f);		// 空白
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(190.0f, 190.0f, 0.0f);	// 大きさ
	}

	namespace name
	{
		const char *FONT = "data\\FONT\\玉ねぎ楷書激無料版v7改.ttf";	// フォントパス
		const bool	ITALIC			= true;		// イタリック
		const float	CHAR_HEIGHT		= 120.0f;	// 文字縦幅
		const float	LINE_HEIGHT		= 120.0f;	// 行間縦幅
		const float	WAIT_TIME_NOR	= 0.017f;	// 文字表示の待機時間
		const float	WAIT_TIME_NOR_FIRST	= 0.17f;	// 文字表示の待機時間

		const D3DXVECTOR3 POS	 = D3DXVECTOR3(90.0f, 40.0f, 0.0f);	// 位置
		const D3DXVECTOR3 OFFSET = D3DXVECTOR3(8.0f, 8.0f, 0.0f);	// 影オフセット
		const D3DXVECTOR3 ROT	 = VEC3_ZERO;						// 向き

		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// 横配置
		const CText2D::EAlignY	 ALIGN_Y = CText2D::YALIGN_TOP;		// 縦配置
		const D3DXCOLOR COL_SHADOW	= XCOL_BLUE;	// 影の色
		const D3DXCOLOR COL_NAME	= XCOL_WHITE;	// 名前の色
	}
}

//************************************************************
//	子クラス [CBalloonManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBalloonManager::CBalloonManager(CTransPoint* pParent, const bool bOpen) :
	m_pParent	(pParent),		// 遷移ポイント
	m_pStage	(nullptr),		// ステージ画面
	m_pFrame	(nullptr),		// フレーム
	m_pCont		(nullptr),		// 操作方法
	m_pShadow	(nullptr),		// ステージ名の影
	m_pName		(nullptr),		// ステージ名
	m_state		(STATE_NONE),	// 状態
	m_bOpen		(bOpen)			// ステージ解放フラグ
{
	// メンバ変数をクリア
	memset(&m_apGodItem[0], 0, sizeof(m_apGodItem));	// 神器アイコン
	memset(&m_apStar[0], 0, sizeof(m_apStar));			// 手裏剣
}

//============================================================
//	デストラクタ
//============================================================
CBalloonManager::~CBalloonManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBalloonManager::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apGodItem[0], 0, sizeof(m_apGodItem));	// 神器アイコン
	memset(&m_apStar[0], 0, sizeof(m_apStar));			// 手裏剣
	m_pStage	= nullptr;		// ステージ画面
	m_pFrame	= nullptr;		// フレーム
	m_pCont		= nullptr;		// 操作方法
	m_pShadow	= nullptr;		// ステージ名の影
	m_pName		= nullptr;		// ステージ名
	m_state		= STATE_NONE;	// 状態

	// ビルボードシーン内のオブジェクトを全破棄
	CObject::ReleaseAll(CObject::SCENE_BILLBOARD);

	// ポリゴン色を設定
	D3DXCOLOR colPoly = (m_bOpen) ? COL_OPEN : COL_UNOPEN;

	//--------------------------------------------------------
	//	ステージ画面ポリゴンの生成 / 設定
	//--------------------------------------------------------
	// ステージ画面ポリゴンの生成
	m_pStage = CObject2D::Create(stage::POS, stage::SIZE, VEC3_ZERO, colPoly);
	if (m_pStage == nullptr) { assert(false); return E_FAIL; }	// 失敗した場合抜ける

	// 情報の設定
	m_pStage->BindTexture("data\\TEXTURE\\stage000.png");	// ステージ画面のテクスチャ割当	// TODO：固定になってるよ
	m_pStage->SetScene(CObject::SCENE_BILLBOARD);			// オブジェクトシーンをビルボードに
	m_pStage->SetLabel(CObject::LABEL_UI);					// 自動更新/自動破棄するように

	//--------------------------------------------------------
	//	フレームポリゴンの生成 / 設定
	//--------------------------------------------------------
	// フレームポリゴンの生成
	m_pFrame = CObject2D::Create(frame::POS, frame::SIZE, VEC3_ZERO, colPoly);
	if (m_pFrame == nullptr) { assert(false); return E_FAIL; }	// 失敗した場合抜ける

	// 情報の設定
	m_pFrame->BindTexture(frame::TEXTURE);			// フレームのテクスチャ割当
	m_pFrame->SetScene(CObject::SCENE_BILLBOARD);	// オブジェクトシーンをビルボードに
	m_pFrame->SetLabel(CObject::LABEL_UI);			// 自動更新/自動破棄するように

	//--------------------------------------------------------
	//	手裏剣の生成 / 設定
	//--------------------------------------------------------
	for (int i = 0; i < CBalloonManager::NUM_STAR; i++)
	{ // 手裏剣の総数分繰り返す

		// 手裏剣の色を設定
		D3DXCOLOR colStar = (m_bOpen) ? star::COL[i] : COL_UNOPEN;

		// 手裏剣の生成
		m_apStar[i] = CRoll2D::Create(star::POS[i], star::SIZE[i], star::ADD_ROT[i], star::ROT[i], colStar);
		if (m_apStar[i] == nullptr) { assert(false); return E_FAIL; }	// 失敗した場合抜ける

		// 情報の設定
		m_apStar[i]->BindTexture(star::TEXTURE);			// フレームのテクスチャ割当
		m_apStar[i]->SetScene(CObject::SCENE_BILLBOARD);	// オブジェクトシーンをビルボードに
	}

	//--------------------------------------------------------
	//	操作方法の生成 / 設定
	//--------------------------------------------------------
	// 操作方法の生成
	m_pCont = CScale2D::Create(cont::POS, cont::SIZE, cont::MIN_SCALE, cont::MAX_SCALE, cont::INIT_SCALE, cont::CALC_SCALE, 1.0f, 1.0f, VEC3_ZERO, colPoly);
	if (m_pCont == nullptr) { assert(false); return E_FAIL; }	// 失敗した場合抜ける

	// 情報の設定
	m_pCont->BindTexture(cont::TEXTURE);			// 操作方法のテクスチャ割当
	m_pCont->SetScene(CObject::SCENE_BILLBOARD);	// オブジェクトシーンをビルボードに
	m_pCont->SetLabel(CObject::LABEL_UI);			// 自動更新/自動破棄するように

	//--------------------------------------------------------
	//	神器アイコンの生成 / 初期設定
	//--------------------------------------------------------
	// 神器獲得パスを作成
	std::filesystem::path path = m_pParent->GetTransMapPass();	// マップディレクトリ
	path = path.parent_path();	// マップディレクトリにする
	path.append("goditem.bin");	// 神器獲得パスを追加

	// 前回までの勾玉の獲得状況を読み込み
	bool bOldGet[CGodItem::TYPE_MAX] = {};	// 保存されている獲得状況
	CGodItem::LoadPossess(path.string().c_str(), &bOldGet[0]);

	for (int i = 0; i < CGodItem::TYPE_MAX; i++)
	{ // 神器の総数分繰り返す

		// アイコン生成位置を計算
		D3DXVECTOR3 posIcon = item::POS + (item::SPACE * (float)i);

		// 手裏剣の色を設定
		D3DXCOLOR colIcon = (m_bOpen) ? item::COL[(int)bOldGet[i]] : item::COL[(int)false];

		// 神器アイコンの生成
		m_apGodItem[i] = CAnim2D::Create(item::TEX_PART.x, item::TEX_PART.y, posIcon, item::SIZE, VEC3_ZERO, colIcon);
		if (m_apGodItem[i] == nullptr) { assert(false); return E_FAIL; }	// 失敗した場合抜ける

		// 情報の設定
		m_apGodItem[i]->BindTexture(item::TEXTURE);			// 勾玉のテクスチャ割当
		m_apGodItem[i]->SetScene(CObject::SCENE_BILLBOARD);	// オブジェクトシーンをビルボードに
		m_apGodItem[i]->SetLabel(CObject::LABEL_UI);		// 自動更新/自動破棄するように
		m_apGodItem[i]->SetPattern(i);						// テクスチャパターンを設定
	}

	if (m_bOpen)
	{ // 解放されている場合

		//----------------------------------------------------
		//	ステージ名の影の生成 / 設定
		//----------------------------------------------------
		// ステージ名の影の生成
		m_pShadow = CScrollText2D::Create
		( // 引数
			name::FONT,					// フォントパス
			name::ITALIC,				// イタリック
			name::POS + name::OFFSET,	// 原点位置
			name::WAIT_TIME_NOR,		// 文字表示の待機時間
			name::CHAR_HEIGHT,			// 文字縦幅
			name::LINE_HEIGHT,			// 行間縦幅
			name::ALIGN_X,				// 横配置
			name::ALIGN_Y,				// 縦配置
			name::ROT,					// 原点向き
			name::COL_SHADOW			// 色
		);
		if (m_pShadow == nullptr) { assert(false); return E_FAIL; }	// 失敗した場合抜ける

		// 情報の設定
		m_pShadow->SetScene(CObject::SCENE_BILLBOARD);	// オブジェクトシーンをビルボードに

		// テキストを割当
		loadtext::BindText(m_pShadow, loadtext::LoadText(GET_STAGE->Regist(m_pParent->GetTransMapPass().c_str()).sStage.c_str(), 0));

		//----------------------------------------------------
		//	ステージ名の生成 / 設定
		//----------------------------------------------------
		// ステージ名の生成
		m_pName = CScrollText2D::Create
		( // 引数
			name::FONT,				// フォントパス
			name::ITALIC,			// イタリック
			name::POS,				// 原点位置
			name::WAIT_TIME_NOR,	// 文字表示の待機時間
			name::CHAR_HEIGHT,		// 文字縦幅
			name::LINE_HEIGHT,		// 行間縦幅
			name::ALIGN_X,			// 横配置
			name::ALIGN_Y,			// 縦配置
			name::ROT,				// 原点向き
			name::COL_NAME			// 色
		);
		if (m_pName == nullptr) { assert(false); return E_FAIL; }	// 失敗した場合抜ける

		// 情報の設定
		m_pName->SetScene(CObject::SCENE_BILLBOARD);	// オブジェクトシーンをビルボードに

		// テキストを割当
		loadtext::BindText(m_pName, loadtext::LoadText(GET_STAGE->Regist(m_pParent->GetTransMapPass().c_str()).sStage.c_str(), 0));
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CBalloonManager::Uninit(void)
{
	// オブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CBalloonManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:
		break;

	case STATE_STAG:

		if (!m_pName->IsScroll())
		{ // 文字送りが終了した場合

			for (int i = 0; i < CBalloonManager::NUM_STAR; i++)
			{ // 手裏剣の総数分繰り返す

				// 自動回転をONにする
				m_apStar[i]->SetRoll(true);
			}

			// 自動拡縮をONにする
			m_pCont->SetScale(true);

			// 終了状態にする
			m_state = STATE_END;
		}
		break;

	case STATE_END:
		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
//	描画処理
//============================================================
void CBalloonManager::Draw(CShader * /*pShader*/)
{

}

//============================================================
//	生成処理
//============================================================
CBalloonManager *CBalloonManager::Create(CTransPoint* pParent, const bool bOpen)
{
	// 吹き出しマネージャーの生成
	CBalloonManager *pBalloonManager = new CBalloonManager(pParent, bOpen);
	if (pBalloonManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 吹き出しマネージャーの初期化
		if (FAILED(pBalloonManager->Init()))
		{ // 初期化に失敗した場合

			// 吹き出しマネージャーの破棄
			SAFE_DELETE(pBalloonManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pBalloonManager;
	}
}

//============================================================
//	初回演出開始の設定処理
//============================================================
void CBalloonManager::SetFirstStag(void)
{
	// 文字表示の待機時間設定
	m_pShadow->SetNextTime(name::WAIT_TIME_NOR_FIRST);	// ステージ名の影
	m_pName->SetNextTime(name::WAIT_TIME_NOR_FIRST);	// ステージ名

	// 文字送りを開始
	m_pShadow->SetEnableDraw(true);
	m_pName->SetEnableDraw(true);

	for (int i = 0; i < CBalloonManager::NUM_STAR; i++)
	{ // 手裏剣の総数分繰り返す

		// 自動回転をONにする
		m_apStar[i]->SetRoll(true);
	}

	// 終了状態にする
	m_state = STATE_END;
}

//============================================================
//	演出開始の設定処理
//============================================================
void CBalloonManager::SetStag(void)
{
	if (m_bOpen)
	{ // 解放されている場合

		// 文字表示の待機時間設定
		m_pShadow->SetNextTime(name::WAIT_TIME_NOR);	// ステージ名の影
		m_pName->SetNextTime(name::WAIT_TIME_NOR);		// ステージ名

		// 文字送りを開始
		m_pShadow->SetEnableScroll(true);
		m_pName->SetEnableScroll(true);

		// 演出状態にする
		m_state = STATE_STAG;
	}
	else
	{ // 解放されていない場合

		// 終了状態にする
		m_state = STATE_END;
	}
}
