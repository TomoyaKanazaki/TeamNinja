//============================================================
//
//	�����o������ [balloon.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "balloon.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 0;			// �����o���̗D�揇��
	const int ALPHA_NUMREF = 180;	// ���e�X�g�̎Q�ƒl

	const D3DXVECTOR3 SIZE = D3DXVECTOR3(320.0f, 180.0f, 0.0f);	// �傫��
}

//************************************************************
//	�q�N���X [CBalloon] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBalloon::CBalloon() : CObjectBillboard(CObject::LABEL_UI),
	m_fScale	 (0.0f),	// �g�嗦
	m_fDestScale (0.0f)		// �ڕW�g�嗦
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CBalloon::~CBalloon()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBalloon::Init(void)
{
	// �����o�ϐ���������
	m_fScale	 = 0.0f;	// �g�嗦
	m_fDestScale = 0.0f;	// �ڕW�g�嗦

	// �I�u�W�F�N�g�r���{�[�h�̏�����
	if (FAILED(CObjectBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �ʒu��ݒ�
	SetVec3Position(D3DXVECTOR3(0.0f, 240.0f, 0.0f));

	// �傫����ݒ�
	SetVec3Sizing(SIZE);

	// �e�N�X�`��������
	BindTexture("data\\TEXTURE\\balloon000.png");

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaTest(true);			// ���e�X�g�̗L�� / �����̐ݒ�
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaNumRef(ALPHA_NUMREF);	// ���e�X�g�̎Q�ƒl�ݒ�

	// ��΂ɕ`�悪��������悤�ɂ���
	pRenderState->SetZFunc(D3DCMP_ALWAYS);

	// ���C�e�B���O��OFF�ɂ���
	pRenderState->SetLighting(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CBalloon::Uninit(void)
{
	// �I�u�W�F�N�g�r���{�[�h�̏I��
	CObjectBillboard::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CBalloon::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g�r���{�[�h�̍X�V
	CObjectBillboard::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CBalloon::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g�r���{�[�h�̕`��
	CObjectBillboard::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CBalloon *CBalloon::Create(void)
{
	// �����o���̐���
	CBalloon *pBalloon = new CBalloon;
	if (pBalloon == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �����o���̏�����
		if (FAILED(pBalloon->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �����o���̔j��
			SAFE_DELETE(pBalloon);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pBalloon;
	}
}
