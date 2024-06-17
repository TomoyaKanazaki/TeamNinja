#pragma once
//=========================================
//
//  朽ちた床ヘッダー (gimmick_decayed.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "field.h"

//=========================================
//  クラス定義
//=========================================
class CGimmickDecayed : public CField
{
public:

	// メンバ関数
	CGimmickDecayed();
	~CGimmickDecayed() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

	bool IsFall() const override { return m_bFall; } // 落下判定

private:

	// メンバ関数
	void Hit(CPlayerClone* pClone) override; // 当たっていた場合の処理
	void Miss(CPlayerClone* pClone) override; // 当たっていない場合の処理
	void Count(); // 乗っているキャラクター総数の計算処理

	// メンバ変数
	bool m_bFall; // 床抜けフラグ

};
