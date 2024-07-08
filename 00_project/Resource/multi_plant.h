#pragma once
//===========================================
//
//  植物管理クラス(multi_plant.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "object3D.h"
#include "gimmick.h"

//===========================================
//  クラス定義
//===========================================
class CMultiPlant : public CObject3D
{
public:

	// メンバ関数
	CMultiPlant();
	~CMultiPlant();

	virtual HRESULT Init(void) override; // 初期化
	virtual void Uninit(void) override; // 終了
	virtual void Update(const float fDeltaTime) override; // 更新
	virtual void Draw(CShader* pShader = nullptr) override; // 描画

	void Grow(); // 花の生成

	// 静的メンバ関数
	static CMultiPlant* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const CGimmick::EType type, int nNum); // 生成

private:

	// メンバ変数
	CGimmick::EType m_Type; // 生成する植物の種類
	int m_nNum; // 生成数
	bool m_bGrow; // 植物生成フラグ

};
