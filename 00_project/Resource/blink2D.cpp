//============================================================
//
//	点滅オブジェクト2D処理 [blink2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "blink2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// 点滅オブジェクト2D表示の優先順位
}

//************************************************************
//	親クラス [CBlink2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBlink2D::CBlink2D() : CObject2D(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
	m_state		(STATE_NONE),	// 状態
	m_fWaitTime	(0.0f),			// 現在の余韻時間
	m_fMaxWait	(0.0f)			// 余韻時間
{

}

//============================================================
//	デストラクタ
//============================================================
CBlink2D::~CBlink2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBlink2D::Init(void)
{
	// メンバ変数を初期化
	m_state		= STATE_NONE;	// 状態
	m_fWaitTime	= 0.0f;			// 現在の余韻時間
	m_fMaxWait	= 0.0f;			// 余韻時間

	// オブジェクト2Dの初期化
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 透明色を設定
	SetColor(XCOL_AWHITE);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CBlink2D::Uninit(void)
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CBlink2D::Update(const float fDeltaTime)
{
	// 何もしない状態の場合抜ける
	if (m_state == STATE_NONE) { return; }

	// 色を取得
	D3DXCOLOR colFade = GetColor();

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_FADEOUT:

		// 不透明にしていく
		colFade.a += m_fAddOut * fDeltaTime;
		if (colFade.a >= 1.0f)
		{ // 不透明になった場合

			// α値を補正
			colFade.a = 1.0f;

			// 表示状態にする
			m_state = STATE_DISP;
		}
		break;

	case STATE_DISP:

		// 余韻時間を加算
		m_fWaitTime += fDeltaTime;
		if (m_fWaitTime >= m_fMaxWait)
		{ // 余韻終了した場合

			// 余韻時間を初期化
			m_fWaitTime = 0.0f;

			// フェードイン状態にする
			m_state = STATE_FADEIN;
		}
		break;

	case STATE_FADEIN:

		// 透明にしていく
		colFade.a -= m_fSubIn * fDeltaTime;
		if (colFade.a <= 0.0f)
		{ // 透明になった場合

			// α値を補正
			colFade.a = 0.0f;

			// 何もしない状態にする
			m_state = STATE_NONE;
		}
		break;

	default:
		assert(false);
		break;
	}

	// 色を反映
	SetColor(colFade);

	// オブジェクト2Dの更新
	CObject2D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CBlink2D::Draw(CShader *pShader)
{
	// オブジェクト2Dの描画
	CObject2D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CBlink2D *CBlink2D::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rSize,	// 大きさ
	const float fSubIn,			// インのα値減少量
	const float fAddOut,		// アウトのα値増加量
	const float fMaxWait,		// 余韻時間
	const D3DXVECTOR3& rRot		// 向き
)
{
	// 点滅オブジェクト2Dの生成
	CBlink2D *pBlink2D = new CBlink2D;
	if (pBlink2D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 点滅オブジェクト2Dの初期化
		if (FAILED(pBlink2D->Init()))
		{ // 初期化に失敗した場合

			// 点滅オブジェクト2Dの破棄
			SAFE_DELETE(pBlink2D);
			return nullptr;
		}

		// 位置を設定
		pBlink2D->SetVec3Position(rPos);

		// 向きを設定
		pBlink2D->SetVec3Rotation(rRot);

		// 大きさを設定
		pBlink2D->SetVec3Sizing(rSize);

		// 確保したアドレスを返す
		return pBlink2D;
	}
}

//============================================================
//	表示設定処理
//============================================================
void CBlink2D::SetDisp(void)
{
	// 既に表示中の場合抜ける
	if (m_state == STATE_DISP) { return; }

	// カウンターを初期化
	m_fWaitTime = 0;

	// フェードアウト状態にする
	m_state = STATE_FADEOUT;
}
