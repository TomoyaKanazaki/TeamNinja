#pragma once
//===========================================
//
//  植物クラス(plant.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "objectBillboard.h"

//===========================================
//  クラス定義
//===========================================
class CPlant : public CObjectBillboard
{
public:

	// メンバ関数
	CPlant();
	~CPlant();

	virtual HRESULT Init(void) override; // 初期化処理
	virtual void Uninit(void) override; // 終了処理
	virtual void Update(const float fDeltaTime) override; // 更新処理
	virtual void Draw(CShader* pShader = nullptr) override; // 描画処理

	// 静的メンバ関数
	static CPlant* Create(const D3DXVECTOR3& rPos, const char* sPass);
	static CListManager<CPlant>* GetList(void); // リスト取得

private:

	// 静的メンバ変数
	static CListManager<CPlant>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CPlant>::AIterator m_iterator; // イテレーター

};
