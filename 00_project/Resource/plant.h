#pragma once
//===========================================
//
//  植物クラス(plant.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "object3D.h"

//===========================================
//  クラス定義
//===========================================
class CPlant : public CObject3D
{
public:

	// メンバ関数
	CPlant();
	~CPlant();

	virtual HRESULT Init(void) override;	// 初期化
	virtual void Uninit(void) override;		// 終了
	virtual void Update(const float fDeltaTime) override;		// 更新
	virtual void Draw(CShader* pShader = nullptr) override;		// 描画

private:

};
