float4x4 matWorld : world;			// ���[���h�ϊ��s��
float4x4 matCameraView : view;		// �J�����r���[�ϊ��s��
float4x4 matCameraProj : projection;	// �ˉe�ϊ��s��
float4   m_Ambient = 0.0f;			//�F
float4 m_EyePos;                    //���_
float3 m_LightVec;                    //�����_

sampler tex0 : register(s0);		// �I�u�W�F�N�g�̃e�N�X�`���[


sampler DefSampler = sampler_state	// �T���v���[�X�e�[�g
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
	float4 Pos : POSITION;  			 // �ˉe�ϊ����W
	float2 Tex       : TEXCOORD0;
	float4 PosWVP : TEXCOORD1;
	float4 Col : COLOR0;				// �f�B�t���[�Y�F
};


// ���_�V�F�[�_
VS_OUTPUT Draw_VS(float4 Pos : POSITION, float3 Norm : NORMAL, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// ���ʂɃJ�����̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	float4x4 mat;
	mat = mul(matWorld, matCameraView);
	mat = mul(mat, matCameraProj);
	Out.Pos = mul(Pos, mat);
	Out.Tex = Tex;

	// �@���ƃ��C�g�̕������璸�_�̐F������
	//  �Z���Ȃ肷���Ȃ��悤�ɒ��߂��Ă��܂�
	float3 N = normalize(mul(Norm, matWorld));

	/*  float3 L = -LightDirect.xyz;
	  float3 N = normalize(Norm.xyz);*/

	Out.Col = max(m_Ambient, (dot(N, -m_LightVec) * 0.5f));
	Out.Col = float4(0.9f, 0.9f, 1.0f, 0.6) * (0.4 + dot(N, -m_LightVec) * (1 - 0.4f));
	Out.Col.a = 1.0f;
	Out.PosWVP = mul(Pos, matWorld);

	return Out;
}


// �s�N�Z���V�F�[�_
float4 Draw_PS(float4 Col : COLOR, float4 ZCalcTex : TEXCOORD1, VS_OUTPUT In) : COLOR
{
	// ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o

	  return Col;
}


// �e�N�j�b�N
technique DrawTec
{
	pass P0
	{
		VertexShader = compile vs_3_0 Draw_VS();
		PixelShader = compile ps_3_0 Draw_PS();
	}
}
