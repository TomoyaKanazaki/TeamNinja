//============================================================
//
//	トゥーンシェーダー+シャドウマッピング [ToonShadow.cpp]
//	Author：丹野竜之介
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "ToonShadow.h"
#include "ZTexture.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "texture.h"
#include "camera.h"
//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* TEXTURE_FILE[] =	// テクスチャ定数
	{
		"data\\TEXTURE\\SHADER\\toon004.png",	// トゥーンマップテクスチャ
	};

	const char* EFFECT_FX = "data\\SHADER\\Toonshadow.fx";	// トゥーンシェーダーのエフェクトファイル
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CToonShadow::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CToonShadow* CToonShadow::m_pShader = nullptr;	// シェーダー情報

//************************************************************
//	親クラス [CToonShadow] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CToonShadow::CToonShadow() :
	m_hTextureToon(nullptr),	// トゥーンマップテクスチャ
	m_hDirectLight(nullptr),	// 平行光源の方向ベクトル
	m_hDiffuse(nullptr),	// 拡散光
	m_hAmbient(nullptr),	// 環境光
	m_hEmissive(nullptr),	// 放射光
	m_hLightProjMat(nullptr),
	m_hLightViewMat(nullptr),
	m_hShadowMapTex(nullptr)

{

}

//============================================================
//	デストラクタ
//============================================================
CToonShadow::~CToonShadow()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CToonShadow::Init(void)
{
	// 変数を宣言
	D3DCAPS9 caps;	// ハードウェア機能
	HRESULT hr;		// 異常終了の確認用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報
	LPD3DXBUFFER pError = nullptr;	// コンパイルエラー情報
	LPD3DXEFFECT pEffect = nullptr;	// エフェクト設定用

	// メンバ変数を初期化
	m_hTextureToon = nullptr;	// トゥーンマップテクスチャ
	m_hDirectLight = nullptr;	// 平行光源の方向ベクトル
	m_hDiffuse = nullptr;	// 拡散光
	m_hAmbient = nullptr;	// 環境光
	m_hEmissive = nullptr;	// 放射光
	
	// シェーダーの初期化
	if (FAILED(CShader::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ハードウェア機能を取得
	pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1)
		&& caps.PixelShaderVersion >= D3DPS_VERSION(2, 0))
	{ // 頂点・ピクセルシェーダのバージョンが使用可能な場合

		// エフェクトファイルの読込
		hr = D3DXCreateEffectFromFile
		( // 引数
			pDevice,	// デバイスへのポインタ
			EFFECT_FX,	// エフェクトファイル
			nullptr,	// プリプロセッサ定義
			nullptr,	// インクルード操作
			0,			// 読込オプションフラグ
			nullptr,	// ハンドルインターフェース
			&pEffect,	// エフェクトインターフェース
			&pError		// コンパイルエラー情報
		);
		if (SUCCEEDED(hr))
		{ // 読込に成功した場合

			// エフェクト情報を設定
			SetEffect(pEffect);

			// テクニック関数を取得・設定
			SetTechnique(pEffect->GetTechniqueByName("TShader"));

			// 各マトリックスのハンドルを取得・設定
			SetMatrixWorld(pEffect->GetParameterByName(nullptr, "s_mtxWorld"));		// ワールドマトリックス
			SetMatrixView(pEffect->GetParameterByName(nullptr, "s_mtxView"));		// ビューマトリックス
			SetMatrixProjection(pEffect->GetParameterByName(nullptr, "s_mtxProj"));	// プロジェクションマトリックス

			// ハンドルを取得
			m_hTextureToon = pEffect->GetParameterByName(nullptr, "s_textureToon");	// トゥーンマップテクスチャ
			m_hDirectLight = pEffect->GetParameterByName(nullptr, "s_dirLight");		// 平行光源の方向ベクトル
			m_hDiffuse = pEffect->GetParameterByName(nullptr, "s_diffuse");		// 拡散光
			m_hAmbient = pEffect->GetParameterByName(nullptr, "s_ambient");		// 環境光
			m_hEmissive = pEffect->GetParameterByName(nullptr, "s_emissive");		// 放射光
			
			m_hLightProjMat = pEffect->GetParameterByName(nullptr, "s_mtxLightProj");		// 射影変換行列
			m_hLightViewMat = pEffect->GetParameterByName(nullptr, "s_mtxLightView");		// ライトビュー変換行列
			m_hShadowMapTex = pEffect->GetParameterByName(nullptr, "s_texShadowMap");		// シャドウマップテクスチャ
			m_hEyePos = pEffect->GetParameterByName(nullptr, "s_EyePos");					// 視点

			m_hFogColor = pEffect->GetParameterByName(nullptr, "s_FogColor");				// フォグ色
			m_hFogNear = pEffect->GetParameterByName(nullptr, "s_FogNear");					// フォグ開始距離
			m_hFogFar = pEffect->GetParameterByName(nullptr, "s_FogFar");					// フォグ終了距離


			// トゥーンマップテクスチャを設定
			SetToonMapTexture(TEXTURE_TOON);
		}
		else
		{ // 読込に失敗した場合
			if (pError)
			{
				OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
				//デバッグコンソールに表示する
				MessageBoxA(NULL, (LPCSTR)pError->GetBufferPointer(), "Shader Error", MB_OK);
			}
			// 読み込み失敗を返す
			assert(false);
			return E_FAIL;
		}
	}
	else
	{ // バージョンが使用不可な場合

		// 古いバージョンによる失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CToonShadow::Uninit(void)
{
	// シェーダーの終了
	CShader::Uninit();
}

//============================================================
//	テクスチャの設定処理 (ポインタ)
//============================================================
void CToonShadow::SetTexture(const CTexture::STexture& rTexture)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// エフェクトにテクスチャの使用状況を設定
	if (rTexture.pTexture == nullptr)
	{ // テクスチャが使用されていない場合

		
	}
	else
	{ // テクスチャが使用されている場合


	}
}

//============================================================
//	テクスチャの設定処理 (インデックス)
//============================================================
void CToonShadow::SetTexture(const int nTextureID)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// エフェクトにテクスチャの使用状況を設定
	if (nTextureID == NONE_IDX)
	{ // テクスチャが使用されていない場合

		
	}
	else if (nTextureID > NONE_IDX)
	{ // テクスチャが使用されている場合

	
	}
	else { assert(false); }	// 使用不可インデックス
}

//============================================================
//	マテリアルの設定処理
//============================================================
void CToonShadow::SetMaterial(const D3DMATERIAL9& rMaterial)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// ポインタを宣言
	LPD3DXEFFECT pEffect = GetEffect();	// エフェクト情報

	// 変数を宣言
	D3DXVECTOR4 diffuse = D3DXVECTOR4(rMaterial.Diffuse.r, rMaterial.Diffuse.g, rMaterial.Diffuse.b, rMaterial.Diffuse.a);	// 拡散光
	D3DXVECTOR4 ambient = D3DXVECTOR4(rMaterial.Ambient.r, rMaterial.Ambient.g, rMaterial.Ambient.b, rMaterial.Ambient.a);	// 環境光
	D3DXVECTOR4 emissive = D3DXVECTOR4(rMaterial.Emissive.r, rMaterial.Emissive.g, rMaterial.Emissive.b, rMaterial.Emissive.a);	// 放射光

	// エフェクトにマテリアルの拡散光・環境光・放射光を設定
	pEffect->SetVector(m_hDiffuse, &diffuse);
	pEffect->SetVector(m_hAmbient, &ambient);
	pEffect->SetVector(m_hEmissive, &emissive);
}

//============================================================
//	拡散光の設定処理
//============================================================
void CToonShadow::SetDiffuse(const D3DXCOLOR& rDiffuse)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// 変数を宣言
	D3DXVECTOR4 diffuse = D3DXVECTOR4(rDiffuse.r, rDiffuse.g, rDiffuse.b, rDiffuse.a);	// 拡散光

	// エフェクトに拡散光を設定
	GetEffect()->SetVector(m_hDiffuse, &diffuse);
}

//============================================================
//	環境光の設定処理
//============================================================
void CToonShadow::SetAmbient(const D3DXCOLOR& rAmbient)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// 変数を宣言
	D3DXVECTOR4 ambient = D3DXVECTOR4(rAmbient.r, rAmbient.g, rAmbient.b, rAmbient.a);	// 環境光

	// エフェクトに環境光を設定
	GetEffect()->SetVector(m_hAmbient, &ambient);
}

//============================================================
//	放射光の設定処理
//============================================================
void CToonShadow::SetEmissive(const D3DXCOLOR& rEmissive)
{
	if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用

	// 変数を宣言
	D3DXVECTOR4 emissive = D3DXVECTOR4(rEmissive.r, rEmissive.g, rEmissive.b, rEmissive.a);	// 放射光

	// エフェクトに放射光を設定
	GetEffect()->SetVector(m_hEmissive, &emissive);
}

//============================================================
//	拡散光のみ設定処理
//============================================================
void CToonShadow::SetOnlyDiffuse(const D3DXCOLOR& rDiffuse)
{
	// 拡散光の設定
	SetDiffuse(rDiffuse);

	// 環境光の設定
	SetAmbient(XCOL_ABLACK);

	// 放射光の設定
	SetEmissive(XCOL_ABLACK);
}

//============================================================
//	フォグ設定処理
//============================================================
void CToonShadow::SetFog(const D3DXCOLOR& rColor, const float& rNear, const float& rFar)
{
	// フォグ色の設定
	D3DXVECTOR4 Color = D3DXVECTOR4(rColor.r, rColor.g, rColor.b, rColor.a);
	GetEffect()->SetVector(m_hFogColor, &Color);
	// 開始距離
	GetEffect()->SetFloat(m_hFogNear, rNear);
	//終了距離
	GetEffect()->SetFloat(m_hFogFar, rFar);
}
//============================================================
//	フォグ色設定処理
//============================================================
void CToonShadow::SetFogColor(const D3DXCOLOR& rColor)
{
	// フォグ色の設定
	D3DXVECTOR4 Color = D3DXVECTOR4(rColor.r, rColor.g, rColor.b, rColor.a);
	GetEffect()->SetVector(m_hFogColor, &Color);
}
//============================================================
//	フォグ開始距離設定処理
//============================================================
void CToonShadow::SetFogNear(const float& rNear)
{
	// 開始距離
	GetEffect()->SetFloat(m_hFogNear, rNear);
}
//============================================================
//	フォグ終了距離設定処理
//============================================================
void CToonShadow::SetFogFar(const float& rFar)
{
	// 開始距離
	GetEffect()->SetFloat(m_hFogFar, rFar);
}
//============================================================
//	シャドウマッピング設定処理
//============================================================
void CToonShadow::SetShadowMap()
{
	CZTexture* pZtex = CZTexture::GetInstance();
	D3DXVECTOR3 vec = FOCUS_POINT - VIEW_POINT;
	D3DXVec3Normalize(&vec, &vec);
	vec = useful::VectorToAngles(vec);
	D3DXVECTOR4 vec4 = {};
	vec4.x = vec.x;
	vec4.y = vec.y;
	vec4.z = vec.z;
	D3DXMATRIX mtxView, mtxProj;
	mtxView = pZtex->GetViewMtx();
	mtxProj = pZtex->GetProjMtx();
	GetEffect()->SetMatrix(m_hLightViewMat,&mtxView);
	GetEffect()->SetMatrix(m_hLightProjMat, &mtxProj);
	GetEffect()->SetVector(m_hDirectLight, &vec4);
	IDirect3DTexture9* pTexture;
	pZtex->GetZTex(&pTexture);
	GetEffect()->SetTexture(m_hShadowMapTex, pTexture);
	D3DXVECTOR3 camera = GET_MANAGER->GetCamera()->GetPositionV();
	D3DXVECTOR4 camera_vec4 = {};
	camera_vec4.x = camera.x;
	camera_vec4.y = camera.y;
	camera_vec4.z = camera.z;
	GetEffect()->SetVector(m_hEyePos, &camera_vec4);
}
//============================================================
//	生成処理
//============================================================
CToonShadow* CToonShadow::Create(void)
{
	// インスタンス使用中
	assert(m_pShader == nullptr);

	// トゥーンシェーダーの生成
	m_pShader = new CToonShadow;
	if (m_pShader == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// トゥーンシェーダーの初期化
		if (FAILED(m_pShader->Init()))
		{ // 初期化に失敗した場合

			// トゥーンシェーダーの破棄
			SAFE_DELETE(m_pShader);
			return nullptr;
		}

		// 確保したアドレスを返す
		return m_pShader;
	}
}

//============================================================
//	取得処理
//============================================================
CToonShadow* CToonShadow::GetInstance(void)
{
	if (m_pShader == nullptr)
	{
		m_pShader = CToonShadow::Create();
	}

	// ポインタを返す
	return m_pShader;
}

//============================================================
//	破棄処理
//============================================================
void CToonShadow::Release(void)
{
	// トゥーンシェーダーの終了
	assert(m_pShader != nullptr);
	m_pShader->Uninit();

	// メモリ開放
	SAFE_DELETE(m_pShader);
}

//============================================================
//	トゥーンマップテクスチャの設定処理
//============================================================
void CToonShadow::SetToonMapTexture(const ETexture texture)
{
	if (texture > NONE_IDX && texture < TEXTURE_MAX)
	{ // インデックスが範囲内の場合

		if (!IsEffectOK()) { assert(false); return; }	// エフェクト未使用
		CTexture* pTexture = GET_MANAGER->GetTexture();	// テクスチャ情報

		// エフェクトにトゥーンマップ用テクスチャを設定
		GetEffect()->SetTexture(m_hTextureToon, pTexture->GetPtr(pTexture->Regist(TEXTURE_FILE[texture])));
	}
	else { assert(false); }	// 範囲外
}
