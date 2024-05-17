//============================================================
//
//	���g�o�����UI���� [cloneAngleUI.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "cloneAngleUI.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* TEXTURE_FILE = "";		// �e�N�X�`���t�@�C��

	const int	PRIORITY = 2;			// �e�̗D�揇��
	const D3DXVECTOR3 RADIUS = D3DXVECTOR3(50.0f, 0.0f, 50.0f);		// ���a(�T�C�Y)
}

//************************************************************
//	�q�N���X [CCloneAngleUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CCloneAngleUI::CCloneAngleUI() : CObject3D(CObject::LABEL_NONE, CObject::DIM_3D, PRIORITY),
m_pParentObject(nullptr)		// �e�I�u�W�F�N�g
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CCloneAngleUI::~CCloneAngleUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CCloneAngleUI::Init(void)
{
	// �����o�ϐ���������
	m_pParentObject = nullptr;		// �e�I�u�W�F�N�g

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
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
void CCloneAngleUI::Uninit(void)
{
	// �e�I�u�W�F�N�g�� NULL �ɂ���
	m_pParentObject = nullptr;

	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CCloneAngleUI::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CCloneAngleUI::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CCloneAngleUI* CCloneAngleUI::Create(CObject *pParent)
{
	// �e�̐���
	CCloneAngleUI* pUI = new CCloneAngleUI;

	if (pUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �e�̏�����
		if (FAILED(pUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �e�̔j��
			SAFE_DELETE(pUI);
			return nullptr;
		}

		// �ʒu��ݒ�
		pUI->SetVec3Position(pParent->GetVec3Position());

		// �T�C�Y��ݒ�
		pUI->SetVec3Sizing(RADIUS);

		// �e�I�u�W�F�N�g��ݒ�
		pUI->m_pParentObject = pParent;

		// �e�N�X�`����o�^�E����
		//pUI->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE));

		// �m�ۂ����A�h���X��Ԃ�
		return pUI;
	}
}
