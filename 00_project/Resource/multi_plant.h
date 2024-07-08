#pragma once
//===========================================
//
//  植物管理クラス(multi_plant.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "object3D.h"

//===========================================
//  クラス定義
//===========================================
class CMultiPlant : public CObject3D
{
public:

	// 花の種類
	enum EType
	{
		TYPE_TEST = 0, // TODO 種類とか名前とかいい感じにする
		TYPE_MAX
	};

	// メンバ関数
	CMultiPlant();
	~CMultiPlant();

	virtual HRESULT Init(void) override; // 初期化
	virtual void Uninit(void) override; // 終了
	virtual void Update(const float fDeltaTime) override; // 更新
	virtual void Draw(CShader* pShader = nullptr) override; // 描画

	// 静的メンバ関数
	static CMultiPlant* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const EType type, int nNum); // 生成

private:

	// メンバ関数
	void Grow(); // 花の生成

	// メンバ変数
	EType m_Type; // 生成する植物の種類
	D3DXVECTOR3 m_pos; // 中心座標
	D3DXVECTOR3 m_size; // 生成範囲
	int m_nNum; // 生成数
	bool m_bGrow; // 植物生成フラグ

};
