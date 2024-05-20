//============================================================
//
// startUI2D���� [startUI.cpp]
// Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "popupUI.h"
#include "manager.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;			// �X�^�[�gUI�̗D�揇��
	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(1600.0f, 400.0f, 0.0f);		// �T�C�Y

	// �Y�[����Ԃ̒萔
	namespace zoom
	{
		const D3DXVECTOR3 INIT_ZOOM_SUB = D3DXVECTOR3(20.0f, 5.0f, 0.0f);	// �����̃T�C�Y�̌��Z��
		const D3DXVECTOR3 ACCELE_ZOOM_SUB = D3DXVECTOR3(4.0f, 1.0f, 0.0f);	// �T�C�Y�̌��Z�����x
		const D3DXVECTOR3 DEST_SIZE = D3DXVECTOR3(360.0f, 90.0f, 0.0f);		// �o�E���h��ԂɈڍs����T�C�Y
	}

	// �o�E���h��Ԃ̒萔
	namespace bound
	{
		const D3DXVECTOR3 DEST_SIZE = D3DXVECTOR3(440.0f, 110.0f, 0.0f);	// �o�E���h��ԂɈڍs����T�C�Y
		const D3DXVECTOR3 BOUND_SUB = D3DXVECTOR3(15.0f, 3.75f, 0.0f);		// �����̃T�C�Y�̌��Z��
	}

	// �`���Ԃ̒萔
	namespace disp
	{
		const int DISP_COUNT = 50;					// �\���J�E���g
	}

	// �t�F�[�h��Ԃ̒萔
	namespace fade
	{
		const float INIT_ALPHA_SUB = 0.02f;			// �����̓����x�̌��Z��
		const float ACCELE_ALPHA_SUB = 0.01f;		// �����x�̌��Z�����x
	}
}

//************************************************************
//	�q�N���X [CPopUpUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPopUpUI::CPopUpUI() : CObject2D(CObject::LABEL_STARTUI, CObject::DIM_2D, PRIORITY),
m_sizeMove(VEC3_ZERO),				// �T�C�Y�̈ړ���
m_state(STATE_ZOOM),				// ���
m_nDispCount(0),					// �\���J�E���g
m_fSubAlpha(fade::INIT_ALPHA_SUB),	// �����x�̌��Z��
m_fAlpha(1.0f)						// �����x
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPopUpUI::~CPopUpUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPopUpUI::Init(void)
{
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
void CPopUpUI::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPopUpUI::Update(const float fDeltaTime)
{
	// ��ԏ���
	State();
}

//============================================================
//	�`�揈��
//============================================================
void CPopUpUI::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CPopUpUI* CPopUpUI::Create(const char* texture)
{
	// �X�^�[�gUI�̐���
	CPopUpUI* pStartUI = new CPopUpUI;

	if (pStartUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �X�^�[�gUI�̏�����
		if (FAILED(pStartUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �X�^�[�gUI�̔j��
			SAFE_DELETE(pStartUI);
			return nullptr;
		}

		// �l��ݒ�
		pStartUI->SetVec3Position(SCREEN_CENT);		// �ʒu��ݒ�
		pStartUI->SetVec3Rotation(VEC3_ZERO);		// ������ݒ�
		pStartUI->SetVec3Sizing(INIT_SIZE);			// �傫����ݒ�

		if (texture != nullptr)
		{ // �e�N�X�`���� NULL ����Ȃ��ꍇ
			pStartUI->BindTexture(GET_MANAGER->GetTexture()->Regist(texture));	// �e�N�X�`����o�^�E����
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pStartUI;
	}
}

//============================================================
// ��ԏ���
//============================================================
void CPopUpUI::State(void)
{
	switch (m_state)
	{
	case CPopUpUI::STATE_ZOOM:		// �Y�[�����

		// �Y�[������
		Zoom();

		break;

	case CPopUpUI::STATE_BOUND:		// �o�E���h���

		// �o�E���h����
		Bound();

		break;

	case CPopUpUI::STATE_DISP:		// �\�����

		// �\������
		Disp();

		break;

	case CPopUpUI::STATE_FADE:		// �t�F�[�Y���

		if (Fade())
		{ // �t�F�[�h���I������ꍇ

			// ���̐�̏������s��Ȃ�
			return;
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
// �Y�[������
//============================================================
void CPopUpUI::Zoom(void)
{
	// �T�C�Y�����Z����
	D3DXVECTOR3 size = GetVec3Sizing();
	size -= m_sizeMove;

	// �T�C�Y�̈ړ��ʂ����Z����
	m_sizeMove += zoom::ACCELE_ZOOM_SUB;

	if (size.x < zoom::DEST_SIZE.x ||
		size.y < zoom::DEST_SIZE.y)
	{ // ���̃T�C�Y�ɂȂ����ꍇ

		// �T�C�Y��␳����
		size = zoom::DEST_SIZE;

		// �T�C�Y�̈ړ��ʂ�ݒ肷��
		m_sizeMove = bound::BOUND_SUB;

		// �o�E���h��Ԃɂ���
		m_state = STATE_BOUND;
	}

	// �T�C�Y��K�p
	SetVec3Sizing(size);
}

//============================================================
// �o�E���h����
//============================================================
void CPopUpUI::Bound(void)
{
	// �T�C�Y�����Z����
	D3DXVECTOR3 size = GetVec3Sizing();
	size += m_sizeMove;

	if (size.x > bound::DEST_SIZE.x ||
		size.y > bound::DEST_SIZE.y)
	{ // ���̃T�C�Y�ɂȂ����ꍇ

		// �T�C�Y��␳����
		size = bound::DEST_SIZE;

		// �\����Ԃɂ���
		m_state = STATE_DISP;
	}

	// �T�C�Y��K�p
	SetVec3Sizing(size);
}

//============================================================
// �\������
//============================================================
void CPopUpUI::Disp(void)
{
	// �\���J�E���g�����Z����
	m_nDispCount++;

	if (m_nDispCount % disp::DISP_COUNT == 0)
	{ // �J�E���g�����ȏ�ɂȂ����ꍇ

		// �t�F�[�h��Ԃɂ���
		m_state = STATE_FADE;
	}
}

//============================================================
// �t�F�[�h����
//============================================================
bool CPopUpUI::Fade(void)
{
	// �����x�����Z����
	m_fAlpha -= m_fSubAlpha;

	// �����x�̌��Z�ʂ𑝂₷
	m_fSubAlpha += fade::ACCELE_ALPHA_SUB;

	if (m_fAlpha <= 0.0f)
	{ // ���S�ɓ����ɂȂ����ꍇ

		// �����x��␳����
		m_fAlpha = 0.0f;

		// �I������
		Uninit();

		// true ��Ԃ�
		return true;
	}

	// ���_�J���[�̐ݒ菈��
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

	// false ��Ԃ�
	return false;
}
