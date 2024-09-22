#if 1
//============================================================
//
//	�G�f�B�b�g�^�b�`�A�N�^�[���� [editTouchActor.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editTouchActor.h"
#include "editManager.h"
#include "manager.h"
#include "collision.h"
#include "useful.h"
#include "stage.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_CREATE		(DIK_0)	// �����L�[
#define NAME_CREATE		("0")	// �����\��
#define KEY_RELEASE		(DIK_9)	// �j���L�[
#define NAME_RELEASE	("9")	// �j���\��
#define KEY_TYPE		(DIK_4)	// ��ޕύX�L�[
#define NAME_TYPE		("4")	// ��ޕύX�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_touchActor.txt";	// �Z�[�u�e�L�X�g�p�X

	const float	INIT_ALPHA = 0.5f;	// �z�u�O�̃��l
	const int DIGIT_FLOAT = 2;		// �����_�ȉ��̌���
}

//************************************************************
//	�e�N���X [CEditTouchActor] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditTouchActor::CEditTouchActor(CEditStage* pEditor) : CEditorObject(pEditor)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pActor = nullptr;					// �^�b�`�A�N�^�[���
	m_type = CTouchActor::TYPE_STONE;	// ���
	m_bSave = false;					// �ۑ���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditTouchActor::~CEditTouchActor()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditTouchActor::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pActor = nullptr;						// ���
	m_bSave = false;						// �ۑ���

	// �e�N���X�̏�����
	if (FAILED(CEditorObject::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �^�b�`�A�N�^�[�̐���
	m_pActor = CTouchActor::Create(CTouchActor::TYPE_STONE, GetVec3Position());
	if (m_pActor == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}
	// �X�V���ʂ�Ȃ��悤�ɂ���
	m_pActor->SetLabel(CObject::LABEL_NONE);

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
void CEditTouchActor::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorObject::Uninit();

	// �^�b�`�A�N�^�[�̐F�̑S������
	InitAllColorTouchActor();

	// �^�b�`�A�N�^�[�̏I��
	SAFE_UNINIT(m_pActor);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditTouchActor::Update(void)
{
#if _DEBUG

	// ��ޕύX
	ChangeType();

	// �e�N���X�̍X�V
	CEditorObject::Update();

	// �^�b�`�A�N�^�[�̐���
	CreateTouchActor();

	// �^�b�`�A�N�^�[�̔j��
	ReleaseTouchActor();

	// �ʒu�𔽉f
	m_pActor->SetVec3Position(GetVec3Position());

	// �����𔽉f
	m_pActor->SetVec3Rotation(VEC3_ZERO);

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditTouchActor::IsSave(void)
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
void CEditTouchActor::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditTouchActor::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditTouchActor::DrawDebugControl(void)
{
#if _DEBUG

	// ����\���̕`��
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "��ޕύX�F[%s]\n", NAME_TYPE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�폜�F[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditTouchActor::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorObject::DrawDebugInfo();

#endif	// _DEBUG
}

//============================================================
//	��ޕύX�̍X�V����
//============================================================
void CEditTouchActor::ChangeType(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// ��ނ�ύX
	if (pKeyboard->IsTrigger(KEY_TYPE))
	{
		// ��ނ�1�i�߂�
		m_type = (CTouchActor::EType)((m_type + 1) % CTouchActor::TYPE_MAX);

		// �I������
		m_pActor->Uninit();

		// �M�~�b�N���Đ�������
		m_pActor = CTouchActor::Create
		( // ����
			m_type,				// ���
			GetVec3Position()	// �ʒu
		);
	}
}

//============================================================
//	�^�b�`�A�N�^�[�̐�������
//============================================================
void CEditTouchActor::CreateTouchActor(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// �^�b�`�A�N�^�[��z�u
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	�^�b�`�A�N�^�[�̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V�E�����`���ON�ɂ���
		m_pActor->SetEnableUpdate(true);
		m_pActor->SetEnableDraw(true);
		m_pActor->SetLabel(CObject::LABEL_TOUCHACTOR);

#ifdef _DEBUG

		// �����ʒu��ݒ肷��
		m_pActor->SetVec3PosInit(m_pActor->GetVec3Position());

#endif // _DEBUG

		// ���ۑ���ݒ�
		m_bSave = false;

		//----------------------------------------------------
		//	�V�����^�b�`�A�N�^�[�̐���
		//----------------------------------------------------
		// �^�b�`�A�N�^�[�̐���
		m_pActor = CTouchActor::Create(m_type, GetVec3Position());
		assert(m_pActor != nullptr);
		// �X�V���ʂ�Ȃ��悤�ɂ���
		m_pActor->SetLabel(CObject::LABEL_NONE);
	}
}

//============================================================
//	�^�b�`�A�N�^�[�̔j������
//============================================================
void CEditTouchActor::ReleaseTouchActor(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	bool bRelease = false;	// �j����

	// �^�b�`�A�N�^�[���폜
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// �j�������Ԃ�ݒ�
		bRelease = true;
	}

	// �^�b�`�A�N�^�[�̍폜����
	DeleteCollisionTouchActor(bRelease);
}

//============================================================
//	�^�b�`�A�N�^�[�̍폜����
//============================================================
void CEditTouchActor::DeleteCollisionTouchActor(const bool bRelase)
{
	CListManager<CTouchActor>* pListManager = CTouchActor::GetList();	// �^�b�`�A�N�^�[���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return; }							// ���X�g���g�p�̏ꍇ������
	std::list<CTouchActor*> listTouchActor = pListManager->GetList();	// �^�b�`�A�N�^�[���X�g���

	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
	for (auto& rList : listTouchActor)
	{ // �^�b�`�A�N�^�[�����J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pActor) { continue; }

		D3DXVECTOR3 posOther = rList->GetVec3Position();				// �Ώۂ̒n�ʈʒu
		D3DXVECTOR3 scaleThisMax = m_pActor->GetModelData().vtxMax;		// ���g�̍ő�l
		D3DXVECTOR3 scaleThisMin = -m_pActor->GetModelData().vtxMin;	// ���g�̍ŏ��l
		D3DXVECTOR3 scaleOtherMax = rList->GetModelData().vtxMax;		// �Ώۂ̍ő�l
		D3DXVECTOR3 scaleOtherMin = -rList->GetModelData().vtxMin;		// �Ώۂ̍ŏ��l

		// ��`�̓����蔻��
		if (collision::Box3D
		( // ����
			posEdit,		// ����ʒu
			posOther,		// ����ڕW�ʒu
			scaleThisMax,	// ����T�C�Y(�E�E��E��)
			scaleThisMin,	// ����T�C�Y(���E���E�O)
			scaleOtherMax,	// ����ڕW�T�C�Y(�E�E��E��)
			scaleOtherMin	// ����ڕW�T�C�Y(���E���E�O)
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
				rList->SetAllMaterial(material::Red());
			}
		}
		else
		{ // ����O�������ꍇ

			// �ʏ�F��ݒ�
			rList->ResetMaterial();
		}
	}
}

//============================================================
//	�^�b�`�A�N�^�[�̐F�̑S����������
//============================================================
void CEditTouchActor::InitAllColorTouchActor(void)
{
	CListManager<CTouchActor>* pListManager = CTouchActor::GetList();	// �^�b�`�A�N�^�[���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
	std::list<CTouchActor*> listTouchActor = pListManager->GetList();	// �^�b�`�A�N�^�[���X�g���

	for (auto& rList : listTouchActor)
	{ // �^�b�`�A�N�^�[�����J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pActor) { continue; }

		// �ʏ�F��ݒ�
		rList->ResetMaterial();
	}
}

//============================================================
//	�ۑ�����
//============================================================
HRESULT CEditTouchActor::Save(void)
{
#if _DEBUG

	// �_��̃��X�g���擾
	CListManager<CTouchActor>* pListManager = CTouchActor::GetList();	// ���X�g�}�l�[�W���[
	std::list<CTouchActor*> listTouchActor;	// �n�ʃ��X�g
	if (pListManager != nullptr)
	{ // ���X�g�}�l�[�W���[����������Ă���ꍇ

		// ���X�g���擾
		listTouchActor = pListManager->GetList();
	}

	// �t�@�C�����J��
	std::ofstream  file(SAVE_PASS);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�^�b�`�A�N�^�[�z�u�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���o���������o��
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	�^�b�`�A�N�^�[�z�u�̃Z�[�u�f�[�^ [save_touchactor.txt]" << std::endl;
	file << "#	Author : ���c �E��" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# ���̍s���牺���R�s�[�� [touchactor.txt] �ɒ���t��\n" << std::endl;

	// �^�b�`�A�N�^�[�̐F�̑S������
	InitAllColorTouchActor();

	// �����_�����o���̕��@���w��
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// �ǂݍ��݊J�n������������o��
	file << "STAGE_TOUCH_ACTORSET\n" << std::endl;

	for (const auto& rList : listTouchActor)
	{ // �n�ʂ̑������J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pActor) { continue; }

		// �����o�������擾
		CTouchActor::EType type = rList->GetType();		// ���
		D3DXVECTOR3 pos = rList->GetVec3PosInit();		// �����ʒu

		// ���������o��
		file << "	TOUCH_ACTORSET" << std::endl;
		file << "		TYPE	= " << type << std::endl;
		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "	END_TOUCH_ACTORSET\n" << std::endl;
	}

	// �ǂݍ��ݏI��������������o��
	file << "END_STAGE_TOUCH_ACTORSET" << std::endl;

	// �^�b�`�A�N�^�[�̍폜����
	DeleteCollisionTouchActor(false);

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
