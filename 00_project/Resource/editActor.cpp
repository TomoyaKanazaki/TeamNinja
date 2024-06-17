#if 1
//============================================================
//
//	�G�f�B�b�g�A�N�^�[���� [editActor.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editActor.h"
#include "editManager.h"
#include "manager.h"
#include "collision.h"
#include "useful.h"
#include "stage.h"

#include "collisionCube.h"
#include "collisionCylinder.h"
#include "collisionSphere.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_CREATE		(DIK_0)	// �����L�[
#define NAME_CREATE		("0")	// �����\��
#define KEY_RELEASE		(DIK_9)	// �j���L�[
#define NAME_RELEASE	("9")	// �j���\��
#define KEY_TYPE		(DIK_3)	// ��ޕύX�L�[
#define NAME_TYPE		("3")	// ��ޕύX�\��

#define KEY_UP_SIZE		(DIK_T)	// �g��L�[
#define NAME_UP_SIZE	("T")	// �g��\��
#define KEY_DOWN_SIZE	(DIK_G)	// �k���L�[
#define NAME_DOWN_SIZE	("G")	// �k���\��
#define KEY_ROTA_RIGHT	(DIK_Z)	// �E��]�L�[
#define NAME_ROTA_RIGHT	("Z")	// �E��]�\��
#define KEY_ROTA_LEFT	(DIK_C)	// ����]�L�[
#define NAME_ROTA_LEFT	("C")	// ����]�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_actor.txt";	// �Z�[�u�e�L�X�g�p�X

	const D3DXVECTOR3 SCALING = D3DXVECTOR3(0.1f, 0.1f, 0.1f);		// �g�k��
	const float	INIT_ALPHA = 0.5f;	// �z�u�O�̃��l
	const int DIGIT_FLOAT = 2;		// �����_�ȉ��̌���

	const float ROT_MOVE = 0.02f;	// �����̈ړ���
}

//************************************************************
//	�e�N���X [CEditActor] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditActor::CEditActor(CEditStage* pEditor) : CEditorObject(pEditor)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pActor = nullptr;	// �A�N�^�[���
	m_bSave = false;	// �ۑ���
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// �A�N�^�[�z�u���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditActor::~CEditActor()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditActor::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pActor = nullptr;						// ���
	m_infoCreate.type = (CActor::EType)0;	// ���
	m_infoCreate.scale = VEC3_ONE;			// �傫��
	m_bSave = false;						// �ۑ���

	// �e�N���X�̏�����
	if (FAILED(CEditorObject::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �A�N�^�[�̐���
	D3DXCOLOR colActor = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// �n�ʐF
	m_pActor = CActor::Create
	( // ����
		m_infoCreate.type,		// ���
		GetVec3Position(),		// �ʒu
		GetVec3Rotation(),		// ����
		m_infoCreate.scale		// �傫��
	);
	if (m_pActor == nullptr)
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
void CEditActor::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorObject::Uninit();

	// �A�N�^�[�̐F�̑S������
	InitAllColorActor();

	// �A�N�^�[�̏I��
	SAFE_UNINIT(m_pActor);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditActor::Update(void)
{
#if _DEBUG

	// �e�N���X�̍X�V
	CEditorObject::Update();

	// �g�嗦�̍X�V
	UpdateScaling();

	// ��ނ̕ύX
	ChangeType();

	// �A�N�^�[�̐���
	CreateActor();

	// �A�N�^�[�̔j��
	ReleaseActor();

	// �ʒu�𔽉f
	m_pActor->SetVec3Position(GetVec3Position());

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditActor::IsSave(void)
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
void CEditActor::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditActor::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditActor::DrawDebugControl(void)
{
#if _DEBUG

	// ����\���̕`��
	CEditorObject::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "�傫���F[%s/%s+%s]\n", NAME_UP_SIZE, NAME_DOWN_SIZE, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "��ޕύX�F[%s]\n", NAME_TYPE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�폜�F[%s]\n", NAME_RELEASE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditActor::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorObject::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%d�F[���]\n", m_infoCreate.type);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f�F[�傫��]\n", m_infoCreate.scale.x, m_infoCreate.scale.y, m_infoCreate.scale.z);

#endif	// _DEBUG
}

//============================================================
// �ʒu�X�V
//============================================================
void CEditActor::UpdatePosition(void)
{
	// �ʒu�̍X�V
	CEditorObject::UpdatePosition();

	// �ʒu�𔽉f
	m_pActor->SetVec3Position(GetVec3Position());
}

//============================================================
//	�����X�V����
//============================================================
void CEditActor::UpdateRotation(void)
{
	if (m_pActor->GetCube().empty())
	{ // �󔒂̏ꍇ

		// ������ύX
		D3DXVECTOR3 rot = GetVec3Rotation();			// �������擾����
		CInputKeyboard* m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���
		if (!m_pKeyboard->IsPress(KEY_TRIGGER))
		{
			if (m_pKeyboard->IsPress(KEY_ROTA_RIGHT))
			{
				rot.y += ROT_MOVE;
			}
			if (m_pKeyboard->IsPress(KEY_ROTA_LEFT))
			{
				rot.y -= ROT_MOVE;
			}
		}
		else
		{
			if (m_pKeyboard->IsTrigger(KEY_ROTA_RIGHT))
			{
				rot.y += ROT_MOVE;
			}
			if (m_pKeyboard->IsTrigger(KEY_ROTA_LEFT))
			{
				rot.y -= ROT_MOVE;
			}
		}

		// ������ݒ肷��
		SetVec3Rotation(rot);
	}
	else
	{ // ��L�ȊO

		// �����̍X�V
		CEditorObject::UpdateRotation();
	}

#ifdef _DEBUG

	for (auto& rCube : m_pActor->GetCube())
	{ // �R���W�����L���[�u�����J��Ԃ�

		// �I������
		rCube->GetCube()->SetVec3Rotation(GetVec3Rotation());
	}

	for (auto& rCylinder : m_pActor->GetCylinder())
	{ // �R���W�����V�����_�[�����J��Ԃ�

		// �I������
		rCylinder->GetTube()->SetVec3Rotation(GetVec3Rotation());
	}

	for (auto& rSphere : m_pActor->GetSphere())
	{ // �R���W�����X�t�B�A�����J��Ԃ�

		// �I������
		rSphere->GetSphere()->SetVec3Rotation(GetVec3Rotation());
	}

#endif // _DEBUG

	// �����𔽉f
	m_pActor->SetVec3Rotation(GetVec3Rotation());
}

//============================================================
//	�g�嗦�̍X�V����
//============================================================
void CEditActor::UpdateScaling(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// �g�嗦��ύX
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_SIZE))
		{
			m_infoCreate.scale += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_SIZE))
		{
			m_infoCreate.scale -= SCALING;
		}
	}
	else
	{
		if (pKeyboard->IsTrigger(KEY_UP_SIZE))
		{
			m_infoCreate.scale += SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_SIZE))
		{
			m_infoCreate.scale -= SCALING;
		}
	}

	// �g�嗦��␳
	useful::LimitMinNum(m_infoCreate.scale.x, SCALING.x);
	useful::LimitMinNum(m_infoCreate.scale.y, SCALING.y);
	useful::LimitMinNum(m_infoCreate.scale.z, SCALING.z);

#ifdef _DEBUG

	for (auto& rCube : m_pActor->GetCube())
	{ // �R���W�����L���[�u�����J��Ԃ�

		// �g�嗦��K�p����
		rCube->GetCube()->SetVec3Sizing(D3DXVECTOR3
		(
			rCube->GetWidth() * m_infoCreate.scale.x,
			rCube->GetHeight() * m_infoCreate.scale.y,
			rCube->GetDepth() * m_infoCreate.scale.z
		));
	}

	for (auto& rCylinder : m_pActor->GetCylinder())
	{ // �R���W�����V�����_�[�����J��Ԃ�

		// �g�嗦��K�p����
		rCylinder->GetTube()->SetRadius(rCylinder->GetRadius() * m_infoCreate.scale.x);
		rCylinder->GetTube()->SetHeight(rCylinder->GetHeight() * m_infoCreate.scale.y);
	}

	for (auto& rSphere : m_pActor->GetSphere())
	{ // �R���W�����X�t�B�A�����J��Ԃ�

		// �g�嗦��K�p����
		rSphere->GetSphere()->SetRadius(rSphere->GetRadius() * m_infoCreate.scale.x);
	}

#endif // _DEBUG

	// �g�嗦�𔽉f
	m_pActor->SetVec3Scaling(m_infoCreate.scale);
}

//============================================================
//	��ޕύX�̍X�V����
//============================================================
void CEditActor::ChangeType(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// ��ނ�ύX
	if (pKeyboard->IsTrigger(KEY_TYPE))
	{
		m_infoCreate.type = (CActor::EType)((m_infoCreate.type + 1) % CActor::TYPE_MAX);
	}

	// ��ނ𔽉f
	m_pActor->SetType(m_infoCreate.type);
}

//============================================================
//	�A�N�^�[�̐�������
//============================================================
void CEditActor::CreateActor(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// �A�N�^�[��z�u
	if (pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	�A�N�^�[�̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V�E�����`���ON�ɂ���
		m_pActor->SetEnableUpdate(true);
		m_pActor->SetEnableDraw(true);

		// ���ۑ���ݒ�
		m_bSave = false;

		//----------------------------------------------------
		//	�V�����A�N�^�[�̐���
		//----------------------------------------------------
		// �A�N�^�[�̐���
		m_pActor = CActor::Create
		( // ����
			m_infoCreate.type,		// ���
			GetVec3Position(),		// �ʒu
			VEC3_ZERO,				// ����
			m_infoCreate.scale		// �g�嗦
		);
		assert(m_pActor != nullptr);

		// ������ݒ肷��
		m_pActor->SetVec3Rotation(GetVec3Rotation());

		for (auto& rCube : m_pActor->GetCube())
		{ // �R���W�����L���[�u�����J��Ԃ�

			// �傫����ݒ肷��
			rCube->SetWidth(rCube->GetWidth() / m_infoCreate.scale.x);
			rCube->SetHeight(rCube->GetHeight() / m_infoCreate.scale.y);
			rCube->SetDepth(rCube->GetDepth() / m_infoCreate.scale.z);
		}

		for (auto& rCylinder : m_pActor->GetCylinder())
		{ // �R���W�����V�����_�[�����J��Ԃ�

			// �傫����ݒ肷��
			rCylinder->SetRadius(rCylinder->GetRadius() / m_infoCreate.scale.x);
			rCylinder->SetHeight(rCylinder->GetHeight() / m_infoCreate.scale.y);
		}

		for (auto& rSphere : m_pActor->GetSphere())
		{ // �R���W�����X�t�B�A�����J��Ԃ�

			// �傫����ݒ肷��
			rSphere->SetRadius(rSphere->GetRadius() / m_infoCreate.scale.x);
		}
	}
}

//============================================================
//	�A�N�^�[�̔j������
//============================================================
void CEditActor::ReleaseActor(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	bool bRelease = false;	// �j����

	// �A�N�^�[���폜
	if (pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// �j�������Ԃ�ݒ�
		bRelease = true;
	}

	// �A�N�^�[�̍폜����
	DeleteCollisionActor(bRelease);
}

//============================================================
//	�A�N�^�[�̍폜����
//============================================================
void CEditActor::DeleteCollisionActor(const bool bRelase)
{
	CListManager<CActor>* pListManager = CActor::GetList();	// �A�N�^�[���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
	std::list<CActor*> listActor = pListManager->GetList();	// �A�N�^�[���X�g���

	D3DXVECTOR3 posEdit = GetVec3Position();	// �G�f�B�b�g�̈ʒu
	for (auto& rList : listActor)
	{ // �A�N�^�[�����J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pActor) { continue; }

		D3DXVECTOR3 posOther = rList->GetVec3Position();	// �Ώۂ̒n�ʈʒu
		D3DXVECTOR3 scaleThis = VEC3_ZERO;	// ���g�̑傫��
		D3DXVECTOR3 scaleOther = VEC3_ZERO;	// �Ώۂ̑傫��

		// ���g�̑傫����ݒ�
		D3DXVECTOR3 scaleThisActor = m_pActor->GetVec3Scaling();	// ���g�̒n�ʂ̑傫��
		scaleThis.x = scaleThisActor.x;	// ����T�C�YX��ݒ�
		scaleThis.y = scaleThisActor.y;	// ����T�C�YY��ݒ�
		scaleThis.z = scaleThisActor.z;	// ����T�C�YZ��ݒ�
		scaleThis *= 0.5f;				// ����T�C�Y�𔼕���

		// �Ώۂ̑傫����ݒ�
		D3DXVECTOR3 scaleOtherActor = rList->GetVec3Scaling();		// �Ώۂ̒n�ʂ̑傫��
		scaleOther.x = scaleOtherActor.x;	// ����T�C�YX��ݒ�
		scaleOther.y = scaleOtherActor.y;	// ����T�C�YY��ݒ�
		scaleOther.z = scaleOtherActor.z;	// ����T�C�YZ��ݒ�
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
//	�A�N�^�[�̐F�̑S����������
//============================================================
void CEditActor::InitAllColorActor(void)
{
	CListManager<CActor>* pListManager = CActor::GetList();	// �A�N�^�[���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return; }				// ���X�g���g�p�̏ꍇ������
	std::list<CActor*> listActor = pListManager->GetList();	// �A�N�^�[���X�g���

	for (auto& rList : listActor)
	{ // �A�N�^�[�����J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pActor) { continue; }

		// �ʏ�F��ݒ�
		rList->ResetMaterial();
	}
}

//============================================================
//	�ۑ�����
//============================================================
HRESULT CEditActor::Save(void)
{
#if _DEBUG

	// �n�ʂ̃��X�g���擾
	CListManager<CActor>* pListManager = CActor::GetList();	// ���X�g�}�l�[�W���[
	std::list<CActor*> listActor;	// �n�ʃ��X�g
	if (pListManager != nullptr)
	{ // ���X�g�}�l�[�W���[����������Ă���ꍇ

		// ���X�g���擾
		listActor = pListManager->GetList();
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
	file << "#	�A�N�^�[�z�u�̃Z�[�u�f�[�^ [save_actor.txt]" << std::endl;
	file << "#	Author : ���c �E��" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# ���̍s���牺���R�s�[�� [actor.txt] �ɒ���t��\n" << std::endl;

	// �A�N�^�[�̐F�̑S������
	InitAllColorActor();

	// �����_�����o���̕��@���w��
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// �ǂݍ��݊J�n������������o��
	file << "STAGE_ACTORSET\n" << std::endl;

	for (const auto& rList : listActor)
	{ // �n�ʂ̑������J��Ԃ�

		// �����A�h���X�������ꍇ����
		if (rList == m_pActor) { continue; }

		// �����o�������擾
		CActor::EType type = rList->GetType();			// ���
		D3DXVECTOR3 pos = rList->GetVec3Position();		// �ʒu
		D3DXVECTOR3 rot = rList->GetVec3Rotation();		// ����
		D3DXVECTOR3 scale = rList->GetVec3Scaling();	// �傫��

		// ������360�x�ɕϊ�
		rot = D3DXToDegree(rot);

		// ���������o��
		file << "	ACTORSET" << std::endl;
		file << "		TYPE	= " << type << std::endl;
		file << "		POS		= " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		file << "		ROT		= " << rot.x << " " << rot.y << " " << rot.z << std::endl;
		file << "		SCALE	= " << scale.x << " " << scale.y << " " << scale.z << std::endl;
		file << "	END_ACTORSET\n" << std::endl;
	}

	// �ǂݍ��ݏI��������������o��
	file << "END_STAGE_ACTORSET" << std::endl;

	// �A�N�^�[�̍폜����
	DeleteCollisionActor(false);

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
