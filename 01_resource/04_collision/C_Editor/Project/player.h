//============================================================
//
//	プレイヤーヘッダー [player.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectChara.h"
#include <map>
#include <string>

//************************************************************
//	前方宣言
//************************************************************
class CMultiModel;	// マルチモデルクラス
class CShadow;		// 影クラス

//************************************************************
//	クラス定義
//************************************************************
// プレイヤークラス
class CPlayer : public CObjectChara
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_NORMAL,	// 通常状態
		STATE_DEATH,	// 死亡状態
		STATE_MAX		// この列挙型の総数
	};

	// 初期モーション列挙
	enum EMotion
	{
		MOTION_IDOL = 0,	// 待機モーション：ループON
		MOTION_MOVE,		// 移動モーション：ループON
		MOTION_JUMP,		// ジャンプモーション：ループOFF
		MOTION_LAND,		// 着地モーション：ループOFF
		MOTION_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CPlayer();

	// デストラクタ
	~CPlayer() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void SetState(const int nState) override;	// 状態設定
	int GetState(void) const override;			// 状態取得
	float GetRadius(void) const override;		// 半径取得
	float GetHeight(void) const override;		// 縦幅取得
	void SetEnableUpdate(const bool bUpdate) override;	// 更新状況設定
	void SetEnableDraw(const bool bDraw) override;		// 描画状況設定

	// 静的メンバ関数
	static CPlayer *Create(void);	// 生成

private:
	// メンバ関数
	void LoadSetup(void);				// セットアップ
	void UpdateNormal(int *pMotion);	// 通常状態時の更新
	void UpdateMove(int *pMotion);		// 移動操作・目標向きの更新
	void UpdateJump(int *pMotion);		// ジャンプ操作の更新

	void UpdateMotion(int nMotion);			// モーションの更新
	void UpdateOldPosition(void);			// 過去位置の更新
	void UpdateGravity(void);				// 重力の更新
	void UpdateLanding(D3DXVECTOR3 *pPos);	// 着地状況の更新
	void UpdatePosition(D3DXVECTOR3 *pPos);	// 位置の更新
	void UpdateRotation(D3DXVECTOR3 *pRot);	// 向きの更新

	// メンバ変数
	std::map<int, std::string> m_modelFileName;	// モデルファイル名
	CShadow		*m_pShadow;			// 影の情報
	D3DXVECTOR3	m_oldPos;			// 過去位置
	D3DXVECTOR3	m_move;				// 移動量
	D3DXVECTOR3	m_destRot;			// 目標向き
	EState		m_state;			// 状態
	int			m_nCounterState;	// 状態管理カウンター
	bool		m_bJump;			// ジャンプ状況
};

#endif	// _PLAYER_H_
