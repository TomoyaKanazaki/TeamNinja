//============================================================
//
//	フェード処理 [fade.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "loading.h"
#include "fadeState.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
#ifdef _DEBUG

	const CScene::EMode INIT_SCENE = CScene::MODE_SELECT;	// 初期シーン

#else	// NDEBUG

	const CScene::EMode INIT_SCENE = CScene::MODE_TITLE;	// 初期シーン

#endif	// _DEBUG

	const int PRIORITY = 7;	// フェードの優先順位
	const POSGRID2 PART_CIRCLE = POSGRID2(64, 2);	// 切り抜き型の分割数
	const D3DXVECTOR3 SCREEN_EDGE[] =	// 四つ角の頂点
	{
		VEC3_ZERO,								// 左上
		D3DXVECTOR3(SCREEN_SIZE.x, 0.0f, 0.0f),	// 右上
		D3DXVECTOR3(0.0f, SCREEN_SIZE.y, 0.0f),	// 左下
		SCREEN_SIZE,							// 右下
	};
}

//************************************************************
//	親クラス [CFade] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFade::CFade() :
	m_pFuncSetMode	(nullptr),		// モード設定関数ポインタ
	m_pCrop			(nullptr),		// 切り抜き型情報
	m_pState		(nullptr),		// 状態
	m_modeNext		(INIT_SCENE),	// 次シーン
	m_fInTime		(0.0f),			// イン時間
	m_fOutTime		(0.0f)			// アウト時間
{

}

//============================================================
//	デストラクタ
//============================================================
CFade::~CFade()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CFade::Init(void)
{
	// メンバ変数を初期化
	m_pFuncSetMode	= nullptr;		// モード設定関数ポインタ
	m_pCrop			= nullptr;		// 切り抜き型情報
	m_pState		= nullptr;		// 状態
	m_modeNext		= INIT_SCENE;	// 次シーン
	m_fInTime		= 0.0f;			// イン時間
	m_fOutTime		= 0.0f;			// アウト時間

	// フェードイン状態にする
	ChangeState(new CFadeStateIn);

	// オブジェクト2Dの初期化
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 位置をスクリーン中央にする
	SetVec3Position(SCREEN_CENT);

	// 大きさをスクリーンサイズにする
	SetVec3Sizing(SCREEN_SIZE);

	// 色を黒にする
	SetColor(XCOL_BLACK);

	// 優先順位を一番上にする
	SetPriority(PRIORITY);

	// ラベル指定なしにする
	SetLabel(CObject::LABEL_NONE);	// 自動破棄・更新を停止する

	// 切り抜き型の生成
	m_pCrop = CObjectCircle2D::Create(SCREEN_CENT, VEC3_ZERO, XCOL_AWHITE, PART_CIRCLE, 0.0f);
	if (m_pCrop == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 自動更新・自動描画を停止させる
	m_pCrop->SetEnableUpdate(false);
	m_pCrop->SetEnableDraw(false);

	// シーンの初期化
	if (FAILED(GET_MANAGER->InitScene(m_modeNext)))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CFade::Uninit(void)
{
	// 切り抜き型の終了
	SAFE_UNINIT(m_pCrop);

	// 状態の終了
	SAFE_UNINIT(m_pState);

	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CFade::Update(const float fDeltaTime)
{
	// フェードしていない場合抜ける
	if (!IsFade()) { return; }

	// 状態ごとの更新
	assert(m_pState != nullptr);
	m_pState->Update(fDeltaTime);

	// オブジェクト2Dの更新
	CObject2D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CFade::Draw(CShader *pShader)
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// ステンシルテストを有効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	if (typeid(*m_pState) == typeid(CFadeStateIrisIn)
	||  typeid(*m_pState) == typeid(CFadeStateIrisOut))
	{ // アイリスイン・アウト状態の場合

		// 比較参照値を設定する
		pDevice->SetRenderState(D3DRS_STENCILREF, 1);

		// ステンシルマスクを指定する 
		pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

		// ステンシル比較関数を指定する
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

		// ステンシル結果に対しての反映設定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Zテスト・ステンシルテスト成功
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

		// 切り抜き型の描画
		m_pCrop->Draw();
	}

	// 比較参照値を設定する
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);

	// ステンシルマスクを指定する 
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	// ステンシル比較関数を指定する
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);

	// ステンシル結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);	// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);	// Zテスト失敗・ステンシルテスト成功

	// オブジェクト2Dの描画
	CObject2D::Draw(pShader);

	// ステンシルテストを無効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//============================================================
//	次シーン遷移処理
//============================================================
void CFade::TransNextMode(void)
{
	if (m_pFuncSetMode != nullptr)
	{ // モード設定関数が指定されている場合

		// モードの設定
		m_pFuncSetMode(m_modeNext);
	}
}

//============================================================
//	フェード中かの判定取得処理
//============================================================
bool CFade::IsFade(void)
{
	// フェード中かの判定を返す
	assert(m_pState != nullptr);
	return (typeid(*m_pState) != typeid(CFadeStateNone));
}

//============================================================
//	フェードイン中かの判定取得処理
//============================================================
bool CFade::IsFadeIn(void)
{
	// フェード中かの判定を返す
	assert(m_pState != nullptr);
	return (typeid(*m_pState) == typeid(CFadeStateIn) || typeid(*m_pState) == typeid(CFadeStateIrisIn));
}

//============================================================
//	フェードの開始処理
//============================================================
void CFade::SetFade
(
	const float fInTime,	// イン時間
	const float fOutTime,	// アウト時間
	const int nPriority,	// 優先順位
	const D3DXCOLOR& rCol	// 色
)
{
	// フェード中の場合抜ける
	if (IsFade()) { return; }

	// α値加減時間を設定
	m_fInTime	= fInTime;
	m_fOutTime	= fOutTime;

	// 優先順位を設定
	SetPriority(nPriority);

	// 色を黒にする
	SetColor(rCol);

	// モード設定関数ポインタを初期化
	m_pFuncSetMode = nullptr;

	// フェードアウト状態にする
	ChangeState(new CFadeStateOut);
}

//============================================================
//	次シーンの設定処理 (フェードのみ)
//============================================================
void CFade::SetModeFade
(
	const CScene::EMode mode,	// 次シーン
	const float fWaitTime,		// 余韻時間
	const float fInTime,		// イン時間
	const float fOutTime		// アウト時間
)
{
	// フェード中の場合抜ける
	if (IsFade()) { return; }

	// 余韻フレームオーバー
	assert(fWaitTime >= 0.0f);

	// 引数のモードを設定
	m_modeNext = mode;

	// α値加減時間を設定
	m_fInTime	= fInTime;
	m_fOutTime	= fOutTime;

	// 優先順位を設定
	SetPriority(PRIORITY);

	// 色を黒にする
	SetColor(XCOL_ABLACK);

	// ロード画面を挟まないモード設定関数を設定
	m_pFuncSetMode = std::bind(&CManager::SetMode, GET_MANAGER, std::placeholders::_1);

	if (fWaitTime <= 0.0f)
	{ // カウンターが未設定の場合

		// フェードアウト状態にする
		ChangeState(new CFadeStateOut);
	}
	else
	{ // カウンターが設定された場合

		// フェード余韻状態にする
		ChangeState(new CFadeStateWait(fWaitTime, new CFadeStateOut));
	}
}

//============================================================
//	次シーンの設定処理 (ロード画面付き)
//============================================================
void CFade::SetLoadFade
(
	const CScene::EMode mode,	// 次シーン
	const float fWaitTime,		// 余韻時間
	const float fInTime,		// イン時間
	const float fOutTime		// アウト時間
)
{
	// フェード中の場合抜ける
	if (IsFade()) { return; }

	// 余韻フレームオーバー
	assert(fWaitTime >= 0.0f);

	// 引数のモードを設定
	m_modeNext = mode;

	// α値加減時間を設定
	m_fInTime	= fInTime;
	m_fOutTime	= fOutTime;

	// 優先順位を設定
	SetPriority(PRIORITY);

	// 色を黒にする
	SetColor(XCOL_ABLACK);

	// ロード画面を挟むモード設定関数を設定
	m_pFuncSetMode = std::bind(&CManager::SetLoadMode, GET_MANAGER, std::placeholders::_1);

	if (fWaitTime <= 0.0f)
	{ // カウンターが未設定の場合

		// フェードアウト状態にする
		ChangeState(new CFadeStateOut);
	}
	else
	{ // カウンターが設定された場合

		// フェード余韻状態にする
		ChangeState(new CFadeStateWait(fWaitTime, new CFadeStateOut));
	}
}

//============================================================
//	アイリスフェードの開始処理
//============================================================
void CFade::SetIrisFade
(
	AGetPos pFuncPos,		// 位置取得関数
	const float fInTime,	// イン時間
	const float fOutTime,	// アウト時間
	const int nPriority,	// 優先順位
	const D3DXCOLOR& rCol	// 色
)
{
	// フェード中の場合抜ける
	if (IsFade()) { return; }

	// α値加減時間を設定
	m_fInTime	= fInTime;
	m_fOutTime	= fOutTime;

	// 優先順位を設定
	SetPriority(nPriority);

	// 色を黒にする
	SetColor(rCol);

	// モード設定関数ポインタを初期化
	m_pFuncSetMode = nullptr;

	// アイリスフェードアウト状態にする
	ChangeState(new CFadeStateIrisOut(pFuncPos));
}

//============================================================
//	次シーンの設定処理 (アイリスフェードのみ)
//============================================================
void CFade::SetModeIrisFade
(
	const CScene::EMode mode,	// 次シーン
	AGetPos pFuncPos,			// 位置取得関数
	const float fWaitTime,		// 余韻時間
	const float fInTime,		// イン時間
	const float fOutTime		// アウト時間
)
{
	// フェード中の場合抜ける
	if (IsFade()) { return; }

	// 余韻フレームオーバー
	assert(fWaitTime >= 0.0f);

	// 引数のモードを設定
	m_modeNext = mode;

	// α値加減時間を設定
	m_fInTime	= fInTime;
	m_fOutTime	= fOutTime;

	// 優先順位を設定
	SetPriority(PRIORITY);

	// 色を黒にする
	SetColor(XCOL_ABLACK);

	// ロード画面を挟まないモード設定関数を設定
	m_pFuncSetMode = std::bind(&CManager::SetMode, GET_MANAGER, std::placeholders::_1);

	if (fWaitTime <= 0.0f)
	{ // カウンターが未設定の場合

		// アイリスフェードアウト状態にする
		ChangeState(new CFadeStateIrisOut(pFuncPos));
	}
	else
	{ // カウンターが設定された場合

		// フェード余韻状態にする
		ChangeState(new CFadeStateWait(fWaitTime, new CFadeStateIrisOut(pFuncPos)));
	}
}

//============================================================
//	次シーンの設定処理 (ロード画面付きアイリスフェード)
//============================================================
void CFade::SetLoadIrisFade
(
	const CScene::EMode mode,	// 次シーン
	AGetPos pFuncPos,			// 位置取得関数
	const float fWaitTime,		// 余韻時間
	const float fInTime,		// イン時間
	const float fOutTime		// アウト時間
)
{
	// フェード中の場合抜ける
	if (IsFade()) { return; }

	// 余韻フレームオーバー
	assert(fWaitTime >= 0.0f);

	// 引数のモードを設定
	m_modeNext = mode;

	// α値加減時間を設定
	m_fInTime	= fInTime;
	m_fOutTime	= fOutTime;

	// 優先順位を設定
	SetPriority(PRIORITY);

	// 色を黒にする
	SetColor(XCOL_ABLACK);

	// ロード画面を挟むモード設定関数を設定
	m_pFuncSetMode = std::bind(&CManager::SetLoadMode, GET_MANAGER, std::placeholders::_1);

	if (fWaitTime <= 0.0f)
	{ // カウンターが未設定の場合

		// アイリスフェードアウト状態にする
		ChangeState(new CFadeStateIrisOut(pFuncPos));
	}
	else
	{ // カウンターが設定された場合

		// フェード余韻状態にする
		ChangeState(new CFadeStateWait(fWaitTime, new CFadeStateIrisOut(pFuncPos)));
	}
}

//============================================================
//	切り抜き半径の計算処理
//============================================================
float CFade::CalcCropRadius(const D3DXVECTOR3& rPos)
{
	float fMaxLength = 0.0f;	// 最大距離
	for (int i = 0; i < 4; i++)
	{ // 四頂点分繰り返す

		// 四つ角までの距離を求める
		D3DXVECTOR3 length = SCREEN_EDGE[i] - rPos;	// 二点間の距離
		float fLen = fabsf(sqrtf(length.x * length.x + length.y * length.y));	// 距離の絶対値
		if (fMaxLength < fLen)
		{ // より遠い距離感の場合

			// 最大距離を更新
			fMaxLength = fLen;
		}
	}

	// 最大距離を返す
	return fMaxLength;
}

//============================================================
//	生成処理
//============================================================
CFade *CFade::Create(void)
{
	// フェードの生成
	CFade *pFade = new CFade;
	if (pFade == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// フェードの初期化
		if (FAILED(pFade->Init()))
		{ // 初期化に失敗した場合

			// フェードの破棄
			SAFE_DELETE(pFade);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pFade;
	}
}

//============================================================
//	状態の変更処理
//============================================================
HRESULT CFade::ChangeState(CFadeState *pState)
{
	// 状態の生成に失敗している場合抜ける
	if (pState == nullptr) { assert(false); return E_FAIL; }

	// 状態インスタンスを終了
	SAFE_UNINIT(m_pState);

	// 状態インスタンスを変更
	assert(m_pState == nullptr);
	m_pState = pState;

	// 状態にコンテキストを設定
	m_pState->SetContext(this);

	// 状態インスタンスを初期化
	if (FAILED(m_pState->Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}
