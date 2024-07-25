//============================================================
//
//	レンダーテクスチャヘッダー [renderTexture.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// レンダーテクスチャクラス
class CRenderTexture : public CObject
{
public:
	// コンストラクタ
	CRenderTexture();

	// デストラクタ
	~CRenderTexture();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CRenderTexture *Create(void);	// 生成

private:
	// オーバーライド関数
	void Release(void) override { CObject::Release(); }	// 破棄
};

#endif	// _RENDER_TEXTURE_H_
