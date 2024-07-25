//============================================================
//
//	�t�F�[�h�I�u�W�F�N�g2D���� [fade2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fade2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// �t�F�[�h�I�u�W�F�N�g2D�\���̗D�揇��
}

//************************************************************
//	�e�N���X [CFade2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFade2D::CFade2D() : CObject2D(CObject::LABEL_UI, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY),
	m_state		(STATE_NONE),	// ���
	m_fSubIn	(0.0f),			// �C���̃��l������
	m_fAddOut	(0.0f),			// �A�E�g�̃��l������
	m_fWaitTime	(0.0f),			// ���݂̗]�C����
	m_fMaxWait	(0.0f)			// �]�C����
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CFade2D::~CFade2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CFade2D::Init(void)
{
	// �����o�ϐ���������
	m_state		= STATE_NONE;	// ���
	m_fSubIn	= 0.0f;			// �C���̃��l������
	m_fAddOut	= 0.0f;			// �A�E�g�̃��l������
	m_fWaitTime	= 0.0f;			// ���݂̗]�C����
	m_fMaxWait	= 0.0f;			// �]�C����

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
void CFade2D::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CFade2D::Update(const float fDeltaTime)
{
	// �������Ȃ���Ԃ̏ꍇ������
	if (m_state == STATE_NONE) { return; }

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_FADEOUT:
	{
		D3DXCOLOR colFade = GetColor();	// �F���擾

		// �s�����ɂ��Ă���
		colFade.a += m_fAddOut * fDeltaTime;
		if (colFade.a >= 1.0f)
		{ // �s�����ɂȂ����ꍇ

			// ���l��␳
			colFade.a = 1.0f;

			if (m_fMaxWait <= 0.0f)
			{ // �]�C���Ȃ��ꍇ

				// �\����Ԃɂ���
				m_state = STATE_DISP;
			}
			else
			{ // �]�C������ꍇ

				// �]�C��Ԃɂ���
				m_state = STATE_WAIT;
			}
		}

		SetColor(colFade);	// �F�𔽉f
		break;
	}
	case STATE_DISP:
	{
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
	}
	case STATE_WAIT:
	{
		break;
	}
	case STATE_FADEIN:
	{
		D3DXCOLOR colFade = GetColor();	// �F���擾

		// �����ɂ��Ă���
		colFade.a -= m_fSubIn * fDeltaTime;
		if (colFade.a <= 0.0f)
		{ // �����ɂȂ����ꍇ

			// ���l��␳
			colFade.a = 0.0f;

			// �������Ȃ���Ԃɂ���
			m_state = STATE_NONE;
		}

		SetColor(colFade);	// �F�𔽉f
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
void CFade2D::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CFade2D *CFade2D::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize,	// �傫��
	const float fSubIn,			// �C���̃��l������
	const float fAddOut,		// �A�E�g�̃��l������
	const float fMaxWait,		// �]�C����
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol		// �F
)
{
	// �t�F�[�h�I�u�W�F�N�g2D�̐���
	CFade2D *pFade2D = new CFade2D;
	if (pFade2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �t�F�[�h�I�u�W�F�N�g2D�̏�����
		if (FAILED(pFade2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �t�F�[�h�I�u�W�F�N�g2D�̔j��
			SAFE_DELETE(pFade2D);
			return nullptr;
		}

		// �ʒu��ݒ�
		pFade2D->SetVec3Position(rPos);

		// ������ݒ�
		pFade2D->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pFade2D->SetVec3Sizing(rSize);

		// �F��ݒ�
		pFade2D->SetColor(rCol);

		// �����x��ݒ�
		pFade2D->SetAlpha(0.0f);	// �ŏ��͓�������

		// ���l�����ʂ�ݒ�
		pFade2D->SetSubIn(fSubIn);

		// ���l�����ʂ�ݒ�
		pFade2D->SetAddOut(fAddOut);

		// �]�C���Ԃ�ݒ�
		pFade2D->SetMaxWait(fMaxWait);

		// �m�ۂ����A�h���X��Ԃ�
		return pFade2D;
	}
}

//============================================================
//	�\���ݒ菈��
//============================================================
void CFade2D::SetDisp(void)
{
	// ���ɕ\�����̏ꍇ������
	if (m_state == STATE_DISP) { return; }

	// �J�E���^�[��������
	m_fWaitTime = 0;

	// �t�F�[�h�A�E�g��Ԃɂ���
	m_state = STATE_FADEOUT;
}
