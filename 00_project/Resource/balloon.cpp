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

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY	 = 0;		// 吹き出しの優先順位
	const int	ALPHA_NUMREF = 180;		// αテストの参照値
	const float	REV_SCALE	 = 0.4f;	// 拡大率の補正係数
	const float	DISP_SCALE	 = 1.0f;	// 表示する際の拡大率
	const float	UNDISP_SCALE = 0.0f;	// 表示しない際の拡大率

	namespace balloon
	{
		const D3DXVECTOR3 SIZE = D3DXVECTOR3(320.0f, 180.0f, 0.0f);	// 大きさ
	}
}

//************************************************************
//	子クラス [CBalloon] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBalloon::CBalloon() : CObjectBillboard(CObject::LABEL_UI),
	m_fScale	 (0.0f),	// 拡大率
	m_fDestScale (0.0f)		// 目標拡大率
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
	m_fScale	 = 0.0f;	// 拡大率
	m_fDestScale = 0.0f;	// 目標拡大率

	// オブジェクトビルボードの初期化
	if (FAILED(CObjectBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 位置を設定
	SetVec3Position(D3DXVECTOR3(0.0f, 240.0f, 0.0f));

	// 大きさを設定
	SetVec3Sizing(balloon::SIZE * m_fScale);

	// テクスチャを割当
#if 1
	BindTexture("data\\TEXTURE\\balloon000.png");
#else
	BindTexture(GET_RENDERER->GetRenderTextureIndex(CObject::SCENE_BILLBOARD));
#endif

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
	// 拡大率の差分を計算
	float fDiffScale = m_fDestScale - m_fScale;

	// 拡大率を計算
	m_fScale += fDiffScale * REV_SCALE;

	// 大きさを設定
	SetVec3Sizing(balloon::SIZE * m_fScale);

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
//	生成処理
//============================================================
CBalloon *CBalloon::Create(void)
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

		// 確保したアドレスを返す
		return pBalloon;
	}
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
	}
	else
	{ // 表示しない場合

		// 目標拡大率を小さくする
		m_fDestScale = UNDISP_SCALE;
	}
}
