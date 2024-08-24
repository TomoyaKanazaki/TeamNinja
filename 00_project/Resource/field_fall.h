#pragma once
//=========================================
//
//  落とし穴ヘッダー (field_fall.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "field.h"

//*****************************************
// 前方宣言
//*****************************************
class CObjectMeshCube;		// メッシュキューブ

//=========================================
//  クラス定義
//=========================================
class CGimmickFall : public CField
{
public:

	// 状態
	enum EState
	{
		STATE_NONE = 0,		// 通常状態
		STATE_OPEN,			// 開扉状態
		STATE_MAX			// この列挙型の総数
	};

	// 静的メンバ変数
	static constexpr int NUM_FLOOR = 2;			// 床の総数

	// メンバ関数
	CGimmickFall();
	~CGimmickFall() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

	bool IsFall() const override { return m_bFall; } // 落下判定

	// オーバーライド関係
	void SetVec3Position(const D3DXVECTOR3& rPos) override;		// 位置の設定処理
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;		// 向きの設定処理
	void SetVec2Sizing(const D3DXVECTOR2& rSize) override;		// サイズの設定処理
	void SetType(const EType type) override;					// 種類の設定処理

	// 静的メンバ関数
	static int GetNumFloor(void) { return NUM_FLOOR; }		// 床の総数の取得処理

private:

	// メンバ関数
	void Hit(CPlayerClone* pClone) override; // 当たっていた場合の処理
	void Miss(CPlayerClone* pClone) override; // 当たっていない場合の処理
	void Hit(CPlayer* pPlayer) override; // 当たっていた場合の処理
	void Miss(CPlayer* pPlayer) override; // 当たっていない場合の処理

	void Count(); // 乗っているキャラクター総数の計算処理

	// 小原追加
	void State(void);		// 状態処理
	void Floor(void);		// 床の処理

	// メンバ変数
	CObjectMeshCube* m_apFloor[NUM_FLOOR];	// 床の情報
	EState m_state;		// 状態
	bool m_bFall; // 落下フラグ

};
