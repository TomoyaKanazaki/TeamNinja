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
	CMash();
	~CMash();

	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	void SetVec3Position(const D3DXVECTOR3& rPos) override;

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
	void Move(const float fDeltaTime); // 移動

	// メンバ変数
	D3DXVECTOR3 m_posDefault;	// 初期位置を保存する変数
	D3DXVECTOR3 m_offsetMove;	// 移動先のオフセットを保存する変数
	D3DXVECTOR3 m_move;			// 移動量を保存する変数
	EState m_state;				// 状態

};
