#pragma once
//=========================================
//
//  設置ギミック (gimmick_post.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick_action.h"

//=========================================
//  前方宣言
//=========================================
class CObjectMeshCube;

//=========================================
//  クラス定義
//=========================================
class CGimmickPost : public CGimmickAction
{
public:
	// メンバ関数
	CGimmickPost();
	~CGimmickPost() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;
	void SetVec3Position(const D3DXVECTOR3& rPos) override;
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;
	D3DXVECTOR3 CalcWaitPoint(const int Idx) override; // 各分身毎の待機位置を算出

	bool IsSet() override { return IsActive(); } // 設置フラグの取得

private:
	// メンバ関数
	void SetButtonSizing(void);	// ボタン大きさ設定

	// メンバ変数
	CObjectMeshCube* m_pButton;	// ボタン
	CObjectMeshCube* m_pEdge;	// 縁
	float m_fButtonHeight;		// ボタン縦オフセット
};
