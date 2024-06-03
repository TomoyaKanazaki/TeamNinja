#if 1
//============================================================
//
//	�G�f�B�b�g�M�~�b�N���� [editGimmick.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editGimmick.h"
#include "editManager.h"
#include "manager.h"
#include "collision.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_CREATE		(DIK_0)	// �����L�[
#define NAME_CREATE		("0")	// �����\��
#define KEY_RELEASE		(DIK_9)	// �j���L�[
#define NAME_RELEASE	("9")	// �j���\��
#define KEY_TYPE		(DIK_3)	// ��ޕύX�L�[
#define NAME_TYPE		("3")	// ��ޕύX�\��

#define KEY_UP_SIZE_X		(DIK_T)	// X���g��L�[
#define NAME_UP_SIZE_X		("T")	// X���g��\��
#define KEY_DOWN_SIZE_X		(DIK_G)	// X���k���L�[
#define NAME_DOWN_SIZE_X	("G")	// X���k���\��
#define KEY_UP_SIZE_Z		(DIK_Y)	// Z���g��L�[
#define NAME_UP_SIZE_Z		("Y")	// Z���g��\��
#define KEY_DOWN_SIZE_Z		(DIK_H)	// Z���k���L�[
#define NAME_DOWN_SIZE_Z	("H")	// Z���k���\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_gimmick.txt";	// �Z�[�u�e�L�X�g�p�X

	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(editstage::SIZE, 0.0f, editstage::SIZE);	// �����̑傫��
	const D3DXVECTOR3 SIZE_MOVE = D3DXVECTOR3(1.0f, 0.0f, 1.0f);						// �T�C�Y�̈ړ���
	const float	INIT_ALPHA = 0.5f;	// �z�u�O�̃��l
	const int DIGIT_FLOAT = 2;		// �����_�ȉ��̌���
}

//************************************************************
//	�e�N���X [CEditGimmick] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditGimmick::CEditGimmick()
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pGimmick = nullptr;	// �M�~�b�N���
	m_bSave = false;	// �ۑ���
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// �M�~�b�N�z�u���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditGimmick::~CEditGimmick()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditGimmick::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pGimmick = nullptr;				// ���
	m_infoCreate.type = CGimmick::TYPE_JUMPTABLE;	// ���
	m_infoCreate.size = INIT_SIZE;		// �傫��
	m_bSave = false;					// �ۑ���

	// �e�N���X�̏�����
	if (FAILED(CEditorObject::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �M�~�b�N�̐���
	D3DXCOLOR colGimmick = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// �n�ʐF
	m_pGimmick = CGimmick::Create
	( // ����
		GetVec3Position(),		// �ʒu
		m_infoCreate.size,		// �傫��
		m_infoCreate.type		// ���
	);
	if (m_pGimmick == nullptr)
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
void CEditGimmick::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorObject::Uninit();

	// �M�~�b�N�̐F�̑S������
	InitAllColorGimmick();

	// �M�~�b�N�̏I��
	SAFE_UNINIT(m_pGimmick);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditGimmick::Update(void)
{
#if _DEBUG

	// �e�N���X�̍X�V
	CEditorObject::Update();

	// �傫���̍X�V
	UpdateSize();

	// ��ނ̕ύX
	ChangeType();

	// �M�~�b�N�̐���
	CreateGimmick();

	// �M�~�b�N�̔j��
	ReleaseGimmick();

	// �ʒu�𔽉f
	m_pGimmick->SetVec3Position(GetVec3Position());

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditGimmick::IsSave(void)
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
void CEditGimmick::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditGimmick::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditGimmick::DrawDebugControl(void)
{
#if _DEBUG

	// ����\���̕`��
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "�傫���F[%s/%s/%s/%s+%s]\n", NAME_UP_SIZE_X, NAME_DOWN_SIZE_X, NAME_UP_SIZE_Z, NAME_DOWN_SIZE_Z, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "��ޕύX�F[%s]\n", NAME_TYPE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�폜�F[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditGimmick::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorObject::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%d�F[���]\n", m_infoCreate.type);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f�F[�傫��]\n", m_infoCreate.size.x, m_infoCreate.size.z);

#endif	// _DEBUG
}

//============================================================
//	�����X�V����
//============================================================
void CEditGimmick::UpdateRotation(void)
{
	// �����̍X�V
	CEditorObject::UpdateRotation();

	// �����𔽉f
	m_pGimmick->SetVec3Rotation(GetVec3Rotation());
}

//============================================================
//	�傫���̍X�V����
//============================================================
void CEditGimmick::UpdateSize(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

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
		if (pKeyboard->IsTrigger(KEY_UP_SIZE_Z))
		{
			m_infoCreate.size.z += INIT_SIZE.z;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_Z))
		{
			m_infoCreate.size.z -= INIT_SIZE.z;
		}
	}

	// �傫����␳
	useful::LimitMinNum(m_infoCreate.size.x, INIT_SIZE.x);
	useful::LimitMinNum(m_infoCreate.size.y, INIT_SIZE.y);

	// �傫���𔽉f
	m_pGimmick->SetVec3Sizing(m_infoCreate.size);
}

//============================================================
//	��ޕύX�̍X�V����
//============================================================
void CEditGimmick::ChangeType(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// ��ނ�ύX
	if (pKeyboard->IsTrigger(KEY_TYPE))
	{
		m_infoCreate.type = (CGimmick::EType)((m_infoCreate.type + 1) % CGimmick::TYPE_MAX);
	}

	// ��ނ𔽉f
	m_pGimmick->SetType(m_infoCreate.type);
}

//============================================================
//	�M�~�b�N�̐�������
//============================================================
void CEditGimmick::CreateGimmick(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
	D3DXCOLOR colGimmick = XCOL_YELLOW;			// �F�ۑ��p

	// �M�~�b�N��z�u
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	�M�~�b�N�̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V�E�����`���ON�ɂ���
		m_pGimmick->SetEnableUpdate(true);
		m_pGimmick->SetEnableDraw(true);

		// �F��ݒ�
		colGimmick = m_pGimmick->GetColor();	// ���̐F���擾
		m_pGimmick->SetColor(D3DXCOLOR(colGimmick.r, colGimmick.g, colGimmick.b, 1.0f));

		// ���ۑ���ݒ�
		m_bSave = false;

		//----------------------------------------------------
		//	�V�����M�~�b�N�̐���
		//----------------------------------------------------
		// �M�~�b�N�̐���
		m_pGimmick = CGimmick::Create
		( // ����
			GetVec3Position(),		// �ʒu
			m_infoCreate.size,		// �傫��
			m_infoCreate.type		// ���
		);
		assert(m_pGimmick != nullptr);

		// �F��ݒ�
		colGimmick = m_pGimmick->GetColor();	// ���̐F���擾
		m_pGimmick->SetColor(D3DXCOLOR(colGimmick.r, colGimmick.g, colGimmick.b, INIT_ALPHA));
	}
}

//============================================================
//	�M�~�b�N�̔j������
//============================================================
void CEditGimmick::ReleaseGimmick(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	bool bRelease = false;	// �j����

	// �M�~�b�N���폜
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// �j�������Ԃ�ݒ�
		bRelease = true;
	}

	// �M�~�b�N�̍폜����
	DeleteCollisionGimmick(bRelease);
}

//============================================================
//	�M�~�b�N�̍폜����
//============================================================
void CEditGimmick::DeleteCollisionGimmick(const bool bRelase)
{
	CListManager<CGimmick>* pListManager = CGimmick::GetList();	// �M�~�b�N���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
	std::list<CGimmick*> listGimmick = pListManager->GetList();	// �M�~�b�N���X�g���

	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
	for (auto& rList : listGimmick)
	{ // �M�~�b�N�����J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pGimmick) { continue; }

		D3DXVECTOR3 posOther = rList->GetVec3Position();	// �Ώۂ̒n�ʈʒu
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// ���g�̑傫��
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// �Ώۂ̑傫��

		// ���g�̑傫����ݒ�
		D3DXVECTOR3 sizeThisGimmick = m_pGimmick->GetVec3Sizing();	// ���g�̒n�ʂ̑傫��
		sizeThis.x = sizeThisGimmick.x;	// ����T�C�YX��ݒ�
		sizeThis.y = editstage::SIZE;	// ����T�C�YY��ݒ�
		sizeThis.z = sizeThisGimmick.z;	// ����T�C�YZ��ݒ�
		sizeThis *= 0.5f;				// ����T�C�Y�𔼕���

		// �Ώۂ̑傫����ݒ�
		D3DXVECTOR3 sizeOtherGimmick = rList->GetVec3Sizing();	// �Ώۂ̒n�ʂ̑傫��
		sizeOther.x = sizeOtherGimmick.x;	// ����T�C�YX��ݒ�
		sizeOther.y = editstage::SIZE;	// ����T�C�YY��ݒ�
		sizeOther.z = sizeOtherGimmick.z;	// ����T�C�YZ��ݒ�
		sizeOther *= 0.5f;				// ����T�C�Y�𔼕���

		// ��`�̓����蔻��
		if (collision::Box3D
		( // ����
			posEdit,	// ����ʒu
			posOther,	// ����ڕW�ʒu
			sizeThis,	// ����T�C�Y(�E�E��E��)
			sizeThis,	// ����T�C�Y(���E���E�O)
			sizeOther,	// ����ڕW�T�C�Y(�E�E��E��)
			sizeOther	// ����ڕW�T�C�Y(���E���E�O)
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
			rList->SetColor(XCOL_YELLOW);
		}
	}
}

//============================================================
//	�M�~�b�N�̐F�̑S����������
//============================================================
void CEditGimmick::InitAllColorGimmick(void)
{
	CListManager<CGimmick>* pListManager = CGimmick::GetList();	// �M�~�b�N���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
	std::list<CGimmick*> listGimmick = pListManager->GetList();	// �M�~�b�N���X�g���

	for (auto& rList : listGimmick)
	{ // �M�~�b�N�����J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pGimmick) { continue; }

		// �ʏ�F��ݒ�
		rList->SetColor(XCOL_YELLOW);
	}
}

//============================================================
//	�ۑ�����
//============================================================
HRESULT CEditGimmick::Save(void)
{
#if _DEBUG

	// �n�ʂ̃��X�g���擾
	CListManager<CGimmick>* pListManager = CGimmick::GetList();	// ���X�g�}�l�[�W���[
	std::list<CGimmick*> listGimmick;	// �n�ʃ��X�g
	if (pListManager != nullptr)
	{ // ���X�g�}�l�[�W���[����������Ă���ꍇ

		// ���X�g���擾
		listGimmick = pListManager->GetList();
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
	file << "#	�X�e�[�W�n�ʔz�u�̃Z�[�u�f�[�^ [save_gimmick.txt]" << std::endl;
	file << "#	Author : ���c �E��" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# ���̍s���牺���R�s�[�� [stage.txt] �ɒ���t��\n" << std::endl;

	// �M�~�b�N�̐F�̑S������
	InitAllColorGimmick();

	// �����_�����o���̕��@���w��
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// �ǂݍ��݊J�n������������o��
	file << "STAGE_GIMMICKSET\n" << std::endl;

	for (const auto& rList : listGimmick)
	{ // �n�ʂ̑������J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pGimmick) { continue; }

		// �����o�������擾
		CGimmick::EType type = rList->GetType();	// ���
		D3DXVECTOR3 pos = rList->GetVec3Position();	// �ʒu
		D3DXVECTOR3 rot = rList->GetVec3Rotation();	// ����
		D3DXVECTOR2 size = rList->GetVec2Sizing();	// �傫��
		D3DXCOLOR col = rList->GetColor();		// �F

		// ������360�x�ɕϊ�
		D3DXToDegree(rot);

		// ���������o��
		file << "	GIMMICKSET" << std::endl;
		file << "		TYPE	= " << type << std::endl;
		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "		SIZE	= " << size.x << " " << size.y << std::endl;
		file << "	END_GIMMICKSET\n" << std::endl;
	}

	// �ǂݍ��ݏI��������������o��
	file << "END_STAGE_GIMMICKSET" << std::endl;

	// �M�~�b�N�̍폜����
	DeleteCollisionGimmick(false);

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
