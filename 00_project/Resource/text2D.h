//============================================================
//
//	テキスト2Dヘッダー [text2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TEXT2D_H_
#define _TEXT2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "string2D.h"

//************************************************************
//	インクルードファイル
//************************************************************
class CFontChar;	// フォント文字クラス

//************************************************************
//	クラス定義
//************************************************************
// テキスト2Dクラス
class CText2D : public CObject
{
public:
	// 縦配置列挙
	enum EAlignY
	{
		YALIGN_TOP = 0,	// 上揃え
		YALIGN_CENTER,	// 中央揃え
		YALIGN_BOTTOM,	// 下揃え
		YALIGN_MAX,		// この列挙型の総数
	};

	// コンストラクタ
	CText2D();

	// デストラクタ
	~CText2D() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader *pShader = nullptr) override;			// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	D3DXVECTOR3 GetVec3Position(void) const override { return m_pos; }	// 位置取得
	D3DXVECTOR3 GetVec3Rotation(void) const override { return m_rot; }	// 向き取得

	// 静的メンバ関数
	static CText2D *Create	// 生成
	( // 引数
		CFontChar *pFontChar,		// フォント文字情報
		const D3DXVECTOR3& rPos,	// 原点位置
		const float fCharHeight,	// 文字縦幅
		const float fLineHeight,	// 行間縦幅
		const CString2D::EAlignX alignX = CString2D::XALIGN_CENTER,	// 横配置
		const EAlignY alignY = YALIGN_CENTER,	// 縦配置
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 原点向き
		const D3DXCOLOR& rCol = XCOL_WHITE		// 色
	);

	// メンバ関数
	void SetColor(const D3DXCOLOR& rCol);			// 色設定
	void SetFont(CFontChar *pFontChar);				// フォントの設定
	void SetCharHeight(const float fHeight);		// 文字の縦幅設定
	void SetLineHeight(const float fHeight);		// 行間の縦幅設定
	void SetAlignX(const CString2D::EAlignX align);	// 横配置設定
	void SetAlignY(const EAlignY align);			// 縦配置設定
	HRESULT AddString(const std::wstring& rStr);	// 文字列の追加
	float GetTextHeight(void) const;				// テキストの縦幅取得
	CString2D *GetString2D(const int nStrID) const;	// 文字列の取得
	D3DXCOLOR GetColor(void) const				{ return m_col; }			// 色取得
	float GetCharHeight(void) const				{ return m_fCharHeight; }	// 文字の縦幅取得
	float GetLineHeight(void) const				{ return m_fLineHeight; }	// 行間の縦幅取得
	CString2D::EAlignX GetAlignX(void) const	{ return m_alignX; }		// 横配置取得
	EAlignY GetAlignY(void) const				{ return m_alignY; }		// 縦配置取得

private:
	// オーバーライド関数
	void Release(void) override { CObject::Release(); }	// 破棄

	// メンバ関数
	void SetPositionRelative(void);	// 相対位置設定

	// メンバ変数
	std::list<CString2D*> m_listString;	// 文字列リスト
	CFontChar *m_pFontChar;				// フォント文字
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXCOLOR m_col;					// 色
	CString2D::EAlignX m_alignX;		// 横配置
	EAlignY m_alignY;					// 縦配置
	float m_fCharHeight;				// 文字の縦幅
	float m_fLineHeight;				// 文字の行間
};

#endif	// _TEXT2D_H_
