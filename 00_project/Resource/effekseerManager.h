//============================================================
//
//	エフェクシアマネージャーヘッダー [effekseerManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EFFEKSEER_MANAGER_H_
#define _EFFEKSEER_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// エフェクシアマネージャークラス
class CEffekseerManager : public CObject
{
public:
	// コンストラクタ
	CEffekseerManager();

	// デストラクタ
	~CEffekseerManager() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CEffekseerManager *Create(void);	// 生成

private:
	// オーバーライド関数
	void Release(void) override { CObject::Release(); }	// 破棄
};

#endif	// _EFFEKSEER_MANAGER_H_
