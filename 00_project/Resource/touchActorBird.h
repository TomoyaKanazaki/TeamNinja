//============================================================
//
//	タッチアクター(鳥)ヘッダー [touchActorBird.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TOUCH_ACTOR_BIRD_H_
#define _TOUCH_ACTOR_BIRD_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "touchActor.h"

//************************************************************
//	クラス定義
//************************************************************
// タッチアクター(鳥)クラス
class CTouchBird : public CTouchActor
{
public:

	// コンストラクタ
	CTouchBird();

	// デストラクタ
	~CTouchBird() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// メンバ関数
	bool Collision		// 当たり判定
	(
		const D3DXVECTOR3& rPos,
		const D3DXVECTOR3& rPosOld,
		const float fRadius,
		const float fHeight
	) override;

private:

	// オーバーライド関数
	void UpdateNone(const float fDeltaTime) override;		// 通常状態更新処理
	void UpdateAct(const float fDeltaTime) override;		// アクション状態更新処理

	// メンバ変数
	D3DXVECTOR3 m_move;		// 移動量
	int m_nStateCount;		// 状態カウント
};

#endif	// _TOUCH_ACTOR_CAN_H_
