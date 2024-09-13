//============================================================
//
//	神器UI処理 [goditemUI.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "goditemUI.h"

#include "anim2D.h"
#include "goditem_mark.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 0;	// タイマーの優先順位

	const D3DXVECTOR3 POS[CGodItem::TYPE_MAX] =		// 位置
	{
		D3DXVECTOR3(SCREEN_CENT.x, 30.0f, 0.0f),	// 赤
		D3DXVECTOR3(615.0f, 75.0f, 0.0f),	// 緑
		D3DXVECTOR3(665.0f, 75.0f, 0.0f)	// 青
	};
	const D3DXVECTOR3 ROT[CGodItem::TYPE_MAX] =		// 向き
	{
		D3DXVECTOR3(0.0f, 0.0f, (D3DX_PI * 2.0f) * 0.3f),	// 赤
		D3DXVECTOR3(0.0f, 0.0f, (D3DX_PI * 2.0f) * 0.6f),	// 緑
		D3DXVECTOR3(0.0f, 0.0f, (D3DX_PI * 2.0f) * 0.9f)	// 青
	};
	const D3DXVECTOR3 GROUND_SIZE = D3DXVECTOR3(60.0f, 60.0f, 0.0f);	// 下地のサイズ
	const char* TEXTURE = "data\\TEXTURE\\itemGod000.png";		// テクスチャ
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CGodItemUI>* CGodItemUI::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CGodItemUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGodItemUI::CGodItemUI() : CObject(CObject::LABEL_GODITEMUI, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY)
{
	// メンバ変数をクリア
	memset(&m_apGround[0], 0, sizeof(m_apGround));	// 下地の情報
	memset(&m_apMark[0], 0, sizeof(m_apMark));		// マークの情報
}

//============================================================
//	デストラクタ
//============================================================
CGodItemUI::~CGodItemUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGodItemUI::Init(void)
{
	// メンバ変数を初期化
	for (int nCntGround = 0; nCntGround < CGodItem::TYPE_MAX; nCntGround++)
	{
		// 下地が NULL の場合、次に進む
		if (m_apGround[nCntGround] != nullptr) { assert(false); continue; }

		// 下地を生成する
		m_apGround[nCntGround] = CAnim2D::Create
		(
			CGodItem::TYPE_MAX,	// テクスチャの横の分割数
			1,					// テクスチャの縦の分割数
			POS[nCntGround],	// 位置
			GROUND_SIZE,		// サイズ
			ROT[nCntGround],	// 向き
			XCOL_BLACK			// 色
		);

		// テクスチャを割り当てる
		m_apGround[nCntGround]->BindTexture(TEXTURE);

		// 優先順位を設定
		m_apGround[nCntGround]->SetPriority(PRIORITY);

		// カウンターを設定する
		m_apGround[nCntGround]->SetCounter(0);

		// パターンを設定する
		m_apGround[nCntGround]->SetPattern(nCntGround);

		// 停止状況を設定
		m_apGround[nCntGround]->SetEnableStop(false);
	}

	// メンバ変数を初期化
	for (int nCntMark = 0; nCntMark < CGodItem::TYPE_MAX; nCntMark++)
	{
		// マークが NULL の場合、次に進む
		if (m_apMark[nCntMark] != nullptr) { assert(false); continue; }

		// 生成処理
		m_apMark[nCntMark] = CGodItemMark::Create
		(
			POS[nCntMark],
			ROT[nCntMark],
			nCntMark,
			PRIORITY
		);
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CGodItemUI>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGodItemUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < CGodItem::TYPE_MAX; nCnt++)
	{
		// 下地と本体を破棄する
		SAFE_UNINIT(m_apGround[nCnt]);
		SAFE_UNINIT(m_apMark[nCnt]);
	}

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CGodItemUI::Update(const float fDeltaTime)
{
	// 総数が1未満の場合、停止
	if (m_pList->GetNumAll() > 1) { assert(false); }

	for (int nCnt = 0; nCnt < CGodItem::TYPE_MAX; nCnt++)
	{
		if (m_apGround[nCnt] != nullptr)
		{ // 下地が NULL じゃない場合

			// 更新処理
			m_apGround[nCnt]->Update(fDeltaTime);
		}

		if (m_apMark[nCnt] != nullptr)
		{ // 本体が NULL じゃない場合

			// 更新処理
			m_apMark[nCnt]->Update(fDeltaTime);
		}
	}
}

//============================================================
//	描画処理
//============================================================
void CGodItemUI::Draw(CShader* /*pShader*/)
{
	for (int nCntGround = 0; nCntGround < CGodItem::TYPE_MAX; nCntGround++)
	{
		// 下地が NULL じゃない場合、次に進む
		if (m_apGround[nCntGround] == nullptr) { continue; }

		// 描画処理
		m_apGround[nCntGround]->Draw();
	}

	for (int nCntMark = 0; nCntMark < CGodItem::TYPE_MAX; nCntMark++)
	{
		// 本体が NULL じゃない場合、次に進む
		if (m_apMark[nCntMark] == nullptr) { continue; }

		// 描画処理
		m_apMark[nCntMark]->Draw();
	}
}

//============================================================
// 取得処理
//============================================================
void CGodItemUI::Get(const CGodItem::EType type)
{
	// 既に取得していた場合、停止
	if (m_apMark[type]->GetState() != CGodItemMark::STATE_NONE) { assert(false); return; }

	// 拡縮状態にする
	m_apMark[type]->SetState(CGodItemMark::STATE_SCALING);
}

//============================================================
//	生成処理
//============================================================
CGodItemUI* CGodItemUI::Create(void)
{
	// タイマーの生成
	CGodItemUI* pTimer = new CGodItemUI;

	if (pTimer == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// タイマーの初期化
		if (FAILED(pTimer->Init()))
		{ // 初期化に失敗した場合

			// タイマーの破棄
			SAFE_DELETE(pTimer);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pTimer;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CGodItemUI>* CGodItemUI::GetList(void)
{
	// オブジェクトリストを返す
	return m_pList;
}
