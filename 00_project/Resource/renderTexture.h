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
class CRenderTexture
{
public:
	// コンストラクタ
	explicit CRenderTexture(const CObject::EScene scene);

	// デストラクタ
	~CRenderTexture();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Draw(void);	// 描画
	int GetTextureIndex(void) const { return m_nTextureID; }	// テクスチャインデックス取得

	// 静的メンバ関数
	static CRenderTexture *Create(const CObject::EScene scene);	// 生成

private:
	// メンバ変数
	LPDIRECT3DSURFACE9 m_pSurTexture;	// テクスチャサーフェイスへのポインタ
	int m_nTextureID;					// レンダーテクスチャインデックス
	const CObject::EScene m_scene;		// 描画シーン
};

#endif	// _RENDER_TEXTURE_H_
