//============================================================
//
//	�G�f�B�b�g�}�l�[�W���[���� [editManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editManager.h"
#include "manager.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_SAVE			(DIK_F9)	// �ۑ��L�[
#define NAME_SAVE			("F9")		// �ۑ��\��
#define KEY_CHANGE_TYPE		(DIK_1)		// �G�f�B�b�g�^�C�v�ύX�L�[
#define NAME_CHANGE_TYPE	("1")		// �G�f�B�b�g�^�C�v�ύX�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SAVE_TXT = "data\\TXT\\save_stage.txt";	// �X�e�[�W�Z�[�u�e�L�X�g

	// ��ޖ�
	const char *TYPE_NAME[] =
	{
		"�X�e�[�W",
	};
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TYPE_NAME) == CEditManager::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�e�N���X [CEditManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditManager::CEditManager()
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_type	= TYPE_STAGE;	// �G�f�B�b�g�^�C�v
	m_bSave	= false;		// �ۑ���
	m_bEdit	= false;		// �G�f�B�b�g��

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditManager::~CEditManager()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditManager::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_type	= TYPE_STAGE;	// �G�f�B�b�g�^�C�v
	m_bSave	= false;		// �ۑ���
	m_bEdit	= false;		// �G�f�B�b�g��

	// ������Ԃ�
	return S_OK;

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	�I������
//============================================================
void CEditManager::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditManager::Update(void)
{
#if _DEBUG

	// �G�f�B�b�g���[�h����Ȃ��ꍇ������
	if (!m_bEdit) { return; }

	// �G�f�B�b�g�^�C�v�̕ύX
	ChangeType();

	// �X�e�[�W�ۑ�
	SaveStage();

	// ����\���̕`��
	DrawDebugControl();

	// ���\���̕`��
	DrawDebugInfo();

#endif	// _DEBUG
}

//============================================================
//	���ۑ��̐ݒ菈��
//============================================================
void CEditManager::UnSave(void)
{
	// �ۑ����Ă��Ȃ��󋵂ɂ���
	m_bSave = false;
}

//============================================================
//	�G�f�B�b�g�󋵂̐ؑ֏���
//============================================================
void CEditManager::SwitchEnableEdit(void)
{
	// �G�f�B�b�g�󋵂𔽓]
	m_bEdit = !m_bEdit;

#if 0
	if (m_bEdit)
	{ // �G�f�B�b�gON�̏ꍇ

		// �G�f�B�b�g�X�e�[�W�̐���
		if (m_pStage == nullptr)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă��Ȃ��ꍇ

			m_pStage = CEditStage::Create(this, m_thing);
			assert(m_pStage != nullptr);	// �������s
		}

		// ���Ǎ�
		m_pStage->LoadInfo();
	}
	else
	{ // �G�f�B�b�gOFF�̏ꍇ

		// ���ۑ�
		m_pStage->SaveInfo();

		// �G�f�B�b�g�X�e�[�W�̔j��
		if (m_pStage != nullptr)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

			HRESULT hr = CEditStage::Release(m_pStage);
			assert(hr != E_FAIL);	// �j�����s
		}
	}
#endif
}

//============================================================
//	�G�f�B�b�g�󋵂̐ݒ菈��
//============================================================
void CEditManager::SetEnableEdit(const bool bEdit)
{
	// �����̃G�f�B�b�g�󋵂ɂ���
	m_bEdit = bEdit;

#if 0
	if (bEdit)
	{ // �G�f�B�b�g���[�h�̏ꍇ

		// �G�f�B�b�g�X�e�[�W�̐���
		if (m_pStage == nullptr)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă��Ȃ��ꍇ

			m_pStage = CEditStage::Create(this, m_thing);
			assert(m_pStage != nullptr);	// �������s
		}
	}
	else
	{ // �G�f�B�b�g���[�h�ł͂Ȃ��ꍇ

		// �G�f�B�b�g�X�e�[�W�̔j��
		if (m_pStage != nullptr)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

			HRESULT hr = CEditStage::Release(m_pStage);
			assert(hr != E_FAIL);	// �j�����s
		}
	}
#endif
}

//============================================================
//	�G�f�B�b�g�󋵎擾����
//============================================================
bool CEditManager::IsEdit(void) const
{
	// �G�f�B�b�g�󋵂�Ԃ�
	return m_bEdit;
}

//============================================================
//	��������
//============================================================
CEditManager *CEditManager::Create(void)
{
#if _DEBUG

	// �G�f�B�^�[�}�l�[�W���[�̐���
	CEditManager *pEditManager = new CEditManager;
	if (pEditManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�f�B�^�[�}�l�[�W���[�̏�����
		if (FAILED(pEditManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�f�B�^�[�}�l�[�W���[�̔j��
			SAFE_DELETE(pEditManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEditManager;
	}

#else	// NDEBUG

	// nullptr��Ԃ�
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	�j������
//============================================================
void CEditManager::Release(CEditManager *&prEditManager)
{
#if _DEBUG

	// �G�f�B�b�g�}�l�[�W���[�̏I��
	assert(prEditManager != nullptr);
	prEditManager->Uninit();

	// �������J��
	SAFE_DELETE(prEditManager);

#endif	// _DEBUG
}

//============================================================
//	�G�f�B�^�[�̕ύX����
//============================================================
void CEditManager::ChangeType(void)
{
	// �G�f�B�^�[�̃^�C�v�ύX
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	if (pKeyboard->IsTrigger(KEY_CHANGE_TYPE))
	{
#if 0
		// ���ۑ�
		m_pStage->SaveInfo();

		// �G�f�B�b�g�X�e�[�W�̔j��
		if (m_pStage != nullptr)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

			HRESULT hr = CEditStage::Release(m_pStage);
			assert(hr != E_FAIL);	// �j�����s
		}

		// �G�f�B�b�g�^�C�v�̕ύX
		m_type = (EType)((m_type + 1) % TYPE_MAX);

		// �G�f�B�b�g�X�e�[�W�̐���
		if (m_pStage == nullptr)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă��Ȃ��ꍇ

			m_pStage = CEditStage::Create(this, m_thing);
			assert(m_pStage != nullptr);	// �������s
		}

		// ���Ǎ�
		m_pStage->LoadInfo();
#else
		// �G�f�B�b�g�^�C�v�̕ύX
		m_type = (EType)((m_type + 1) % TYPE_MAX);
#endif
	}
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditManager::DrawDebugControl(void)
{
	DebugProc::Print(DebugProc::POINT_RIGHT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "[�G�f�B�b�g����]�@\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "�X�e�[�W�ۑ��F[%s+%s]\n", NAME_DOUBLE, NAME_SAVE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�G�f�B�b�g�^�C�v�ύX�F[%s]\n", NAME_CHANGE_TYPE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "--------------------------------------\n");
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditManager::DrawDebugInfo(void)
{
	DebugProc::Print(DebugProc::POINT_RIGHT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "[�G�f�B�b�g���]�@\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, (m_bSave) ? "�ۑ��ρF[�ۑ���]\n" : "���ۑ��F[�ۑ���]\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "%s�F[�G�f�B�b�g�^�C�v]\n", TYPE_NAME[m_type]);
	DebugProc::Print(DebugProc::POINT_RIGHT, "--------------------------------------\n");
}

//============================================================
//	�X�e�[�W�ۑ�����
//============================================================
void CEditManager::SaveStage(void)
{
	// �X�e�[�W��ۑ�
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	if (pKeyboard->IsPress(KEY_DOUBLE))
	{
		if (pKeyboard->IsTrigger(KEY_SAVE))
		{
			// �ۑ�����
			Save();

			// �ۑ�������Ԃɂ���
			m_bSave = true;
		}
	}
}

//============================================================
//	�ۑ�����
//============================================================
void CEditManager::Save(void)
{
	// �|�C���^��錾
	FILE *pFile = nullptr;	// �t�@�C���|�C���^

	// �t�@�C���������o���`���ŊJ��
	pFile = fopen(SAVE_TXT, "w");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		// ���o���������o��
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#	�X�e�[�W�Z�[�u�e�L�X�g [save_stage.txt]\n");
		fprintf(pFile, "#	Author : you\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "---------->--<---------- �������牺�� �R�s�[���\��t�� ---------->--<----------\n\n");

#if 0
		// ���ۑ�
		m_pStage->SaveInfo();

		for (int nCntThing = 0; nCntThing < CEditStage::THING_MAX; nCntThing++)
		{ // �z�u���̑������J��Ԃ�

			// �G�f�B�b�g�X�e�[�W�̔j��
			if (m_pStage != nullptr)
			{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

				HRESULT hr = CEditStage::Release(m_pStage);
				assert(hr != E_FAIL);	// �j�����s
			}

			// �G�f�B�b�g�X�e�[�W�̐���
			if (m_pStage == nullptr)
			{ // �G�f�B�b�g�X�e�[�W���g�p����Ă��Ȃ��ꍇ

				m_pStage = CEditStage::Create(this, (CEditStage::EThing)nCntThing);
				assert(m_pStage != nullptr);	// �������s
			}

			// �z�u���̕ۑ�
			m_pStage->Save(pFile);
		}

		// �G�f�B�b�g�X�e�[�W�̔j��
		if (m_pStage != nullptr)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

			HRESULT hr = CEditStage::Release(m_pStage);
			assert(hr != E_FAIL);	// �j�����s
		}

		// �G�f�B�b�g�X�e�[�W�̐���
		if (m_pStage == nullptr)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă��Ȃ��ꍇ

			m_pStage = CEditStage::Create(this, m_thing);
			assert(m_pStage != nullptr);	// �������s
		}

		// ���Ǎ�
		m_pStage->LoadInfo();
#endif

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�X�e�|�W�Z�[�u�t�@�C���̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}