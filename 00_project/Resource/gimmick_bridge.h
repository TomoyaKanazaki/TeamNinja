#pragma once
//=========================================
//
//  橋のギミック(gimmick_bridge.h)
//  Author : Tomoya Kanazaki
//
//=========================================
#include "gimmick_action.h"

//===========================================
//  前方宣言
//===========================================
class CField;

//=========================================
//  クラス定義
//=========================================
class CGimmickBridge : public CGimmickAction
{
public:

	// メンバ関数
	CGimmickBridge();
	~CGimmickBridge() override;

	HRESULT Init(void) override; // 初期化処理
	void Uninit(void) override; // 終了処理
	void Update(const float fDeltaTime) override; // 更新処理
	void Draw(CShader* pShader = nullptr) override; // 描画処理
	D3DXVECTOR3 CalcWaitPoint(const int Idx, const CPlayerClone* pClone) override; // 各分身毎の待機位置を算出
	D3DXVECTOR3 CalcWaitRotation(const int Idx, const CPlayerClone* pClone) override; // 各分身毎の待機向きを算出

	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;

private:

	// メンバ関数
	void CalcConectPoint(); // 橋の端の計算処理
	void SetWait(int nIdx) { m_nIdxWait = nIdx; } // 待機中心のインデックス設定
	void Active(const float fDeltaTime); // アクティブ状態の処理
	void Moment(); // アクティブ状態になった瞬間の処理
	void Movement(); // アクティブ状態中の処理

	// メンバ変数
	D3DXVECTOR3 m_ConectPoint[2]; // 橋の端
	D3DXVECTOR3 m_vecToWait; // 中心座標から待機中心へのベクトル(単位ベクトル)
	int m_nIdxWait; // 待機中心のインデックス
	CField* m_pField; // アクティブ状態での足場
	float m_fRot; // 橋の向き
	float m_fMoveTime; // 移動時間
	bool m_bMove; // 移動完了フラグ
};
