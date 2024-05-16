//============================================================
//
//	�G�f�B�b�g�X�e�[�W���� [editStage.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editStage.h"
#include "manager.h"

//************************************************************
//	�e�N���X [CEditStage] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditStage::CEditStage()
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pEditManager = nullptr;	// �G�f�B�b�g�}�l�[�W���[�̏��

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditStage::~CEditStage()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	��������
//============================================================
CEditStage *CEditStage::Create(CEditManager *pEditManager, EType type)
{
#if _DEBUG

	// �|�C���^��錾
	CEditStage *pEditStage = nullptr;	// �G�f�B�b�g�X�e�[�W���

	// �G�f�B�b�g�X�e�[�W�̐���
	switch (type)
	{ // ��ނ��Ƃ̏���
	case TYPE_FIELD:
		//pEditStage = new CEditFiled;	// �G�f�B�b�g�t�B�[���h
		break;

	case TYPE_WALL:
		//pEditStage = new CEditWall;	// �G�f�B�b�g�E�H�[��
		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (pEditStage == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�f�B�b�g�X�e�[�W�̏�����
		if (FAILED(pEditStage->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�f�B�b�g�X�e�[�W�̔j��
			SAFE_DELETE(pEditStage);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEditStage;
	}

#else	// NDEBUG

	// nullptr��Ԃ�
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	�j������
//============================================================
void CEditStage::Release(CEditStage *&prEditStage)
{
#if _DEBUG

	// �G�f�B�b�g�X�e�[�W�̏I��
	assert(prEditStage != nullptr);
	prEditStage->Uninit();

	// �������J��
	SAFE_DELETE(prEditStage);

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	�G�f�B�b�g�}�l�[�W���[�擾����
//============================================================
const CEditManager *CEditStage::GetPtrEditManager(void) const
{
#if _DEBUG

	// �G�f�B�b�g�}�l�[�W���[��Ԃ�
	return m_pEditManager;

#else	// NDEBUG

	// nullptr��Ԃ�
	return nullptr;

#endif	// _DEBUG
}
