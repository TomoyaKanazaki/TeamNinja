//============================================================
//
//	吹き出しマネージャーヘッダー [balloonManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BALLOON_MANAGER_H_
#define _BALLOON_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// 吹き出しマネージャークラス
class CBalloonManager : public CObject
{
public:
	// コンストラクタ
	CBalloonManager();

	// デストラクタ
	~CBalloonManager();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CBalloonManager *Create(void);	// 生成

private:
	// オーバーライド関数
	void Release(void) override { CObject::Release(); }	// 破棄
};

#endif	// _BALLOON_MANAGER_H_
