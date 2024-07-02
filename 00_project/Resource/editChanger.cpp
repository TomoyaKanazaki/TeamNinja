//===========================================
//
//  �J�������ύX�n�_�G�f�B�^�[(editChanger.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "editChanger.h"
#include "editManager.h"
#include "manager.h"
#include "collision.h"
#include "stage.h"

//===========================================
//  �萔�錾
//===========================================
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_changer.txt"; // �Z�[�u�e�L�X�g�p�X

	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(editstage::SIZE, editstage::SIZE, editstage::SIZE); // �傫��
	const float	INIT_ALPHA = 0.5f; // �z�u�O�̃��l
	const int DIGIT_FLOAT = 2; // �����_�ȉ��̌���

	const int KEY_CREATE		= DIK_0; // �����L�[
	const int KEY_RELEASE		= DIK_9; // �j���L�[
	const int KEY_DIRECTION		= DIK_4; // �����ύX�L�[
	const int KEY_ROTATION		= DIK_5; // �p�x�ύX�L�[
	const int KEY_UP_SIZE_X		= DIK_T; // X���g��L�[
	const int KEY_DOWN_SIZE_X	= DIK_G; // X���k���L�[
	const int KEY_UP_SIZE_Y		= DIK_Y; // Y���g��L�[
	const int KEY_DOWN_SIZE_Y	= DIK_H; // Y���k���L�[
	const int KEY_UP_SIZE_Z		= DIK_U; // Z���g��L�[
	const int KEY_DOWN_SIZE_Z	= DIK_J; // Z���k���L�[

	const char* NAME_CREATE			= "0"; // �����\��
	const char* NAME_RELEASE		= "9"; // �j���\��
	const char* NAME_DIRECTION		= "4"; // �����ύX�\��
	const char* NAME_ROTATION		= "5"; // �p�x�ύX�\��
	const char* NAME_UP_SIZE_X		= "T"; // X���g��\��
	const char* NAME_DOWN_SIZE_X	= "G"; // X���k���\��
	const char* NAME_UP_SIZE_Y		= "Y"; // Y���g��\��
	const char* NAME_DOWN_SIZE_Y	= "H"; // Y���k���\��
	const char* NAME_UP_SIZE_Z		= "U"; // Z���g��\��
	const char* NAME_DOWN_SIZE_Z	= "J"; // Z���k���\��

	const char* NAME_DIR[CCameraChanger::DIRECTION_MAX] = // ����
	{
		"���ʂ���", // ����
		"�������", // ���
		"������", // ��
		"�E����", // �E
	};

	const char* NAME_ROT[CCameraChanger::ROTATION_MAX] = // �p�x
	{
		"�f�t�H���g", // �f�t�H���g
		"�ォ��", // ��
		"������" // ��
	};
}

//===========================================
//	�R���X�g���N�^
//===========================================
CEditChanger::CEditChanger(CEditStage* pEditor) : CEditorObject(pEditor),
m_pChanger(nullptr),
m_bSave(false)
{
	// �����o�ϐ����N���A
	memset(&m_infoCreate, 0, sizeof(m_infoCreate)); // �t�B�[���h�z�u���
}

//===========================================
//	�f�X�g���N�^
//===========================================
CEditChanger::~CEditChanger()
{
}

//===========================================
//	����������
//===========================================
HRESULT CEditChanger::Init(void)
{
#if _DEBUG

	// �e�N���X�̏�����
	if (FAILED(CEditorObject::Init())) { assert(false); return E_FAIL; }

	// �I�u�W�F�N�g�̐���
	m_pChanger = CCameraChanger::Create
	(
		GetVec3Position(), // �ʒu
		m_infoCreate.size, // �傫��
		m_infoCreate.dir, // ����
		m_infoCreate.rot // �p�x
	);

	//�����Ɏ��s�����ꍇ�֐��𔲂���
	if (m_pChanger == nullptr) { assert(false); return E_FAIL; }

	// ������Ԃ�
	return S_OK;

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}

//===========================================
//	�I������
//===========================================
void CEditChanger::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorObject::Uninit();

	// �I�u�W�F�N�g�̏I��
	SAFE_UNINIT(m_pChanger);

#endif	// _DEBUG
}

//===========================================
//	�X�V����
//===========================================
void CEditChanger::Update(void)
{
#if _DEBUG

	// �e�N���X�̍X�V
	CEditorObject::Update();

	// �傫���̍X�V
	UpdateSize();

	// �����̕ύX
	ChangeDirection();

	// �p�x�̕ύX
	ChangeRotation();

	// �t�B�[���h�̐���
	CreateField();

	// �t�B�[���h�̔j��
	ReleaseField();

	// �ʒu�𔽉f
	m_pChanger->SetVec3Position(GetVec3Position());

	// �����𔽉f
	m_pChanger->SetVec3Rotation(GetVec3Rotation());

#endif	// _DEBUG
}

//===========================================
//	�ۑ��󋵎擾����
//===========================================
bool CEditChanger::IsSave(void)
{
#if _DEBUG

	// �ۑ��󋵂�Ԃ�
	return m_bSave;

#endif

	// false��Ԃ�
	return false;
}

//===========================================
//	���ۑ�����
//===========================================
void CEditChanger::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//===========================================
//	���Ǎ�����
//===========================================
void CEditChanger::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//===========================================
//	����\���̕`�揈��
//===========================================
void CEditChanger::DrawDebugControl(void)
{
#if _DEBUG

	// ����\���̕`��
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "�傫���F[%s/%s+%s/%s/%s+%s/%s/%s+%s]\n",
		NAME_UP_SIZE_X, NAME_DOWN_SIZE_X, NAME_TRIGGER,
		NAME_UP_SIZE_Y, NAME_DOWN_SIZE_Y, NAME_TRIGGER,
		NAME_UP_SIZE_Z, NAME_DOWN_SIZE_Z, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�����ύX�F[%s]\n", NAME_DIRECTION);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�p�x�ύX�F[%s]\n", NAME_ROTATION);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�폜�F[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//===========================================
//	���\���̕`�揈��
//===========================================
void CEditChanger::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorObject::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f�F[�傫��]\n", m_infoCreate.size.x, m_infoCreate.size.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%s[����]\n", NAME_DIR[m_infoCreate.dir]);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%s[�p�x]\n", NAME_ROT[m_infoCreate.rot]);

#endif	// _DEBUG
}

//===========================================
//	�����X�V����
//===========================================
void CEditChanger::UpdateRotation(void)
{
	// �����̍X�V
	CEditorObject::UpdateRotation();

	// �����𔽉f
	m_pChanger->SetVec3Rotation(GetVec3Rotation());
}

//===========================================
//	�傫���̍X�V����
//===========================================
void CEditChanger::UpdateSize(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// �傫����ύX
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_SIZE_X))
		{
			m_infoCreate.size.x += INIT_SIZE.x;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE_X))
		{
			m_infoCreate.size.x -= INIT_SIZE.x;
		}
		if (pKeyboard->IsPress(KEY_UP_SIZE_Y))
		{
			m_infoCreate.size.y += INIT_SIZE.y;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE_Y))
		{
			m_infoCreate.size.y -= INIT_SIZE.y;
		}
		if (pKeyboard->IsPress(KEY_UP_SIZE_Z))
		{
			m_infoCreate.size.z += INIT_SIZE.z;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE_Z))
		{
			m_infoCreate.size.z -= INIT_SIZE.z;
		}
	}
	else
	{
		if (pKeyboard->IsTrigger(KEY_UP_SIZE_X))
		{
			m_infoCreate.size.x += INIT_SIZE.x;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_X))
		{
			m_infoCreate.size.x -= INIT_SIZE.x;
		}
		if (pKeyboard->IsTrigger(KEY_UP_SIZE_Y))
		{
			m_infoCreate.size.y += INIT_SIZE.y;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_Y))
		{
			m_infoCreate.size.y -= INIT_SIZE.y;
		}
		if (pKeyboard->IsPress(KEY_UP_SIZE_Z))
		{
			m_infoCreate.size.z += INIT_SIZE.z;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE_Z))
		{
			m_infoCreate.size.z -= INIT_SIZE.z;
		}
	}

	// �傫����␳
	useful::LimitMinNum(m_infoCreate.size.x, INIT_SIZE.x);
	useful::LimitMinNum(m_infoCreate.size.y, INIT_SIZE.y);
	useful::LimitMinNum(m_infoCreate.size.z, INIT_SIZE.z);

	// �傫���𔽉f
	m_pChanger->SetVec3Sizing(m_infoCreate.size);
}

//===========================================
//  �����ύX�̍X�V����
//===========================================
void CEditChanger::ChangeDirection(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// ��ނ�ύX
	if (pKeyboard->IsTrigger(KEY_DIRECTION))
	{
		m_infoCreate.dir = (CCameraChanger::EDirection)((m_infoCreate.dir + 1) % CCameraChanger::DIRECTION_MAX);
	}

	// ��ނ𔽉f
	m_pChanger->SetDirection(m_infoCreate.dir);
}

//===========================================
//  �p�x�ύX�̍X�V����
//===========================================
void CEditChanger::ChangeRotation(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// ��ނ�ύX
	if (pKeyboard->IsTrigger(KEY_ROTATION))
	{
		m_infoCreate.rot = (CCameraChanger::ERotation)((m_infoCreate.rot + 1) % CCameraChanger::ROTATION_MAX);
	}

	// ��ނ𔽉f
	m_pChanger->SetRotation(m_infoCreate.rot);
}

//===========================================
//	�t�B�[���h�̐�������
//===========================================
void CEditChanger::CreateField(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
	D3DXCOLOR colChanger = XCOL_WHITE;			// �F�ۑ��p

	// �t�B�[���h��z�u
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	�t�B�[���h�̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V�E�����`���ON�ɂ���
		m_pChanger->SetEnableUpdate(true);
		m_pChanger->SetEnableDraw(true);

		// ���ۑ���ݒ�
		m_bSave = false;

		// �I�u�W�F�N�g�̐���
		m_pChanger = CCameraChanger::Create
		(
			GetVec3Position(), // �ʒu
			m_infoCreate.size, // �傫��
			m_infoCreate.dir, // ����
			m_infoCreate.rot // �p�x
		);
		assert(m_pChanger != nullptr);
	}
}

//===========================================
//	�t�B�[���h�̔j������
//===========================================
void CEditChanger::ReleaseField(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	bool bRelease = false;	// �j����

	// �t�B�[���h���폜
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// �j�������Ԃ�ݒ�
		bRelease = true;
	}

	// �t�B�[���h�̍폜����
	DeleteCollisionField(bRelease);
}

//===========================================
//	�t�B�[���h�̍폜����
//===========================================
void CEditChanger::DeleteCollisionField(const bool bRelase)
{
	// ���X�g���擾
	CListManager<CCameraChanger> *pListManager = CCameraChanger::GetList();
	if (pListManager == nullptr) { return; }
	std::list<CCameraChanger*> listChanger = pListManager->GetList();

	// ���X�g����S�Ċm�F����
	for (auto& rList : listChanger)
	{
		// �����A�h���X�������ꍇ����
		if (rList == m_pChanger) { continue; }

		D3DXVECTOR3 posOther = rList->GetVec3Position();	// �Ώۂ̒n�ʈʒu
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// ���g�̑傫��
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// �Ώۂ̑傫��

		// ���g�̑傫����ݒ�
		D3DXVECTOR3 sizeThisChanger = m_pChanger->GetVec3Sizing();	// ���g�̒n�ʂ̑傫��
		sizeThis.x = sizeThisChanger.x;	// ����T�C�YX��ݒ�
		sizeThis.y = sizeThisChanger.y;	// ����T�C�YY��ݒ�
		sizeThis.z = sizeThisChanger.z;	// ����T�C�YZ��ݒ�
		sizeThis *= 0.5f;				// ����T�C�Y�𔼕���

		// �Ώۂ̑傫����ݒ�
		D3DXVECTOR3 sizeOtherChanger = rList->GetVec3Sizing();	// �Ώۂ̒n�ʂ̑傫��
		sizeOther.x = sizeOtherChanger.x;	// ����T�C�YX��ݒ�
		sizeOther.y = sizeOtherChanger.y;	// ����T�C�YY��ݒ�
		sizeOther.z = sizeOtherChanger.z;	// ����T�C�YZ��ݒ�
		sizeOther *= 0.5f;				// ����T�C�Y�𔼕���

		// ��`�̓����蔻��
		if (collision::Box3D
		( // ����
			GetVec3Position(),	// ����ʒu
			posOther,	// ����ڕW�ʒu
			sizeThis,	// ����T�C�Y(�E�E��E��)
			sizeThis,	// ����T�C�Y(���E���E�O)
			sizeOther,	// ����ڕW�T�C�Y(�E�E��E��)
			sizeOther,	// ����ڕW�T�C�Y(���E���E�O)
			useful::RotToFourDire(GetVec3Rotation().y),			// ���������
			useful::RotToFourDire(rList->GetVec3Rotation().y)	// ����ڕW������
		))
		{ // ������������ꍇ

			if (bRelase)
			{ // �j������ꍇ

				// �I������
				rList->Uninit();

				// ���ۑ���ݒ�
				m_bSave = false;
			}
			else
			{ // �j�����Ȃ��ꍇ

				// �Ԃ�ݒ�
				rList->SetCubeColor(XCOL_RED);
			}
		}
		else
		{ // ����O�������ꍇ

			// �ʏ�F��ݒ�
			rList->SetCubeColor(XCOL_WHITE);
		}
	}
}

//===========================================
//	�ۑ�����
//===========================================
HRESULT CEditChanger::Save(void)
{
#if _DEBUG

	// �n�ʂ̃��X�g���擾
	CListManager<CCameraChanger> *pListManager = CCameraChanger::GetList();	// ���X�g�}�l�[�W���[
	std::list<CCameraChanger*> listChanger;	// �n�ʃ��X�g
	if (pListManager != nullptr)
	{ // ���X�g�}�l�[�W���[����������Ă���ꍇ

		// ���X�g���擾
		listChanger = pListManager->GetList();
	}

	// �t�@�C�����J��
	std::ofstream  file(SAVE_PASS);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�X�e�[�W�n�ʔz�u�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���o���������o��
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	�J�����ύX�n�_�z�u�̃Z�[�u�f�[�^ [save_Changer.txt]" << std::endl;
	file << "#	Author : ���� ����" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# ���̍s���牺���R�s�[�� [stage.txt] �ɒ���t��\n" << std::endl;

	// �����_�����o���̕��@���w��
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// �ǂݍ��݊J�n������������o��
	file << "STAGE_CHANGERSET\n" << std::endl;

	for (const auto& rList : listChanger)
	{ // �n�ʂ̑������J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pChanger) { continue; }

		// �����o�������擾
		CCameraChanger::EDirection dir = rList->GetDir(); // ����
		CCameraChanger::ERotation rot = rList->GetRot(); // �p�x
		D3DXVECTOR3 pos = rList->GetVec3Position(); // �ʒu
		D3DXVECTOR2 size = rList->GetVec2Sizing(); // �傫��

		// ���������o��
		file << "	CHANGERSET" << std::endl;
		file << "		DIR		= " << dir << std::endl;
		file << "		ROT		= " << rot << std::endl;
		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "		SIZE	= " << size.x << " " << size.y << std::endl;
		file << "	END_CHANGERSET\n" << std::endl;
	}

	// �ǂݍ��ݏI��������������o��
	file << "END_STAGE_CHANGERSET" << std::endl;

	// �t�B�[���h�̍폜����
	DeleteCollisionField(false);

	// �ۑ��ς݂ɂ���
	m_bSave = true;

	// ������Ԃ�
	return S_OK;

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}
