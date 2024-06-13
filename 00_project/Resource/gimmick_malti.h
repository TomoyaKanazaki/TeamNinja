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
	// 生成ボタン構造体
	struct SButton
	{
		// コンストラクタ
		SButton(D3DXVECTOR3 Pos, D3DXVECTOR3 Size) :
			pos		(Pos),	// 位置
			size	(Size)	// 大きさ
		{}

		// メンバ変数
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 size;	// 大きさ
	};

	// メンバ関数
	CGimmickMalti();
	~CGimmickMalti() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

	bool IsActive() { return m_bActive; }

	// 静的メンバ関数
	static CGimmickMalti* Create(std::vector<SButton> vecButton); // 生成

private:
	// メンバ関数
	HRESULT CreateButton(std::vector<SButton> vecButton);	// ボタン情報生成

	// メンバ変数
	std::vector<CGimmick*> m_vecButton; // ボタン動的配列
	bool m_bActive; // アクティブフラグ
};
