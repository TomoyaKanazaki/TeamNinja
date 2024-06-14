#if 1
//============================================================
//
//	�G�f�B�b�g�S�[���|�C���g���� [editGoalPoint.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editManager.h"
#include "editGoalPoint.h"
#include "manager.h"
#include "collision.h"
#include "useful.h"
#include "stage.h"
#include "gameManager.h"
#include "sceneGame.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_CREATE		(DIK_0)	// �����L�[
#define NAME_CREATE		("0")	// �����\��
#define KEY_RELEASE		(DIK_9)	// �j���L�[
#define NAME_RELEASE	("9")	// �j���\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_goalpoint.txt";	// �Z�[�u�e�L�X�g�p�X

	const float	INIT_ALPHA = 0.5f;	// �z�u�O�̃��l
	const int DIGIT_FLOAT = 2;		// �����_�ȉ��̌���
}

//************************************************************
//	�e�N���X [CEditGoalPoint] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditGoalPoint::CEditGoalPoint()
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pPoint = nullptr;	// �|�C���g
	m_bSave = false;	// �ۑ���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditGoalPoint::~CEditGoalPoint()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditGoalPoint::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pPoint = nullptr;		// �|�C���g
	m_bSave = false;		// �ۑ���

	// �e�N���X�̏�����
	if (FAILED(CEditorObject::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �S�[���|�C���g�̐���
	m_pPoint = CGoal::Create
	( // ����
		GetVec3Position(),				// �ʒu
		D3DXVECTOR3(0.0f, 0.01f, 0.0f)	// ����
	);
	if (m_pPoint == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

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
void CEditGoalPoint::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorObject::Uninit();

	// �S�[���|�C���g�̏I��
	SAFE_UNINIT(m_pPoint);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditGoalPoint::Update(void)
{
#if _DEBUG

	// �e�N���X�̍X�V
	CEditorObject::Update();

	// �S�[���|�C���g�̐���
	CreateCheckPoint();

	// �S�[���|�C���g�̔j��
	ReleaseCheckPoint();

	// �ʒu�𔽉f
	m_pPoint->SetVec3Position(GetVec3Position());

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditGoalPoint::IsSave(void)
{
#if _DEBUG

	// �ۑ��󋵂�Ԃ�
	return m_bSave;

#else	// NDEBUG

	// false��Ԃ�
	return false;

#endif	// _DEBUG
}

//============================================================
//	���ۑ�����
//============================================================
void CEditGoalPoint::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditGoalPoint::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditGoalPoint::DrawDebugControl(void)
{
#if _DEBUG

	// ����\���̕`��
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "�폜�F[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditGoalPoint::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorObject::DrawDebugInfo();

	if (CSceneGame::GetGameManager()->GetGoal() == nullptr)
	{ // �S�[���� NULL �̏ꍇ

		DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�\\n");
	}
	else
	{ // ��L�ȊO

		DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�s�\\n");
	}

#endif	// _DEBUG
}

//============================================================
// �ʒu�X�V
//============================================================
void CEditGoalPoint::UpdatePosition(void)
{
	// �ʒu�̍X�V
	CEditorObject::UpdatePosition();

	// �ʒu�𔽉f
	m_pPoint->SetVec3Position(GetVec3Position());
}

//============================================================
//	�S�[���|�C���g�̐�������
//============================================================
void CEditGoalPoint::CreateCheckPoint(void)
{
	CGoal* pGoal = CSceneGame::GetGameManager()->GetGoal();		// �S�[�����
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu

	// �S�[���|�C���g��z�u
	if (pKeyboard->IsTrigger(KEY_CREATE) &&
		pGoal == nullptr)
	{
		//----------------------------------------------------
		//	�S�[���|�C���g�̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V�E�����`���ON�ɂ���
		m_pPoint->SetEnableUpdate(true);
		m_pPoint->SetEnableDraw(true);

		// �S�[����ݒ肷��
		CSceneGame::GetGameManager()->SetGoal(m_pPoint);

		// ���ۑ���ݒ�
		m_bSave = false;

		//----------------------------------------------------
		//	�V�����S�[���|�C���g�̐���
		//----------------------------------------------------
		// �S�[���|�C���g�̐���
		m_pPoint = CGoal::Create
		( // ����
			GetVec3Position(),				// �ʒu
			D3DXVECTOR3(0.0f, 0.01f, 0.0f)	// ����
		);
		assert(m_pPoint != nullptr);
	}
}

//============================================================
//	�S�[���|�C���g�̔j������
//============================================================
void CEditGoalPoint::ReleaseCheckPoint(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	bool bRelease = false;	// �j����

	// �S�[���|�C���g���폜
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// �j�������Ԃ�ݒ�
		bRelease = true;
	}

	// �S�[���|�C���g�̍폜����
	DeleteCollisionCheckPoint(bRelease);
}

//============================================================
//	�S�[���|�C���g�̍폜����
//============================================================
void CEditGoalPoint::DeleteCollisionCheckPoint(const bool bRelase)
{
	// �S�[�����擾
	CGoal* pGoal = CSceneGame::GetGameManager()->GetGoal();

	if (pGoal != nullptr)
	{ // �S�[���� NULL ����Ȃ��ꍇ

		if (bRelase == true)
		{ // �폜����ꍇ

			// �S�[�����I������
			pGoal->Uninit();
			CSceneGame::GetGameManager()->SetGoal(nullptr);
		}
	}
}

//============================================================
//	�ۑ�����
//============================================================
HRESULT CEditGoalPoint::Save(void)
{
#if _DEBUG

	// �S�[���̏����擾
	CGoal* pGoal = CSceneGame::GetGameManager()->GetGoal();

	// �S�[���� NULL �̏ꍇ�A������
	if (pGoal == nullptr) { return S_OK; }

	// �t�@�C�����J��
	std::ofstream  file(SAVE_PASS);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�S�[���|�C���g�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���o���������o��
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	�S�[���|�C���g�̃Z�[�u�f�[�^ [save_goalpoint.txt]" << std::endl;
	file << "#	Author : ���c �E��" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# ���̍s���牺���R�s�[�� [Point.txt] �ɒ���t��\n" << std::endl;

	// �����_�����o���̕��@���w��
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// �ǂݍ��݊J�n������������o��
	file << "STAGE_GOALSET\n" << std::endl;

	// �ʒu�������o��
	D3DXVECTOR3 pos = pGoal->GetVec3Position();	// �ʒu
	file << "		POS = " << pos.x << " " << pos.y << " " << pos.z << std::endl;

	// �ǂݍ��ݏI��������������o��
	file << "END_STAGE_GOALSET" << std::endl;

	// �ۑ��ς݂ɂ���
	m_bSave = true;

	// ������Ԃ�
	return S_OK;

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}
#endif
