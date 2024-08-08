//============================================================
//
//	�g�k�I�u�W�F�N�g2D���� [scale2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scale2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// �g�k�I�u�W�F�N�g2D�\���̗D�揇��
}

//************************************************************
//	�e�N���X [CScale2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScale2D::CScale2D() : CObject2D(CObject::LABEL_UI, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY),
	m_sizeOrigin (VEC3_ZERO),	// ���_�傫��
	m_state		 (STATE_NONE),	// ���
	m_fSinScale	 (0.0f),		// �g�k����
	m_fAddSinRot (0.0f),		// �g�k�����̉��Z��
	m_fCurScale	 (0.0f),		// ���݊g�嗦
	m_fMinScale	 (0.0f),		// �Œ�g�嗦
	m_fMaxScale	 (0.0f),		// �ő�g�嗦
	m_fInitScale (0.0f),		// �����g�嗦
	m_fSubIn	 (0.0f),		// �C���̊g�嗦������
	m_fAddOut	 (0.0f)			// �A�E�g�̊g�嗦������
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CScale2D::~CScale2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScale2D::Init(void)
{
	// �����o�ϐ���������
	m_sizeOrigin = VEC3_ZERO;	// ���_�傫��
	m_state		 = STATE_NONE;	// ���
	m_fSinScale	 = HALF_PI;		// �g�k����
	m_fAddSinRot = 0.0f;		// �g�k�����̉��Z��
	m_fCurScale	 = 0.0f;		// ���݊g�嗦
	m_fMinScale	 = 0.0f;		// �Œ�g�嗦
	m_fMaxScale	 = 0.0f;		// �ő�g�嗦
	m_fInitScale = 0.0f;		// �����g�嗦
	m_fSubIn	 = 0.0f;		// �C���̊g�嗦������
	m_fAddOut	 = 0.0f;		// �A�E�g�̊g�嗦������

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
void CScale2D::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CScale2D::Update(const float fDeltaTime)
{
	// �������Ȃ���Ԃ̏ꍇ������
	if (m_state == STATE_NONE) { return; }

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_FADEOUT:
	{
		// �Œ���̊g�嗦�ɂ��Ă���
		m_fCurScale += m_fAddOut * fDeltaTime;
		if (m_fCurScale >= m_fMinScale)
		{ // �Œ���̊g�嗦�ɂȂ����ꍇ

			// �g�嗦��␳
			m_fCurScale = m_fMinScale;

			// �g�k��Ԃɂ���
			m_state = STATE_SCALE;
		}

		// �傫���𔽉f
		SetVec3Sizing(m_sizeOrigin * m_fCurScale);
		break;
	}
	case STATE_SCALE:
	{
		// �T�C���J�[�u��������]
		m_fSinScale += m_fAddSinRot * fDeltaTime;
		useful::NormalizeRot(m_fSinScale);	// �������K��

		// �g�嗦���Z�ʂ��v�Z
		float fAddScale = ((m_fMaxScale - m_fMinScale) * 0.5f) * (sinf(m_fSinScale) - 1.0f) * -1.0f;

		// �g�嗦���v�Z
		m_fCurScale = m_fMinScale + fAddScale;

		// �傫���𔽉f
		SetVec3Sizing(m_sizeOrigin * m_fCurScale);
		break;
	}
	case STATE_FADEIN:
	{
		// ��������g�嗦�ɂ��Ă���
		m_fCurScale -= m_fSubIn * fDeltaTime;
		if (m_fCurScale >= 0.0f)
		{ // ��������g�嗦�ɂȂ����ꍇ

			// �g�嗦��␳
			m_fCurScale = 0.0f;

			// �������Ȃ���Ԃɂ���
			m_state = STATE_NONE;
		}

		// �傫���𔽉f
		SetVec3Sizing(m_sizeOrigin * m_fCurScale);
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
void CScale2D::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CScale2D *CScale2D::Create
(
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rOriginSize,	// ���_�傫��
	const float fMinScale,			// �Œ�g�嗦
	const float fMaxScale,			// �ő�g�嗦
	const float fInitScale,			// �����g�嗦
	const float fCalcScale,			// �g�嗦�̉�����
	const float fSubIn,				// �g�k�O�̃C���̊g�嗦������
	const float fAddOut,			// �g�k��̃A�E�g�̊g�嗦������
	const D3DXVECTOR3& rRot,		// ����
	const D3DXCOLOR& rCol			// �F
)
{
	// �g�k�I�u�W�F�N�g2D�̐���
	CScale2D *pScale2D = new CScale2D;
	if (pScale2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �g�k�I�u�W�F�N�g2D�̏�����
		if (FAILED(pScale2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �g�k�I�u�W�F�N�g2D�̔j��
			SAFE_DELETE(pScale2D);
			return nullptr;
		}

		// �ʒu��ݒ�
		pScale2D->SetVec3Position(rPos);

		// ������ݒ�
		pScale2D->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pScale2D->SetVec3Sizing(rOriginSize * fInitScale);

		// ���_�傫����ݒ�
		pScale2D->SetOriginSizing(rOriginSize);

		// �F��ݒ�
		pScale2D->SetColor(rCol);

		// �Œ�g�嗦��ݒ�
		pScale2D->SetMinScale(fMinScale);

		// �ő�g�嗦��ݒ�
		pScale2D->SetMaxScale(fMaxScale);

		// �����g�嗦��ݒ�
		pScale2D->SetInitScale(fInitScale);
		pScale2D->m_fCurScale = fInitScale;	// ���݂̊g�嗦���ݒ�

		// �g�k�����̉��Z�ʂ�ݒ�
		pScale2D->SetCalcScale(fCalcScale);

		// �g�嗦�����ʂ�ݒ�
		pScale2D->SetSubIn(fSubIn);

		// �g�嗦�����ʂ�ݒ�
		pScale2D->SetAddOut(fAddOut);

		// �m�ۂ����A�h���X��Ԃ�
		return pScale2D;
	}
}

//============================================================
//	�g�k�ݒ菈��
//============================================================
void CScale2D::SetScale(const bool bScale)
{
	// �g�k������������
	m_fSinScale = HALF_PI;

	if (bScale)
	{ // �g�k���J�n����ꍇ

		// �t�F�[�h�A�E�g��Ԃɂ���
		m_state = STATE_FADEOUT;
	}
	else
	{ // �g�k���I������ꍇ

		// �t�F�[�h�C����Ԃɂ���
		m_state = STATE_FADEIN;
	}
}
