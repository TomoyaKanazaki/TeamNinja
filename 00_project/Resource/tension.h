#pragma once
//===========================================
//
//  士気力ゲージ(tension.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "object2D.h"

//===========================================
//  クラス定義
//===========================================
class CTension : CObject2D
{
public:

	// メンバ関数
	CTension();
	~CTension();

	HRESULT Init(void) override; // 初期化処理
	void Uninit(void) override; // 終了処理
	void Update(const float fDeltaTime) override; // 更新処理
	void Draw(CShader* pShader = nullptr) override; // 描画処理

	// 静的メンバ関数
	static CTension* Create();

private:

	// メンバ変数


};