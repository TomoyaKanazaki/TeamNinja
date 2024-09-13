//============================================================
//
//	神器マーク処理 [goditem_mark.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "goditem_mark.h"
#include "godItem.h"
#include "manager.h"

#include "sound.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 STOP_SIZE = D3DXVECTOR3(60.0f, 60.0f, 0.0f);		// 下地のサイズ
	const D3DXVECTOR3 BODY_SIZE = D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// 本体のサイズ
	const char* TEXTURE = "data\\TEXTURE\\itemGod000.png";		// テクスチャ
	const int WIDTH_PATTERN = 3;		// テクスチャの横分割数
	const int HEIGHT_PATTERN = 1;		// テクスチャの縦分割数

	const float ADD_ALPHA = 0.025f;		// 透明度の加算数
	const float ADD_SIZE = 0.2f;		// サイズの加算数
}

//************************************************************
//	子クラス [CGodItemMark] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGodItemMark::CGodItemMark(const int nPriority) : CAnim2D(CObject::LABEL_NONE, CObject::SCENE_MAIN, CObject::DIM_2D, nPriority),
m_state(STATE_NONE),	// 状態
m_fScaling(0.0f)		// 拡縮率
{

}

//============================================================
//	デストラクタ
//============================================================
CGodItemMark::~CGodItemMark()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGodItemMark::Init(void)
{
	// 結果を返す
	return CAnim2D::Init();
}

//============================================================
//	終了処理
//============================================================
void CGodItemMark::Uninit(void)
{
	// 終了処理
	CAnim2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGodItemMark::Update(const float fDeltaTime)
{
	switch (m_state)
	{
	case CGodItemMark::STATE_NONE:

		// 透明にしておく
		SetAlpha(0.0f);

		break;

	case CGodItemMark::STATE_SCALING:

		// 拡縮処理
		UpdateScaling();

		break;

	case CGodItemMark::STATE_STOP:



		break;

	default:
		assert(false);
		break;
	}

	// 更新処理
	CAnim2D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CGodItemMark::Draw(CShader* /*pShader*/)
{
	// 描画処理
	CAnim2D::Draw();
}

//============================================================
// 情報の設定処理
//============================================================
void CGodItemMark::SetData
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const int nIdx				// インデックス
)
{
	// 情報を設定
	SetVec3Position(rPos);				// 位置
	SetVec3Rotation(rRot);				// 向き
	SetVec3Sizing(BODY_SIZE);			// 大きさ
	SetColor(XCOL_AWHITE);				// 色
	SetCounter(0);						// カウンターを設定する
	SetPattern(nIdx);					// パターンを設定する
	SetEnableStop(false);				// 停止状況
	SetWidthPattern(WIDTH_PATTERN);		// テクスチャ横分割数
	SetHeightPattern(HEIGHT_PATTERN);	// テクスチャ縦分割数

	// テクスチャの割り当て
	BindTexture(TEXTURE);

	if (CGodItem::IsGet((CGodItem::EType)(nIdx)))
	{ // 入手済みの場合

		// 停止状態にする
		m_state = STATE_STOP;
		SetAlpha(1.0f);
		SetVec3Sizing(STOP_SIZE);
	}
	else
	{ // 上記以外

		// 初期状態にする
		m_state = STATE_NONE;
	}
}

//============================================================
//	生成処理
//============================================================
CGodItemMark* CGodItemMark::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const int nIdx,				// インデックス
	const int nPriority			// 優先順位
)
{
	// タイマーの生成
	CGodItemMark* pTimer = new CGodItemMark(nPriority);

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

		// 情報の設定処理
		pTimer->SetData(rPos, rRot, nIdx);

		// 確保したアドレスを返す
		return pTimer;
	}
}

//============================================================
// 拡縮処理
//============================================================
void CGodItemMark::UpdateScaling(void)
{
	D3DXVECTOR3 size = GetVec3Sizing();

	// 透明度を設定
	SetAlpha(GetAlpha() + ADD_ALPHA);

	// 拡縮率を加算する
	m_fScaling += ADD_SIZE;

	// 小さくする
	size.x -= m_fScaling;
	size.y -= m_fScaling;

	if (size.x <= STOP_SIZE.x ||
		size.y <= STOP_SIZE.z)
	{ // サイズが一定以下になった場合

		// サイズを補正する
		size = STOP_SIZE;

		// 透明度を補正する
		SetAlpha(1.0f);

		// 停止状態にする
		m_state = STATE_STOP;

		// 神器UIの音を鳴らす
		PLAY_SOUND(CSound::LABEL_SE_GODITEMUI);
	}

	// サイズを反映
	SetVec3Sizing(size);
}
