#pragma once
//=========================================
//
//  複数管理ギミック (gimmick_malti.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick.h"

//=========================================
//  クラス定義
//=========================================
class CGimmickMalti : public CGimmick
{
public:

	// メンバ関数
	CGimmickMalti();
	~CGimmickMalti() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

	bool IsActive() { return m_bActive; }

private:

	// メンバ関数
	HRESULT Create(); // 設置ギミックの生成

	// メンバ変数
	bool m_bActive; // アクティブフラグ
	CListManager<CGimmick>::AIterator m_iterator; // イテレーター
	CListManager<CGimmick>* m_pGimmick; // オブジェクトリスト

};
