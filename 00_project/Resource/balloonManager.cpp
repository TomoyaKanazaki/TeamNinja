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
#include "scrollText2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
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

	namespace name
	{
		const char *FONT = "data\\FONT\\零ゴシック.otf";	// フォントパス
		const bool	ITALIC			= true;		// イタリック
		const float	CHAR_HEIGHT		= 140.0f;	// 文字縦幅
		const float	LINE_HEIGHT		= 120.0f;	// 行間縦幅
		const float	WAIT_TIME_NOR	= 0.017f;	// 文字表示の待機時間

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
CBalloonManager::CBalloonManager(CTransPoint* pParent) :
	m_pParent	(pParent)	// 遷移ポイント情報
{

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
	//--------------------------------------------------------
	//	生成済みのテクスチャ作成用オブジェクトの破棄
	//--------------------------------------------------------
	// ビルボードシーン内のオブジェクトを全破棄
	CObject::ReleaseAll(CObject::SCENE_BILLBOARD);

	//--------------------------------------------------------
	//	ステージ画面ポリゴンの生成 / 設定
	//--------------------------------------------------------
	// ステージ画面ポリゴンの生成
	CObject2D* pStage = CObject2D::Create(stage::POS, stage::SIZE);
	if (pStage == nullptr) { assert(false); return E_FAIL; }	// 失敗した場合抜ける

	// 情報の設定
	pStage->BindTexture("data\\TEXTURE\\stage000.png");	// ステージ画面のテクスチャ割当	// TODO：固定になってるよ
	pStage->SetScene(CObject::SCENE_BILLBOARD);			// オブジェクトシーンをビルボードに
	pStage->SetLabel(CObject::LABEL_UI);				// 自動更新/自動破棄するように

	//--------------------------------------------------------
	//	フレームポリゴンの生成 / 設定
	//--------------------------------------------------------
	// フレームポリゴンの生成
	CObject2D* pFrame = CObject2D::Create(frame::POS, frame::SIZE);
	if (pFrame == nullptr) { assert(false); return E_FAIL; }	// 失敗した場合抜ける

	// 情報の設定
	pFrame->BindTexture(frame::TEXTURE);		// フレームのテクスチャ割当
	pFrame->SetScene(CObject::SCENE_BILLBOARD);	// オブジェクトシーンをビルボードに
	pFrame->SetLabel(CObject::LABEL_UI);		// 自動更新/自動破棄するように

	//--------------------------------------------------------
	//	ステージ名の影の生成 / 設定
	//--------------------------------------------------------
	// ステージ名の影の生成
	CScrollText2D* pShadow = CScrollText2D::Create
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
	if (pShadow == nullptr) { assert(false); return E_FAIL; }	// 失敗した場合抜ける

	// 情報の設定
	pShadow->SetScene(CObject::SCENE_BILLBOARD);	// オブジェクトシーンをビルボードに
	pShadow->SetEnableScroll(true);					// 文字送りを開始

	// テキストを割当
	loadtext::BindText(pShadow, loadtext::LoadText(GET_STAGE->Regist(m_pParent->GetTransMapPass().c_str()).sStage.c_str(), 0));

	//--------------------------------------------------------
	//	ステージ名の生成 / 設定
	//--------------------------------------------------------
	// ステージ名の生成
	CScrollText2D* pName = CScrollText2D::Create
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
	if (pName == nullptr) { assert(false); return E_FAIL; }	// 失敗した場合抜ける

	// 情報の設定
	pName->SetScene(CObject::SCENE_BILLBOARD);	// オブジェクトシーンをビルボードに
	pName->SetEnableScroll(true);				// 文字送りを開始

	// テキストを割当
	loadtext::BindText(pName, loadtext::LoadText(GET_STAGE->Regist(m_pParent->GetTransMapPass().c_str()).sStage.c_str(), 0));

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

}

//============================================================
//	描画処理
//============================================================
void CBalloonManager::Draw(CShader *pShader)
{

}

//============================================================
//	生成処理
//============================================================
CBalloonManager *CBalloonManager::Create(CTransPoint* pParent)
{
	// 吹き出しマネージャーの生成
	CBalloonManager *pBalloonManager = new CBalloonManager(pParent);
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
