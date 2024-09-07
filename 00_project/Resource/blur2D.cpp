//============================================================
//
//	ブラー2D処理 [blur2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "blur2D.h"
#include "anim2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 5;	// ブラー2Dの優先順位
}

//************************************************************
//	親クラス [CBlur2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBlur2D::CBlur2D() : CObject(CObject::LABEL_BLUR, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY),
	m_pParent		(nullptr),		// 親オブジェクト
	m_col			(XCOL_WHITE),	// ブラー反映色
	m_fStartAlpha	(0.0f),			// ブラーの開始透明度
	m_nMaxLength	(0),			// 保持する親オブジェクトの最大数
	m_state			(STATE_NONE),	// 状態
	m_nCounterState	(0)				// 状態管理カウンター
{
	// メンバ変数をクリア
	m_oldObject.clear();	// 過去オブジェクト情報
}

//============================================================
//	デストラクタ
//============================================================
CBlur2D::~CBlur2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBlur2D::Init(void)
{
	// メンバ変数を初期化
	m_oldObject.clear();			// 過去オブジェクト情報
	m_pParent		= nullptr;		// 親オブジェクト
	m_col			= XCOL_WHITE;	// ブラー反映色
	m_fStartAlpha	= 0.0f;			// ブラーの開始透明度
	m_nMaxLength	= 0;			// 保持する親オブジェクトの最大数
	m_state			= STATE_NONE;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CBlur2D::Uninit(void)
{
	for (auto& rVec : m_oldObject)
	{ // 保存オブジェクト数分繰り返す

		// アニメーション2Dの終了
		SAFE_UNINIT(rVec);
	}

	// 保存オブジェクト情報をクリア
	m_oldObject.clear();

	// オブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CBlur2D::Update(const float fDeltaTime)
{
	// 残像の生成
	if (m_state == STATE_NORMAL)
	{ // 通常状態の場合

#if 0
		// オブジェクト保存用の動的配列を生成
		std::vector<CObjectModel*> tempBlur2D;
		tempBlur2D.resize((size_t)m_pParent->GetNumParts());	// パーツ数分配列生成

		int nCntParts = 0;	// パーツインデックス
		for (auto& rVecParts : tempBlur2D)
		{ // パーツ数分繰り返す

			// パーツの生成
			rVecParts = CObjectModel::Create(VEC3_ZERO, VEC3_ZERO);
			if (rVecParts != nullptr)
			{ // パーツの生成に成功した場合

				CObjectModel *pBlur2DParts = rVecParts;	// 残像パーツ
				CMultiModel *pOriginParts = m_pParent->GetParts(nCntParts);		// 原点パーツ
				D3DXMATRIX mtxParts = pOriginParts->GetMtxWorld();				// 残像生成元のマトリックス

				// 情報を生成元と一致させる
				pBlur2DParts->BindModel(pOriginParts->GetModelID());	// 同一モデルを割当
				pBlur2DParts->SetMtxWorld(mtxParts);					// 同一マトリックスを設定

				// 位置・向き・大きさをマトリックスから取得し設定
				pBlur2DParts->SetVec3Position(useful::GetMatrixPosition(mtxParts));
				pBlur2DParts->SetVec3Rotation(useful::GetMatrixRotation(mtxParts));
				pBlur2DParts->SetVec3Scaling(useful::GetMatrixScaling(mtxParts));

				// 加算合成にする
				pBlur2DParts->GetRenderState()->SetAlphaBlend(CRenderState::BLEND_ADD);
			}

			// パーツインデックスを進める
			nCntParts++;
		}
#endif

		// アニメーション2D保存用インスタンスを生成
		CAnim2D* pTempBlur2D = CAnim2D::Create(1, 1, VEC3_ZERO, VEC3_ZERO);
		if (pTempBlur2D != nullptr)
		{ // 生成に成功した場合

			// 情報を生成元と一致させる
			pTempBlur2D->BindTexture(m_pParent->GetTextureIndex());	// 同一テクスチャを割当

			// アニメーション情報を取得し設定
			pTempBlur2D->SetWidthPattern(m_pParent->GetWidthPattern());
			pTempBlur2D->SetHeightPattern(m_pParent->GetHeightPattern());
			pTempBlur2D->SetPattern(m_pParent->GetPattern());

			// 位置・向き・大きさを取得し設定
			pTempBlur2D->SetVec3Position(m_pParent->GetVec3Position());
			pTempBlur2D->SetVec3Rotation(m_pParent->GetVec3Rotation());
			pTempBlur2D->SetVec3Sizing(m_pParent->GetVec3Sizing());

			// 加算合成にする
			pTempBlur2D->GetRenderState()->SetAlphaBlend(CRenderState::BLEND_ADD);
		}

		// 配列の最後尾に残像を追加
		m_oldObject.push_back(pTempBlur2D);
	}

	// 残像の色反映
	{
		int nCntBlur2D = 0;	// ブラーインデックス
		for (auto& rVec : m_oldObject)
		{ // 保存オブジェクト数分繰り返す

			// 透明度を後ろになるにつれて下げていく
			D3DXCOLOR colSet = m_col;	// 設定色
			colSet.a = (m_fStartAlpha / m_nMaxLength) * nCntBlur2D;

			// 色を反映
			rVec->SetColor(colSet);

			// ブラーインデックスを進める
			nCntBlur2D++;
		}
	}

	// 残像の削除
	{
		bool bDelete = false;	// 削除状況
		switch (m_state)
		{ // 状態ごとの処理
		case STATE_NONE:
			break;

		case STATE_NORMAL:

			// 削除条件を指定 (残像が最大数を超えた場合削除)
			bDelete = (int)m_oldObject.size() > m_nMaxLength;
			break;

		case STATE_VANISH:

			// 削除条件を指定 (確定削除)
			bDelete = true;
			break;

		default:
			assert(false);
			break;
		}

		if (bDelete)
		{ // 削除を行う場合

			if (m_oldObject.size() <= 0)
			{ // 配列要素がない場合
			
				// 何もしない状態にする
				m_state = STATE_NONE;

				// 処理を抜ける
				return;
			}

			// 先頭の終了
			SAFE_UNINIT(m_oldObject.front());

			// 残像の最後尾を配列から削除
			m_oldObject.erase(m_oldObject.begin());
		}
	}
	
}

//============================================================
//	描画処理
//============================================================
void CBlur2D::Draw(CShader *pShader)
{
	// 何もしない状態の場合抜ける
	if (m_state == STATE_NONE) { return; }

	for (auto& rVec : m_oldObject)
	{ // 保存オブジェクト数分繰り返す

		// パーツの描画
		rVec->Draw(pShader);
	}
}

//============================================================
//	生成処理
//============================================================
CBlur2D *CBlur2D::Create
(
	CAnim2D *pParent,			// 親オブジェクト
	const D3DXCOLOR& rCol,		// ブラー反映色
	const float fStartAlpha,	// ブラー開始透明度
	const int nMaxLength		// 保持オブジェクト最大数
)
{
	// ブラー2Dの生成
	CBlur2D *pBlur2D = new CBlur2D;
	if (pBlur2D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ブラー2Dの初期化
		if (FAILED(pBlur2D->Init()))
		{ // 初期化に失敗した場合

			// ブラー2Dの破棄
			SAFE_DELETE(pBlur2D);
			return nullptr;
		}

		// 親オブジェクトを設定
		pBlur2D->m_pParent = pParent;

		// ブラー反映色を設定
		pBlur2D->SetColor(rCol);

		// ブラーの開始透明度を設定
		pBlur2D->SetStartAlpha(fStartAlpha);

		// 保持オブジェクトの最大数を設定
		pBlur2D->SetMaxLength(nMaxLength);

		// 確保したアドレスを返す
		return pBlur2D;
	}
}

//============================================================
//	状態の設定処理
//============================================================
void CBlur2D::SetState(const EState state)
{
	if (state == m_state && state != STATE_NORMAL)
	{ // 設定する状態が現在の状態且つ、設定する状態が通常状態の場合

		// 処理を抜ける
		return;
	}

	if (m_state == STATE_NONE && state == STATE_VANISH)
	{ // すでに消えているのに消失させようとしている場合

		// 処理を抜ける
		return;
	}

	// 引数の状態を設定
	m_state = state;

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない状態
	case STATE_VANISH:	// 消失状態
	case STATE_NORMAL:	// 通常状態
		break;

	default:	// 例外状態
		assert(false);
		break;
	}
}
