//============================================================
//
//	Zテクスチャ生成 [ZTexCreator.fx]
//	Author：丹野竜之介
//
//============================================================
float4x4 matWorld : world;		// ワールド変換行列
float4x4 matView : view;		// ビュー変換行列
float4x4 matProj : projection;		// 射影変換行列

float NearClip = 5000.0f;
float FarClip = 10000.0f;

struct VS_OUTPUT
{
   float4 Pos : POSITION;   // 射影変換座標
   float4 ShadowMapTex : TEXCOORD0;   // Zバッファテクスチャ
};
float CalculateLogDepth(float z, float C, float B)
{
    return log(C * (z + B));
}
// 頂点シェーダ
VS_OUTPUT ZBufferCalc_VS( float4 Pos : POSITION )
{
   VS_OUTPUT Out = (VS_OUTPUT)0;


   // ワールドビュー行列を計算
   float4x4 mat = mul(matWorld, matView);

   // 射影変換
   Out.Pos = mul(Pos, mul(mat, matProj));

   // ビュー空間での深度計算
   float4 viewSpacePos = mul(Pos, mat);
   Out.ShadowMapTex = viewSpacePos;

   return Out;
}


// ピクセルシェーダ
float4 ZBufferPlot_PS( float4 ShadowMapTex : TEXCOORD0 ) : COLOR
{
    // ビュー空間のz値からリニア深度を計算
   // float linearDepth = (ShadowMapTex.z - NearClip) / (FarClip - NearClip);
     float linearDepth = ShadowMapTex.z / ShadowMapTex.w;

    // 高精度な深度パッキング (32ビットのカラーとして深度を格納)
    float4 packedDepth;
    packedDepth.x = linearDepth;
    packedDepth.y = linearDepth * linearDepth;
    packedDepth.z = linearDepth * linearDepth * linearDepth;
    packedDepth.w = linearDepth * linearDepth * linearDepth * linearDepth;
    
    return packedDepth;
}

technique ZValuePlotTec
{
   pass P0
   {
      VertexShader = compile vs_2_0 ZBufferCalc_VS();
      PixelShader = compile ps_2_0 ZBufferPlot_PS();
   }
}
