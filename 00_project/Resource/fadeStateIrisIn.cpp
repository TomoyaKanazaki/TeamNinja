//============================================================
//
//	アイリスイン状態処理 [fadeStateIrisIn.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "fadeStateIrisIn.h"
#include "fade.h"

//************************************************************
//	子クラス [CFadeStateIrisIn] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFadeStateIrisIn::CFadeStateIrisIn(std::function<D3DXVECTOR3(void)> pFuncPos) :
	m_pFuncPos	(pFuncPos),	// 切り抜き型位置関数ポインタ
	m_fInitRad	(0.0f),		// 初期半径
	m_fCurTime	(0.0f)		// 現在の経過時間
{

}

//============================================================
//	デストラクタ
//============================================================
CFadeStateIrisIn::~CFadeStateIrisIn()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CFadeStateIrisIn::Init(void)
{
	// メンバ変数を初期化
	m_fInitRad	= 0.0f;	// 初期半径
	m_fCurTime	= 0.0f;	// 現在の経過時間

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CFadeStateIrisIn::Uninit(void)
{
	// 自身の破棄
	delete this;
}

//============================================================
//	更新処理
//============================================================
void CFadeStateIrisIn::Update(const float fDeltaTime)
{
	D3DXVECTOR3 posIris = (m_pFuncPos == nullptr) ? SCREEN_CENT : m_pFuncPos();	// 切り抜き型位置
	m_pContext->SetCropPosition(posIris);	// 切り抜き型の位置を設定

	// タイマーを加算
	m_fCurTime += fDeltaTime;

	const float fInTime = m_pContext->GetInTime();					// イン時間
	const float fEndRadius = m_pContext->CalcCropRadius(posIris);	// 切り抜き最大半径
	const float fDiffRad = fEndRadius - m_fInitRad;					// 半径差分
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, fInTime);		// 経過時刻の割合を計算

	// 半径を反映
	m_pContext->SetCropRadius(m_fInitRad + (fDiffRad * fRate));
	if (m_fCurTime >= fInTime)
	{ // 大きくなきった場合

		// 半径を補正
		m_pContext->SetCropRadius(fEndRadius);

		// 切り抜き先のポリゴンを透明にする
		m_pContext->SetAlpha(0.0f);

		// 何もしない状態にする
		m_pContext->ChangeState(new CFadeStateNone);
	}
}
