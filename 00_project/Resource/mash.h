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

	// 状態
	enum EState
	{
		STATE_CLOSE = 0,	// 閉扉状態
		STATE_OPEN,			// 開扉状態
		STATE_MAX			// この列挙型の総数
	};

	// メンバ関数
	CMash(const D3DXVECTOR3& rPos);
	~CMash();

	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	void SetVec3Scaling(const D3DXVECTOR3& rScale) override;

	// 初期位置の取得処理
	D3DXVECTOR3 GetDefaultPos(void) const { return m_posDefault; }

#ifdef _DEBUG

	void SetDefaultPos(const D3DXVECTOR3& rPos);		// 初期位置の設定処理

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
	void State(const float fDeltaTime);	// 状態処理
	void Close(const float fDeltaTime);	// 閉扉処理
	void Open(const float fDeltaTime);	// 開扉状態

	void CollSizeSet(const D3DXVECTOR3& rScale);		// 当たり判定のサイズの設定処理

	// メンバ変数
	D3DXVECTOR3 m_posDefault;	// 初期位置を保存する変数
	D3DXVECTOR3 m_offsetMove;	// 移動先のオフセットを保存する変数
	D3DXVECTOR3 m_move;			// 移動量を保存する変数
	D3DXVECTOR3 m_collMax;		// 当たり判定の最大値
	D3DXVECTOR3 m_collMin;		// 当たり判定の最小値
	EState m_state;				// 状態

};
