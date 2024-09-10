//============================================================
//
//	タイトルロゴ2D処理 [titleLogo2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "titleLogo2D.h"
#include "manager.h"
#include "texture.h"
#include "camera.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// タイトルロゴ2D表示の優先順位

	namespace logo
	{
		const float	DEST_ALPHA	 = 1.0f;	// 目標透明度
		const float	INIT_ALPHA	 = 0.0f;	// 初期透明度
		const D3DXCOLOR DEST_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, DEST_ALPHA);	// 目標色
		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// 初期色
		const D3DXCOLOR DIFF_COL = DEST_COL - INIT_COL;						// 差分色
	}

	namespace aura
	{
		const float WAIT_TIME	 = 0.25f;	// 待機時間
		const float MOVE_TIME	 = 1.0f;	// 移動時間
		const float	DEST_ALPHA	 = 1.0f;	// 目標透明度
		const float	INIT_ALPHA	 = 0.0f;	// 初期透明度
		const D3DXCOLOR DEST_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, DEST_ALPHA);	// 目標色
		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// 初期色
		const D3DXCOLOR DIFF_COL = DEST_COL - INIT_COL;						// 差分色
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CTitleLogo2D> *CTitleLogo2D::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	親クラス [CTitleLogo2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTitleLogo2D::CTitleLogo2D(const char* pBlurTexPath) : CAnim2D(CObject::LABEL_UI, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY),
	m_pBlurTexPath	(pBlurTexPath),	// ブラーテクスチャパス
	m_pAura			(nullptr),		// オーラ情報
	m_pBlur			(nullptr),		// ブラー情報
	m_state			(STATE_NONE),	// 状態
	m_fMoveTimeOne	(0.0f),			// 第一移動時間
	m_fMoveTimeTwo	(0.0f),			// 第二移動時間
	m_fWaitTimeOne	(0.0f),			// 第一待機時間
	m_fWaitTimeTwo	(0.0f),			// 第二待機時間
	m_fCurTime		(0.0f),			// 現在の待機時間
	m_fSinAlpha		(0.0f),			// 透明向き
	m_offset		(VEC3_ZERO),	// 初期位置オフセット
	m_initPos		(VEC3_ZERO),	// 初期位置
	m_destPosOne	(VEC3_ZERO),	// 第一目標位置
	m_destPosTwo	(VEC3_ZERO)		// 第二目標位置
{

}

//============================================================
//	デストラクタ
//============================================================
CTitleLogo2D::~CTitleLogo2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTitleLogo2D::Init(void)
{
	// メンバ変数を初期化
	m_pAura			= nullptr;		// オーラ情報
	m_pBlur			= nullptr;		// ブラー情報
	m_state			= STATE_NONE;	// 状態
	m_fMoveTimeOne	= 0.0f;			// 第一移動時間
	m_fMoveTimeTwo	= 0.0f;			// 第二移動時間
	m_fWaitTimeOne	= 0.0f;			// 第一待機時間
	m_fWaitTimeTwo	= 0.0f;			// 第二待機時間
	m_fCurTime		= 0.0f;			// 現在の待機時間
	m_fSinAlpha		= -HALF_PI;		// 透明向き
	m_offset		= VEC3_ZERO;	// 初期位置オフセット
	m_initPos		= VEC3_ZERO;	// 初期位置
	m_destPosOne	= VEC3_ZERO;	// 第一目標位置
	m_destPosTwo	= VEC3_ZERO;	// 第二目標位置

	// アニメーション2Dの初期化
	if (FAILED(CAnim2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 向きを設定
	SetVec3Rotation(VEC3_ZERO);

	// 色を設定
	SetColor(XCOL_AWHITE);

	// オーラの生成
	m_pAura = CAnim2D::Create(1, 1, VEC3_ZERO);
	if (m_pAura == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 向きを設定
	m_pAura->SetVec3Rotation(VEC3_ZERO);

	// 色を設定
	m_pAura->SetColor(XCOL_AWHITE);

	// 加算合成にする
	m_pAura->GetRenderState()->SetAlphaBlend(CRenderState::BLEND_ADD);

	// ブラーの生成
	m_pBlur = CBlur2D::Create(this, XCOL_WHITE, 0.025f, 40, GET_MANAGER->GetTexture()->Regist(m_pBlurTexPath));
	if (m_pBlur == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CTitleLogo2D>::Create();
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
void CTitleLogo2D::Uninit(void)
{
	// オーラの終了
	SAFE_UNINIT(m_pAura);

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// アニメーション2Dの終了
	CAnim2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CTitleLogo2D::Update(const float fDeltaTime)
{
	// 何もしない状態の場合抜ける
	if (m_state == STATE_NONE) { return; }

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_ONE_MOVE_WAIT:

		// 第一移動待機の更新
		UpdateMoveOneWait(fDeltaTime);
		break;

	case STATE_ONE_MOVE:

		// 第一移動の更新
		UpdateMoveOne(fDeltaTime);
		break;

	case STATE_TWO_MOVE_WAIT:

		// 第二移動待機の更新
		UpdateMoveTwoWait(fDeltaTime);
		break;

	case STATE_TWO_MOVE:

		// 第二移動の更新
		UpdateMoveTwo(fDeltaTime);
		break;

	case STATE_AURA_WAIT:

		// オーラ待機の更新
		UpdateAuraWait(fDeltaTime);
		break;

	case STATE_AURA:

		// オーラの更新
		UpdateAura(fDeltaTime);
		break;

	case STATE_END:

		// オーラ点滅の更新
		UpdateBlinkAura(fDeltaTime);
		break;

	default:
		assert(false);
		break;
	}

	// オーラの更新
	m_pAura->Update(fDeltaTime);

	// アニメーション2Dの更新
	CAnim2D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CTitleLogo2D::Draw(CShader *pShader)
{
	// アニメーション2Dの描画
	CAnim2D::Draw(pShader);
}

//============================================================
//	位置の設定処理
//============================================================
void CTitleLogo2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 親の位置を設定
	CAnim2D::SetVec3Position(rPos);

	// オーラの位置を設定
	m_pAura->SetVec3Position(rPos);
}

//============================================================
//	大きさの設定処理
//============================================================
void CTitleLogo2D::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// 親の大きさを設定
	CAnim2D::SetVec3Sizing(rSize);

	// オーラの大きさを設定
	m_pAura->SetVec3Sizing(rSize);
}

//============================================================
//	パターンの設定処理
//============================================================
void CTitleLogo2D::SetPattern(const int nPattern)
{
	// 親のパターンを設定
	CAnim2D::SetPattern(nPattern);

	// オーラのパターンを設定
	m_pAura->SetPattern(nPattern);
}

//============================================================
//	テクスチャの横分割数の設定処理
//============================================================
void CTitleLogo2D::SetWidthPattern(const int nWidthPtrn)
{
	// 親のテクスチャ横分割数を設定
	CAnim2D::SetWidthPattern(nWidthPtrn);

	// オーラのテクスチャ横分割数を設定
	m_pAura->SetWidthPattern(nWidthPtrn);
}

//============================================================
//	テクスチャの縦分割数の設定処理
//============================================================
void CTitleLogo2D::SetHeightPattern(const int nHeightPtrn)
{
	// 親のテクスチャ縦分割数を設定
	CAnim2D::SetHeightPattern(nHeightPtrn);

	// オーラのテクスチャ縦分割数を設定
	m_pAura->SetHeightPattern(nHeightPtrn);
}

//============================================================
//	生成処理
//============================================================
CTitleLogo2D *CTitleLogo2D::Create
(
	const char* pBlurTexPath,		// ブラーテクスチャパス
	const D3DXVECTOR3& rDestPosOne,	// 第一目標位置
	const D3DXVECTOR3& rDestPosTwo,	// 第二目標位置
	const D3DXVECTOR3& rOffset,		// オフセット
	const D3DXVECTOR3& rSize,		// 大きさ
	const float fMoveTimeOne,		// 第一移動時間
	const float fMoveTimeTwo,		// 第二移動時間
	const float fWaitTimeOne,		// 第一待機時間
	const float fWaitTimeTwo		// 第二待機時間
)
{
	// タイトルロゴ2Dの生成
	CTitleLogo2D *pTitleLogo2D = new CTitleLogo2D(pBlurTexPath);
	if (pTitleLogo2D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// タイトルロゴ2Dの初期化
		if (FAILED(pTitleLogo2D->Init()))
		{ // 初期化に失敗した場合

			// タイトルロゴ2Dの破棄
			SAFE_DELETE(pTitleLogo2D);
			return nullptr;
		}

		// 初期位置オフセットを設定
		pTitleLogo2D->m_offset = rOffset;

		// 初期位置を設定
		pTitleLogo2D->m_initPos = rDestPosOne + pTitleLogo2D->m_offset;

		// 第一目標位置を設定
		pTitleLogo2D->m_destPosOne = rDestPosOne;

		// 第二目標位置を設定
		pTitleLogo2D->m_destPosTwo = rDestPosTwo;

		// 開始位置を設定
		pTitleLogo2D->SetVec3Position(pTitleLogo2D->m_initPos);

		// 大きさを設定
		pTitleLogo2D->SetVec3Sizing(rSize);

		// 第一移動時間を設定
		pTitleLogo2D->m_fMoveTimeOne = fMoveTimeOne;

		// 第二移動時間を設定
		pTitleLogo2D->m_fMoveTimeTwo = fMoveTimeTwo;

		// 第一待機時間を設定
		pTitleLogo2D->m_fWaitTimeOne = fWaitTimeOne;

		// 第二待機時間を設定
		pTitleLogo2D->m_fWaitTimeTwo = fWaitTimeTwo;

		// 確保したアドレスを返す
		return pTitleLogo2D;
	}
}

//============================================================
//	演出スキップ処理
//============================================================
void CTitleLogo2D::EndStag(void)
{
	// タイマーを初期化
	m_fCurTime = 0.0f;

	// カメラの更新をオンにする
	GET_CAMERA->SetEnableUpdate(true);

	// ブラーの終了
	m_pBlur->SetState(CBlur2D::STATE_NONE);

	// 位置を補正
	SetVec3Position(m_destPosTwo);

	// 色を補正
	SetColor(logo::DEST_COL);

	// オーラ色を補正
	m_pAura->SetColor(aura::DEST_COL);

	// 終了状態にする
	m_state = STATE_END;
}

//============================================================
//	移動待機の更新処理
//============================================================
void CTitleLogo2D::UpdateMoveOneWait(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= m_fWaitTimeOne)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// ブラーの開始
		m_pBlur->SetState(CBlur2D::STATE_NORMAL);

		// 移動状態にする
		m_state = STATE_ONE_MOVE;
	}
}

//============================================================
//	移動の更新処理
//============================================================
void CTitleLogo2D::UpdateMoveOne(const float fDeltaTime)
{
	// 差分位置を計算
	const D3DXVECTOR3 DIFF_POS = m_destPosOne - m_initPos;

	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, m_fMoveTimeOne);

	// 色を反映
	SetColor(logo::INIT_COL + (logo::DIFF_COL * fRate));

	// 位置を反映
	SetVec3Position(m_initPos + (DIFF_POS * fRate));

	if (m_fCurTime >= m_fMoveTimeOne)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 色を補正
		SetColor(logo::DEST_COL);

		// 位置を補正
		SetVec3Position(m_destPosOne);

		// 移動待機状態にする
		m_state = STATE_TWO_MOVE_WAIT;
	}
}

//============================================================
//	第二移動待機の更新処理
//============================================================
void CTitleLogo2D::UpdateMoveTwoWait(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= m_fWaitTimeTwo)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// カメラの更新をオンにする
		GET_CAMERA->SetEnableUpdate(true);

		// 移動状態にする
		m_state = STATE_TWO_MOVE;
	}
}

//============================================================
//	第二移動の更新処理
//============================================================
void CTitleLogo2D::UpdateMoveTwo(const float fDeltaTime)
{
	// 差分位置を計算
	const D3DXVECTOR3 DIFF_POS = m_destPosTwo - m_destPosOne;

	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InOutQuart(m_fCurTime, 0.0f, m_fMoveTimeTwo);

	// 位置を反映
	SetVec3Position(m_destPosOne + (DIFF_POS * fRate));

	if (m_fCurTime >= m_fMoveTimeTwo)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 位置を補正
		SetVec3Position(m_destPosTwo);

		// ブラーの終了
		m_pBlur->SetState(CBlur2D::STATE_VANISH);

		// オーラ待機状態にする
		m_state = STATE_AURA_WAIT;
	}
}

//============================================================
//	オーラ待機の更新処理
//============================================================
void CTitleLogo2D::UpdateAuraWait(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= aura::WAIT_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// オーラ状態にする
		m_state = STATE_AURA;
	}
}

//============================================================
//	オーラの更新処理
//============================================================
void CTitleLogo2D::UpdateAura(const float fDeltaTime)
{
	// タイマーを加算
	m_fCurTime += fDeltaTime;

	// 経過時刻の割合を計算
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, aura::MOVE_TIME);

	// 色を反映
	m_pAura->SetColor(aura::INIT_COL + (aura::DIFF_COL * fRate));

	if (m_fCurTime >= aura::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 色を補正
		m_pAura->SetColor(aura::DEST_COL);

		// 終了状態にする
		m_state = STATE_END;
	}
}

//============================================================
//	全終了判定処理
//============================================================
bool CTitleLogo2D::IsAllEnd(void)
{
	// リストがない場合抜ける
	if (m_pList == nullptr) { return false; }	// 始まってすらない

	std::list< CTitleLogo2D*> list = m_pList->GetList();	// 内部リスト
	for (const auto& rList : list)
	{ // リスト要素数分繰り返す

		// 終了状態以外の場合抜ける
		if (rList->m_state != STATE_END) { return false; }	// 終了していない
	}

	// ここまで来たら全部終了している
	return true;
}

//============================================================
//	オーラ点滅の更新処理
//============================================================
void CTitleLogo2D::UpdateBlinkAura(const float fDeltaTime)
{
	const float m_fAddSinRot	= 3.0f;		// 透明向きの加算量
	const float m_fMinAlpha		= 0.25f;	// 最低透明度
	const float m_fMaxAlpha		= 1.0f;		// 最大透明度

	if (CTitleLogo2D::IsAllEnd())
	{ // 全タイトルロゴが演出終了している場合

		// サインカーブ向きを回転
		m_fSinAlpha += m_fAddSinRot * fDeltaTime;
		useful::NormalizeRot(m_fSinAlpha);	// 向き正規化

		// 透明度加算量を計算
		float fAddAlpha = ((m_fMaxAlpha - m_fMinAlpha) * 0.5f) * (sinf(m_fSinAlpha) - 1.0f) * -1.0f;

		// 透明度を設定
		m_pAura->SetAlpha(m_fMinAlpha + fAddAlpha);
	}
}
