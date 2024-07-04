#pragma once
//=========================================
//
//  大砲のギミック(gimmick_canon.h)
//  Author : Tomoya Kanazaki
//
//=========================================
#include "gimmick_action.h"

//=========================================
//  クラス定義
//=========================================
class CGimmickCanon : public CGimmickAction
{
public:

	// 目標のZ座標列挙
	enum ETarget
	{
		TARGET_LEFT = -1, // 左側
		TARGET_CENTER, // 中央
		TARGET_RIGHT, // 右側
		TARGET_MAX
	};

	// メンバ関数
	CGimmickCanon();
	~CGimmickCanon() override;

	HRESULT Init(void) override; // 初期化処理
	void Uninit(void) override; // 終了処理
	void Update(const float fDeltaTime) override; // 更新処理
	void Draw(CShader* pShader = nullptr) override; // 描画処理
	D3DXVECTOR3 CalcWaitPoint(const int Idx) override; // 各分身毎の待機位置を算出
	D3DXVECTOR3 CalcWaitRotation(const int Idx, const D3DXVECTOR3& rPos) override; // 各分身毎の待機向きを算出

	void SetTarget(const float fTarget) { m_fTarget = fTarget; } // 目標位置の設定

	// 静的メンバ関数
	static CGimmickCanon* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, ETarget eTarget);

private:

	// メンバ関数
	void Shoot(); // 発射

	// メンバ変数
	float m_fTarget; // 目標位置
	bool m_bShoot; // 発射済み
};
