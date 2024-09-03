#pragma once
//==========================================
//
//  回転扉ヘッダー(spin_wall.h)
//  Author：小原立暉
//
//==========================================
#include "actor.h"

//==========================================
//  クラス定義
//==========================================
class CSpinWall : public CActor
{
public:

	// 状態
	enum EState
	{
		STATE_STOP = 0,		// 停止状態
		STATE_SPIN,			// 回転状態
		STATE_MAX			// この列挙型の総数
	};

	// メンバ関数
	CSpinWall(const D3DXVECTOR3& rRot);
	~CSpinWall();

	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	void SetVec3Scaling(const D3DXVECTOR3& rScale) override;

	// 初期位置の取得処理
	D3DXVECTOR3 GetDefaultRot(void) const { return m_rotDefault; }

#ifdef _DEBUG

	void SetDefaultRot(const D3DXVECTOR3& rRot);	// 初期向きの設定処理

#endif // _DEBUG


private:

	void Collision						// 当たり判定処理
	(
		D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rPosOld,		// 前回の位置
		const float fRadius,			// 半径
		const float fHeight,			// 高さ
		D3DXVECTOR3& rMove,				// 移動量
		bool& bJump						// ジャンプ状況
	) override;

	void Collision						// 当たり判定処理(判定を返すオーバーライド)
	(
		D3DXVECTOR3& rPos,				// 位置
		const D3DXVECTOR3& rPosOld,		// 前回の位置
		const float fRadius,			// 半径
		const float fHeight,			// 高さ
		D3DXVECTOR3& rMove,				// 移動量
		bool& bJump,					// ジャンプ状況
		bool& bHit						// 衝突判定
	) override;

	// メンバ関数
	void State(void);			// 状態処理

	void CollSizeSet(const D3DXVECTOR3& rScale);		// 当たり判定のサイズの設定処理

	// メンバ変数
	D3DXVECTOR3 m_rotDefault;	// 初期向きを保存する変数
	D3DXVECTOR3 m_collMax;		// 当たり判定の最大値
	D3DXVECTOR3 m_collMin;		// 当たり判定の最小値
	EState m_state;				// 状態
	float m_fRotMove;			// 向きの移動量

};
