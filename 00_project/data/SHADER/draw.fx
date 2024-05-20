float4x4 matWorld : world;			// ワールド変換行列
float4x4 matCameraView : view;		// カメラビュー変換行列
float4x4 matCameraProj : projection;	// 射影変換行列
float4   m_Ambient = 0.0f;			//色
float4 m_EyePos;                    //視点
float3 m_LightVec;                    //光視点

sampler tex0 : register(s0);		// オブジェクトのテクスチャー


sampler DefSampler = sampler_state	// サンプラーステート
{
	texture = (texShadowMap);
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};


struct VS_OUTPUT
{
	float4 Pos : POSITION;  			 // 射影変換座標
	float2 Tex       : TEXCOORD0;
	float4 PosWVP : TEXCOORD1;
	float4 Col : COLOR0;				// ディフューズ色
};


// 頂点シェーダ
VS_OUTPUT Draw_VS(float4 Pos : POSITION, float3 Norm : NORMAL, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// 普通にカメラの目線によるワールドビュー射影変換をする
	float4x4 mat;
	mat = mul(matWorld, matCameraView);
	mat = mul(mat, matCameraProj);
	Out.Pos = mul(Pos, mat);
	Out.Tex = Tex;

	// 法線とライトの方向から頂点の色を決定
	//  濃くなりすぎないように調節しています
	float3 N = normalize(mul(Norm, matWorld));

	/*  float3 L = -LightDirect.xyz;
	  float3 N = normalize(Norm.xyz);*/

	Out.Col = max(m_Ambient, (dot(N, -m_LightVec) * 0.5f));
	Out.Col = float4(0.9f, 0.9f, 1.0f, 0.6) * (0.4 + dot(N, -m_LightVec) * (1 - 0.4f));
	Out.Col.a = 1.0f;
	Out.PosWVP = mul(Pos, matWorld);

	return Out;
}


// ピクセルシェーダ
float4 Draw_PS(float4 Col : COLOR, float4 ZCalcTex : TEXCOORD1, VS_OUTPUT In) : COLOR
{
	// ライト目線によるZ値の再算出

	  return Col;
}


// テクニック
technique DrawTec
{
	pass P0
	{
		VertexShader = compile vs_3_0 Draw_VS();
		PixelShader = compile ps_3_0 Draw_PS();
	}
}
