//============================================================
//
//	タイトルマネージャー処理 [titleManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "titleManager.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"
#include "object2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 5;	// タイトルの優先順位

	namespace icon_bg
	{
		const float	MOVE_TIME	= 0.68f;	// 移動時間
		const float	PLUS_TIME	= 0.1f;		// 経過の延長時間
		const float	WAIT_TIME	= 0.15f;	// アイコン背景待機時間
		const float	DEST_ALPHA	= 1.0f;		// 目標透明度
		const float	INIT_ALPHA	= 0.0f;		// 初期透明度
		const float	DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// 差分透明度
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, DEST_ALPHA);		// 目標色
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);		// 初期色
		const D3DXVECTOR3 SPACE		= D3DXVECTOR3(140.0f, 0.0f, 0.0f);				// 空白
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(865.0f, 585.0f, 0.0f);			// 目標位置
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// 初期位置
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// 差分位置
	}
}

//************************************************************
//	親クラス [CTitleManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTitleManager::CTitleManager()
{
	// メンバ変数をクリア
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));	// タイトル情報
}

//============================================================
//	デストラクタ
//============================================================
CTitleManager::~CTitleManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTitleManager::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));	// タイトル情報

#if 0
	//--------------------------------------------------------
	//	タイトルの生成・設定
	//--------------------------------------------------------
	for (int i = 0; i < NUM_LOGO; i++)
	{ // ロゴの文字数分繰り返す

		// タイトルの生成
		m_apLogo[i] = CObject2D::Create
		( // 引数
			logo::POS,			// 位置
			logo::INIT_SIZE		// 大きさ
		);
		if (m_apLogo[i] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを割当
		m_apLogo[i]->BindTexture("");

		// 優先順位を設定
		m_apLogo[i]->SetPriority(PRIORITY);

		// ラベルを設定
		m_apLogo[i]->SetLabel(CObject::LABEL_UI);

		// 描画をOFFにする
		m_apLogo[i]->SetEnableDraw(false);
	}
#endif

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTitleManager::Uninit(void)
{
	for (int i = 0; i < NUM_LOGO; i++)
	{ // ロゴの文字数分繰り返す

		// タイトル情報の終了
		SAFE_UNINIT(m_apLogo[i]);
	}
}

//============================================================
//	更新処理
//============================================================
void CTitleManager::Update(const float fDeltaTime)
{

}

//============================================================
//	生成処理
//============================================================
CTitleManager *CTitleManager::Create(void)
{
	// タイトルマネージャーの生成
	CTitleManager *pTitleManager = new CTitleManager;
	if (pTitleManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// タイトルマネージャーの初期化
		if (FAILED(pTitleManager->Init()))
		{ // 初期化に失敗した場合

			// タイトルマネージャーの破棄
			SAFE_DELETE(pTitleManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pTitleManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CTitleManager::Release(CTitleManager *&prTitleManager)
{
	// タイトルマネージャーの終了
	assert(prTitleManager != nullptr);
	prTitleManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prTitleManager);
}
