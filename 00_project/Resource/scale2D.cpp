//============================================================
//
//	拡縮オブジェクト2D処理 [scale2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "scale2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// 拡縮オブジェクト2D表示の優先順位
}

//************************************************************
//	親クラス [CScale2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScale2D::CScale2D() : CObject2D(CObject::LABEL_UI, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY),
	m_sizeOrigin (VEC3_ZERO),	// 原点大きさ
	m_state		 (STATE_NONE),	// 状態
	m_fSinScale	 (0.0f),		// 拡縮向き
	m_fAddSinRot (0.0f),		// 拡縮向きの加算量
	m_fCurScale	 (0.0f),		// 現在拡大率
	m_fMinScale	 (0.0f),		// 最低拡大率
	m_fMaxScale	 (0.0f),		// 最大拡大率
	m_fInitScale (0.0f),		// 初期拡大率
	m_fSubIn	 (0.0f),		// インの拡大率減少量
	m_fAddOut	 (0.0f)			// アウトの拡大率増加量
{

}

//============================================================
//	デストラクタ
//============================================================
CScale2D::~CScale2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CScale2D::Init(void)
{
	// メンバ変数を初期化
	m_sizeOrigin = VEC3_ZERO;	// 原点大きさ
	m_state		 = STATE_NONE;	// 状態
	m_fSinScale	 = HALF_PI;		// 拡縮向き
	m_fAddSinRot = 0.0f;		// 拡縮向きの加算量
	m_fCurScale	 = 0.0f;		// 現在拡大率
	m_fMinScale	 = 0.0f;		// 最低拡大率
	m_fMaxScale	 = 0.0f;		// 最大拡大率
	m_fInitScale = 0.0f;		// 初期拡大率
	m_fSubIn	 = 0.0f;		// インの拡大率減少量
	m_fAddOut	 = 0.0f;		// アウトの拡大率増加量

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
void CScale2D::Uninit(void)
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CScale2D::Update(const float fDeltaTime)
{
	// 何もしない状態の場合抜ける
	if (m_state == STATE_NONE) { return; }

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_FADEOUT:
	{
		// 最低限の拡大率にしていく
		m_fCurScale += m_fAddOut * fDeltaTime;
		if (m_fCurScale >= m_fMinScale)
		{ // 最低限の拡大率になった場合

			// 拡大率を補正
			m_fCurScale = m_fMinScale;

			// 拡縮状態にする
			m_state = STATE_SCALE;
		}

		// 大きさを反映
		SetVec3Sizing(m_sizeOrigin * m_fCurScale);
		break;
	}
	case STATE_SCALE:
	{
		// サインカーブ向きを回転
		m_fSinScale += m_fAddSinRot * fDeltaTime;
		useful::NormalizeRot(m_fSinScale);	// 向き正規化

		// 拡大率加算量を計算
		float fAddScale = ((m_fMaxScale - m_fMinScale) * 0.5f) * (sinf(m_fSinScale) - 1.0f) * -1.0f;

		// 拡大率を計算
		m_fCurScale = m_fMinScale + fAddScale;

		// 大きさを反映
		SetVec3Sizing(m_sizeOrigin * m_fCurScale);
		break;
	}
	case STATE_FADEIN:
	{
		// 消失する拡大率にしていく
		m_fCurScale -= m_fSubIn * fDeltaTime;
		if (m_fCurScale >= 0.0f)
		{ // 消失する拡大率になった場合

			// 拡大率を補正
			m_fCurScale = 0.0f;

			// 何もしない状態にする
			m_state = STATE_NONE;
		}

		// 大きさを反映
		SetVec3Sizing(m_sizeOrigin * m_fCurScale);
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
void CScale2D::Draw(CShader *pShader)
{
	// オブジェクト2Dの描画
	CObject2D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CScale2D *CScale2D::Create
(
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rOriginSize,	// 原点大きさ
	const float fMinScale,			// 最低拡大率
	const float fMaxScale,			// 最大拡大率
	const float fInitScale,			// 初期拡大率
	const float fCalcScale,			// 拡大率の加減量
	const float fSubIn,				// 拡縮前のインの拡大率減少量
	const float fAddOut,			// 拡縮後のアウトの拡大率増加量
	const D3DXVECTOR3& rRot,		// 向き
	const D3DXCOLOR& rCol			// 色
)
{
	// 拡縮オブジェクト2Dの生成
	CScale2D *pScale2D = new CScale2D;
	if (pScale2D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 拡縮オブジェクト2Dの初期化
		if (FAILED(pScale2D->Init()))
		{ // 初期化に失敗した場合

			// 拡縮オブジェクト2Dの破棄
			SAFE_DELETE(pScale2D);
			return nullptr;
		}

		// 位置を設定
		pScale2D->SetVec3Position(rPos);

		// 向きを設定
		pScale2D->SetVec3Rotation(rRot);

		// 大きさを設定
		pScale2D->SetVec3Sizing(rOriginSize * fInitScale);

		// 原点大きさを設定
		pScale2D->SetOriginSizing(rOriginSize);

		// 色を設定
		pScale2D->SetColor(rCol);

		// 最低拡大率を設定
		pScale2D->SetMinScale(fMinScale);

		// 最大拡大率を設定
		pScale2D->SetMaxScale(fMaxScale);

		// 初期拡大率を設定
		pScale2D->SetInitScale(fInitScale);
		pScale2D->m_fCurScale = fInitScale;	// 現在の拡大率も設定

		// 拡縮向きの加算量を設定
		pScale2D->SetCalcScale(fCalcScale);

		// 拡大率減少量を設定
		pScale2D->SetSubIn(fSubIn);

		// 拡大率増加量を設定
		pScale2D->SetAddOut(fAddOut);

		// 確保したアドレスを返す
		return pScale2D;
	}
}

//============================================================
//	拡縮設定処理
//============================================================
void CScale2D::SetScale(const bool bScale)
{
	// 拡縮向きを初期化
	m_fSinScale = HALF_PI;

	if (bScale)
	{ // 拡縮を開始する場合

		// フェードアウト状態にする
		m_state = STATE_FADEOUT;
	}
	else
	{ // 拡縮を終了する場合

		// フェードイン状態にする
		m_state = STATE_FADEIN;
	}
}
