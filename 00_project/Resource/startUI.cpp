//============================================================
//
// startUI2D���� [startUI.cpp]
// Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "startUI.h"
#include "manager.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* TEXTURE_FILE = "TODO";		// �e�N�X�`��
	const int PRIORITY = 6;			// �X�^�[�gUI�̗D�揇��
	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(1600.0f, 400.0f, 0.0f);		// �T�C�Y

	// �Y�[����Ԃ̒萔
	namespace zoom
	{
		const D3DXVECTOR3 INIT_ZOOM_SUB = D3DXVECTOR3(20.0f, 5.0f, 0.0f);		// �����̃T�C�Y�̌��Z��
		const D3DXVECTOR3 ACCELE_ZOOM_SUB = D3DXVECTOR3(4.0f, 1.0f, 0.0f);		// �T�C�Y�̌��Z�����x
		const D3DXVECTOR3 DEST_SIZE = D3DXVECTOR3(360.0f, 90.0f, 0.0f);			// �o�E���h��ԂɈڍs����T�C�Y
	}

	// �o�E���h��Ԃ̒萔
	namespace bound
	{
		const D3DXVECTOR3 DEST_SIZE = D3DXVECTOR3(440.0f, 110.0f, 0.0f);		// �o�E���h��ԂɈڍs����T�C�Y
		const D3DXVECTOR3 BOUND_SUB = D3DXVECTOR3(15.0f, 3.75f, 0.0f);			// �����̃T�C�Y�̌��Z��
	}

	// ��~��Ԃ̒萔
	namespace stop
	{
		const int DELETE_COUNT = 60;		// ��~��Ԃ̃J�E���g��
	}
}

//************************************************************
//	�q�N���X [CStartUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CStartUI::CStartUI() : CObject2D(CObject::LABEL_STARTUI, CObject::DIM_2D, PRIORITY),
m_sizeMove(VEC3_ZERO),		// �T�C�Y�̈ړ���
m_state(STATE_ZOOM),		// �Y�[�����
m_nDeleteCount(0)			// �����܂ł̃J�E���g
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CStartUI::~CStartUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CStartUI::Init(void)
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
void CStartUI::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CStartUI::Update(const float fDeltaTime)
{
	// ��ԏ���
	State();
}

//============================================================
//	�`�揈��
//============================================================
void CStartUI::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CStartUI* CStartUI::Create(void)
{
	// �X�^�[�gUI�̐���
	CStartUI* pStartUI = new CStartUI;

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
		//pStartUI->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE));// �e�N�X�`����o�^�E����

		// �m�ۂ����A�h���X��Ԃ�
		return pStartUI;
	}
}

//============================================================
// ��ԏ���
//============================================================
void CStartUI::State(void)
{
	switch (m_state)
	{
	case CStartUI::STATE_ZOOM:		// �Y�[�����

		// �Y�[������
		Zoom();

		break;

	case CStartUI::STATE_BOUND:		// �o�E���h���

		// �o�E���h����
		Bound();

		break;

	case CStartUI::STATE_FADE:		// �t�F�[�Y���

		// �����J�E���g�����Z����
		m_nDeleteCount++;

		if (m_nDeleteCount >= stop::DELETE_COUNT)
		{ // �J�E���g�����ȏ�ɂȂ����ꍇ

			// �I������
			Uninit();

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
void CStartUI::Zoom(void)
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
void CStartUI::Bound(void)
{
	// �T�C�Y�����Z����
	D3DXVECTOR3 size = GetVec3Sizing();
	size += m_sizeMove;

	if (size.x > bound::DEST_SIZE.x ||
		size.y > bound::DEST_SIZE.y)
	{ // ���̃T�C�Y�ɂȂ����ꍇ

		// �T�C�Y��␳����
		size = bound::DEST_SIZE;

		// �t�F�[�Y��Ԃɂ���
		m_state = STATE_FADE;
	}

	// �T�C�Y��K�p
	SetVec3Sizing(size);
}

//============================================================
// �t�F�[�h����
//============================================================
void CStartUI::Fade(void)
{

}
