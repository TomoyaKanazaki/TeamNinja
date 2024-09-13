//============================================================
//
// playerbackUI処理 [playerbackUI.cpp]
// Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "playerbackUI.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 2;									// スタートUIの優先順位
	const char* TEXTURE = "data\\TEXTURE\\alcohol.png";		// テクスチャ
	const D3DXVECTOR3 POS = D3DXVECTOR3(1100.0f, 600.0f, 0.0f);			// 位置
	const D3DXVECTOR3 RADIUS = D3DXVECTOR3(200.0f, 200.0f, 0.0f);		// 通常サイズ

	const float ADD_MAGNI = 0.9f;		// 加算の倍率
	const float SUB_MAGNI = 1.4f;		// 減算の倍率
}

//************************************************************
//	子クラス [CPlayerBackUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayerBackUI::CPlayerBackUI() : CObject2D(CObject::LABEL_NONE, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY),
m_state(STATE_NONE)		// 状態
{

}

//============================================================
//	デストラクタ
//============================================================
CPlayerBackUI::~CPlayerBackUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayerBackUI::Init(void)
{
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
void CPlayerBackUI::Uninit(void)
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayerBackUI::Update(const float fDeltaTime)
{
	switch (m_state)
	{
	case CPlayerBackUI::STATE_NONE:

		// 透明度を0.0fに設定
		SetAlpha(0.0f);

		break;

	case CPlayerBackUI::STATE_ADD:

		// 加算状態処理
		AddFunc(fDeltaTime);

		break;

	case CPlayerBackUI::STATE_SUB:

		// 減算状態処理
		SubFunc(fDeltaTime);

		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
//	描画処理
//============================================================
void CPlayerBackUI::Draw(CShader* pShader)
{
	// オブジェクト2Dの描画
	CObject2D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CPlayerBackUI* CPlayerBackUI::Create(void)
{
	// スタートUIの生成
	CPlayerBackUI* pStartUI = new CPlayerBackUI;

	if (pStartUI == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// スタートUIの初期化
		if (FAILED(pStartUI->Init()))
		{ // 初期化に失敗した場合

			// スタートUIの破棄
			SAFE_DELETE(pStartUI);
			return nullptr;
		}

		// 値を設定
		pStartUI->SetVec3Position(POS);			// 位置
		pStartUI->SetVec3Rotation(VEC3_ZERO);	// 向き
		pStartUI->SetVec3Sizing(RADIUS);		// 大きさ
		pStartUI->SetAlpha(0.0f);				// 透明度

		// テクスチャの割り当て
		pStartUI->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE));

		// 確保したアドレスを返す
		return pStartUI;
	}
}

//============================================================
// 加算状態処理
//============================================================
void CPlayerBackUI::AddFunc(const float fDeltaTime)
{
	// 透明度を取得
	float fAlpha = GetAlpha();

	// 透明度を加算する
	fAlpha += (fDeltaTime * ADD_MAGNI);
	
	if (fAlpha > 1.0f)
	{ // 透明度が1.0f超過の場合

		// 透明度を補正
		fAlpha = 1.0f;
	}

	// 透明度を反映
	SetAlpha(fAlpha);
}

//============================================================
// 減算状態処理
//============================================================
void CPlayerBackUI::SubFunc(const float fDeltaTime)
{
	// 透明度を取得
	float fAlpha = GetAlpha();

	// 透明度を加算する
	fAlpha -= (fDeltaTime * SUB_MAGNI);

	if (fAlpha < 0.0f)
	{ // 透明度が0.0f未満の場合

		// 透明度を補正
		fAlpha = 0.0f;

		// 減算状態にする
		m_state = STATE_NONE;
	}

	// 透明度を反映
	SetAlpha(fAlpha);
}
