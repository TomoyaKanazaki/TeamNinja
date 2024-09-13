#pragma once
//==========================================
//
//  溺死UIクラス(drown.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "object2D.h"

//==========================================
//  前方宣言
//==========================================
class CObject2D;

//==========================================
//  クラス定義
//==========================================
class CDrown : public CObject2D
{
public:

	// メンバ関数
	CDrown();
	~CDrown();

	// オーバーライド関数
	HRESULT Init(void) override; // 初期化
	void Uninit(void) override; // 終了
	void Update(const float fDeltaTime) override; // 更新
	void Draw(CShader *pShader = nullptr) override; // 描画
	void Move(const float fDeltaTime); // 移動

	void SetMove(bool bFrag) { m_bMove = bFrag; } // フラグを設定
	bool IsBrind() { m_bBrind; } // 目隠しフラグの取得

	// 静的メンバ関数
	static CDrown* Create(void);

private:

	// メンバ変数
	bool m_bMove; // 移動フラグ
	bool m_bBrind; // 目隠し完了フラグ
	float m_fMove; // 移動量

};
