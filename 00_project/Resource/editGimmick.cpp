#if 1
//============================================================
//
//	�G�f�B�b�g�M�~�b�N���� [editGimmick.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editGimmick.h"
#include "editManager.h"
#include "manager.h"
#include "stage.h"

#include "collision.h"
#include "object3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_CREATE				(DIK_0)	// �����L�[
#define NAME_CREATE				("0")	// �����\��
#define KEY_RELEASE				(DIK_9)	// �j���L�[
#define NAME_RELEASE			("9")	// �j���\��
#define KEY_TYPE				(DIK_4)	// ��ޕύX�L�[
#define NAME_TYPE				("4")	// ��ޕύX�\��
#define KEY_UP_NUM_ACTIVE		(DIK_5)	// �����\�l���̑����L�[
#define NAME_UP_NUM_ACTIVE		("5")	// �����\�l���̑����\��
#define KEY_DOWN_NUM_ACTIVE		(DIK_6)	// �����\�l���̌����L�[
#define NAME_DOWN_NUM_ACTIVE	("6")	// �����\�l���̌����\��

#define KEY_UP_SIZE_X		(DIK_T)	// X���g��L�[
#define NAME_UP_SIZE_X		("T")	// X���g��\��
#define KEY_DOWN_SIZE_X		(DIK_G)	// X���k���L�[
#define NAME_DOWN_SIZE_X	("G")	// X���k���\��
#define KEY_UP_SIZE_Y		(DIK_Y)	// Y���g��L�[
#define NAME_UP_SIZE_Y		("Y")	// Y���g��\��
#define KEY_DOWN_SIZE_Y		(DIK_H)	// Y���k���L�[
#define NAME_DOWN_SIZE_Y	("H")	// Y���k���\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_gimmick.txt";	// �Z�[�u�e�L�X�g�p�X
	const char* ANGLE_TEXTURE = "data\\TEXTURE\\EDITOR\\Angle.png";	// �����̃e�N�X�`��

	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(editstage::SIZE, 0.0f, editstage::SIZE);	// �傫��
	const float	INIT_ALPHA = 0.5f;		// �z�u�O�̃��l
	const int DIGIT_FLOAT = 2;			// �����_�ȉ��̌���
	const D3DXVECTOR3 ANGLE_SIZE = D3DXVECTOR3(50.0f, 0.0f, 50.0f);	// ���̃T�C�Y
	const D3DXVECTOR3 ANGLE_SHIFT = D3DXVECTOR3(0.0f, 30.0f, 0.0f);	// ���̍���

	const char* TYPE_NAME[] =	// �I�u�W�F�N�g�^�C�v��
	{
		"�W�����v��",
		"���ݑ�",
		"�ݒu",
		"��э~��",
		"�d����",
		"��",
		"������΂����",
	};
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TYPE_NAME) == CGimmick::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�e�N���X [CEditGimmick] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditGimmick::CEditGimmick(CEditStage* pEditor) : CEditorObject(pEditor)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pGimmick = nullptr;	// �M�~�b�N���
	m_pAngleSign = nullptr;	// ���̏��
	m_bSave = false;		// �ۑ���
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
	m_pGimmick = nullptr;					// �M�~�b�N
	m_infoCreate.type = CGimmick::EType(0);	// ���
	m_infoCreate.size = INIT_SIZE;			// �傫��
	m_bSave = false;						// �ۑ���

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
		GetVec3Position(),
		GetAngle(),
		m_infoCreate.size,
		m_infoCreate.type,
		m_infoCreate.nNumActive
	);
	if (m_pGimmick == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����𐶐�
	m_pAngleSign = CObject3D::Create
	(
		GetVec3Position() + ANGLE_SHIFT,
		ANGLE_SIZE,
		GetVec3Rotation()
	);
	if (m_pAngleSign == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`���̊��蓖�ď���
	m_pAngleSign->BindTexture(ANGLE_TEXTURE);

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

	// �����̏I��
	SAFE_UNINIT(m_pAngleSign);

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

	// �����\�l���̍X�V
	UpdateNumActive();

	// �M�~�b�N�̐���
	CreateGimmick();

	// �M�~�b�N�̔j��
	ReleaseGimmick();

	// �ʒu�𔽉f
	m_pGimmick->SetVec3Position(GetVec3Position());

	// �����𔽉f
	m_pGimmick->SetVec3Rotation(GetVec3Rotation());

	// ���̈ʒu�𔽉f
	m_pAngleSign->SetVec3Position(GetVec3Position() + ANGLE_SHIFT);

	// ���̌����𔽉f
	m_pAngleSign->SetVec3Rotation(GetVec3Rotation());

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

	DebugProc::Print(DebugProc::POINT_RIGHT, "�傫���F[%s/%s/%s/%s+%s]\n", NAME_UP_SIZE_X, NAME_DOWN_SIZE_X, NAME_UP_SIZE_Y, NAME_DOWN_SIZE_Y, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "��ޕύX�F[%s]\n", NAME_TYPE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�����\�l���F[%s/%s]\n", NAME_UP_NUM_ACTIVE, NAME_DOWN_NUM_ACTIVE);
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

	DebugProc::Print(DebugProc::POINT_RIGHT, "%s�F[���]\n", TYPE_NAME[m_infoCreate.type]);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f�F[�傫��]\n", m_infoCreate.size.x, m_infoCreate.size.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d�F[�����\�l��]\n", m_infoCreate.nNumActive);

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
		if (pKeyboard->IsPress(KEY_UP_SIZE_Y))
		{
			m_infoCreate.size.z += INIT_SIZE.z;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE_Y))
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
			m_infoCreate.size.z += INIT_SIZE.z;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE_Y))
		{
			m_infoCreate.size.z -= INIT_SIZE.z;
		}
	}

	// �傫����␳
	useful::LimitMinNum(m_infoCreate.size.x, INIT_SIZE.x);
	useful::LimitMinNum(m_infoCreate.size.z, INIT_SIZE.z);

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

		// �I������
		m_pGimmick->Uninit();

		// �M�~�b�N���Đ�������
		m_pGimmick = CGimmick::Create
		( // ����
			GetVec3Position(),			// �ʒu
			GetAngle(),					// ����
			m_infoCreate.size,			// �T�C�Y
			m_infoCreate.type,			// ���
			m_infoCreate.nNumActive		// �����\�l��
		);
	}
}

//============================================================
// �����\�l���X�V
//============================================================
void CEditGimmick::UpdateNumActive(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// �����\�l����ύX
	if (pKeyboard->IsTrigger(KEY_UP_NUM_ACTIVE))
	{
		m_infoCreate.nNumActive++;
	}
	if (pKeyboard->IsTrigger(KEY_DOWN_NUM_ACTIVE))
	{
		m_infoCreate.nNumActive--;
	}

	// �����\�l���𔽉f
	m_pGimmick->SetNumActive(m_infoCreate.nNumActive);
}

//============================================================
//	�M�~�b�N�̐�������
//============================================================
void CEditGimmick::CreateGimmick(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
	D3DXCOLOR colGimmick = XCOL_WHITE;			// �F�ۑ��p

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
			GetVec3Position(),			// �ʒu
			GetAngle(),					// ����
			m_infoCreate.size,			// �T�C�Y
			m_infoCreate.type,			// ���
			m_infoCreate.nNumActive		// �����\�l��
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
		sizeThis.z = sizeThisGimmick.y;	// ����T�C�YZ��ݒ�
		sizeThis *= 0.5f;				// ����T�C�Y�𔼕���

		// �Ώۂ̑傫����ݒ�
		D3DXVECTOR3 sizeOtherGimmick = rList->GetVec3Sizing();	// �Ώۂ̒n�ʂ̑傫��
		sizeOther.x = sizeOtherGimmick.x;	// ����T�C�YX��ݒ�
		sizeOther.y = editstage::SIZE;	// ����T�C�YY��ݒ�
		sizeOther.z = sizeOtherGimmick.y;	// ����T�C�YZ��ݒ�
		sizeOther *= 0.5f;				// ����T�C�Y�𔼕���

		// ��`�̓����蔻��
		if (collision::Box3D
		( // ����
			posEdit,	// ����ʒu
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
				rList->SetColor(XCOL_RED);
			}
		}
		else
		{ // ����O�������ꍇ

			// �ʏ�F��ݒ�
			rList->SetColor(XCOL_WHITE);
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
		rList->SetColor(XCOL_WHITE);
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
		MessageBox(nullptr, "�M�~�b�N�z�u�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���o���������o��
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	�M�~�b�N�z�u�̃Z�[�u�f�[�^ [save_gimmick.txt]" << std::endl;
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
		D3DXVECTOR3 pos = rList->GetVec3Position();	// �ʒu
		EAngle angle = rList->GetAngle();			// ����
		D3DXVECTOR3 size = rList->GetVec3Sizing();	// �T�C�Y
		CGimmick::EType type = rList->GetType();	// ���
		int nNumActive = rList->GetNumActive();		// �����\�l��

		// ���������o��
		file << "	GIMMICKSET" << std::endl;
		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "		ANGLE	= " << angle << std::endl;
		file << "		SIZE	= " << size.x << " " << size.z << std::endl;
		file << "		TYPE	= " << type << std::endl;
		file << "		NUMACT	= " << nNumActive << std::endl;
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
