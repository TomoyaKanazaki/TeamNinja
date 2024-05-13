//==========================================
//
//  ゴール(goal.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#pragma once
#include "objectModel.h"

//==========================================
//  クラス定義
//==========================================
class CGoal : public CObjectModel
{
public:

	// メンバ関数
	CGoal();
	~CGoal();

	HRESULT Init() override; // 初期化
	void Uninit() override; // 終了
	void Update(const float fDeltaTime) override; // 更新
	void Draw(CShader* pShader = nullptr) override; // 描画

	bool GetClear() const { return m_bClear; }// クリア状態の取得

	// 静的メンバ関数
	static CGoal* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot); // 生成処理

private:

	// メンバ関数
	void CollisionPlayer();	// プレイヤーとの当たり判定
	void Load(); // 外部情報の読み込み

	// メンバ変数
	bool m_bClear; // クリアフラグ
	float m_fRadius; // 当たり判定半径
	float m_fRotSpeed; // 回転速度

};
