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
    // UV���W�̕ϊ�
    float2 scaledUV = lerp(StartUV, EndUV, input.TexCoord);

    // �e�N�X�`���̓���̈悩��T���v�����O
    return tex2D(InputTexture, scaledUV);
}

//============================================================
//	�e�N�j�b�N�֐�
//============================================================
technique Comp
{
    pass P0
    {
        PixelShader = compile ps_2_0 PS_Main();
    }
}
