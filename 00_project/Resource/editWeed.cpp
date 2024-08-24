//===========================================
//
//  ���G�f�B�^�[(editWeed.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "editWeed.h"
#include "editManager.h"
#include "manager.h"
#include "collision.h"
#include "stage.h"
#include "objectMeshCylinder.h"

//===========================================
//  �萔�錾
//===========================================
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_weed.txt"; // �Z�[�u�e�L�X�g�p�X

	const float	INIT_ALPHA = 0.5f; // �z�u�O�̃��l
	const int DIGIT_FLOAT = 2; // �����_�ȉ��̌���

	const int KEY_CREATE		= DIK_0; // �����L�[
	const int KEY_RELEASE		= DIK_9; // �j���L�[
	const int KEY_UP_SIZE		= DIK_T; // �g��L�[
	const int KEY_DOWN_SIZE		= DIK_G; // �k���L�[
	const int KEY_UP_TIME		= DIK_Y; // �g��L�[
	const int KEY_DOWN_TIME		= DIK_H; // �k���L�[
	const int KEY_UP_NUM		= DIK_U; // �g��L�[
	const int KEY_DOWN_NUM		= DIK_J; // �k���L�[

	const char* NAME_CREATE			= "0"; // �����\��
	const char* NAME_RELEASE		= "9"; // �j���\��
	const char* NAME_UP_SIZE		= "T"; // �g��\��
	const char* NAME_DOWN_SIZE		= "G"; // �k���\��
	const char* NAME_UP_TIME		= "Y"; // �g��\��
	const char* NAME_DOWN_TIME		= "H"; // �k���\��
	const char* NAME_UP_NUM			= "U"; // �g��\��
	const char* NAME_DOWN_NUM		= "J"; // �k���\��

	int CREATE_TIME = 30; // �����p�x
	const int INIT_NUM = 3; // ����������
	const float PERMISSION = 0.05f; // �d�Ȃ�̋��e�͈�

}

//===========================================
//	�R���X�g���N�^
//===========================================
CEditWeed::CEditWeed(CEditStage* pEditor) : CEditorObject(pEditor),
m_pCylinder(nullptr),
m_bSave(false),
m_nCoolTime(0),
m_nNum(0)
{
	// �����o�ϐ����N���A
	memset(&m_infoCreate, 0, sizeof(m_infoCreate)); // �z�u���
}

//===========================================
//	�f�X�g���N�^
//===========================================
CEditWeed::~CEditWeed()
{
}

//===========================================
//	����������
//===========================================
HRESULT CEditWeed::Init(void)
{
#if _DEBUG

	// �e�N���X�̏�����
	if (FAILED(CEditorObject::Init())) { assert(false); return E_FAIL; }

	// �I�u�W�F�N�g�̐���
	m_pCylinder = CObjectMeshCylinder::Create
	(
		GetVec3Position(),					// �ʒu
		VEC3_ZERO,							// ����
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),	// �F
		POSGRID2(16, 1),					// ������
		POSGRID2(1, 1),						// �e�N�X�`��������
		m_infoCreate.fSize,					// ���a
		25.0f								// ����
	);
	m_pCylinder->GetRenderState()->SetCulling(D3DCULL_NONE);	// �J�����O�I�t

	// �������̏�����
	m_nCoolTime = CREATE_TIME;
	m_nNum = INIT_NUM;

	//�����Ɏ��s�����ꍇ�֐��𔲂���
	if (m_pCylinder == nullptr) { assert(false); return E_FAIL; }

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
void CEditWeed::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorObject::Uninit();

	// �I�u�W�F�N�g�̏I��
	SAFE_UNINIT(m_pCylinder);

#endif	// _DEBUG
}

//===========================================
//	�X�V����
//===========================================
void CEditWeed::Update(void)
{
#if _DEBUG

	// �e�N���X�̍X�V
	CEditorObject::Update();

	// �傫���̍X�V
	UpdateSize();

	// �����p�x�̍X�V
	UpdateTime();

	// �������̍X�V
	UpdateNum();

	// ����
	Create();

	// �j��
	Release();

	// �ʒu�𔽉f
	m_pCylinder->SetVec3Position(GetVec3Position());

	// �����𔽉f
	m_pCylinder->SetVec3Rotation(GetVec3Rotation());

#endif	// _DEBUG
}

//===========================================
//	�ۑ��󋵎擾����
//===========================================
bool CEditWeed::IsSave(void)
{
#if _DEBUG

	// �ۑ��󋵂�Ԃ�
	return m_bSave;

#else	// NDEBUG

	// false��Ԃ�
	return false;

#endif	// _DEBUG
}

//===========================================
//	���ۑ�����
//===========================================
void CEditWeed::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//===========================================
//	���Ǎ�����
//===========================================
void CEditWeed::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//===========================================
//	����\���̕`�揈��
//===========================================
void CEditWeed::DrawDebugControl(void)
{
#if _DEBUG

	// ����\���̕`��
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "�����͈́F[ %s / %s+%s ]\n",
		NAME_UP_SIZE, NAME_DOWN_SIZE, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�����p�x�F[ %s / %s+%s ]\n",
		NAME_UP_TIME, NAME_DOWN_TIME, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "������  �F[ %s / %s ]\n",
		NAME_UP_NUM, NAME_DOWN_NUM);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�폜�F[ %s ]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�F[ %s ]\n", NAME_CREATE);

#endif	// _DEBUG
}

//===========================================
//	���\���̕`�揈��
//===========================================
void CEditWeed::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorObject::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f�F[ �����͈� ]\n", m_infoCreate.fSize);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d�F[ �����p�x ]\n", CREATE_TIME);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d�F  [ ������ ]\n", m_nNum);

#endif	// _DEBUG
}

//===========================================
//	�傫���̍X�V����
//===========================================
void CEditWeed::UpdateSize(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// �傫����ύX
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_SIZE))
		{
			m_infoCreate.fSize += editstage::SIZE * 0.1f;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE))
		{
			m_infoCreate.fSize -= editstage::SIZE * 0.1f;
		}
	}
	else
	{
		if (pKeyboard->IsTrigger(KEY_UP_SIZE))
		{
			m_infoCreate.fSize += editstage::SIZE * 0.1f;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE))
		{
			m_infoCreate.fSize -= editstage::SIZE * 0.1f;
		}
	}

	// �傫����␳
	useful::LimitMinNum(m_infoCreate.fSize, editstage::SIZE);

	// �傫���𔽉f
	m_pCylinder->SetRadius(m_infoCreate.fSize);
}

//==========================================
//  �����p�x�̍X�V����
//==========================================
void CEditWeed::UpdateTime(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// �����p�x�̕ύX
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_TIME))
		{
			CREATE_TIME += 1;
		}
		if (pKeyboard->IsPress(KEY_DOWN_TIME))
		{
			CREATE_TIME -= 1;
		}
	}
	else
	{
		if (pKeyboard->IsTrigger(KEY_UP_TIME))
		{
			CREATE_TIME += 1;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_TIME))
		{
			CREATE_TIME -= 1;
		}
	}

	// �����p�x�̕␳
	if (CREATE_TIME < 1)
	{
		CREATE_TIME = 1;
	}
}

//==========================================
//  �������̍X�V����
//==========================================
void CEditWeed::UpdateNum(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// �������̕ύX
	if (pKeyboard->IsTrigger(KEY_UP_NUM))
	{
		m_nNum += 1;
	}
	if (pKeyboard->IsTrigger(KEY_DOWN_NUM))
	{
		m_nNum -= 1;
	}

	// �������̕␳
	if (m_nNum < 1)
	{
		m_nNum = 1;
	}
}

//===========================================
//	��������
//===========================================
void CEditWeed::Create(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// �z�u
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		// ���ۑ���ݒ�
		m_bSave = false;

		// ����
		D3DXVECTOR3 posCent = GetVec3Position();	// �������S�ʒu
		D3DXVECTOR3 posSet;	// �ʒu�ݒ�p
		D3DXVECTOR3 rotSet;	// �����ݒ�p
		for (int i = 0; i < m_nNum; ++i)
		{
			// �����ʒu��ݒ�
			posSet = posCent;
			posSet.x += (float)(rand() % ((int)m_infoCreate.fSize * 2) - (int)m_infoCreate.fSize + 1);
			posSet.y += 0.0f;
			posSet.z += (float)(rand() % ((int)m_infoCreate.fSize * 2) - (int)m_infoCreate.fSize + 1);
			float fDistance = (float)(rand() % 100 + 1) * 0.01f;

			// �����ʒu��ݒ肷��
			D3DXVECTOR3 pos = GetVec3Position();
			pos.x += sinf(fTheta) * m_pCylinder->GetRadius() * fDistance;
			pos.z += cosf(fTheta) * m_pCylinder->GetRadius() * fDistance;

			// ����������ݒ�
			rotSet = D3DXVECTOR3(0.0f, (float)(rand() % 628 + 1) * 0.01f, 0.0f);

			// ���I�u�W�F�N�g�̐���
			CWeed::Create(posSet, rotSet);
		}
	}
}

//===========================================
//	�j������
//===========================================
void CEditWeed::Release(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	bool bRelease = false;	// �j����

	// �폜
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// �j�������Ԃ�ݒ�
		bRelease = true;
	}

	// �폜����
	DeleteCollision(bRelease);
}

//===========================================
//	�폜����
//===========================================
void CEditWeed::DeleteCollision(const bool bRelase)
{
	// ���X�g���擾
	CListManager<CWeed> *pListManager = CWeed::GetList();
	if (pListManager == nullptr) { return; }
	std::list<CWeed*> listWeed = pListManager->GetList();

	// ���X�g����S�Ċm�F����
	for (auto& rList : listWeed)
	{
		D3DXVECTOR3 posOther = rList->GetVec3Position();	// �Ώۂ̒n�ʈʒu

		// �~�`�̓����蔻��
		if (collision::Circle3D
		( // ����
			GetVec3Position(),	// ����ʒu
			posOther,			// ����ڕW�ʒu
			m_pCylinder->GetRadius(),	// ���蔼�a
			50.0f
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
				rList->SetColor(XCOL_RED);
			}
		}
		else
		{ // ����O�������ꍇ

			// �ʏ�F��ݒ�
			rList->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//===========================================
//	�ۑ�����
//===========================================
HRESULT CEditWeed::Save(void)
{
#if _DEBUG

	// �n�ʂ̃��X�g���擾
	CListManager<CWeed> *pListManager = CWeed::GetList();	// ���X�g�}�l�[�W���[
	std::list<CWeed*> listChanger;	// �n�ʃ��X�g
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
		MessageBox(nullptr, "���̏����o���Ɏ��s^^", "�G���[��www", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���o���������o��
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	���̃Z�[�u�f�[�^w [save_weed.txt]" << std::endl;
	file << "#	Author : ���� ����" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# ���̍s���牺���R�s�[�� [stage.txt] �ɒ���t��\n" << std::endl;

	// �����_�����o���̕��@���w��
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// �ǂݍ��݊J�n������������o��
	file << "STAGE_WEEDSET\n" << std::endl;

	for (const auto& rList : listChanger)
	{ // ���̑������J��Ԃ�

		// �����o�������擾
		D3DXVECTOR3 pos = rList->GetVec3Position(); // �ʒu
		D3DXVECTOR3 rot = rList->GetVec3Rotation(); // ����

		// ���������o��
		file << "	WEEDSET" << std::endl;
		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "		ROT		= " << rot.x << " " << rot.y << " " << rot.z << std::endl;
		file << "	END_WEEDSET\n" << std::endl;
	}

	// �ǂݍ��ݏI��������������o��
	file << "END_STAGE_WEEDSET" << std::endl;

	// �폜����
	DeleteCollision(false);

	// �ۑ��ς݂ɂ���
	m_bSave = true;

	// ������Ԃ�
	return S_OK;

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}
