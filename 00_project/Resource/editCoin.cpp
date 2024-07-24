#if 1
//============================================================
//
//	�G�f�B�b�g�R�C������ [editCoin.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editCoin.h"
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

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_coin.txt";	// �Z�[�u�e�L�X�g�p�X

	const D3DXVECTOR3 SCALING = D3DXVECTOR3(0.1f, 0.1f, 0.1f);		// �g�k��
	const float	INIT_ALPHA = 0.5f;	// �z�u�O�̃��l
	const int DIGIT_FLOAT = 2;		// �����_�ȉ��̌���
}

//************************************************************
//	�e�N���X [CEditCoin] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditCoin::CEditCoin(CEditStage* pEditor) : CEditorObject(pEditor)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pCoin = nullptr;	// �R�C�����
	m_bSave = false;	// �ۑ���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditCoin::~CEditCoin()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditCoin::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pCoin = nullptr;						// ���
	m_bSave = false;						// �ۑ���

	// �e�N���X�̏�����
	if (FAILED(CEditorObject::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �R�C���̐���
	m_pCoin = CCoin::Create(GetVec3Position());
	if (m_pCoin == nullptr)
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
void CEditCoin::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorObject::Uninit();

	// �R�C���̐F�̑S������
	InitAllColorCoin();

	// �R�C���̏I��
	SAFE_UNINIT(m_pCoin);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditCoin::Update(void)
{
#if _DEBUG

	// �e�N���X�̍X�V
	CEditorObject::Update();

	// �R�C���̐���
	CreateCoin();

	// �R�C���̔j��
	ReleaseCoin();

	// �ʒu�𔽉f
	m_pCoin->SetVec3Position(GetVec3Position());

	// �����𔽉f
	m_pCoin->SetVec3Rotation(VEC3_ZERO);

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditCoin::IsSave(void)
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
void CEditCoin::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditCoin::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditCoin::DrawDebugControl(void)
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
void CEditCoin::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorObject::DrawDebugInfo();

#endif	// _DEBUG
}

//============================================================
// �ʒu�X�V
//============================================================
void CEditCoin::UpdatePosition(void)
{
	// �ʒu�̍X�V
	CEditorObject::UpdatePosition();

	// �ʒu�𔽉f
	m_pCoin->SetVec3Position(GetVec3Position());
}

//============================================================
//	�R�C���̐�������
//============================================================
void CEditCoin::CreateCoin(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// �R�C����z�u
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	�R�C���̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V�E�����`���ON�ɂ���
		m_pCoin->SetEnableUpdate(true);
		m_pCoin->SetEnableDraw(true);

		// ���ۑ���ݒ�
		m_bSave = false;

		//----------------------------------------------------
		//	�V�����R�C���̐���
		//----------------------------------------------------
		// �R�C���̐���
		m_pCoin = CCoin::Create(GetVec3Position());
		assert(m_pCoin != nullptr);
	}
}

//============================================================
//	�R�C���̔j������
//============================================================
void CEditCoin::ReleaseCoin(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	bool bRelease = false;	// �j����

	// �R�C�����폜
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// �j�������Ԃ�ݒ�
		bRelease = true;
	}

	// �R�C���̍폜����
	DeleteCollisionCoin(bRelease);
}

//============================================================
//	�R�C���̍폜����
//============================================================
void CEditCoin::DeleteCollisionCoin(const bool bRelase)
{
	CListManager<CCoin>* pListManager = CCoin::GetList();	// �R�C�����X�g�}�l�[�W���[
	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
	std::list<CCoin*> listCoin = pListManager->GetList();	// �R�C�����X�g���

	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
	for (auto& rList : listCoin)
	{ // �R�C�������J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pCoin) { continue; }

		D3DXVECTOR3 posOther = rList->GetVec3Position();	// �Ώۂ̒n�ʈʒu
		D3DXVECTOR3 scaleThis = VEC3_ZERO;	// ���g�̑傫��
		D3DXVECTOR3 scaleOther = VEC3_ZERO;	// �Ώۂ̑傫��

		// ���g�̑傫����ݒ�
		D3DXVECTOR3 scaleThisCoin = m_pCoin->GetVec3Scaling();	// ���g�̒n�ʂ̑傫��
		scaleThis.x = scaleThisCoin.x;	// ����T�C�YX��ݒ�
		scaleThis.y = scaleThisCoin.y;	// ����T�C�YY��ݒ�
		scaleThis.z = scaleThisCoin.z;	// ����T�C�YZ��ݒ�
		scaleThis *= 0.5f;				// ����T�C�Y�𔼕���

		// �Ώۂ̑傫����ݒ�
		D3DXVECTOR3 scaleOtherCoin = rList->GetVec3Scaling();		// �Ώۂ̒n�ʂ̑傫��
		scaleOther.x = scaleOtherCoin.x;	// ����T�C�YX��ݒ�
		scaleOther.y = scaleOtherCoin.y;	// ����T�C�YY��ݒ�
		scaleOther.z = scaleOtherCoin.z;	// ����T�C�YZ��ݒ�
		scaleOther *= 0.5f;				// ����T�C�Y�𔼕���

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
//	�R�C���̐F�̑S����������
//============================================================
void CEditCoin::InitAllColorCoin(void)
{
	CListManager<CCoin>* pListManager = CCoin::GetList();	// �R�C�����X�g�}�l�[�W���[
	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
	std::list<CCoin*> listCoin = pListManager->GetList();	// �R�C�����X�g���

	for (auto& rList : listCoin)
	{ // �R�C�������J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pCoin) { continue; }

		// �ʏ�F��ݒ�
		rList->ResetMaterial();
	}
}

//============================================================
//	�ۑ�����
//============================================================
HRESULT CEditCoin::Save(void)
{
#if _DEBUG

	// �n�ʂ̃��X�g���擾
	CListManager<CCoin>* pListManager = CCoin::GetList();	// ���X�g�}�l�[�W���[
	std::list<CCoin*> listCoin;	// �n�ʃ��X�g
	if (pListManager != nullptr)
	{ // ���X�g�}�l�[�W���[����������Ă���ꍇ

		// ���X�g���擾
		listCoin = pListManager->GetList();
	}

	// �t�@�C�����J��
	std::ofstream  file(SAVE_PASS);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�R�C���z�u�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���o���������o��
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	�R�C���z�u�̃Z�[�u�f�[�^ [save_coin.txt]" << std::endl;
	file << "#	Author : ���c �E��" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# ���̍s���牺���R�s�[�� [coin.txt] �ɒ���t��\n" << std::endl;

	// �R�C���̐F�̑S������
	InitAllColorCoin();

	// �����_�����o���̕��@���w��
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// �ǂݍ��݊J�n������������o��
	file << "STAGE_COINSET\n" << std::endl;

	for (const auto& rList : listCoin)
	{ // �n�ʂ̑������J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pCoin) { continue; }

		// �����o�������擾
		D3DXVECTOR3 pos = rList->GetVec3Position();		// �ʒu

		// ���������o��
		file << "	COINSET	= " << pos.x << " " << pos.y << " " << pos.z  << "\n" << std::endl;
	}

	// �ǂݍ��ݏI��������������o��
	file << "END_STAGE_COINSET" << std::endl;

	// �R�C���̍폜����
	DeleteCollisionCoin(false);

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
