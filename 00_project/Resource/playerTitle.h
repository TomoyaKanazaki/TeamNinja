//============================================================
//
//	タイトル画面プレイヤーヘッダー [playerTitle.h]
//	Author：藤田勇一
// 
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_TITLE_H_
#define _PLAYER_TITLE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "player.h"

//************************************************************
//	クラス定義
//************************************************************
// タイトル画面プレイヤークラス
class CPlayerTitle : public CPlayer
{
public:
	// コンストラクタ
	CPlayerTitle();

	// デストラクタ
	~CPlayerTitle() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;			// 更新
	EMotion UpdateState(const float fDeltaTime) override;	// 状態更新
	void SetSpawn(void) override;	// スポーン設定
	void SetStart(void) override;	// 演出開始設定

private:
	// メンバ関数
	EMotion UpdateIdol(const float fDeltaTime);	// 仁王立ち状態時の更新
	EMotion UpdateMove(const float fDeltaTime);	// 移動状態時の更新
};

#endif	// _PLAYER_TITLE_H_
