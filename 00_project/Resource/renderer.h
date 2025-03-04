//============================================================
//
//	レンダラーヘッダー [renderer.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "renderTexture.h"

//************************************************************
//	前方宣言
//************************************************************
class CRenderTexture;	// レンダーテクスチャクラス
class CScreen;			// スクリーンクラス

//************************************************************
//	クラス定義
//************************************************************
// レンダラークラス
class CRenderer
{
public:
	// コンストラクタ
	CRenderer();

	// デストラクタ
	~CRenderer();

	// メンバ関数
	HRESULT Init(HWND hWnd, BOOL bWindow);		// 初期化
	void Uninit(void);							// 終了
	void Update(const float fDeltaTime);		// 更新
	void Draw(void);							// 描画
	HRESULT CreateRenderTexture(void);			// レンダーテクスチャー生成
	LPDIRECT3DDEVICE9 GetDevice(void) const;	// デバイス取得
	D3DXCOLOR GetClearColor(void) const;		// 画面クリア

	void DrawRenderTexture	// レンダーテクスチャ描画
	( // 引数
		const CObject::EScene scene,	// 描画シーン
		LPDIRECT3DSURFACE9* pSurface	// テクスチャサーフェイス
	);
	int GetRenderTextureIndex(const CObject::EScene scene) { return m_apRenderScene[scene]->GetTextureIndex(); }	// レンダーテクスチャインデックス取得

	// 静的メンバ関数
	static CRenderer *Create(HWND hWnd, BOOL bWindow);	// 生成
	static void Release(CRenderer *&prRenderer);		// 破棄

private:
	// メンバ関数
	HRESULT CreateDevice(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp);	// デバイス生成

	// メンバ変数
	CRenderTexture*		m_apRenderScene[CObject::SCENE_MAX];	// シーンレンダーテクスチャ
	CScreen*			m_pDrawScreen;		// スクリーン描画ポリゴン
	LPDIRECT3DSURFACE9	m_pDefSurScreen;	// 元の描画サーフェイス保存用
	LPDIRECT3D9			m_pD3D;				// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9	m_pD3DDevice;		// Direct3Dデバイス
};

#endif	// _RENDERER_H_
