//============================================================
//
//	文字2D処理 [char2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "char2D.h"
#include "manager.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// 文字2Dの優先順位
}

//************************************************************
//	子クラス [CChar2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CChar2D::CChar2D() : CObject2D(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
	m_wcChar		(0),	// 指定文字
	m_fSizeRate		(0.0f),	// 縦幅の割合
	m_fAbsOriginX	(0.0f)	// X原点オフセットの絶対値
{
	// メンバ変数をクリア
	memset(&m_infoChar, 0, sizeof(m_infoChar));	// 文字情報
}

//============================================================
//	デストラクタ
//============================================================
CChar2D::~CChar2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CChar2D::Init(void)
{
	// メンバ変数を初期化
	memset(&m_infoChar, 0, sizeof(m_infoChar));	// 文字情報
	m_wcChar		= 0;	// 指定文字
	m_fSizeRate		= 1.0f;	// 縦幅の割合
	m_fAbsOriginX	= 0.0f;	// X原点オフセットの絶対値

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
void CChar2D::Uninit(void)
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CChar2D::Update(void)
{
	// オブジェクト2Dの更新
	CObject2D::Update();
}

//============================================================
//	描画処理
//============================================================
void CChar2D::Draw(CShader *pShader)
{
	// オブジェクト2Dの描画
	CObject2D::Draw(pShader);
}

//============================================================
//	大きさの設定処理
//============================================================
void CChar2D::SetVec3Sizing(const D3DXVECTOR3& /*rSize*/)
{
	/*
		こっちで大きさを自由に変えられると
		文字の比率がおかしくなるのと、
		縦幅の割合なども変更できないため
		大きさ変更は SetHeight を利用してね。
	*/

	assert(false);
}

//============================================================
//	縦幅の設定処理
//============================================================
void CChar2D::SetHeight(const float fHeight)
{
	int nTexID = GetTextureIndex();	// フォントのテクスチャインデックス
	float fTexWidth = useful::GetTexWidthFromAspect(fHeight, nTexID);			// テクスチャ横幅
	D3DXIMAGE_INFO status = GET_MANAGER->GetTexture()->GetInfo(nTexID).status;	// テクスチャステータス

	// 縦幅の増減割合を保存
	m_fSizeRate = fHeight / (float)status.Height;

	// 大きさを設定
	CObject2D::SetVec3Sizing(D3DXVECTOR3(fTexWidth, fHeight, 0.0f));
}

//============================================================
//	生成処理
//============================================================
CChar2D *CChar2D::Create
(
	CFontChar *pFontChar,		// フォント文字情報
	const wchar_t wcChar,		// 指定文字
	const D3DXVECTOR3& rPos,	// 位置
	const float fHeight,		// 縦幅
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXCOLOR& rCol		// 色
)
{
	// 文字2Dの生成
	CChar2D *pChar2D = new CChar2D;
	if (pChar2D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 文字2Dの初期化
		if (FAILED(pChar2D->Init()))
		{ // 初期化に失敗した場合

			// 文字2Dの破棄
			SAFE_DELETE(pChar2D);
			return nullptr;
		}

		// フォント・文字を設定
		pChar2D->SetFontChar(pFontChar, wcChar);

		// 位置を設定
		pChar2D->SetVec3Position(rPos);

		// 向きを設定
		pChar2D->SetVec3Rotation(rRot);

		// 縦幅を設定
		pChar2D->SetHeight(fHeight);

		// 色を設定
		pChar2D->SetColor(rCol);

		// 確保したアドレスを返す
		return pChar2D;
	}
}

//============================================================
//	フォント・文字の設定処理
//============================================================
void CChar2D::SetFontChar
(
	CFontChar *pFontChar,	// フォント文字情報
	const wchar_t wcChar	// 指定文字
)
{
	// フォント文字の取得
	m_infoChar = pFontChar->Regist(wcChar);

	// 指定文字を保存
	m_wcChar = wcChar;

	// X原点オフセットの絶対値を保存
	m_fAbsOriginX = fabsf((float)m_infoChar.glyph.gmptGlyphOrigin.x);

	// テクスチャを作成・割当
	BindTexture(m_infoChar.nTexID);
}

//============================================================
//	ブラックボックスの左上オフセット取得処理
//============================================================
D3DXVECTOR2 CChar2D::GetOffsetBlackBoxLU(void)
{
	D3DXVECTOR2 tempOffset;	// float変換オフセット格納用
	tempOffset.x = (float)m_infoChar.offsetBlackBox.lu.x;
	tempOffset.y = (float)m_infoChar.offsetBlackBox.lu.y;

	// ブラックボックスの左上オフセットを返す
	return tempOffset * m_fSizeRate;
}

//============================================================
//	ブラックボックスの右下オフセット取得処理
//============================================================
D3DXVECTOR2 CChar2D::GetOffsetBlackBoxRD(void)
{
	D3DXVECTOR2 tempOffset;	// float変換オフセット格納用
	tempOffset.x = (float)m_infoChar.offsetBlackBox.rd.x;
	tempOffset.y = (float)m_infoChar.offsetBlackBox.rd.y;

	// ブラックボックスの右下オフセットを返す
	return tempOffset * m_fSizeRate;
}

//============================================================
//	原点のオフセット取得処理
//============================================================
float CChar2D::GetOffsetOrigin(void)
{
	// 文字ブラックボックスの横幅
	float fBlackBoxX = (float)m_infoChar.glyph.gmBlackBoxX;

	// 文字原点のオフセットを返す
	return (m_fAbsOriginX + fBlackBoxX * 0.5f) * m_fSizeRate;
}

//============================================================
//	次の文字までの距離取得処理
//============================================================
float CChar2D::GetNext(void)
{
	// 次の文字の距離を返す
	return (float)m_infoChar.glyph.gmCellIncX * m_fSizeRate;
}
