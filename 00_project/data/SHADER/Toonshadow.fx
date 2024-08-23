//============================================================
//
//	�g�D�[���V�F�[�_�[+�V���h�E�}�b�s���O�̃G�t�F�N�g�t�@�C�� [Toonshadow.fx]
//	Author�F�O�엳�V��
//
//============================================================
//************************************************************
//	�O���[�o���ϐ��錾
//************************************************************
float4x4 s_mtxWorld	: WORLD;		// ���[���h�}�g���b�N�X
float4x4 s_mtxView	: VIEW;			// �r���[�}�g���b�N�X
float4x4 s_mtxProj	: PROJECTION;	// �v���W�F�N�V�����}�g���b�N�X
texture  s_textureToon;				// �g�D�[���}�b�v�e�N�X�`��

float3	 s_dirLight = float3(0.0f, 0.0f, 0.0f);			// ���s�����̕����x�N�g��
float4x4 s_mtxLightView;								// ���C�g�r���[�ϊ��s��
float4x4 s_mtxLightProj;								// �ˉe�ϊ��s��
float4   s_LightColor = float4(0.5f, 0.7f, 0.8f, 1.0f);		// ���C�g�F


float4	s_diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);		// �g�U��
float4	s_ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);		// ����
float4	s_emissive = float4(0.0f, 0.0f, 0.0f, 0.0f);	// ���ˌ�

float4 s_EyePos;										// ���_
float4 s_FogColor = float4(0.2f, 0.3f, 0.3f, 1.0f);	// �t�H�O�F
float  s_FogNear = 500.0f;								// �t�H�O�̊J�n�ʒu
float  s_FogFar = 10000.0f;								// �t�H�O�̏I���ʒu
float4 s_AltiFogColor = float4(1.0f, 1.0f, 1.0f, 0.6f);	// �����t�H�O�F
float  s_AltiFogStart = 60.0f;						// �����t�H�O�J�n�ʒu
float  s_AltiFogEnd	= -500.0f;								// �����t�H�O�I���ʒu

float	s_fRefEdge = 1.0f;								// �G�b�W�����Q�ƒl

//************************************************************
//	�T���v���[�錾
//************************************************************
sampler texObject : register(s0);	// �I�u�W�F�N�g�e�N�X�`��
sampler texToon =	// �g�D�[���}�b�v�e�N�X�`��
sampler_state		// �T���v���[�X�e�[�g
{
	// �Ώۃe�N�X�`��
	Texture = <s_textureToon>;	// �g�D�[���}�b�v

	// �g�k�E�~�b�v�}�b�v
	Filter = MIN_MAG_MIP_LINEAR;	// ���`�⊮

	// �e�N�X�`���A�h���b�V���O���[�h
	AddressU = Clamp;	// U�����̒[���������΂�
	AddressV = Clamp;	// V�����̒[���������΂�
};

texture s_texShadowMap;					// �V���h�E�}�b�v�e�N�X�`��
sampler ShadowSampler = sampler_state	// �T���v���[�X�e�[�g
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
//	�\���̒�`
//************************************************************
// ���_�V�F�[�_�[�o�͏��
struct VS_OUTPUT
{
	float4 pos		: POSITION;		// ���_���W
	float2 tex		: TEXCOORD0;	// �e�N�Z�����W
	float4 ZCalcTex : TEXCOORD1;	// Z�l�Z�o�p�e�N�X�`��
	float4 PosWVP	: TEXCOORD2;	// ���_���W(�t���O�����g�V�F�[�_�ł�POSITION���g�p�ł��Ȃ����ߑ�p����)
	float3 nor		: TEXCOORD3;	// �@��
	float4 Col		: COLOR0;		// �o�͐F
};

//************************************************************
//	�O���[�o���֐�
//************************************************************
//============================================================
//	���_���W�̎ˉe�ϊ�
//============================================================
float4 TransVertex
(
	in float4	inPos		: POSITION,		// ���_���W
	in float4x4	inMtxWorld	: WORLD,			// ���[���h�}�g���b�N�X
	in float4x4	inMtxView	: VIEW,			// �r���[�}�g���b�N�X
	in float4x4	inMtxProj	: PROJECTION		// �v���W�F�N�V�����}�g���b�N�X
)
{
	// ���_���W���ˉe�ϊ�
	inPos = mul(inPos, inMtxWorld);
	inPos = mul(inPos, inMtxView);
	inPos = mul(inPos, inMtxProj);

	// �ϊ���̒��_���W��Ԃ�
	return inPos;
}

//============================================================
//	���_�V�F�[�_�[
//============================================================
VS_OUTPUT VS
(
	in	float4		inPos	: POSITION,		// ���_���W
	in	float3		inNor : NORMAL,		// �@���x�N�g��
	in	float2		inTex : TEXCOORD0	// �e�N�Z�����W
)
{
	VS_OUTPUT outVertex = (VS_OUTPUT)0;		// ���_���


	// ���ʂɃJ�����̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	float4x4 mat;
	mat = mul(s_mtxWorld, s_mtxView);
	mat = mul(mat, s_mtxProj);
	outVertex.pos = mul(inPos, mat);
	outVertex.PosWVP = mul(inPos, s_mtxWorld);
	outVertex.tex = inTex;
	// ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	mat = mul(s_mtxWorld, s_mtxLightView);
	mat = mul(mat, s_mtxLightProj);
	outVertex.ZCalcTex = mul(inPos, mat);
	// �e�N�Z�����W��ݒ�
	outVertex.tex = inTex;
	outVertex.nor = inNor;
	
	return outVertex;
}

//============================================================
//	�s�N�Z���V�F�[�_�[
//============================================================
void PS
(
	in	VS_OUTPUT	inVertex,			// ���_���
	float4 ZCalcTex : TEXCOORD1,		// �[�x���
	out	float4		outCol : COLOR0,	// �s�N�Z���F
	out	float4		outRef : COLOR1		// �s�N�Z�������Q�ƒl
)
{
	// �ϐ���錾
	float  fLight = 0.0f;	// ���C�e�B���O����
	float4 toonCol = float4(0.0f, 0.0f, 0.0f, 0.0f);	// �g�D�[���}�b�v�e�N�Z���F
	float4 Col = float4(0.0f, 0.0f, 0.0f, 0.0f);



	//===============================
	//		�g�D�[���}�b�s���O
	//===============================
	// �ʏ탉�C�e�B���O�̌��ʂ����߂�
	fLight = dot(normalize(inVertex.nor), normalize(-s_dirLight));
	fLight = fLight * 0.5f + 0.5f;
	fLight = fLight * fLight;

	// ���C�e�B���O�̌��ʂ��g�D�[���}�b�v�̃e�N�Z��U�����ɕϊ�
	toonCol = tex2D(texToon, float2(fLight, 0.5f));

	// �s�N�Z���̐F����ݒ�
	outCol = s_diffuse;		// �g�U����ݒ�
	outCol += s_ambient;		// ���������Z
	outCol += s_emissive;	// ���ˌ������Z
	outCol *= toonCol;		// �g�D�[���}�b�v�e�N�Z���F����Z
	outCol *= s_LightColor;	// ���C�g�F����Z


	if ((tex2D(texObject, inVertex.tex).r + tex2D(texObject, inVertex.tex).g + tex2D(texObject, inVertex.tex).b) != 0.0f)
	{ // �e�N�X�`�����w�肳��Ă���ꍇ

		// �e�N�Z���̐F����Z
		outCol *= tex2D(texObject, inVertex.tex);
	}

	// �s�N�Z�������x���g�U���̂��̂ɂ���
	outCol.a = s_diffuse.a;

	//===============================
	//		�V���h�E�}�b�s���O
	//===============================
	// ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
	float ZValue = ZCalcTex.z / ZCalcTex.w;

	// �e�N�X�`�����W�ɕϊ�
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + ZCalcTex.x / ZCalcTex.w) * 0.5f;
	TransTexCoord.y = (1.0f - ZCalcTex.y / ZCalcTex.w) * 0.5f;
	if (TransTexCoord.x <= 1.0f && TransTexCoord.x >= 0.0f && TransTexCoord.y <= 1.0f && TransTexCoord.y >= 0.0f)
	{
		float4 TexCol = tex2D(ShadowSampler, TransTexCoord);
		// �������W��Z�l�𒊏o
		float SM_Z = (TexCol.x + (TexCol.y + (TexCol.z / 256.0f) / 256.0f) / 256.0f);

		// �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
		if (ZValue > SM_Z + 0.0001f) {
			outCol.rgb = outCol.rgb * 0.8f;
		}
	}


	//===============================
	//			�����t�H�O
	//===============================
	float Af = (s_AltiFogStart - inVertex.PosWVP.y) / (s_AltiFogStart - s_AltiFogEnd); //�t�H�O�̓K�����������Ƃ߂�
	Af = clamp(Af, 0.0f, 1.0f);
	//Af *= -1.f;
	Af*= s_AltiFogColor.a;
	outCol.rgb = outCol.rgb * (1.0f-Af)+s_AltiFogColor.rgb * ( Af);
	//===============================
	//			�t�H�O
	//===============================
	float d = distance(inVertex.PosWVP.xyz, s_EyePos.xyz);
	float f = (s_FogFar - d) / (s_FogFar - s_FogNear); //�t�H�O�̓K�����������Ƃ߂�
	f = clamp(f, 0.0f, 1.0f);
	f *= s_FogColor.a;
	outCol.rgb = outCol.rgb * (f ) + s_FogColor.rgb * (1.0f - f);

	// �s�N�Z���̉����Q�ƒl��ݒ�
	outRef = float4(s_fRefEdge, s_fRefEdge, s_fRefEdge, 1.0f);
}

//============================================================
//	�e�N�j�b�N�֐�
//============================================================
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}
