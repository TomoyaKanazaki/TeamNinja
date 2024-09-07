sampler2D InputTexture : register(s0);

struct VS_OUTPUT
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
};

float2 StartUV; 
float2 EndUV; 

float4 PS_Main(VS_OUTPUT input) : COLOR
{
    // UV座標の変換
    float2 scaledUV = lerp(StartUV, EndUV, input.TexCoord);

    // テクスチャの特定領域からサンプリング
    return tex2D(InputTexture, scaledUV);
}

//============================================================
//	テクニック関数
//============================================================
technique Comp
{
    pass P0
    {
        PixelShader = compile ps_2_0 PS_Main();
    }
}
