//============================================================
//
//	アイリスアウト状態処理 [fadeStateIrisOut.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "fadeStateIrisOut.h"
#include "fade.h"

//************************************************************
//	子クラス [CFadeStateIrisOut] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFadeStateIrisOut::CFadeStateIrisOut(std::function<D3DXVECTOR3(void)> pFuncPos) :
	m_pFuncPos	(pFuncPos),	// 切り抜き型位置関数ポインタ
	m_fInitRad	(0.0f),		// 初期半径
	m_fCurTime	(0.0f)		// 現在の経過時間
{

}

//============================================================
//	デストラクタ
//============================================================
CFadeStateIrisOut::~CFadeStateIrisOut()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CFadeStateIrisOut::Init(void)
{
	D3DXVECTOR3 posIris = (m_pFuncPos == nullptr) ? SCREEN_CENT : m_pFuncPos();	// 切り抜き型位置

	// メンバ変数を初期化
	m_fInitRad	= m_pContext->CalcCropRadius(posIris);	// 初期半径
	m_fCurTime	= 0.0f;	// 現在の経過時間

	// 切り抜き型の位置を初期化
	m_pContext->SetCropPosition(posIris);

	// 切り抜き型の半径を初期化
	m_pContext->SetCropRadius(m_fInitRad);

	// 切り抜き先のポリゴンを不透明にする
	m_pContext->SetAlpha(1.0f);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CFadeStateIrisOut::Uninit(void)
{
	// 自身の破棄
	delete this;
}

//============================================================
//	更新処理
//============================================================
void CFadeStateIrisOut::Update(const float fDeltaTime)
{
	D3DXVECTOR3 posIris = (m_pFuncPos == nullptr) ? SCREEN_CENT : m_pFuncPos();	// 切り抜き型位置
	m_pContext->SetCropPosition(posIris);	// 切り抜き型の位置を設定

	// タイマーを加算
	m_fCurTime += fDeltaTime;

	const float fDiffRad = 0.0f - m_fInitRad;	// 半径差分
	float fRate = easeing::OutQuad(m_fCurTime, 0.0f, 1.0f);	// 経過時刻の割合を計算

	// 半径を反映
	m_pContext->SetCropRadius(m_fInitRad + (fDiffRad * fRate));

	if (m_fCurTime >= 1.0f)
	{ // 小さくなりきった場合

		// 次シーンへ遷移する
		m_pContext->TransNextMode();

		// 半径を反映
		m_pContext->SetCropRadius(0.0f);

		// アイリスイン状態にする
		m_pContext->ChangeState(new CFadeStateIrisIn(m_pFuncPos));
	}
}
