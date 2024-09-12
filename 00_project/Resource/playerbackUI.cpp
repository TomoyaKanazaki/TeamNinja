//============================================================
//
// playerbackUI���� [playerbackUI.cpp]
// Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "playerbackUI.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 2;									// �X�^�[�gUI�̗D�揇��
	const char* TEXTURE = "data\\TEXTURE\\alcohol.png";		// �e�N�X�`��
	const D3DXVECTOR3 POS = D3DXVECTOR3(1100.0f, 600.0f, 0.0f);			// �ʒu
	const D3DXVECTOR3 RADIUS = D3DXVECTOR3(200.0f, 200.0f, 0.0f);		// �ʏ�T�C�Y

	const float ADD_MAGNI = 0.9f;		// ���Z�̔{��
	const float SUB_MAGNI = 1.4f;		// ���Z�̔{��
}

//************************************************************
//	�q�N���X [CPlayerBackUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerBackUI::CPlayerBackUI() : CObject2D(CObject::LABEL_NONE, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY),
m_state(STATE_NONE)		// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayerBackUI::~CPlayerBackUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayerBackUI::Init(void)
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
void CPlayerBackUI::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayerBackUI::Update(const float fDeltaTime)
{
	switch (m_state)
	{
	case CPlayerBackUI::STATE_NONE:

		// �����x��0.0f�ɐݒ�
		SetAlpha(0.0f);

		break;

	case CPlayerBackUI::STATE_ADD:

		// ���Z��ԏ���
		AddFunc(fDeltaTime);

		break;

	case CPlayerBackUI::STATE_SUB:

		// ���Z��ԏ���
		SubFunc(fDeltaTime);

		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CPlayerBackUI::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CPlayerBackUI* CPlayerBackUI::Create(void)
{
	// �X�^�[�gUI�̐���
	CPlayerBackUI* pStartUI = new CPlayerBackUI;

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
		pStartUI->SetVec3Position(POS);			// �ʒu
		pStartUI->SetVec3Rotation(VEC3_ZERO);	// ����
		pStartUI->SetVec3Sizing(RADIUS);		// �傫��
		pStartUI->SetAlpha(0.0f);				// �����x

		// �e�N�X�`���̊��蓖��
		pStartUI->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE));

		// �m�ۂ����A�h���X��Ԃ�
		return pStartUI;
	}
}

//============================================================
// ���Z��ԏ���
//============================================================
void CPlayerBackUI::AddFunc(const float fDeltaTime)
{
	// �����x���擾
	float fAlpha = GetAlpha();

	// �����x�����Z����
	fAlpha += (fDeltaTime * ADD_MAGNI);
	
	if (fAlpha > 1.0f)
	{ // �����x��1.0f���߂̏ꍇ

		// �����x��␳
		fAlpha = 1.0f;
	}

	// �����x�𔽉f
	SetAlpha(fAlpha);
}

//============================================================
// ���Z��ԏ���
//============================================================
void CPlayerBackUI::SubFunc(const float fDeltaTime)
{
	// �����x���擾
	float fAlpha = GetAlpha();

	// �����x�����Z����
	fAlpha -= (fDeltaTime * SUB_MAGNI);

	if (fAlpha < 0.0f)
	{ // �����x��0.0f�����̏ꍇ

		// �����x��␳
		fAlpha = 0.0f;

		// ���Z��Ԃɂ���
		m_state = STATE_NONE;
	}

	// �����x�𔽉f
	SetAlpha(fAlpha);
}
