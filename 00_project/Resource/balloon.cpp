//============================================================
//
//	吹き出し処理 [balloon.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "balloon.h"
#include "manager.h"
#include "renderer.h"
#include "sceneSelect.h"
#include "selectManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY	 = 0;		// 吹き出しの優先順位
	const int	ALPHA_NUMREF = 120;		// αテストの参照値
	const float	REV_SCALE	 = 0.4f;	// 拡大率の補正係数
	const float	DISP_SCALE	 = 1.4f;	// 表示する際の拡大率
	const float	UNDISP_SCALE = 0.0f;	// 表示しない際の拡大率

	namespace balloon
	{
		const D3DXVECTOR3 SIZE	 = D3DXVECTOR3(320.0f, 180.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 OFFSET = D3DXVECTOR3(0.0f, 240.0f, 0.0f);		// オフセット
	}
}

//************************************************************
//	子クラス [CBalloon] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBalloon::CBalloon() : CObjectBillboard(CObject::LABEL_UI),
	m_state		 (STATE_NONE),	// 状態
	m_fCurTime	 (0.0f),		// 現在時間
	m_fScale	 (0.0f),		// 拡大率
	m_fDestScale (0.0f)			// 目標拡大率
{

}

//============================================================
//	デストラクタ
//============================================================
CBalloon::~CBalloon()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBalloon::Init(void)
{
	// メンバ変数を初期化
	m_state		 = STATE_NONE;	// 状態
	m_fCurTime	 = 0.0f;		// 現在時間
	m_fScale	 = 0.0f;		// 拡大率
	m_fDestScale = 0.0f;		// 目標拡大率

	// オブジェクトビルボードの初期化
	if (FAILED(CObjectBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 大きさを設定
	SetVec3Sizing(balloon::SIZE * m_fScale);

	// ビルボードシーンのレンダーテクスチャを割当
	BindTexture(GET_RENDERER->GetRenderTextureIndex(CObject::SCENE_BILLBOARD));

	// レンダーステートの情報を取得
	CRenderState *pRenderState = GetRenderState();

	// αテストの設定
	pRenderState->SetAlphaTest(true);			// αテストの有効 / 無効の設定
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// αテストの設定
	pRenderState->SetAlphaNumRef(ALPHA_NUMREF);	// αテストの参照値設定

	// 絶対に描画が成功するようにする
	pRenderState->SetZFunc(D3DCMP_ALWAYS);

	// ライティングをOFFにする
	pRenderState->SetLighting(false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CBalloon::Uninit(void)
{
	// オブジェクトビルボードの終了
	CObjectBillboard::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CBalloon::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:
		break;

	case STATE_OPEN:
	{
		// 現在時間を進める
		m_fCurTime += fDeltaTime;
		if (useful::LimitMaxNum(m_fCurTime, 1.0f))
		{ // 時間が経過しきった場合

			// 終了状態にする
			m_state = STATE_END;
		}

		// 時間で割合を求める
		float fRate = easeing::OutSine(m_fCurTime, 0.0f, 1.0f);

		// 拡大率を計算
		m_fScale = m_fDestScale * fRate;

		// 大きさを設定
		SetVec3Sizing(balloon::SIZE * m_fScale);
		break;
	}
	case STATE_NORMAL:
	{
		// 拡大率の差分を計算
		float fDiffScale = m_fDestScale - m_fScale;

		// 拡大率を計算
		m_fScale += fDiffScale * REV_SCALE;
		if (fabsf(DISP_SCALE - m_fScale) <= 0.05f)
		{ // 拡大しきった場合

			// 表示拡大率に補正
			m_fScale = DISP_SCALE;

			// 終了状態にする
			m_state = STATE_END;
		}

		// 大きさを設定
		SetVec3Sizing(balloon::SIZE * m_fScale);
		break;
	}
	case STATE_END:
	{
		// 拡大率の差分を計算
		float fDiffScale = m_fDestScale - m_fScale;

		// 拡大率を計算
		m_fScale += fDiffScale * REV_SCALE;
		if (fabsf(UNDISP_SCALE - m_fScale) <= 0.05f)
		{ // 拡大しきった場合

			// 非表示拡大率に補正
			m_fScale = UNDISP_SCALE;

			// 何もしない状態にする
			m_state = STATE_NONE;
		}

		// 大きさを設定
		SetVec3Sizing(balloon::SIZE * m_fScale);
		break;
	}
	default:
		assert(false);
		break;
	}

	// オブジェクトビルボードの更新
	CObjectBillboard::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CBalloon::Draw(CShader *pShader)
{
	// オブジェクトビルボードの描画
	CObjectBillboard::Draw(pShader);
}

//============================================================
//	位置の設定処理
//============================================================
void CBalloon::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 自身の位置を設定
	CObjectBillboard::SetVec3Position(rPos + balloon::OFFSET);
}

//============================================================
//	生成処理
//============================================================
CBalloon *CBalloon::Create(const D3DXVECTOR3& rPosParent)
{
	// 吹き出しの生成
	CBalloon *pBalloon = new CBalloon;
	if (pBalloon == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 吹き出しの初期化
		if (FAILED(pBalloon->Init()))
		{ // 初期化に失敗した場合

			// 吹き出しの破棄
			SAFE_DELETE(pBalloon);
			return nullptr;
		}

		// 位置を設定
		pBalloon->SetVec3Position(rPosParent);

		// 確保したアドレスを返す
		return pBalloon;
	}
}

//============================================================
//	吹き出し初表示の設定処理
//============================================================
void CBalloon::SetFirstDisp(void)
{
	// 目標拡大率を大きくする
	m_fDestScale = DISP_SCALE;

	// 解放状態にする
	m_state = STATE_OPEN;
}

//============================================================
//	吹き出し表示の設定処理
//============================================================
void CBalloon::SetDisp(const bool bDisp)
{
	if (bDisp)
	{ // 表示する場合

		// 目標拡大率を大きくする
		m_fDestScale = DISP_SCALE;

		// 通常状態にする
		m_state = STATE_NORMAL;
	}
	else
	{ // 表示しない場合

		// 目標拡大率を小さくする
		m_fDestScale = UNDISP_SCALE;

		// 終了状態にする
		m_state = STATE_END;
	}
}

//============================================================
//	吹き出し拡大状況の取得処理
//============================================================
bool CBalloon::IsSizeDisp(void) const
{
	// 大きくなっているかを返す
	return (m_state == STATE_END);
}
