//==========================================
//
//  チェックポイント(checkpoint.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#pragma once
#include "objectModel.h"

//==========================================
//  クラス定義
//==========================================
class CCheckPoint : public CObjectModel
{
public:

	// メンバ関数
	CCheckPoint();
	~CCheckPoint();

	HRESULT Init(void) override; // 初期化
	void Uninit(void) override; // 終了
	void Update(const float fDeltaTime) override; // 更新
	void Draw(CShader* pShader = nullptr) override; // 描画

	// 静的メンバ関数
	static CCheckPoint* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot); // 生成処理

private:

	// メンバ関数
	void CollisionPlayer(void);	// プレイヤーとの当たり判定

	// メンバ変数
	bool m_bSave; // セーブフラグ
	float m_fRadius; // 当たり判定半径
	int m_nSaveTension; // セーブ時の士気力

	// 静的メンバ変数
	static int m_nNumAll;

};
