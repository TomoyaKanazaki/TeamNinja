//============================================================
//
//	文字列2Dヘッダー [string2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _STRING2D_H_
#define _STRING2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CFontChar;	// フォント文字クラス
class CChar2D;		// 文字2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 文字列2Dクラス
class CString2D : public CObject
{
public:
	// 横配置列挙
	enum EAlignX
	{
		XALIGN_LEFT = 0,	// 左揃え
		XALIGN_CENTER,		// 中央揃え
		XALIGN_RIGHT,		// 右揃え
		XALIGN_MAX,			// この列挙型の総数
	};

	// コンストラクタ
	CString2D();

	// デストラクタ
	~CString2D() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(CShader *pShader = nullptr) override;			// 描画
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// 位置設定
	D3DXVECTOR3 GetVec3Position(void) const override;		// 位置取得
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// 向き設定
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// 向き取得
	void SetHeight(const float fHeight) override;			// 縦幅設定
	float GetHeight(void) const override;					// 縦幅取得

	// 静的メンバ関数
	static CString2D *Create	// 生成
	( // 引数
		CFontChar *pFontChar,		// フォント文字情報
		const std::wstring &rStr,	// 指定文字列
		const D3DXVECTOR3 &rPos,	// 原点位置
		const float fHeight = 100.0f,			// 文字縦幅
		const EAlignX alignX = XALIGN_CENTER,	// 横配置
		const D3DXVECTOR3& rRot = VEC3_ZERO		// 原点向き

	);

	// メンバ関数
	HRESULT SetFontString	// フォント・文字列の設定
	( // 引数
		CFontChar *pFontChar,		// フォント文字情報
		const std::wstring &rStr	// 指定文字列
	);
	CChar2D *GetChar2D(const int nCharID) const;	// 文字取得
	float GetStrWidth(void) const;					// 文字列の横幅取得
	void SetAlignX(const EAlignX align);			// 横配置設定
	EAlignX GetAlignX(void) const;					// 横配置取得

private:
	// オーバーライド関数
	void Release(void) override;	// 破棄

	// メンバ関数
	void SetPositionRelative(void);	// 相対位置設定

	// メンバ変数
	CChar2D **m_ppChar;		// 文字ポリゴンの情報
	D3DXVECTOR3 m_pos;		// 位置
	EAlignX m_alignX;		// 横配置
	std::wstring m_wsStr;	// 指定文字列
};

#endif	// _STRING2D_H_
