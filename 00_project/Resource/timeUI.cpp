//============================================================
//
//	タイムUI処理 [timeUI.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "timeUI.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "value.h"
#include "object2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// テクスチャファイル
	{
		"data\\TEXTURE\\timePart000.png",	// 通常区切り
	};

	const int PRIORITY = 6;	// タイマーの優先順位
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CValue::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	子クラス [CTimeUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTimeUI::CTimeUI() : CObject(CObject::LABEL_UI),
	m_type			(CValue::TYPE_NORMAL),	// 数字種類
	m_pos			(VEC3_ZERO),			// 位置
	m_sizeValue		(VEC3_ZERO),			// 数字の大きさ
	m_sizePart		(VEC3_ZERO),			// 区切りの大きさ
	m_spaceValue	(VEC3_ZERO),			// 数字の空白
	m_spacePart		(VEC3_ZERO),			// 区切りの空白
	m_col			(XCOL_WHITE)			// 色
{
	// メンバ変数をクリア
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	memset(&m_apPart[0], 0, sizeof(m_apPart));		// 区切りの情報
}

//============================================================
//	デストラクタ
//============================================================
CTimeUI::~CTimeUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTimeUI::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	memset(&m_apPart[0], 0, sizeof(m_apPart));		// 区切りの情報
	m_type = CValue::TYPE_NORMAL;	// 数字種類
	m_pos			= VEC3_ZERO;	// 位置
	m_sizeValue		= VEC3_ZERO;	// 数字の大きさ
	m_sizePart		= VEC3_ZERO;	// 区切りの大きさ
	m_spaceValue	= VEC3_ZERO;	// 数字の空白
	m_spacePart		= VEC3_ZERO;	// 区切りの空白
	m_col			= XCOL_WHITE;	// 色

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の生成
		m_apValue[nCntValue] = CValue::Create();
		if (m_apValue[nCntValue] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの生成
		m_apPart[nCntPart] = CObject2D::Create(VEC3_ZERO);
		if (m_apPart[nCntPart] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// 優先順位を設定
	SetPriority(PRIORITY);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTimeUI::Uninit(void)
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の終了
		SAFE_UNINIT(m_apValue[nCntValue]);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの終了
		SAFE_UNINIT(m_apPart[nCntPart]);
	}

	// オブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CTimeUI::Update(const float fDeltaTime)
{
	// TODO
#if 1
	if (GET_INPUTKEY->IsPress(DIK_W))
	{
		m_pos.y -= 1.0f;
	}
	if (GET_INPUTKEY->IsPress(DIK_S))
	{
		m_pos.y += 1.0f;
	}
	if (GET_INPUTKEY->IsPress(DIK_A))
	{
		m_pos.x -= 1.0f;
	}
	if (GET_INPUTKEY->IsPress(DIK_D))
	{
		m_pos.x += 1.0f;
	}
#endif

	// 相対位置の設定
	SetPositionRelative();

	// 数字のテクスチャ座標の設定
	SetTexNum();

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の更新
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->Update(fDeltaTime);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの更新
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->Update(fDeltaTime);
	}
}

//============================================================
//	描画処理
//============================================================
void CTimeUI::Draw(CShader * /*pShader*/)
{

}

//============================================================
//	位置の設定処理
//============================================================
void CTimeUI::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	優先順位の設定処理
//============================================================
void CTimeUI::SetPriority(const int nPriority)
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の優先順位を設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetPriority(nPriority);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの優先順位を設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetPriority(nPriority);
	}
}

//============================================================
//	更新状況の設定処理
//============================================================
void CTimeUI::SetEnableUpdate(const bool bUpdate)
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の更新状況を設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetEnableUpdate(bUpdate);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの更新状況を設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetEnableUpdate(bUpdate);
	}
}

//============================================================
//	描画状況の設定処理
//============================================================
void CTimeUI::SetEnableDraw(const bool bDraw)
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の描画状況を設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetEnableDraw(bDraw);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの描画状況を設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetEnableDraw(bDraw);
	}
}

//============================================================
//	生成処理
//============================================================
CTimeUI *CTimeUI::Create
(
	const D3DXVECTOR3& rPos,		// 位置
	const D3DXVECTOR3& rSizeValue,	// 数字の大きさ
	const D3DXVECTOR3& rSizePart,	// 区切りの大きさ
	const D3DXVECTOR3& rSpaceValue,	// 数字の空白
	const D3DXVECTOR3& rSpacePart,	// 区切りの空白
	const CValue::EType type,		// 数字種類
	const D3DXCOLOR& rCol			// 色
)
{
	// タイムUIの生成
	CTimeUI *pTimeUI = new CTimeUI;
	if (pTimeUI == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// タイムUIの初期化
		if (FAILED(pTimeUI->Init()))
		{ // 初期化に失敗した場合

			// タイムUIの破棄
			SAFE_DELETE(pTimeUI);
			return nullptr;
		}

		// 数字種類を設定
		pTimeUI->SetValueType(type);

		// 数字の大きさを設定
		pTimeUI->SetSizingValue(rSizeValue);

		// 区切りの大きさを設定
		pTimeUI->SetSizingPart(rSizePart);

		// 数字の空白を設定
		pTimeUI->SetSpaceValue(rSpaceValue);

		// 区切りの空白を設定
		pTimeUI->SetSpacePart(rSpacePart);

		// 色を設定
		pTimeUI->SetColor(rCol);

		// 位置を設定
		pTimeUI->SetVec3Position(rPos);

		// 確保したアドレスを返す
		return pTimeUI;
	}
}

//============================================================
//	数字の種類の設定処理
//============================================================
void CTimeUI::SetValueType(const CValue::EType type)
{
	// 設定された数字の種類を保存
	m_type = type;

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の種類を設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetType(type);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// テクスチャを割当
		assert(m_apPart[nCntPart] != nullptr);
		//m_apPart[nCntPart]->BindTexture(TEXTURE_FILE[(int)type]);	// TODO
	}
}

//============================================================
//	数字の大きさの設定処理
//============================================================
void CTimeUI::SetSizingValue(const D3DXVECTOR3& rSize)
{
	// 引数の数字の大きさを設定
	m_sizeValue = rSize;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	区切りの大きさの設定処理
//============================================================
void CTimeUI::SetSizingPart(const D3DXVECTOR3& rSize)
{
	// 引数の区切りの大きさを設定
	m_sizePart = rSize;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	数字の空白の設定処理
//============================================================
void CTimeUI::SetSpaceValue(const D3DXVECTOR3& rSpace)
{
	// 引数の数字の空白を設定
	m_spaceValue = rSpace;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	区切りの空白の設定処理
//============================================================
void CTimeUI::SetSpacePart(const D3DXVECTOR3& rSpace)
{
	// 引数の区切りの空白を設定
	m_spacePart = rSpace;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	色の設定処理
//============================================================
void CTimeUI::SetColor(const D3DXCOLOR& rCol)
{
	// 設定された色を保存
	m_col = rCol;

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の色を設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetColor(rCol);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの色を設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetColor(rCol);
	}
}

//============================================================
//	数字全体の横幅取得処理
//============================================================
float CTimeUI::GetTimeWidth(void) const
{
#if 0
	// 数字がない場合抜ける
	if ((int)m_listValue.size() <= 0) { assert(false); return 0.0f; }

	float fValueWidth = 0.0f;	// 数字全体の縦幅
	int nEndNumID = (int)m_listValue.size() - 1;	// 終端数字のインデックス
	for (int i = 0; i < nEndNumID; i++)
	{ // 終端数字を抜いた桁数分繰り返す

		// 次の数字までの列間を加算
		fValueWidth += m_space.x;
	}

	// 先頭と終端の数字の無視されたサイズを加算
	fValueWidth += m_listValue.front()->GetVec3Sizing().x * 0.5f;	// 先頭数字の原点左サイズ
	fValueWidth += m_listValue.back()->GetVec3Sizing().x * 0.5f;	// 終端数字の原点右サイズ

	// 数字全体の縦幅を返す
	return fValueWidth;
#else
	return 100.0f;
#endif
}

//============================================================
//	数字全体の縦幅取得処理
//============================================================
float CTimeUI::GetTimeHeight(void) const
{
#if 0
	// 数字がない場合抜ける
	if ((int)m_listValue.size() <= 0) { assert(false); return 0.0f; }

	float fValueHeight = 0.0f;	// 数字全体の縦幅
	int nEndNumID = (int)m_listValue.size() - 1;	// 終端数字のインデックス
	for (int i = 0; i < nEndNumID; i++)
	{ // 終端数字を抜いた桁数分繰り返す

		// 次の数字までの行間を加算
		fValueHeight += m_space.y;
	}

	// 先頭と終端の数字の無視されたサイズを加算
	fValueHeight += m_listValue.front()->GetVec3Sizing().y * 0.5f;	// 先頭数字の原点上サイズ
	fValueHeight += m_listValue.back()->GetVec3Sizing().y * 0.5f;	// 終端数字の原点下サイズ

	// 数字全体の縦幅を返す
	return fValueHeight;
#else
	return 100.0f;
#endif
}

//============================================================
//	数字全体の大きさ取得処理
//============================================================
D3DXVECTOR3 CTimeUI::GetTimeSize(void) const
{
	// 数字全体の大きさを返す
	return D3DXVECTOR3(GetTimeWidth(), GetTimeHeight(), 0.0f);
}

//============================================================
//	相対位置の設定処理
//============================================================
void CTimeUI::SetPositionRelative(void)
{
	D3DXVECTOR3 spaceValue = m_spaceValue * 0.5f;	// 数字の空白
	D3DXVECTOR3 spacePart = m_spacePart * 0.5f;		// 区切りの空白
	D3DXVECTOR3 posPoly = m_pos - spaceValue;		// ポリゴン生成位置
	int nNumValue = 0;	// 数字の生成数
	int nNumPart = 0;	// 区切りの生成数

	for (int nCntTimer = 0; nCntTimer < timeUI::MAX_DIGIT + timeUI::MAX_PART; nCntTimer++)
	{ // 数字の数 + 区切りの数分繰り返す

		if (nCntTimer == timeUI::MAX_MIN || nCntTimer == timeUI::MAX_MIN + timeUI::MAX_SEC + 1)
		{ // 区切りタイミングの場合

			assert(m_apPart[nNumValue] != nullptr);

			// ポリゴン生成位置をずらす
			posPoly += spacePart;

			// 区切りの位置を設定
			m_apPart[nNumValue]->SetVec3Position(posPoly);

			// 区切りの大きさを設定
			m_apPart[nNumValue]->SetVec3Sizing(m_sizePart);

			// 区切り生成数を加算
			nNumValue++;

			// ポリゴン生成位置をずらす
			posPoly += spacePart;
		}
		else
		{ // 数字タイミングの場合

			assert(m_apValue[nNumPart] != nullptr);

			// ポリゴン生成位置をずらす
			posPoly += spaceValue;

			// 数字の位置を設定
			m_apValue[nNumPart]->SetVec3Position(posPoly);

			// 数字の大きさを設定
			m_apValue[nNumPart]->SetVec3Sizing(m_sizeValue);

			// 数字生成数を加算
			nNumPart++;

			// ポリゴン生成位置をずらす
			posPoly += spaceValue;
		}
	}
}

//============================================================
//	数字のテクスチャ座標の設定処理
//============================================================
void CTimeUI::SetTexNum(void)
{
	int aNumDivide[timeUI::MAX_DIGIT];	// 数値分解用

#if 0
	// 分を桁数ごとに分解
	useful::DivideDigitNum(&aNumDivide[0], GetMin(), timeUI::MAX_MIN);

	// 秒を桁数ごとに分解
	useful::DivideDigitNum(&aNumDivide[timeUI::MAX_MIN], GetSec(), timeUI::MAX_SEC);

	// ミリ秒を桁数ごとに分解
	useful::DivideDigitNum(&aNumDivide[timeUI::MAX_MIN + timeUI::MAX_SEC], GetMSec(), timeUI::MAX_MSEC);
#else
	// 分を桁数ごとに分解
	useful::DivideDigitNum(&aNumDivide[0], 0, timeUI::MAX_MIN);

	// 秒を桁数ごとに分解
	useful::DivideDigitNum(&aNumDivide[timeUI::MAX_MIN], 0, timeUI::MAX_SEC);

	// ミリ秒を桁数ごとに分解
	useful::DivideDigitNum(&aNumDivide[timeUI::MAX_MIN + timeUI::MAX_SEC], 0, timeUI::MAX_MSEC);
#endif

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の設定
		m_apValue[nCntValue]->SetNumber(aNumDivide[nCntValue]);
	}
}
