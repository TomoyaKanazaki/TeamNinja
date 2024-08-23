//============================================================
//
//	トゥーンシェーダー+シャドウマッピングのエフェクトファイル [Toonshadow.fx]
//	Author：丹野竜之介
//
//============================================================
//************************************************************
//	グローバル変数宣言
//************************************************************
float4x4 s_mtxWorld	: WORLD;		// ワールドマトリックス
float4x4 s_mtxView	: VIEW;			// ビューマトリックス
float4x4 s_mtxProj	: PROJECTION;	// プロジェクションマトリックス
texture  s_textureToon;				// トゥーンマップテクスチャ

float3	 s_dirLight = float3(0.0f, 0.0f, 0.0f);			// 平行光源の方向ベクトル
float4x4 s_mtxLightView;								// ライトビュー変換行列
float4x4 s_mtxLightProj;								// 射影変換行列
float4   s_LightColor = float4(0.5f, 0.7f, 0.8f, 1.0f);		// ライト色


float4	s_diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);		// 拡散光
float4	s_ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);		// 環境光
float4	s_emissive = float4(0.0f, 0.0f, 0.0f, 0.0f);	// 放射光

float4 s_EyePos;										// 視点
float4 s_FogColor = float4(0.2f, 0.3f, 0.3f, 1.0f);	// フォグ色
float  s_FogNear = 500.0f;								// フォグの開始位置
float  s_FogFar = 10000.0f;								// フォグの終了位置
float4 s_AltiFogColor = float4(1.0f, 1.0f, 1.0f, 0.6f);	// 高さフォグ色
float  s_AltiFogStart = 60.0f;						// 高さフォグ開始位置
float  s_AltiFogEnd	= -500.0f;								// 高さフォグ終了位置

float	s_fRefEdge = 1.0f;								// エッジ生成参照値

//************************************************************
//	サンプラー宣言
//************************************************************
sampler texObject : register(s0);	// オブジェクトテクスチャ
sampler texToon =	// トゥーンマップテクスチャ
sampler_state		// サンプラーステート
{
	// 対象テクスチャ
	Texture = <s_textureToon>;	// トゥーンマップ

	// 拡縮・ミップマップ
	Filter = MIN_MAG_MIP_LINEAR;	// 線形補完

	// テクスチャアドレッシングモード
	AddressU = Clamp;	// U成分の端を引き延ばす
	AddressV = Clamp;	// V成分の端を引き延ばす
};

texture s_texShadowMap;					// シャドウマップテクスチャ
sampler ShadowSampler = sampler_state	// サンプラーステート
{
	texture = (s_texShadowMap);
	AddressU =	CLAMP;
	AddressV =	CLAMP;
	AddressW =	CLAMP;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

//************************************************************
//	構造体定義
//************************************************************
// 頂点シェーダー出力情報
struct VS_OUTPUT
{
	float4 pos		: POSITION;		// 頂点座標
	float2 tex		: TEXCOORD0;	// テクセル座標
	float4 ZCalcTex : TEXCOORD1;	// Z値算出用テクスチャ
	float4 PosWVP	: TEXCOORD2;	// 頂点座標(フラグメントシェーダではPOSITIONが使用できないため代用する)
	float3 nor		: TEXCOORD3;	// 法線
	float4 Col		: COLOR0;		// 出力色
};

//************************************************************
//	グローバル関数
//************************************************************
//============================================================
//	頂点座標の射影変換
//============================================================
float4 TransVertex
(
	in float4	inPos		: POSITION,		// 頂点座標
	in float4x4	inMtxWorld	: WORLD,			// ワールドマトリックス
	in float4x4	inMtxView	: VIEW,			// ビューマトリックス
	in float4x4	inMtxProj	: PROJECTION		// プロジェクションマトリックス
)
{
	// 頂点座標を射影変換
	inPos = mul(inPos, inMtxWorld);
	inPos = mul(inPos, inMtxView);
	inPos = mul(inPos, inMtxProj);

	// 変換後の頂点座標を返す
	return inPos;
}

//============================================================
//	頂点シェーダー
//============================================================
VS_OUTPUT VS
(
	in	float4		inPos	: POSITION,		// 頂点座標
	in	float3		inNor : NORMAL,		// 法線ベクトル
	in	float2		inTex : TEXCOORD0	// テクセル座標
)
{
	VS_OUTPUT outVertex = (VS_OUTPUT)0;		// 頂点情報


	// 普通にカメラの目線によるワールドビュー射影変換をする
	float4x4 mat;
	mat = mul(s_mtxWorld, s_mtxView);
	mat = mul(mat, s_mtxProj);
	outVertex.pos = mul(inPos, mat);
	outVertex.PosWVP = mul(inPos, s_mtxWorld);
	outVertex.tex = inTex;
	// ライトの目線によるワールドビュー射影変換をする
	mat = mul(s_mtxWorld, s_mtxLightView);
	mat = mul(mat, s_mtxLightProj);
	outVertex.ZCalcTex = mul(inPos, mat);
	// テクセル座標を設定
	outVertex.tex = inTex;
	outVertex.nor = inNor;
	
	return outVertex;
}

//============================================================
//	ピクセルシェーダー
//============================================================
void PS
(
	in	VS_OUTPUT	inVertex,			// 頂点情報
	float4 ZCalcTex : TEXCOORD1,		// 深度情報
	out	float4		outCol : COLOR0,	// ピクセル色
	out	float4		outRef : COLOR1		// ピクセル縁取り参照値
)
{
	// 変数を宣言
	float  fLight = 0.0f;	// ライティング光量
	float4 toonCol = float4(0.0f, 0.0f, 0.0f, 0.0f);	// トゥーンマップテクセル色
	float4 Col = float4(0.0f, 0.0f, 0.0f, 0.0f);



	//===============================
	//		トゥーンマッピング
	//===============================
	// 通常ライティングの光量を求める
	fLight = dot(normalize(inVertex.nor), normalize(-s_dirLight));
	fLight = fLight * 0.5f + 0.5f;
	fLight = fLight * fLight;

	// ライティングの光量をトゥーンマップのテクセルU成分に変換
	toonCol = tex2D(texToon, float2(fLight, 0.5f));

	// ピクセルの色情報を設定
	outCol = s_diffuse;		// 拡散光を設定
	outCol += s_ambient;		// 環境光を加算
	outCol += s_emissive;	// 放射光を加算
	outCol *= toonCol;		// トゥーンマップテクセル色を乗算
	outCol *= s_LightColor;	// ライト色を乗算


	if ((tex2D(texObject, inVertex.tex).r + tex2D(texObject, inVertex.tex).g + tex2D(texObject, inVertex.tex).b) != 0.0f)
	{ // テクスチャが指定されている場合

		// テクセルの色を乗算
		outCol *= tex2D(texObject, inVertex.tex);
	}

	// ピクセル透明度を拡散光のものにする
	outCol.a = s_diffuse.a;

	//===============================
	//		シャドウマッピング
	//===============================
	// ライト目線によるZ値の再算出
	float ZValue = ZCalcTex.z / ZCalcTex.w;

	// テクスチャ座標に変換
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + ZCalcTex.x / ZCalcTex.w) * 0.5f;
	TransTexCoord.y = (1.0f - ZCalcTex.y / ZCalcTex.w) * 0.5f;
	if (TransTexCoord.x <= 1.0f && TransTexCoord.x >= 0.0f && TransTexCoord.y <= 1.0f && TransTexCoord.y >= 0.0f)
	{
		float4 TexCol = tex2D(ShadowSampler, TransTexCoord);
		// 同じ座標のZ値を抽出
		float SM_Z = (TexCol.x + (TexCol.y + (TexCol.z / 256.0f) / 256.0f) / 256.0f);

		// 算出点がシャドウマップのZ値よりも大きければ影と判断
		if (ZValue > SM_Z + 0.0001f) {
			outCol.rgb = outCol.rgb * 0.8f;
		}
	}


	//===============================
	//			高さフォグ
	//===============================
	float Af = (s_AltiFogStart - inVertex.PosWVP.y) / (s_AltiFogStart - s_AltiFogEnd); //フォグの適応割合をもとめる
	Af = clamp(Af, 0.0f, 1.0f);
	//Af *= -1.f;
	Af*= s_AltiFogColor.a;
	outCol.rgb = outCol.rgb * (1.0f-Af)+s_AltiFogColor.rgb * ( Af);
	//===============================
	//			フォグ
	//===============================
	float d = distance(inVertex.PosWVP.xyz, s_EyePos.xyz);
	float f = (s_FogFar - d) / (s_FogFar - s_FogNear); //フォグの適応割合をもとめる
	f = clamp(f, 0.0f, 1.0f);
	f *= s_FogColor.a;
	outCol.rgb = outCol.rgb * (f ) + s_FogColor.rgb * (1.0f - f);

	// ピクセルの縁取り参照値を設定
	outRef = float4(s_fRefEdge, s_fRefEdge, s_fRefEdge, 1.0f);
}

//============================================================
//	テクニック関数
//============================================================
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}
