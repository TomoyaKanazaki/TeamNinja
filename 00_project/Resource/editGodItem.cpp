#if 1
//============================================================
//
//	�G�f�B�b�g�_�폈�� [editGodItem.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editGodItem.h"
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
	const char* MODEL = "data\\MODEL\\GODITEM\\Magatama.x";			// ���f��
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_goditem.txt";	// �Z�[�u�e�L�X�g�p�X

	const float	INIT_ALPHA = 0.5f;	// �z�u�O�̃��l
	const int DIGIT_FLOAT = 2;		// �����_�ȉ��̌���
}

//************************************************************
//	�e�N���X [CEditGodItem] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditGodItem::CEditGodItem(CEditStage* pEditor) : CEditorObject(pEditor)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pGodItem = nullptr;	// �_����
	m_bSave = false;	// �ۑ���
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// �_��z�u���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditGodItem::~CEditGodItem()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditGodItem::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pGodItem = nullptr;					// ���
	m_infoCreate.type = (CGodItem::EType)0;	// ���
	m_bSave = false;						// �ۑ���

	// �e�N���X�̏�����
	if (FAILED(CEditorObject::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �S�[���|�C���g�̐���
	m_pGodItem = CObjectModel::Create
	( // ����
		GetVec3Position(),		// �ʒu
		VEC3_ZERO				// ����
	);
	if (m_pGodItem == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f�������蓖�Ă�
	m_pGodItem->BindModel(MODEL);

	// �}�e���A���̊��蓖��
	switch (m_infoCreate.type)
	{
	case CGodItem::TYPE_RED:
		m_pGodItem->SetAllMaterial(material::Red());	// �ԐF
		break;

	case CGodItem::TYPE_GREEN:
		m_pGodItem->SetAllMaterial(material::Green());	// �ΐF
		break;

	case CGodItem::TYPE_BLUE:
		m_pGodItem->SetAllMaterial(material::Blue());	// �F
		break;

	default:
		assert(false);
		break;
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
void CEditGodItem::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorObject::Uninit();

	// �_��̏I��
	SAFE_UNINIT(m_pGodItem);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditGodItem::Update(void)
{
#if _DEBUG

	// �e�N���X�̍X�V
	CEditorObject::Update();

	// ��ނ̕ύX
	ChangeType();

	// �_��̐���
	CreateGodItem();

	// �_��̔j��
	ReleaseGodItem();

	// �ʒu�𔽉f
	m_pGodItem->SetVec3Position(GetVec3Position());

	// �����𔽉f
	m_pGodItem->SetVec3Rotation(VEC3_ZERO);

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditGodItem::IsSave(void)
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
void CEditGodItem::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditGodItem::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditGodItem::DrawDebugControl(void)
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
void CEditGodItem::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorObject::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%d�F[���]\n", m_infoCreate.type);

#endif	// _DEBUG
}

//============================================================
//	��ޕύX�̍X�V����
//============================================================
void CEditGodItem::ChangeType(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// ��ނ�ύX
	if (pKeyboard->IsTrigger(KEY_TYPE))
	{
		m_infoCreate.type = (CGodItem::EType)((m_infoCreate.type + 1) % CGodItem::TYPE_MAX);

		// �}�e���A���̊��蓖��
		switch (m_infoCreate.type)
		{
		case CGodItem::TYPE_RED:
			m_pGodItem->SetAllMaterial(material::Red());	// �ԐF
			break;

		case CGodItem::TYPE_GREEN:
			m_pGodItem->SetAllMaterial(material::Green());	// �ΐF
			break;

		case CGodItem::TYPE_BLUE:
			m_pGodItem->SetAllMaterial(material::Blue());	// �F
			break;

		default:
			assert(false);
			break;
		}
	}
}

//============================================================
//	�_��̐�������
//============================================================
void CEditGodItem::CreateGodItem(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	if (CGodItem::GetList() != nullptr)
	{ // ���X�g������ꍇ

		for (auto& item : CGodItem::GetList()->GetList())
		{
			if (item->GetType() == m_infoCreate.type)
			{ // ���ݑI�����Ă���_�킪���݂��Ă���ꍇ

				// ���̐�̏������s��Ȃ�
				return;
			}
		}
	}

	// �_���z�u
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		// �S�[���|�C���g�̐���
		CGodItem::Create
		(
			GetVec3Position(),
			m_infoCreate.type
		);

		// ���ۑ���ݒ�
		m_bSave = false;
	}
}

//============================================================
//	�_��̔j������
//============================================================
void CEditGodItem::ReleaseGodItem(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	bool bRelease = false;	// �j����

	// �_����폜
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// �j�������Ԃ�ݒ�
		bRelease = true;
	}

	// �_��̍폜����
	DeleteCollisionGodItem(bRelease);
}

//============================================================
//	�_��̍폜����
//============================================================
void CEditGodItem::DeleteCollisionGodItem(const bool bRelase)
{
	// �폜���肪 false �̏ꍇ������
	if (!bRelase) { return; }

	CListManager<CGodItem>* pListManager = CGodItem::GetList();	// �_�탊�X�g�}�l�[�W���[
	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
	std::list<CGodItem*> listGodItem = pListManager->GetList();	// �_�탊�X�g���

	for (auto& rList : listGodItem)
	{ // �_�퐔���J��Ԃ�

		if (rList->GetType() == m_infoCreate.type)
		{ // ��ނ���v�����ꍇ

			// �I������
			rList->Uninit();
		}
	}
}

//============================================================
//	�ۑ�����
//============================================================
HRESULT CEditGodItem::Save(void)
{
#if _DEBUG

	// �_��̃��X�g���擾
	CListManager<CGodItem>* pListManager = CGodItem::GetList();	// ���X�g�}�l�[�W���[
	std::list<CGodItem*> listGodItem;	// �n�ʃ��X�g
	if (pListManager != nullptr)
	{ // ���X�g�}�l�[�W���[����������Ă���ꍇ

		// ���X�g���擾
		listGodItem = pListManager->GetList();
	}

	// �t�@�C�����J��
	std::ofstream  file(SAVE_PASS);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�_��z�u�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���o���������o��
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	�_��z�u�̃Z�[�u�f�[�^ [save_goditem.txt]" << std::endl;
	file << "#	Author : ���c �E��" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# ���̍s���牺���R�s�[�� [goditem.txt] �ɒ���t��\n" << std::endl;

	// �����_�����o���̕��@���w��
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// �ǂݍ��݊J�n������������o��
	file << "STAGE_GODITEMSET\n" << std::endl;

	for (const auto& rList : listGodItem)
	{ // �n�ʂ̑������J��Ԃ�

		// �����o�������擾
		CGodItem::EType type = rList->GetType();		// ���
		D3DXVECTOR3 pos = rList->GetVec3Position();		// �ʒu
		float fPosInitY = rList->GetInitPosY();			// �����ʒu(Y��)

		// ���������o��
		file << "	GODITEMSET" << std::endl;
		file << "		POS		= " << pos.x << " " << fPosInitY << " " << pos.z << std::endl;
		file << "		TYPE	= " << type << std::endl;
		file << "		" << std::endl;
		file << "		TEXT_" << type << std::endl;
		file << "			STR = ���ꂪ" << type << "�ڂ̌���" << std::endl;
		file << "		END_TEXT" << std::endl;
		file << "	END_GODITEMSET\n" << std::endl;
	}

	// �ǂݍ��ݏI��������������o��
	file << "END_STAGE_GODITEMSET" << std::endl;

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
