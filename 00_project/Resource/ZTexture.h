//============================================================
//
//	Zテクスチャ生成ヘッダー [ZTexture.h]
//	Author：丹野 竜之介
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ZTexture_H_
#define _ZTexture_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const UINT ZTEX_WIDTH = SCREEN_WIDTH;								//Zテクスチャの幅
	const UINT ZTEX_HEIGHT = SCREEN_WIDTH;										//Zテクスチャの高さ
	const float VIEWING_ANGLE = 45.0f;										//視野角
	const float NEAR_CLIP = 1000.0f;											//描画最小深度
	const float FAR_CLIP = 10000.0f;										//描画最大深度
	const int NUM_TEX = 9;
	const D3DXVECTOR3 VIEW_POINT = D3DXVECTOR3(1000.0f, 8000.0f, 1000.0f);		//視点
	const D3DXVECTOR3 FOCUS_POINT = D3DXVECTOR3(1100.0f, -10.0f, 0.0f);		//注視点
	const D3DXVECTOR3 LOOK_UP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				//上向きのベクトル *警告:書き換えるな*
}
//************************************************************
//	クラス定義
//************************************************************
// アニメーション2Dクラス
class CZTexture
{
public:
	// コンストラクタ
	CZTexture();
	// デストラクタ
	~CZTexture();
	// 初期化メソッド
	bool Init();

	// 描画対象オブジェクトのワールド変換行列を設定
	void SetWorldMatrix(D3DXMATRIX *pMat);

	// ビュー行列を設定
	void SetViewMatrix(D3DXMATRIX *pMat);

	// 射影変換行列を設定
	void SetProjMatrix(D3DXMATRIX *pMat);

	// 描画の開始を宣言する
	HRESULT Begin();

	// 描画の終了を宣言する
	HRESULT End();

	// パスの開始を宣言する
	HRESULT BeginPass();

	// パスの終了を宣言する
	HRESULT EndPass();

	void DrawSprite();

	void Blur();

	// 登録されているパラメータ情報をエフェクトにセット
	bool SetParamToEffect();

	// Z値テクスチャを取得する
	bool GetZTex(IDirect3DTexture9 **cpTex);

	bool GetIsBegin(void) { return m_bBegin; }

	// 静的メンバ関数
	static CZTexture* Create(void);		// 生成
	static CZTexture* GetInstance(void);	// 取得
	static void Release(void);				// 破棄

	D3DXMATRIX GetViewMtx() { return m_matView; }
	D3DXMATRIX GetProjMtx() { return m_matProj; }

private:

	// 静的メンバ変数
	static CZTexture* m_pShader;	// シェーダー情報

	// メンバ変数
	IDirect3DTexture9 * m_cpZTex;				// Z値テクスチャ
	IDirect3DSurface9 * m_cpZTexSurf;			// Z値テクスチャサーフェイス
	IDirect3DTexture9* m_cpBlurTex;				// ブラーテクスチャ
	IDirect3DSurface9* m_cpBlurTexSurf;			// ブラーテクスチャサーフェイス
	IDirect3DSurface9 * m_cpDepthBuff;			// 深度バッファ
	IDirect3DSurface9 * m_cpDevBuffer;			// デバイスバックバッファ
	IDirect3DSurface9 * m_cpDevDepth;			// デバイス深度バッファ
	ID3DXEffect * m_cpEffect;					// Z値プロットエフェクト
	ID3DXEffect* m_cpBlur;					// Z値プロットエフェクト
	D3DXMATRIX m_matWorld;						// ワールド変換行列
	D3DXMATRIX m_matView;						// ビュー変換行列
	D3DXMATRIX m_matProj;						// 射影変換行列
	D3DXHANDLE m_hWorldMat;						// ワールド変換行列ハンドル
	D3DXHANDLE m_hViewMat;						// ビュー変換行列ハンドル
	D3DXHANDLE m_hProjMat;						// 射影変換行列ハンドル
	D3DXHANDLE m_hTechnique;					// テクニックへのハンドル
	D3DXHANDLE m_hTechniqueBlur;					// テクニックへのハンドル
	ID3DXSprite* m_pSprite;						//シャドウ用スプライト

	bool m_bBegin;
};

#endif	// _ZTexture_H_
