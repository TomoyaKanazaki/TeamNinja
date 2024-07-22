#pragma once
//=========================================
//
//  複数管理ギミック (gimmick_mulch.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick.h"

//=========================================
//  前方宣言
//=========================================
class CActor;	// アクタークラス

//=========================================
//  クラス定義
//=========================================
class CGimmickMulch : public CGimmick
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
	CGimmickMulch();
	~CGimmickMulch() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

	void SetVec3Position(const D3DXVECTOR3& rPos);
	bool IsActive() { return m_bActive; }

	// 静的メンバ関数
	static CGimmickMulch* Create(const D3DXVECTOR3& rPos, const EAngle angle, std::vector<SButton> vecButton); // 生成

#ifdef _DEBUG
	// メンバ関数
	const std::vector<CGimmick*> GetVectorButton(void) override { return m_vecButton; }	// ボタン動的配列取得
#endif // _DEBUG

private:
	// メンバ関数
	HRESULT CreateButton(std::vector<SButton> vecButton);	// ボタン情報生成

	// メンバ変数
	CActor* m_pModel; // モデル情報
	std::vector<CGimmick*> m_vecButton; // ボタン動的配列
	bool m_bActive; // アクティブフラグ
};
