//============================================================
//
//	タッチアクター(枝)ヘッダー [touchActorBranch.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TOUCH_ACTOR_BRANCH_H_
#define _TOUCH_ACTOR_BRANCH_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "touchActor.h"

//************************************************************
//	クラス定義
//************************************************************
// タッチアクター(枝)クラス
class CTouchBranch : public CTouchActor
{
public:

	// コンストラクタ
	CTouchBranch();

	// デストラクタ
	~CTouchBranch() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	void SetData				// 情報の設定処理
	(
		const EType type,			// 種類
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot,	// 向き
		const D3DXVECTOR3& rScale	// 拡大率
	) override;

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
	bool m_bSound;		// サウンド状況
};

#endif	// _TOUCH_ACTOR_CAN_H_
