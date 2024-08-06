//============================================================
//
//	回転オブジェクト2Dヘッダー [roll2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ROLL2D_H_
#define _ROLL2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D.h"

//************************************************************
//	クラス定義
//************************************************************
// 回転オブジェクト2Dクラス
class CRoll2D : public CObject2D
{
public:
	// 定数
	static constexpr float ADD_ROT = 1.0f;	// 向きの加算量

	// コンストラクタ
	CRoll2D();

	// デストラクタ
	~CRoll2D() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CRoll2D *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,		// 位置
		const D3DXVECTOR3& rSize,		// 大きさ
		const float fAddRot = ADD_ROT,			// 回転量
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// 向き
		const D3DXCOLOR& rCol = XCOL_WHITE		// 色
	);

	// メンバ関数
	void SetRoll(const bool bRoll)		{ m_bRoll = bRoll; }	// 回転設定
	bool IsRoll(void) const				{ return m_bRoll; }		// 回転取得
	void  SetAddRot(const float fAdd)	{ m_fAddRot = fAdd; }	// 向き加算量設定
	float GetAddRot(void) const			{ return m_fAddRot; }	// 向き加算量取得

private:
	// メンバ変数
	float m_fAddRot;	// 向きの加算量
	bool m_bRoll;		// 回転状況
};

#endif	// _ROLL2D_H_
