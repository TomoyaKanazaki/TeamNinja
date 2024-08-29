//============================================================
//
//	セレクト画面プレイヤーヘッダー [playerSelect.h]
//	Author：藤田勇一
// 
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_SELECT_H_
#define _PLAYER_SELECT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "player.h"

//************************************************************
//	クラス定義
//************************************************************
// セレクト画面プレイヤークラス
class CPlayerSelect : public CPlayer
{
public:
	// コンストラクタ
	CPlayerSelect();

	// デストラクタ
	~CPlayerSelect() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;			// 更新
	EMotion UpdateState(const float fDeltaTime) override;	// 状態更新

private:
	// メンバ関数
	EMotion UpdateSpawn(const float fDeltaTime);	// スポーン状態時の更新
	EMotion UpdateNormal(const float fDeltaTime);	// 通常状態時の更新
	void UpdateTrans(D3DXVECTOR3& rPos);			// ステージ遷移の更新
};

#endif	// _PLAYER_SELECT_H_
