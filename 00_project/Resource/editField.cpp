#if 1
//============================================================
//
//	�G�f�B�b�g�t�B�[���h���� [editField.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editField.h"
#include "editManager.h"
#include "manager.h"
#include "collision.h"
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
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_field.txt";	// �Z�[�u�e�L�X�g�p�X

	const D3DXVECTOR2 INIT_SIZE = D3DXVECTOR2(editstage::SIZE, editstage::SIZE);	// �傫��
	const float	INIT_ALPHA = 0.5f;	// �z�u�O�̃��l
	const int DIGIT_FLOAT = 2;		// �����_�ȉ��̌���
}

//************************************************************
//	�e�N���X [CEditField] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditField::CEditField(CEditStage* pEditor) : CEditorObject(pEditor)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pField = nullptr;	// �t�B�[���h���
	m_bSave = false;	// �ۑ���
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// �t�B�[���h�z�u���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditField::~CEditField()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditField::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pField			 = nullptr;				// ���
	m_infoCreate.type	 = (CField::EType)0;	// ���
	m_infoCreate.size	 = INIT_SIZE;			// �傫��
	m_infoCreate.part	 = GRID2_ONE;			// ������
	m_infoCreate.texPart = GRID2_ONE;			// �e�N�X�`��������
	m_bSave				 = false;				// �ۑ���

	// �e�N���X�̏�����
	if (FAILED(CEditorObject::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �t�B�[���h�̐���
	D3DXCOLOR colField = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// �n�ʐF
	m_pField = CField::Create
	( // ����
		m_infoCreate.type,		// ���
		GetVec3Position(),		// �ʒu
		GetVec3Rotation(),		// ����
		m_infoCreate.size,		// �傫��
		colField,				// �F
		m_infoCreate.part,		// ������
		m_infoCreate.texPart	// �e�N�X�`��������
	);
	if (m_pField == nullptr)
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
void CEditField::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorObject::Uninit();

	// �t�B�[���h�̐F�̑S������
	InitAllColorField();

	// �t�B�[���h�̏I��
	SAFE_UNINIT(m_pField);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditField::Update(void)
{
#if _DEBUG

	// �e�N���X�̍X�V
	CEditorObject::Update();

	// �傫���̍X�V
	UpdateSizing();

	// �e�N�X�`�������̍X�V
	UpdateTexPart();

	// ��ނ̕ύX
	ChangeType();

	// �t�B�[���h�̐���
	CreateField();

	// �t�B�[���h�̔j��
	ReleaseField();

	// �ʒu�𔽉f
	m_pField->SetVec3Position(GetVec3Position());

	// �����𔽉f
	m_pField->SetVec3Rotation(GetVec3Rotation());

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditField::IsSave(void)
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
void CEditField::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditField::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditField::DrawDebugControl(void)
{
#if _DEBUG

	// ����\���̕`��
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "�傫���F[%s/%s/%s/%s+%s]\n", NAME_UP_SIZE_X, NAME_DOWN_SIZE_X, NAME_UP_SIZE_Y, NAME_DOWN_SIZE_Y, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "��ޕύX�F[%s]\n", NAME_TYPE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�폜�F[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditField::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorObject::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%d�F[���]\n", m_infoCreate.type);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f�F[�傫��]\n", m_infoCreate.size.x, m_infoCreate.size.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d %d�F[����]\n", m_infoCreate.part.x, m_infoCreate.part.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d %d�F[�e�N�X�`������]\n", m_infoCreate.texPart.x, m_infoCreate.texPart.y);

#endif	// _DEBUG
}

//============================================================
//	�����X�V����
//============================================================
void CEditField::UpdateRotation(void)
{
	// �����̍X�V
	CEditorObject::UpdateRotation();

	// �����𔽉f
	m_pField->SetVec3Rotation(GetVec3Rotation());
}

//============================================================
//	�傫���̍X�V����
//============================================================
void CEditField::UpdateSizing(void)
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
	}

	// �傫����␳
	useful::LimitMinNum(m_infoCreate.size.x, INIT_SIZE.x);
	useful::LimitMinNum(m_infoCreate.size.y, INIT_SIZE.y);

	// �傫���𔽉f
	m_pField->SetVec2Sizing(m_infoCreate.size);
}

//============================================================
//	�e�N�X�`�������̍X�V����
//============================================================
void CEditField::UpdateTexPart(void)
{
	// �e�N�X�`����������ݒ�
	m_infoCreate.texPart.x = (int)(m_infoCreate.size.x / INIT_SIZE.x);
	m_infoCreate.texPart.y = (int)(m_infoCreate.size.y / INIT_SIZE.y);

	// �e�N�X�`����������ݒ�
	m_pField->SetTexPattern(m_infoCreate.texPart);
}

//============================================================
//	��ޕύX�̍X�V����
//============================================================
void CEditField::ChangeType(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// ��ނ�ύX
	if (pKeyboard->IsTrigger(KEY_TYPE))
	{
		m_infoCreate.type = (CField::EType)((m_infoCreate.type + 1) % CField::TYPE_MAX);
	}

	// ��ނ𔽉f
	m_pField->SetType(m_infoCreate.type);
}

//============================================================
//	�t�B�[���h�̐�������
//============================================================
void CEditField::CreateField(void)
{
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
	D3DXCOLOR colField = XCOL_WHITE;			// �F�ۑ��p

	// �t�B�[���h��z�u
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	�t�B�[���h�̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V�E�����`���ON�ɂ���
		m_pField->SetEnableUpdate(true);
		m_pField->SetEnableDraw(true);

		// �F��ݒ�
		colField = m_pField->GetColor();	// ���̐F���擾
		m_pField->SetColor(D3DXCOLOR(colField.r, colField.g, colField.b, 1.0f));

		// ���ۑ���ݒ�
		m_bSave = false;

		//----------------------------------------------------
		//	�V�����t�B�[���h�̐���
		//----------------------------------------------------
		// �t�B�[���h�̐���
		m_pField = CField::Create
		( // ����
			m_infoCreate.type,		// ���
			GetVec3Position(),		// �ʒu
			GetVec3Rotation(),		// ����
			m_infoCreate.size,		// �傫��
			colField,				// �F
			m_infoCreate.part,		// ������
			m_infoCreate.texPart	// �e�N�X�`��������
		);
		assert(m_pField != nullptr);

		// �F��ݒ�
		colField = m_pField->GetColor();	// ���̐F���擾
		m_pField->SetColor(D3DXCOLOR(colField.r, colField.g, colField.b, INIT_ALPHA));
	}
}

//============================================================
//	�t�B�[���h�̔j������
//============================================================
void CEditField::ReleaseField(void)
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

//============================================================
//	�t�B�[���h�̍폜����
//============================================================
void CEditField::DeleteCollisionField(const bool bRelase)
{
	CListManager<CField> *pListManager = CField::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
	std::list<CField*> listField = pListManager->GetList();	// �t�B�[���h���X�g���

	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
	for (auto& rList : listField)
	{ // �t�B�[���h�����J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pField) { continue; }

		D3DXVECTOR3 posOther = rList->GetVec3Position();	// �Ώۂ̒n�ʈʒu
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// ���g�̑傫��
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// �Ώۂ̑傫��

		// ���g�̑傫����ݒ�
		D3DXVECTOR2 sizeThisField = m_pField->GetVec2Sizing();	// ���g�̒n�ʂ̑傫��
		sizeThis.x = sizeThisField.x;	// ����T�C�YX��ݒ�
		sizeThis.y = editstage::SIZE;	// ����T�C�YY��ݒ�
		sizeThis.z = sizeThisField.y;	// ����T�C�YZ��ݒ�
		sizeThis *= 0.5f;				// ����T�C�Y�𔼕���

		// �Ώۂ̑傫����ݒ�
		D3DXVECTOR2 sizeOtherField = rList->GetVec2Sizing();	// �Ώۂ̒n�ʂ̑傫��
		sizeOther.x = sizeOtherField.x;	// ����T�C�YX��ݒ�
		sizeOther.y = editstage::SIZE;	// ����T�C�YY��ݒ�
		sizeOther.z = sizeOtherField.y;	// ����T�C�YZ��ݒ�
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
//	�t�B�[���h�̐F�̑S����������
//============================================================
void CEditField::InitAllColorField(void)
{
	CListManager<CField> *pListManager = CField::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
	std::list<CField*> listField = pListManager->GetList();	// �t�B�[���h���X�g���

	for (auto& rList : listField)
	{ // �t�B�[���h�����J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pField) { continue; }

		// �ʏ�F��ݒ�
		rList->SetColor(XCOL_WHITE);
	}
}

//============================================================
//	�ۑ�����
//============================================================
HRESULT CEditField::Save(void)
{
#if _DEBUG

	// �n�ʂ̃��X�g���擾
	CListManager<CField> *pListManager = CField::GetList();	// ���X�g�}�l�[�W���[
	std::list<CField*> listField;	// �n�ʃ��X�g
	if (pListManager != nullptr)
	{ // ���X�g�}�l�[�W���[����������Ă���ꍇ

		// ���X�g���擾
		listField = pListManager->GetList();
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
	file << "#	�X�e�[�W�n�ʔz�u�̃Z�[�u�f�[�^ [save_field.txt]" << std::endl;
	file << "#	Author : ���c �E��" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# ���̍s���牺���R�s�[�� [stage.txt] �ɒ���t��\n" << std::endl;

	// �t�B�[���h�̐F�̑S������
	InitAllColorField();

	// �����_�����o���̕��@���w��
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// �ǂݍ��݊J�n������������o��
	file << "STAGE_FIELDSET\n" << std::endl;

	for (const auto& rList : listField)
	{ // �n�ʂ̑������J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pField) { continue; }

		// �����o�������擾
		CField::EType type	= rList->GetType();			// ���
		D3DXVECTOR3 pos		= rList->GetVec3Position();	// �ʒu
		D3DXVECTOR3 rot		= rList->GetVec3Rotation();	// ����
		D3DXVECTOR2 size	= rList->GetVec2Sizing();	// �傫��
		D3DXCOLOR col		= rList->GetColor();		// �F
		POSGRID2 part		= rList->GetPattern();		// ������
		POSGRID2 texPart	= rList->GetTexPattern();	// �e�N�X�`��������

		// ������360�x�ɕϊ�
		rot = D3DXToDegree(rot);

		// ���������o��
		file << "	FIELDSET" << std::endl;
		file << "		TYPE	= " << type << std::endl;
		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "		ROT		= " << rot.x << " " << rot.y << " " << rot.z << std::endl;
		file << "		SIZE	= " << size.x << " " << size.y << std::endl;
		file << "		COL		= " << col.r << " " << col.g << " " << col.b << " " << col.a << std::endl;
		file << "		PART	= " << part.x << " " << part.y << std::endl;
		file << "		TEXPART	= " << texPart.x << " " << texPart.y << std::endl;
		file << "	END_FIELDSET\n" << std::endl;
	}

	// �ǂݍ��ݏI��������������o��
	file << "END_STAGE_FIELDSET" << std::endl;

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
#endif
