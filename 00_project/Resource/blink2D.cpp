//============================================================
//
//	�_�ŃI�u�W�F�N�g2D���� [blink2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "blink2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// �_�ŃI�u�W�F�N�g2D�\���̗D�揇��
}

//************************************************************
//	�e�N���X [CBlink2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBlink2D::CBlink2D() : CObject2D(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
	m_state		(STATE_NONE),	// ���
	m_fWaitTime	(0.0f),			// ���݂̗]�C����
	m_fMaxWait	(0.0f)			// �]�C����
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CBlink2D::~CBlink2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBlink2D::Init(void)
{
	// �����o�ϐ���������
	m_state		= STATE_NONE;	// ���
	m_fWaitTime	= 0.0f;			// ���݂̗]�C����
	m_fMaxWait	= 0.0f;			// �]�C����

	// �I�u�W�F�N�g2D�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����F��ݒ�
	SetColor(XCOL_AWHITE);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CBlink2D::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CBlink2D::Update(const float fDeltaTime)
{
	// �������Ȃ���Ԃ̏ꍇ������
	if (m_state == STATE_NONE) { return; }

	// �F���擾
	D3DXCOLOR colFade = GetColor();

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_FADEOUT:

		// �s�����ɂ��Ă���
		colFade.a += m_fAddOut * fDeltaTime;
		if (colFade.a >= 1.0f)
		{ // �s�����ɂȂ����ꍇ

			// ���l��␳
			colFade.a = 1.0f;

			// �\����Ԃɂ���
			m_state = STATE_DISP;
		}
		break;

	case STATE_DISP:

		// �]�C���Ԃ����Z
		m_fWaitTime += fDeltaTime;
		if (m_fWaitTime >= m_fMaxWait)
		{ // �]�C�I�������ꍇ

			// �]�C���Ԃ�������
			m_fWaitTime = 0.0f;

			// �t�F�[�h�C����Ԃɂ���
			m_state = STATE_FADEIN;
		}
		break;

	case STATE_FADEIN:

		// �����ɂ��Ă���
		colFade.a -= m_fSubIn * fDeltaTime;
		if (colFade.a <= 0.0f)
		{ // �����ɂȂ����ꍇ

			// ���l��␳
			colFade.a = 0.0f;

			// �������Ȃ���Ԃɂ���
			m_state = STATE_NONE;
		}
		break;

	default:
		assert(false);
		break;
	}

	// �F�𔽉f
	SetColor(colFade);

	// �I�u�W�F�N�g2D�̍X�V
	CObject2D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CBlink2D::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CBlink2D *CBlink2D::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize,	// �傫��
	const float fSubIn,			// �C���̃��l������
	const float fAddOut,		// �A�E�g�̃��l������
	const float fMaxWait,		// �]�C����
	const D3DXVECTOR3& rRot		// ����
)
{
	// �_�ŃI�u�W�F�N�g2D�̐���
	CBlink2D *pBlink2D = new CBlink2D;
	if (pBlink2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �_�ŃI�u�W�F�N�g2D�̏�����
		if (FAILED(pBlink2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �_�ŃI�u�W�F�N�g2D�̔j��
			SAFE_DELETE(pBlink2D);
			return nullptr;
		}

		// �ʒu��ݒ�
		pBlink2D->SetVec3Position(rPos);

		// ������ݒ�
		pBlink2D->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pBlink2D->SetVec3Sizing(rSize);

		// �m�ۂ����A�h���X��Ԃ�
		return pBlink2D;
	}
}

//============================================================
//	�\���ݒ菈��
//============================================================
void CBlink2D::SetDisp(void)
{
	// ���ɕ\�����̏ꍇ������
	if (m_state == STATE_DISP) { return; }

	// �J�E���^�[��������
	m_fWaitTime = 0;

	// �t�F�[�h�A�E�g��Ԃɂ���
	m_state = STATE_FADEOUT;
}
