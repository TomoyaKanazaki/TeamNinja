sampler2D ShadowMapSampler : register(s0);
float2 TexelSize = 1.0f /  float2(1280, 1280);  // テクセルのサイズ (1.0 / シャドウマップの解像度)
float kernel[5] = { 0.227027f, 0.316216f, 0.070270f, 0.316216f, 0.227027f };

float4 GaussianBlur_H(float2 uv : TEXCOORD0) : COLOR
{
    float4 color = float4(0.0, 0.0, 0.0, 0.0);

    // ガウシアンカーネルに基づいて水平フィルタリング
    for (int i = -2; i <= 2; ++i)
    {
        color += tex2D(ShadowMapSampler, uv + float2(TexelSize.x * i, 0.0)) * kernel[i + 2];
    }

    return color;
}

float4 GaussianBlur_V(float2 uv : TEXCOORD0) : COLOR
{
    float4 color = float4(0.0, 0.0, 0.0, 0.0);

    // ガウシアンカーネルに基づいて垂直フィルタリング
    for (int i = -2; i <= 2; ++i)
    {
        color += tex2D(ShadowMapSampler, uv + float2(0.0, TexelSize.y * i)) * kernel[i + 2];
    }

    return color;
}

technique GaussianBlur
{
    pass P0
    {
        // 水平方向のブラー
        PixelShader = compile ps_2_0 GaussianBlur_H();
    }

    pass P1
    {
        // 垂直方向のブラー
        PixelShader = compile ps_2_0 GaussianBlur_V();
    }
}