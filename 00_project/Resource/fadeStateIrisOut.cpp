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
#include "objectCircle2D.h"	// TODO

//************************************************************
//	子クラス [CFadeStateIrisOut] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFadeStateIrisOut::CFadeStateIrisOut(std::function<D3DXVECTOR3(void)> pFuncPos) :
	m_pFuncPos	(pFuncPos)	// 切り抜き型位置関数ポインタ
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

	// 切り抜き型の位置を設定
	m_pContext->m_pCrop->SetVec3Position(posIris);

	float fRadiusIris = m_pContext->m_pCrop->GetRadius();	// 切り抜き型半径

	// TODO：ここひどすぎる
	// 円を小さくしていく
	fRadiusIris -= 450.0f * fDeltaTime;
	if (fRadiusIris <= 0.0f)
	{ // 透明になった場合

		// 半径を補正
		fRadiusIris = 0.0f;

		// 次シーンへ遷移する
		m_pContext->TransNextMode();

		// 半径を反映
		m_pContext->m_pCrop->SetRadius(fRadiusIris);

		m_pContext->ChangeState(new CFadeStateIn);

		return;
	}

	// 半径を反映
	m_pContext->m_pCrop->SetRadius(fRadiusIris);
}

//============================================================
//	コンテキスト設定処理
//============================================================
void CFadeStateIrisOut::SetContext(CFade* pContext)
{
	D3DXVECTOR3 posIris = (m_pFuncPos == nullptr) ? SCREEN_CENT : m_pFuncPos();	// 切り抜き型位置

	// コンテキストの設定
	CFadeState::SetContext(pContext);

	// TODO：ここで切り抜き型位置設定
	m_pContext->m_pCrop->SetVec3Position(posIris);

	// TODO：ここで透明度修正
	D3DXCOLOR col = m_pContext->GetColor();
	col.a = 1.0f;
	m_pContext->SetColor(col);

	// TODO：ここで半径修正
	m_pContext->m_pCrop->SetRadius(640.0f);
}
