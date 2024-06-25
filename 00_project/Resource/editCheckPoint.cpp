#if 1
//============================================================
//
//	�G�f�B�b�g�`�F�b�N�|�C���g���� [editCheckPoint.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editManager.h"
#include "editCheckPoint.h"
#include "manager.h"
#include "collision.h"
#include "useful.h"
#include "stage.h"

#include "effect3D.h"

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
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_checkpoint.txt";	// �Z�[�u�e�L�X�g�p�X

	const float	INIT_ALPHA = 0.5f;	// �z�u�O�̃��l
	const int DIGIT_FLOAT = 2;		// �����_�ȉ��̌���

	const D3DXVECTOR3 DELETE_EFFECT_MOVE = D3DXVECTOR3(0.0f, 30.0f, 0.0f);	// �����G�t�F�N�g�̈ړ���
	const D3DXCOLOR DELETE_EFFECT_COL = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	// �����G�t�F�N�g�̐F
	const float DELETE_EFFECT_RADIUS = 100.0f;		// �����G�t�F�N�g�̔��a
	const float DELETE_EFFECT_SUB_RADIUS = 7.0f;	// �����G�t�F�N�g�̌��Z���锼�a
	const int DELETE_EFFECT_LIFE = 10;				// �����G�t�F�N�g�̎���
}

//************************************************************
//	�e�N���X [CEditCheckPoint] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditCheckPoint::CEditCheckPoint(CEditStage* pEditor) : CEditorObject(pEditor)
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
CEditCheckPoint::~CEditCheckPoint()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditCheckPoint::Init(void)
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

	// �`�F�b�N�|�C���g�̐���
	m_pPoint = CCheckPoint::Create
	( // ����
		GetVec3Position()		// �ʒu
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
void CEditCheckPoint::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorObject::Uninit();

	// �`�F�b�N�|�C���g�̏I��
	SAFE_UNINIT(m_pPoint);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditCheckPoint::Update(void)
{
#if _DEBUG

	// �e�N���X�̍X�V
	CEditorObject::Update();

	// �`�F�b�N�|�C���g�̐���
	CreateCheckPoint();

	// �`�F�b�N�|�C���g�̔j��
	ReleaseCheckPoint();

	// �ʒu�𔽉f
	m_pPoint->SetVec3Position(GetVec3Position());

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditCheckPoint::IsSave(void)
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
void CEditCheckPoint::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditCheckPoint::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditCheckPoint::DrawDebugControl(void)
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
void CEditCheckPoint::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorObject::DrawDebugInfo();

#endif	// _DEBUG
}

//============================================================
// �ʒu�X�V
//============================================================
void CEditCheckPoint::UpdatePosition(void)
{
	// �ʒu�̍X�V
	CEditorObject::UpdatePosition();

	// �ʒu�𔽉f
	m_pPoint->SetVec3Position(GetVec3Position());
}

//============================================================
//	�`�F�b�N�|�C���g�̐�������
//============================================================
void CEditCheckPoint::CreateCheckPoint(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu

	// �`�F�b�N�|�C���g��z�u
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	�`�F�b�N�|�C���g�̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V�E�����`���ON�ɂ���
		m_pPoint->SetEnableUpdate(true);
		m_pPoint->SetEnableDraw(true);

		// ���ۑ���ݒ�
		m_bSave = false;

		//----------------------------------------------------
		//	�V�����`�F�b�N�|�C���g�̐���
		//----------------------------------------------------
		// �`�F�b�N�|�C���g�̐���
		m_pPoint = CCheckPoint::Create
		( // ����
			GetVec3Position()		// �ʒu
		);
		assert(m_pPoint != nullptr);
	}
}

//============================================================
//	�`�F�b�N�|�C���g�̔j������
//============================================================
void CEditCheckPoint::ReleaseCheckPoint(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	bool bRelease = false;	// �j����

	// �`�F�b�N�|�C���g���폜
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// �j�������Ԃ�ݒ�
		bRelease = true;
	}

	// �`�F�b�N�|�C���g�̍폜����
	DeleteCollisionCheckPoint(bRelease);
}

//============================================================
//	�`�F�b�N�|�C���g�̍폜����
//============================================================
void CEditCheckPoint::DeleteCollisionCheckPoint(const bool bRelase)
{
	CListManager<CCheckPoint>* pListManager = CCheckPoint::GetList();	// �`�F�b�N�|�C���g���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
	std::list<CCheckPoint*> listCheckPoint = pListManager->GetList();	// �`�F�b�N�|�C���g���X�g���

	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
	for (auto& rList : listCheckPoint)
	{ // �`�F�b�N�|�C���g�����J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pPoint) { continue; }

		D3DXVECTOR3 posOther = rList->GetVec3Position();	// �Ώۂ̒n�ʈʒu
		D3DXVECTOR3 scaleThis = VEC3_ZERO;	// ���g�̑傫��
		D3DXVECTOR3 scaleOther = VEC3_ZERO;	// �Ώۂ̑傫��

		// ���g�̑傫����ݒ�
		scaleThis = D3DXVECTOR3
		(
			m_pPoint->GetRadius(),
			m_pPoint->GetRadius(),
			m_pPoint->GetRadius()
		);

		// �Ώۂ̑傫����ݒ�
		scaleOther = D3DXVECTOR3
		(
			rList->GetRadius(),
			rList->GetRadius(),
			rList->GetRadius()
		);

		// ��`�̓����蔻��
		if (collision::Box3D
		( // ����
			posEdit,	// ����ʒu
			posOther,	// ����ڕW�ʒu
			scaleThis,	// ����T�C�Y(�E�E��E��)
			scaleThis,	// ����T�C�Y(���E���E�O)
			scaleOther,	// ����ڕW�T�C�Y(�E�E��E��)
			scaleOther	// ����ڕW�T�C�Y(���E���E�O)
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

				// �G�t�F�N�g�𐶐�
				CEffect3D::Create
				(
					rList->GetVec3Position(),
					DELETE_EFFECT_RADIUS,
					CEffect3D::TYPE_NORMAL,
					DELETE_EFFECT_LIFE,
					DELETE_EFFECT_MOVE,
					VEC3_ZERO,
					DELETE_EFFECT_COL,
					DELETE_EFFECT_SUB_RADIUS
				);
			}
		}
	}
}

//============================================================
//	�ۑ�����
//============================================================
HRESULT CEditCheckPoint::Save(void)
{
#if _DEBUG

	// �n�ʂ̃��X�g���擾
	CListManager<CCheckPoint>* pListManager = CCheckPoint::GetList();	// ���X�g�}�l�[�W���[
	std::list<CCheckPoint*> listCheckPoint;	// �n�ʃ��X�g
	if (pListManager != nullptr)
	{ // ���X�g�}�l�[�W���[����������Ă���ꍇ

		// ���X�g���擾
		listCheckPoint = pListManager->GetList();
	}

	// �t�@�C�����J��
	std::ofstream  file(SAVE_PASS);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�`�F�b�N�|�C���g�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���o���������o��
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	�`�F�b�N�|�C���g�̃Z�[�u�f�[�^ [save_checkpoint.txt]" << std::endl;
	file << "#	Author : ���c �E��" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# ���̍s���牺���R�s�[�� [Point.txt] �ɒ���t��\n" << std::endl;

	// �����_�����o���̕��@���w��
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// �ǂݍ��݊J�n������������o��
	file << "STAGE_CHECKSET\n" << std::endl;

	for (const auto& rList : listCheckPoint)
	{ // �n�ʂ̑������J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pPoint) { continue; }

		// �����o�������擾
		D3DXVECTOR3 pos = rList->GetVec3Position();	// �ʒu

		// ���������o��
		file << "	CHECKSET" << std::endl;
		file << "		POS = " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "	END_CHECKSET\n" << std::endl;
	}

	// �ǂݍ��ݏI��������������o��
	file << "END_STAGE_CHECKSET" << std::endl;

	// �`�F�b�N�|�C���g�̍폜����
	DeleteCollisionCheckPoint(false);

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
