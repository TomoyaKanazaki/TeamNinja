#pragma once
//==========================================
//
//  ふすま(mash.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "actor.h"

//==========================================
//  クラス定義
//==========================================
class CMash : public CActor
{
public:

	// メンバ関数
	CMash();
	~CMash();

	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	void SetVec3Position(const D3DXVECTOR3& rPos) override;

private:

	// メンバ関数
	void Move(const float fDeltaTime); // 移動

	// メンバ変数
	D3DXVECTOR3 m_posDefault; // 初期位置を保存する変数
	D3DXVECTOR3 m_offsetMove; // 移動先のオフセットを保存する変数
	D3DXVECTOR3 m_move; // 移動量を保存する変数

};
