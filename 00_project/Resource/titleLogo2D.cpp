#if 0
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
CBlink2D::CBlink2D() : CObject2D(CObject::LABEL_UI, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY),
	m_state		 (STATE_NONE),	// 状態
	m_fSinAlpha	 (0.0f),		// 透明向き
	m_fAddSinRot (0.0f),		// 透明向きの加算量
	m_fMinAlpha	 (0.0f),		// 最低透明度
	m_fMaxAlpha	 (0.0f),		// 最大透明度
	m_fSubIn	 (0.0f),		// インのα値減少量
	m_fAddOut	 (0.0f)			// アウトのα値増加量
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
	m_state		 = STATE_NONE;	// 状態
	m_fSinAlpha	 = HALF_PI;		// 透明向き
	m_fAddSinRot = 0.0f;		// 透明向きの加算量
	m_fMinAlpha	 = 0.0f;		// 最低透明度
	m_fMaxAlpha	 = 0.0f;		// 最大透明度
	m_fSubIn	 = 0.0f;		// インのα値減少量
	m_fAddOut	 = 0.0f;		// アウトのα値増加量

	// オブジェクト2Dの初期化
	if (FAILED(CObject2D::Init()))
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

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_FADEOUT:
	{
		D3DXCOLOR colBlink = GetColor();	// 色を取得

		// 最低限の透明度にしていく
		colBlink.a += m_fAddOut * fDeltaTime;
		if (colBlink.a >= m_fMinAlpha)
		{ // 最低限の透明度になった場合

			// α値を補正
			colBlink.a = m_fMinAlpha;

			// 点滅状態にする
			m_state = STATE_BLINK;
		}

		SetColor(colBlink);	// 色を反映
		break;
	}
	case STATE_BLINK:
	{
		// サインカーブ向きを回転
		m_fSinAlpha += m_fAddSinRot * fDeltaTime;
		useful::NormalizeRot(m_fSinAlpha);	// 向き正規化

		// 透明度加算量を計算
		float fAddAlpha = ((m_fMaxAlpha - m_fMinAlpha) * 0.5f) * (sinf(m_fSinAlpha) - 1.0f) * -1.0f;

		// 透明度を設定
		SetAlpha(m_fMinAlpha + fAddAlpha);
		break;
	}
	case STATE_FADEIN:
	{
		D3DXCOLOR colBlink = GetColor();	// 色を取得

		// 透明にしていく
		colBlink.a -= m_fSubIn * fDeltaTime;
		if (colBlink.a <= 0.0f)
		{ // 透明になった場合

			// α値を補正
			colBlink.a = 0.0f;

			// 何もしない状態にする
			m_state = STATE_NONE;
		}

		SetColor(colBlink);	// 色を反映
		break;
	}
	default:
		assert(false);
		break;
	}

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
	const float fMinAlpha,		// 最低透明度
	const float fMaxAlpha,		// 最大透明度
	const float fCalcAlpha,		// 透明度の加減量
	const float fSubIn,			// インのα値減少量
	const float fAddOut,		// アウトのα値増加量
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXCOLOR& rCol		// 色
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

		// 色を設定
		pBlink2D->SetColor(rCol);

		// 透明度を設定
		pBlink2D->SetAlpha(0.0f);	// 最初は透明から

		// 最低透明度を設定
		pBlink2D->SetMinAlpha(fMinAlpha);

		// 最大透明度を設定
		pBlink2D->SetMaxAlpha(fMaxAlpha);

		// 透明向きの加算量を設定
		pBlink2D->SetCalcAlpha(fCalcAlpha);

		// α値減少量を設定
		pBlink2D->SetSubIn(fSubIn);

		// α値増加量を設定
		pBlink2D->SetAddOut(fAddOut);

		// 確保したアドレスを返す
		return pBlink2D;
	}
}

//============================================================
//	点滅設定処理
//============================================================
void CBlink2D::SetBlink(const bool bBlink)
{
	// 透明向きを初期化
	m_fSinAlpha = HALF_PI;

	if (bBlink)
	{ // 点滅を開始する場合

		// フェードアウト状態にする
		m_state = STATE_FADEOUT;
	}
	else
	{ // 点滅を終了する場合

		// フェードイン状態にする
		m_state = STATE_FADEIN;
	}
}
#endif
