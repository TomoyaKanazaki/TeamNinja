//============================================================
//
//	トゥーンシャドウヘッダー [ToonShadow.h]
//	Author：丹野竜之介
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TOON_SHADOW_H_
#define _TOON_SHADOW_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "shader.h"

//************************************************************
//	クラス定義
//************************************************************
// トゥーンシェーダークラス
class CToonShadow : public CShader
{
public:
	// テクスチャ列挙
	enum ETexture
	{
		TEXTURE_TOON = 0,	// トゥーンマップテクスチャ
		TEXTURE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CToonShadow();

	// デストラクタ
	~CToonShadow();

	// オーバーライド関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);		// 終了

	// メンバ関数
	void SetTexture(const CTexture::STexture& rTexture);	// テクスチャ設定 (ポインタ)
	void SetTexture(const int nTextureID);					// テクスチャ設定 (インデックス)
	void SetMaterial(const D3DMATERIAL9& rMaterial);		// マテリアル設定
	void SetDiffuse(const D3DXCOLOR& rDiffuse);				// 拡散光設定
	void SetAmbient(const D3DXCOLOR& rAmbient);				// 環境光設定
	void SetEmissive(const D3DXCOLOR& rEmissive);			// 放射光設定
	
	void SetOnlyDiffuse(const D3DXCOLOR& rDiffuse);			// 拡散光のみ設定

	void SetShadowMap();									//シャドウマッピング用のデータ設定

	// 静的メンバ関数
	static CToonShadow* Create(void);		// 生成
	static CToonShadow* GetInstance(void);	// 取得
	static void Release(void);				// 破棄

private:
	// メンバ関数
	void SetToonMapTexture(const ETexture texture);	// トゥーンマップテクスチャ設定

	// 静的メンバ変数
	static CToonShadow* m_pShader;	// シェーダー情報

	// メンバ変数
	D3DXHANDLE m_hTextureToon;	// トゥーンマップテクスチャ
	D3DXHANDLE m_hDirectLight;	// 平行光源の方向ベクトル
	D3DXHANDLE m_hDiffuse;		// 拡散光
	D3DXHANDLE m_hAmbient;		// 環境光
	D3DXHANDLE m_hEmissive;		// 放射光

	D3DXHANDLE m_hEyePos;		// 視点座標
	D3DXHANDLE m_hLightViewMat;	// ライトビュー変換行列ハンドル
	D3DXHANDLE m_hLightProjMat;	// ライト射影変換行列ハンドル
	D3DXHANDLE m_hShadowMapTex;	// シャドウマップテクスチャハンドル
	
};

#endif	// _TOON_SHADOW_H_
