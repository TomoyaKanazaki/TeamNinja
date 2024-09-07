#if 0
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
CBlink2D::CBlink2D() : CObject2D(CObject::LABEL_UI, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY),
	m_state		 (STATE_NONE),	// ���
	m_fSinAlpha	 (0.0f),		// ��������
	m_fAddSinRot (0.0f),		// ���������̉��Z��
	m_fMinAlpha	 (0.0f),		// �Œᓧ���x
	m_fMaxAlpha	 (0.0f),		// �ő哧���x
	m_fSubIn	 (0.0f),		// �C���̃��l������
	m_fAddOut	 (0.0f)			// �A�E�g�̃��l������
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
	m_state		 = STATE_NONE;	// ���
	m_fSinAlpha	 = HALF_PI;		// ��������
	m_fAddSinRot = 0.0f;		// ���������̉��Z��
	m_fMinAlpha	 = 0.0f;		// �Œᓧ���x
	m_fMaxAlpha	 = 0.0f;		// �ő哧���x
	m_fSubIn	 = 0.0f;		// �C���̃��l������
	m_fAddOut	 = 0.0f;		// �A�E�g�̃��l������

	// �I�u�W�F�N�g2D�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

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

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_FADEOUT:
	{
		D3DXCOLOR colBlink = GetColor();	// �F���擾

		// �Œ���̓����x�ɂ��Ă���
		colBlink.a += m_fAddOut * fDeltaTime;
		if (colBlink.a >= m_fMinAlpha)
		{ // �Œ���̓����x�ɂȂ����ꍇ

			// ���l��␳
			colBlink.a = m_fMinAlpha;

			// �_�ŏ�Ԃɂ���
			m_state = STATE_BLINK;
		}

		SetColor(colBlink);	// �F�𔽉f
		break;
	}
	case STATE_BLINK:
	{
		// �T�C���J�[�u��������]
		m_fSinAlpha += m_fAddSinRot * fDeltaTime;
		useful::NormalizeRot(m_fSinAlpha);	// �������K��

		// �����x���Z�ʂ��v�Z
		float fAddAlpha = ((m_fMaxAlpha - m_fMinAlpha) * 0.5f) * (sinf(m_fSinAlpha) - 1.0f) * -1.0f;

		// �����x��ݒ�
		SetAlpha(m_fMinAlpha + fAddAlpha);
		break;
	}
	case STATE_FADEIN:
	{
		D3DXCOLOR colBlink = GetColor();	// �F���擾

		// �����ɂ��Ă���
		colBlink.a -= m_fSubIn * fDeltaTime;
		if (colBlink.a <= 0.0f)
		{ // �����ɂȂ����ꍇ

			// ���l��␳
			colBlink.a = 0.0f;

			// �������Ȃ���Ԃɂ���
			m_state = STATE_NONE;
		}

		SetColor(colBlink);	// �F�𔽉f
		break;
	}
	default:
		assert(false);
		break;
	}

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
	const float fMinAlpha,		// �Œᓧ���x
	const float fMaxAlpha,		// �ő哧���x
	const float fCalcAlpha,		// �����x�̉�����
	const float fSubIn,			// �C���̃��l������
	const float fAddOut,		// �A�E�g�̃��l������
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol		// �F
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

		// �F��ݒ�
		pBlink2D->SetColor(rCol);

		// �����x��ݒ�
		pBlink2D->SetAlpha(0.0f);	// �ŏ��͓�������

		// �Œᓧ���x��ݒ�
		pBlink2D->SetMinAlpha(fMinAlpha);

		// �ő哧���x��ݒ�
		pBlink2D->SetMaxAlpha(fMaxAlpha);

		// ���������̉��Z�ʂ�ݒ�
		pBlink2D->SetCalcAlpha(fCalcAlpha);

		// ���l�����ʂ�ݒ�
		pBlink2D->SetSubIn(fSubIn);

		// ���l�����ʂ�ݒ�
		pBlink2D->SetAddOut(fAddOut);

		// �m�ۂ����A�h���X��Ԃ�
		return pBlink2D;
	}
}

//============================================================
//	�_�Őݒ菈��
//============================================================
void CBlink2D::SetBlink(const bool bBlink)
{
	// ����������������
	m_fSinAlpha = HALF_PI;

	if (bBlink)
	{ // �_�ł��J�n����ꍇ

		// �t�F�[�h�A�E�g��Ԃɂ���
		m_state = STATE_FADEOUT;
	}
	else
	{ // �_�ł��I������ꍇ

		// �t�F�[�h�C����Ԃɂ���
		m_state = STATE_FADEIN;
	}
}
#endif
