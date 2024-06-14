//============================================================
//
//	敵の刀ヘッダー [enemy_katana.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_KATANA_H_
#define _ENEMY_KATANA_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "enemy_item.h"

//************************************************************
//	クラス定義
//************************************************************
// 敵の刀クラス
class CEnemyKatana : public CEnemyItem
{
public:

	// コンストラクタ
	CEnemyKatana();

	// デストラクタ
	~CEnemyKatana() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

private:

};

#endif	// _ACTOR_H_
