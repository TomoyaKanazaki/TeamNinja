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
#define KEY_TYPE		(DIK_2)	// ��ޕύX�L�[
#define NAME_TYPE		("2")	// ��ޕύX�\��

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
	const D3DXVECTOR2 INIT_SIZE = D3DXVECTOR2(100.0f, 100.0f);	// �傫��
	const float	MAX_SIZE = 10000.0f;	// �ő�̑傫��
	const float	INIT_ALPHA = 0.5f;		// �z�u�O�̃��l
}

//************************************************************
//	�e�N���X [CEditField] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditField::CEditField(CEditManager *pEditManager) : CEditStage(pEditManager)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pField = nullptr;	// �t�B�[���h���
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
	m_infoCreate.col	 = XCOL_WHITE;			// �F
	m_infoCreate.part	 = GRID2_ONE;			// ������
	m_infoCreate.texPart = GRID2_ONE;			// �e�N�X�`��������

	// �e�N���X�̏�����
	if (FAILED(CEditStage::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	CEditManager *pEditManager = GetPtrEditManager();	// �G�f�B�b�g�}�l�[�W���[
	if (pEditManager == nullptr)
	{ // �G�f�B�b�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �t�B�[���h�̐���
	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
	m_pField = CField::Create
	( // ����
		m_infoCreate.type,		// ���
		posEdit,				// �ʒu
		VEC3_ZERO,				// ����
		m_infoCreate.size,		// �傫��
		m_infoCreate.col,		// �F
		m_infoCreate.part,		// ������
		m_infoCreate.texPart	// �e�N�X�`��������
	);
	if (m_pField == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �F��ݒ�
	D3DXCOLOR col = m_pField->GetColor();	// ���̐F���擾
	m_pField->SetColor(D3DXCOLOR(col.r, col.g, col.b, INIT_ALPHA));

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
	CEditStage::Uninit();

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

	CEditManager *pEditManager = GetPtrEditManager();	// �G�f�B�b�g�}�l�[�W���[
	if (pEditManager == nullptr)
	{ // �G�f�B�b�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	// �G�f�B�b�g��OFF�Ȃ甲����
	if (!pEditManager->IsEdit()) { return; }

	// �e�N���X�̍X�V
	CEditStage::Update();

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

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditField::DrawDebugControl(void)
{
	// ����\���̕`��
	CEditStage::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "�傫���F[%s/%s/%s/%s+%s]\n", NAME_UP_SIZE_X, NAME_DOWN_SIZE_X, NAME_UP_SIZE_Y, NAME_DOWN_SIZE_Y, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "��ޕύX�F[%s]\n", NAME_TYPE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�폜�F[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditField::DrawDebugInfo(void)
{
	// ���\���̕`��
	CEditStage::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%d�F[���]\n", m_infoCreate.type);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f�F[�傫��]\n", m_infoCreate.size.x, m_infoCreate.size.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d %d�F[����]\n", m_infoCreate.part.x, m_infoCreate.part.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d %d�F[�e�N�X�`������]\n", m_infoCreate.texPart.x, m_infoCreate.texPart.y);
}

//============================================================
//	���ۑ�����
//============================================================
void CEditField::SaveInfo(void)
{
	// ���݂̏���ۑ�
	//m_save = m_infoCreate;
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditField::LoadInfo(void)
{
	// �ۑ�����ݒ�
	//m_infoCreate = m_save;
}

//============================================================
//	�ۑ�����
//============================================================
void CEditField::Save(FILE *pFile)
{
#if _DEBUG

#if 0

	if (pFile != nullptr)
	{ // �t�@�C�������݂���ꍇ

		// ���o���������o��
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#	�t�B�[���h�̔z�u���\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		// ���J�n�n�_�������o��
		fprintf(pFile, "STAGE_BLOCKSET\n\n");

		for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
		{ // �D�揇�ʂ̑������J��Ԃ�

			// �|�C���^��錾
			CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

			if (pObjectTop != nullptr)
			{ // �擪�����݂���ꍇ

				// �|�C���^��錾
				CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

				while (pObjCheck != nullptr)
				{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

					// �|�C���^��錾
					CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

					if (pObjCheck->GetLabel() != CObject::LABEL_BLOCK)
					{ // �I�u�W�F�N�g���x�����t�B�[���h�ł͂Ȃ��ꍇ

						// ���̃I�u�W�F�N�g�ւ̃|�C���^����
						pObjCheck = pObjectNext;

						// ���̌J��Ԃ��Ɉڍs
						continue;
					}

					if (pObjCheck == (CObject*)m_pField)
					{ // �����A�h���X�������ꍇ

						// ���̃I�u�W�F�N�g�ւ̃|�C���^����
						pObjCheck = pObjectNext;

						// ���̌J��Ԃ��Ɉڍs
						continue;
					}

					// �t�B�[���h�̏����擾
					D3DXVECTOR3 posField = pObjCheck->GetVec3Position();		// �ʒu
					D3DXVECTOR3 rotField = pObjCheck->GetVec3Rotation();		// ����
					D3DXVECTOR3 sizeField = pObjCheck->GetVec3Sizing();		// �傫��
					D3DXVECTOR2 partTexXField = pObjCheck->GetTexturePatternX();	// �e�N�X�`������X
					D3DXVECTOR2 partTexYField = pObjCheck->GetTexturePatternY();	// �e�N�X�`������Y
					D3DXVECTOR2 partTexZField = pObjCheck->GetTexturePatternZ();	// �e�N�X�`������Z
					int nType = pObjCheck->GetType();	// ���

					// ���������o��
					fprintf(pFile, "	BLOCKSET\n");
					fprintf(pFile, "		TYPE = %d\n", nType);
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posField.x, posField.y, posField.z);
					fprintf(pFile, "		ROT = %.2f %.2f %.2f\n", rotField.x, rotField.y, rotField.z);
					fprintf(pFile, "		SIZE = %.2f %.2f %.2f\n", sizeField.x, sizeField.y, sizeField.z);
					fprintf(pFile, "		PARTX = %.2f %.2f \n", partTexXField.x, partTexXField.y);
					fprintf(pFile, "		PARTY = %.2f %.2f \n", partTexYField.x, partTexYField.y);
					fprintf(pFile, "		PARTZ = %.2f %.2f \n", partTexZField.x, partTexZField.y);
					fprintf(pFile, "	END_BLOCKSET\n\n");

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;
				}
			}
		}

		// ���I���n�_�������o��
		fprintf(pFile, "END_STAGE_BLOCKSET\n\n");
	}

#endif

#endif	// _DEBUG
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
	useful::LimitNum(m_infoCreate.size.x, INIT_SIZE.x, MAX_SIZE);
	useful::LimitNum(m_infoCreate.size.y, INIT_SIZE.y, MAX_SIZE);

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
	CEditManager *pEditManager = GetPtrEditManager();	// �G�f�B�b�g�}�l�[�W���[
	if (pEditManager == nullptr)
	{ // �G�f�B�b�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
	D3DXCOLOR colField = XCOL_WHITE;	// �F�ۑ��p

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
		pEditManager->UnSave();

		//----------------------------------------------------
		//	�V�����t�B�[���h�̐���
		//----------------------------------------------------
		// �t�B�[���h�̐���
		m_pField = CField::Create
		( // ����
			m_infoCreate.type,		// ���
			posEdit,				// �ʒu
			VEC3_ZERO,				// ����
			m_infoCreate.size,		// �傫��
			m_infoCreate.col,		// �F
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
	CEditManager *pEditManager = GetPtrEditManager();	// �G�f�B�b�g�}�l�[�W���[
	if (pEditManager == nullptr)
	{ // �G�f�B�b�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	CListManager<CField> *pListManager = CField::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
	std::list<CField*> listField = pListManager->GetList();	// �t�B�[���h���X�g���

	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
	for (auto& rList : listField)
	{ // �t�B�[���h�����J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pField) { continue; }

		D3DXVECTOR3 posField  = rList->GetVec3Position();	// �t�B�[���h�ʒu
		D3DXVECTOR3 sizeThisField = D3DXVECTOR3(0.0f, 25.0f, 0.0f);		// �t�B�[���h�傫��
		D3DXVECTOR3 sizeOtherField = D3DXVECTOR3(0.0f, 25.0f, 0.0f);	// �t�B�[���h�傫��

		D3DXVECTOR2 sizeThis = m_pField->GetVec2Sizing();
		sizeThisField.x = sizeThis.x;
		sizeThisField.z = sizeThis.y;

		D3DXVECTOR2 sizeOther = rList->GetVec2Sizing();
		sizeOtherField.x = sizeOther.x;
		sizeOtherField.z = sizeOther.y;

		sizeThisField *= 0.5f;	// �t�B�[���h�傫��
		sizeOtherField *= 0.5f;	// �t�B�[���h�傫��

		// TODO�F���肫������[
		// ��`�̓����蔻��
		if (collision::Box3D
		( // ����
			posEdit,		// ����ʒu
			posField,		// ����ڕW�ʒu
			sizeThisField,
			sizeThisField,
			sizeOtherField,
			sizeOtherField
		))
		{ // ������������ꍇ

			if (bRelase)
			{ // �j������ꍇ

				// �I������
				rList->Uninit();

				// ���ۑ���ݒ�
				pEditManager->UnSave();
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

		// �ʏ�F��ݒ�
		rList->SetColor(XCOL_WHITE);
	}
}
#endif
