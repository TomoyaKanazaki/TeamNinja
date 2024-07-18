//============================================================
//
//	�G�f�B�b�g�����蔻�菈�� [editCollision.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "editCollision.h"
#include "actor.h"
#include "editManager.h"
#include "collision.h"
#include "effect3D.h"

#include "editCollCube.h"
#include "editCollCylinder.h"
#include "editCollSphere.h"
#include "editCollPolygon.h"

#include "gameManager.h"
#include "sceneGame.h"
#include "collManager.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_CREATE					(DIK_0)	// �����L�[
#define NAME_CREATE					("0")	// �����\��
#define KEY_RELEASE					(DIK_9)	// �j���L�[
#define NAME_RELEASE				("9")	// �j���\��
#define KEY_CHANGE_OBJECT_FRONT		(DIK_2)	// �I�u�W�F�N�g�^�C�v�O�i�ύX�L�[
#define NAME_CHANGE_OBJECT_FRONT	("2")	// �I�u�W�F�N�g�^�C�v�O�i�ύX�\��
#define KEY_CHANGE_OBJECT_BACK		(DIK_3)	// �I�u�W�F�N�g�^�C�v��i�ύX�L�[
#define NAME_CHANGE_OBJECT_BACK		("3")	// �I�u�W�F�N�g�^�C�v��i�ύX�\��
#define KEY_CHANGE_ACTOR			(DIK_4)	// �A�N�^�[�̎�ޕύX�L�[
#define NAME_CHANGE_ACTOR			("4")	// �A�N�^�[�̎�ޕύX�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SAVE_PASS = "Debug\\DEBUG_SAVE\\save_collision.txt";	// �Z�[�u�e�L�X�g�p�X

	const char* TYPE_NAME[] =	// �I�u�W�F�N�g�^�C�v��
	{
		"�L���[�u",			// �L���[�u
		"�V�����_�[",		// �V�����_�[
		"�X�t�B�A",			// �X�t�B�A
		"�|���S��",			// �|���S��
	};
	const int DIGIT_FLOAT = 2;			// �����_�ȉ��̌���
	const float DELETE_RANGE = 30.0f;	// ��������͈�
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);			// �F
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TYPE_NAME) == CEditCollision::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�e�N���X [CEditCollision] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditCollision::CEditCollision(CEditManager* pEditManager) : CEditor(pEditManager)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pEditor = nullptr;				// �G�f�B�^�[���
	m_pActor = nullptr;					// �A�N�^�[�̏��
	m_type = TYPE_CUBE;					// �I�u�W�F�N�g�^�C�v
	m_actorType = CActor::TYPE_ROCK_S;	// �A�N�^�[�^�C�v
	m_cube = {};						// �L���[�u�̉ϒ��z��
	m_cylinder = {};					// �V�����_�[�̉ϒ��z��
	m_sphere = {};						// �X�t�B�A�̉ϒ��z��
	m_polygon = {};						// �|���S���̉ϒ��z��

	// �I�u�W�F�N�g�̕`��֌W����
	DispObject(false);

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditCollision::~CEditCollision()
{
#if _DEBUG

	// �I�u�W�F�N�g�̕`��֌W����
	DispObject(true);

#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditCollision::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pEditor = nullptr;				// �G�f�B�^�[���
	m_pActor = nullptr;					// �A�N�^�[�̏��
	m_type = TYPE_CUBE;					// �I�u�W�F�N�g�^�C�v
	m_actorType = CActor::TYPE_ROCK_S;	// �A�N�^�[�^�C�v
	m_cube = {};						// �L���[�u�̉ϒ��z��
	m_cylinder = {};					// �V�����_�[�̉ϒ��z��
	m_sphere = {};						// �X�t�B�A�̉ϒ��z��
	m_polygon = {};						// �|���S���̉ϒ��z��

	// �G�f�B�^�[���̐���
	m_pEditor = CEditorCollShape::Create(m_type, 0);
	if (m_pEditor == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �A�N�^�[�̐���
	m_pActor = CActor::Create(m_actorType, VEC3_ZERO, VEC3_ZERO, VEC3_ONE);
	m_pActor->ClearCollision();

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
void CEditCollision::Uninit(void)
{
#if _DEBUG

	for (auto& rCube : m_cube)
	{ // �R���W�����L���[�u�����J��Ԃ�

		// �I������
		rCube.Uninit();
	}

	for (auto& rCylinder : m_cylinder)
	{ // �R���W�����V�����_�[�����J��Ԃ�

		// �I������
		rCylinder.Uninit();
	}

	for (auto& rSphere : m_sphere)
	{ // �R���W�����X�t�B�A�����J��Ԃ�

		// �I������
		rSphere.Uninit();
	}

	for (auto& rPolygon : m_polygon)
	{ // �R���W�����|���S�������J��Ԃ�

		// �I������
		rPolygon.Uninit();
	}

	// �z���S��������
	m_cube.clear();
	m_cylinder.clear();
	m_sphere.clear();
	m_polygon.clear();

	// �G�f�B�^�[���̔j��
	SAFE_REF_RELEASE(m_pEditor);

	// �A�N�^�[�̔j��
	SAFE_UNINIT(m_pActor);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditCollision::Update(void)
{
#if _DEBUG

	// �I�u�W�F�N�g�^�C�v�̕ύX
	ChangeObjectType();

	// �A�N�^�[�̎�ޕύX
	ChangeActorType();

	// �G�f�B�^�[���̍X�V
	assert(m_pEditor != nullptr);
	m_pEditor->Update();

	// ��������
	Create();

	// ��������
	Delete();

#endif	// _DEBUG
}

//============================================================
//	�ۑ�����
//============================================================
HRESULT CEditCollision::Save(void)
{
#if _DEBUG

	// �}�l�[�W���[�̃Z�[�u����
	ManagerSave();

	// �t�@�C�����J��
	std::ofstream  file(SAVE_PASS);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�����蔻��z�u�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
}

	// ���o���������o��
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	�����蔻��z�u�̃Z�[�u�f�[�^ [save_collision.txt]" << std::endl;
	file << "#	Author : ���c �E��" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;
	file << "# ���̍s���牺���R�s�[�� [Collision.txt] �ɒ���t��\n" << std::endl;

	// �����_�����o���̕��@���w��
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// �����蔻��̏��
	CCollManager::SCollision coll = {};

	for (int nCntSave = 0; nCntSave < CActor::TYPE_MAX; nCntSave++)
	{
		// �����蔻��̏����擾����
		coll = CSceneGame::GetCollManager()->GetCollInfo((CActor::EType)nCntSave);

		// �ǂݍ��݊J�n������������o��
		file << "#=================================================================================" << std::endl;
		file << "ACTOR_TYPE = " << nCntSave << std::endl;

		// �ǂݍ��݊J�n������������o��
		file << "STAGE_COLLSET\n" << std::endl;

//============================================================
//	�L���[�u�̓����蔻��
//============================================================

		// �ǂݍ��݊J�n������������o��
		file << "CUBESET" << std::endl;

		// �����蔻��̐��������o��
		file << "	NUM	= " << coll.m_cube.size() << "\n" << std::endl;

		for (const auto& rCube : coll.m_cube)
		{ // �L���[�u�̑������J��Ԃ�

			// ���������o��
			file << "	COLLSET" << std::endl;
			file << "		OFFSET	= " << rCube.offset.x << " " << rCube.offset.y << " " << rCube.offset.z << std::endl;
			file << "		WIDTH	= " << rCube.fWidth << std::endl;
			file << "		HEIGHT	= " << rCube.fHeight << std::endl;
			file << "		DEPTH	= " << rCube.fDepth << std::endl;
			file << "	END_COLLSET" << std::endl;
		}

		// �ǂݍ��ݏI��������������o��
		file << "END_CUBESET\n" << std::endl;

//============================================================
//	�V�����_�[�̓����蔻��
//============================================================

		// �ǂݍ��݊J�n������������o��
		file << "CYLINDERSET" << std::endl;

		// �����蔻��̐��������o��
		file << "	NUM	= " << coll.m_cylinder.size() << std::endl;

		for (const auto& rCylinder : coll.m_cylinder)
		{ // �V�����_�[�̑������J��Ԃ�

			// ���������o��
			file << "	COLLSET" << std::endl;
			file << "		OFFSET	= " << rCylinder.offset.x << " " << rCylinder.offset.y << " " << rCylinder.offset.z << std::endl;
			file << "		RADIUS	= " << rCylinder.fRadius << std::endl;
			file << "		HEIGHT	= " << rCylinder.fHeight << std::endl;
			file << "	END_COLLSET" << std::endl;
		}

		// �ǂݍ��ݏI��������������o��
		file << "END_CYLINDERSET\n" << std::endl;

//============================================================
//	�X�t�B�A�̓����蔻��
//============================================================

		// �ǂݍ��݊J�n������������o��
		file << "SPHERESET" << std::endl;

		// �����蔻��̐��������o��
		file << "	NUM	= " << coll.m_sphere.size() << std::endl;

		for (const auto& rSphere : coll.m_sphere)
		{ // �X�t�B�A�̑������J��Ԃ�

			// ���������o��
			file << "	COLLSET" << std::endl;
			file << "		OFFSET	= " << rSphere.offset.x << " " << rSphere.offset.y << " " << rSphere.offset.z << std::endl;
			file << "		RADIUS	= " << rSphere.fRadius << std::endl;
			file << "	END_COLLSET" << std::endl;
		}

		// �ǂݍ��ݏI��������������o��
		file << "END_SPHERESET\n" << std::endl;

//============================================================
//	�|���S���̓����蔻��
//============================================================

		// �ǂݍ��݊J�n������������o��
		file << "POLYGONSET" << std::endl;

		// �����蔻��̐��������o��
		file << "	NUM	= " << coll.m_polygon.size() << std::endl;

		for (const auto& rPolygon : coll.m_polygon)
		{ // �|���S���̑������J��Ԃ�

			// ���������o��
			file << "	COLLSET" << std::endl;
			file << "		OFFSET	= " << rPolygon.offset.x << " " << rPolygon.offset.y << " " << rPolygon.offset.z << std::endl;
			file << "		ROT	= "		<< rPolygon.rot.x << " " << rPolygon.rot.y << " " << rPolygon.rot.z << std::endl;
			file << "		WIDTH	= " << rPolygon.size.x << std::endl;
			file << "		DEPTH	= " << rPolygon.size.z << std::endl;
			file << "	END_COLLSET" << std::endl;
		}

		// �ǂݍ��ݏI��������������o��
		file << "END_POLYGONSET\n" << std::endl;

		// �ǂݍ��ݏI��������������o��
		file << "END_STAGE_COLLSET" << std::endl;
		file << "#=================================================================================\n\n" << std::endl;
	}

	// ������Ԃ�
	return S_OK;

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditCollision::IsSave(void)
{
#if _DEBUG

	// �G�f�B�^�[���̕ۑ�
	assert(m_pEditor != nullptr);
	return m_pEditor->IsSave();

#else	// NDEBUG

	// false��Ԃ�
	return false;

#endif	// _DEBUG
}

//============================================================
//	���ۑ�����
//============================================================
void CEditCollision::SaveInfo(void)
{
#if _DEBUG

	// �G�f�B�^�[���̏��ۑ�
	assert(m_pEditor != nullptr);
	m_pEditor->SaveInfo();

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditCollision::LoadInfo(void)
{
#if _DEBUG

	// �G�f�B�^�[���̏��Ǎ�
	assert(m_pEditor != nullptr);
	m_pEditor->LoadInfo();

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditCollision::DrawDebugControl(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "�G�f�B�b�g�X�e�[�W�^�C�v�O�i�ύX�F[%s]\n�G�f�B�b�g�X�e�[�W�^�C�v��i�ύX�F[%s]\n", NAME_CHANGE_OBJECT_FRONT, NAME_CHANGE_OBJECT_BACK);

	// �G�f�B�^�[���̑���\��
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugControl();

#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditCollision::DrawDebugInfo(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "%s�F[�G�f�B�b�g�R���W�����^�C�v]\n", TYPE_NAME[m_type]);

	// �G�f�B�^�[���̏��\��
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugInfo();

#endif	// _DEBUG
}

//============================================================
// �I�u�W�F�N�g�̕`��֌W����
//============================================================
void CEditCollision::DispObject(const bool bDisp)
{
	for (int nCntDim = 0; nCntDim < CObject::DIM_MAX; nCntDim++)
	{ // �����̑������J��Ԃ�

		for (int nCntPri = 0; nCntPri < object::MAX_PRIO; nCntPri++)
		{ // �D�揇�ʂ̑������J��Ԃ�

			// �I�u�W�F�N�g�̐擪����
			CObject* pObject = CObject::GetTop(static_cast<CObject::EDim>(nCntDim), nCntPri);

			while (pObject != nullptr)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// ���̃I�u�W�F�N�g����
				CObject* pObjectNext = pObject->GetNext();

				if (pObject->GetLabel() == CObject::LABEL_NONE)
				{ // �����j�����Ȃ����x���̏ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObject = pObjectNext;
					continue;
				}

				if (pObject->IsDeath())
				{ // ���S���Ă���ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObject = pObjectNext;
					continue;
				}

				if (pObject->GetLabel() == CObject::LABEL_PLAYER)
				{ // �v���C���[�̏ꍇ

					// �X�V�󋵂�ω�����
					pObject->SetEnableUpdate(bDisp);
				}

#ifdef _DEBUG

				if (pObject->GetLabel() != CObject::LABEL_DEBUG &&
					pObject->GetLabel() != CObject::LABEL_FIELD)
				{ // �f�o�b�O���x���A�t�B�[���h���x���ȊO

					// �I�u�W�F�N�g�������Ȃ�����
					pObject->SetEnableDebugDisp(bDisp);
				}

#endif // _DEBUG

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObject = pObjectNext;
			}
		}
	}
}

//============================================================
//	�I�u�W�F�N�g�^�C�v�̕ύX����
//============================================================
void CEditCollision::ChangeObjectType(void)
{
	// �I�u�W�F�N�g�^�C�v�̕ύX
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	if (pKeyboard->IsTrigger(KEY_CHANGE_OBJECT_FRONT))
	{
		// �G�f�B�^�[���̔j��
		SAFE_REF_RELEASE(m_pEditor);

		// �I�u�W�F�N�g�^�C�v�̕ύX
		m_type = (CEditCollision::EType)((m_type + 1) % CEditCollision::TYPE_MAX);

		if (m_pEditor == nullptr)
		{ // �G�f�B�^�[�� NULL �̏ꍇ

			// �C���f�b�N�X
			int nIdx = NONE_IDX;

			// �C���f�b�N�X����
			switch (m_type)
			{
			case CEditCollision::TYPE_CUBE:
				nIdx = m_cube.size();
				break;
			case CEditCollision::TYPE_CYLINDER:
				nIdx = m_cylinder.size();
				break;
			case CEditCollision::TYPE_SPHERE:
				nIdx = m_sphere.size();
				break;
			case CEditCollision::TYPE_POLYGON:
				nIdx = m_polygon.size();
				break;
			default:		// ��O����
				assert(false);
				break;
			}

			// �G�f�B�^�[���̐���
			m_pEditor = CEditorCollShape::Create(m_type, nIdx);
			assert(m_pEditor != nullptr);	// �������s
		}
	}
	else if (pKeyboard->IsTrigger(KEY_CHANGE_OBJECT_BACK))
	{
		// �G�f�B�^�[���̔j��
		SAFE_REF_RELEASE(m_pEditor);

		// �I�u�W�F�N�g�^�C�v�̕ύX
		m_type = (CEditCollision::EType)((m_type + (CEditCollision::TYPE_MAX - 1)) % CEditCollision::TYPE_MAX);

		if (m_pEditor == nullptr)
		{ // �G�f�B�^�[�� NULL �̏ꍇ

			// �C���f�b�N�X
			int nIdx = NONE_IDX;

			// �C���f�b�N�X����
			switch (m_type)
			{
			case CEditCollision::TYPE_CUBE:
				nIdx = m_cube.size();
				break;
			case CEditCollision::TYPE_CYLINDER:
				nIdx = m_cylinder.size();
				break;
			case CEditCollision::TYPE_SPHERE:
				nIdx = m_sphere.size();
				break;
			case CEditCollision::TYPE_POLYGON:
				nIdx = m_polygon.size();
				break;
			default:		// ��O����
				assert(false);
				break;
			}

			// �G�f�B�^�[���̐���
			m_pEditor = CEditorCollShape::Create(m_type, nIdx);
			assert(m_pEditor != nullptr);	// �������s
		}
	}
}

//============================================================
//	�A�N�^�[�ύX����
//============================================================
void CEditCollision::ChangeActorType(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// ��ނ�ύX
	if (pKeyboard->IsTrigger(KEY_CHANGE_ACTOR))
	{
		m_actorType = (CActor::EType)((m_actorType + 1) % CActor::TYPE_MAX);

		// ���f���𐶐�������
		m_pActor->Uninit();
		m_pActor = CActor::Create(m_actorType, VEC3_ZERO);
		m_pActor->ClearCollision();
		for (auto& rCube : m_cube)
		{ // �R���W�����L���[�u�����J��Ԃ�

			// �I������
			rCube.Uninit();
		}
		for (auto& rCylinder : m_cylinder)
		{ // �R���W�����V�����_�[�����J��Ԃ�

			// �I������
			rCylinder.Uninit();
		}
		for (auto& rSphere : m_sphere)
		{ // �R���W�����X�t�B�A�����J��Ԃ�

			// �I������
			rSphere.Uninit();
		}
		for (auto& rPolygon : m_polygon)
		{ // �R���W�����|���S�������J��Ԃ�

			// �I������
			rPolygon.Uninit();
		}
		// �z���S��������
		m_cube.clear();
		m_cylinder.clear();
		m_sphere.clear();
		m_polygon.clear();
	}
}

//============================================================
// ��������
//============================================================
void CEditCollision::Create(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_CREATE))
	{ // �����L�[���������ꍇ

		CEditCollCube* pCube = nullptr;
		CEditCollCylinder* pCylinder = nullptr;
		CEditCollSphere* pSphere = nullptr;
		CEditCollPolygon* pPolygon = nullptr;

		switch (m_type)
		{
		case CEditCollision::TYPE_CUBE:

			pCube = dynamic_cast<CEditCollCube*>(m_pEditor);

			if (pCube != nullptr)
			{ // �L���[�u�� NULL ����Ȃ��ꍇ

				// �ǉ�����
				m_cube.push_back(*pCube);
			}

			break;
		case CEditCollision::TYPE_CYLINDER:

			pCylinder = dynamic_cast<CEditCollCylinder*>(m_pEditor);

			if (pCylinder != nullptr)
			{ // �V�����_�[�� NULL ����Ȃ��ꍇ

				// �ǉ�����
				m_cylinder.push_back(*pCylinder);
			}

			break;
		case CEditCollision::TYPE_SPHERE:

			pSphere = dynamic_cast<CEditCollSphere*>(m_pEditor);

			if (pSphere != nullptr)
			{ // �X�t�B�A�� NULL ����Ȃ��ꍇ

				// �ǉ�����
				m_sphere.push_back(*pSphere);
			}

			break;
		case CEditCollision::TYPE_POLYGON:

			pPolygon = dynamic_cast<CEditCollPolygon*>(m_pEditor);

			if (pPolygon != nullptr)
			{ // �|���S���� NULL ����Ȃ��ꍇ

				// �ǉ�����
				m_polygon.push_back(*pPolygon);
			}

			break;
		default:		// ��O����
			assert(false);
			break;
		}

		// ��������
		m_pEditor->Create();
	}
}

//============================================================
// ��������
//============================================================
void CEditCollision::Delete(void)
{
	bool bRelease = false;

	if (GET_INPUTKEY->IsTrigger(KEY_RELEASE))
	{ // �����L�[���������ꍇ

		// ���������ʂ�
		bRelease = true;
	}

	switch (m_type)
	{
	case CEditCollision::TYPE_CUBE:

		// �L���[�u�̏�������
		DeleteCollCube(bRelease);

		break;

	case CEditCollision::TYPE_CYLINDER:

		// �V�����_�[�̏�������
		DeleteCollCylinder(bRelease);

		break;

	case CEditCollision::TYPE_SPHERE:

		// �X�t�B�A�̏�������
		DeleteCollSphere(bRelease);

		break;

	case CEditCollision::TYPE_POLYGON:

		// �|���S���̏�������
		DeleteCollPolygon(bRelease);

		break;

	default:		// ��O����
		assert(false);
		break;
	}
}

//============================================================
// �}�l�[�W���[�̃Z�[�u����
//============================================================
void CEditCollision::ManagerSave(void)
{
	// ���[�J���ϐ��錾
	std::vector<CCollManager::SCollCube> saveCube;			// �L���[�u�̏��z��
	std::vector<CCollManager::SCollCylinder> saveCylinder;	// �V�����_�[�̏��z��
	std::vector<CCollManager::SCollSphere> saveSphere;		// �X�t�B�A�̏��z��
	std::vector<CCollManager::SCollPolygon> savePolygon;	// �|���S���̏��z��
	CCollManager::SCollCube cube;				// �L���[�u
	CCollManager::SCollCylinder cylinder;		// �V�����_�[
	CCollManager::SCollSphere sphere;			// �X�t�B�A
	CCollManager::SCollPolygon polygon;			// �|���S��

	for (const auto& rCube : m_cube)
	{
		// �����擾
		cube.offset = rCube.GetVec3OffSet();		// �I�t�Z�b�g���W
		cube.fWidth = rCube.GetInfo().fWidth;		// ��
		cube.fHeight = rCube.GetInfo().fHeight;		// ����
		cube.fDepth = rCube.GetInfo().fDepth;		// ���s

		// ����ݒ�
		saveCube.push_back(cube);
	}

	for (const auto& rCylinder : m_cylinder)
	{
		// �����擾
		cylinder.offset = rCylinder.GetVec3OffSet();		// �I�t�Z�b�g���W
		cylinder.fRadius = rCylinder.GetInfo().fRadius;		// ���a
		cylinder.fHeight = rCylinder.GetInfo().fHeight;		// ����

		// ����ݒ�
		saveCylinder.push_back(cylinder);
	}

	for (const auto& rSphere : m_sphere)
	{
		// �����擾
		sphere.offset = rSphere.GetVec3OffSet();		// �I�t�Z�b�g���W
		sphere.fRadius = rSphere.GetInfo().fRadius;		// ���a

		// ����ݒ�
		saveSphere.push_back(sphere);
	}

	for (const auto& rPolygon : m_polygon)
	{
		// �����擾
		polygon.offset = rPolygon.GetVec3OffSet();		// �I�t�Z�b�g���W
		polygon.rot = rPolygon.GetInfo().rot;			// ����
		polygon.size = rPolygon.GetInfo().size;			// �T�C�Y

		// ����ݒ�
		savePolygon.push_back(polygon);
	}

	// �����蔻��}�l�[�W���[�ɐݒ�
	CSceneGame::GetCollManager()->SetCollInfo(m_actorType, CCollManager::SCollision(saveCube, saveCylinder, saveSphere, savePolygon));
}

//============================================================
//	�R���W�����L���[�u�̐F�̑S����������
//============================================================
void CEditCollision::InitAllColorCollCube(void)
{
#ifdef _DEBUG

	for (auto& rCube : m_cube)
	{ // �R���W�����L���[�u�����J��Ԃ�

		// �ʏ�F��ݒ�
		rCube.GetCube()->GetCube()->SetCubeColor(COL);
	}

#endif // _DEBUG
}

//============================================================
// �V�����_�[�̐F�S������
//============================================================
void CEditCollision::InitAllColorCollCylinder(void)
{
#ifdef _DEBUG

	for (auto& rCylinder : m_cylinder)
	{ // �R���W�����V�����_�[�����J��Ԃ�

		// �ʏ�F��ݒ�
		rCylinder.GetCylinder()->GetTube()->SetColor(COL);
	}

#endif // _DEBUG
}

//============================================================
// �X�t�B�A�̐F�S������
//============================================================
void CEditCollision::InitAllColorCollSphere(void)
{
#ifdef _DEBUG

	for (auto& rSphere : m_sphere)
	{ // �R���W�����X�t�B�A�����J��Ԃ�

		// �ʏ�F��ݒ�
		rSphere.GetSphere()->GetSphere()->SetColor(COL);
	}

#endif // _DEBUG
}

//============================================================
// �|���S���̐F�S������
//============================================================
void CEditCollision::InitAllColorCollPolygon(void)
{
#ifdef _DEBUG

	for (auto& rPolygon : m_polygon)
	{ // �R���W�����X�t�B�A�����J��Ԃ�

		// �ʏ�F��ݒ�
		rPolygon.GetPolygon()->GetPolygon()->SetColor(COL);
	}

#endif // _DEBUG
}

//============================================================
// �L���[�u�̏�������
//============================================================
void CEditCollision::DeleteCollCube(const bool bRelease)
{
	D3DXVECTOR3 posEdit = m_pEditor->GetVec3OffSet();	// �G�f�B�b�g�̈ʒu
	CEffect3D::Create(posEdit, DELETE_RANGE);			// �G�t�F�N�g�𐶐�

	for (auto cube = m_cube.begin(); cube != m_cube.end(); cube++)
	{ // �t�B�[���h�����J��Ԃ�

		D3DXVECTOR3 posOther = cube->GetVec3OffSet();	// �Ώۂ̒n�ʈʒu
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// ���g�̑傫��
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// �Ώۂ̑傫��

		// ���g�̑傫����ݒ�
		sizeThis.x = DELETE_RANGE;		// ����T�C�YX��ݒ�
		sizeThis.y = DELETE_RANGE;		// ����T�C�YY��ݒ�
		sizeThis.z = DELETE_RANGE;		// ����T�C�YZ��ݒ�
		sizeThis *= 0.5f;				// ����T�C�Y�𔼕���

		// �Ώۂ̑傫����ݒ�
		sizeOther.x = cube->GetCube()->GetWidth();	// ����T�C�YX��ݒ�
		sizeOther.y = cube->GetCube()->GetHeight();	// ����T�C�YY��ݒ�
		sizeOther.z = cube->GetCube()->GetDepth();	// ����T�C�YZ��ݒ�
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

			if (bRelease)
			{ // �j������ꍇ

				// �I������
				cube->Uninit();

				// ��������
				m_cube.erase(cube);

				// �����o��
				break;
			}
			else
			{ // �j�����Ȃ��ꍇ

#ifdef _DEBUG

				// ��ݒ�
				cube->GetCube()->GetCube()->SetCubeColor(XCOL_BLUE);

#endif // _DEBUG

				// �����o��
				break;
			}
		}
		else
		{ // ����O�������ꍇ

#ifdef _DEBUG

			// �ʏ�F��ݒ�
			cube->GetCube()->GetCube()->SetCubeColor(COL);

#endif // _DEBUG
		}
	}
}

//============================================================
// �V�����_�[�̏�������
//============================================================
void CEditCollision::DeleteCollCylinder(const bool bRelease)
{
	D3DXVECTOR3 posEdit = m_pEditor->GetVec3OffSet();	// �G�f�B�b�g�̈ʒu
	CEffect3D::Create(posEdit, DELETE_RANGE);			// �G�t�F�N�g�𐶐�

	for (auto cylinder = m_cylinder.begin(); cylinder != m_cylinder.end(); cylinder++)
	{ // �t�B�[���h�����J��Ԃ�

		D3DXVECTOR3 posOther = cylinder->GetVec3OffSet();	// �Ώۂ̒n�ʈʒu
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// ���g�̑傫��
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// �Ώۂ̑傫��

		// ���g�̑傫����ݒ�
		sizeThis.x = DELETE_RANGE;		// ����T�C�YX��ݒ�
		sizeThis.y = DELETE_RANGE;		// ����T�C�YY��ݒ�
		sizeThis.z = DELETE_RANGE;		// ����T�C�YZ��ݒ�
		sizeThis *= 0.5f;				// ����T�C�Y�𔼕���

		// �Ώۂ̑傫����ݒ�
		sizeOther.x = cylinder->GetCylinder()->GetRadius();	// ����T�C�YX��ݒ�
		sizeOther.y = cylinder->GetCylinder()->GetHeight();	// ����T�C�YY��ݒ�
		sizeOther.z = cylinder->GetCylinder()->GetRadius();	// ����T�C�YZ��ݒ�
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

			if (bRelease)
			{ // �j������ꍇ

				// �I������
				cylinder->Uninit();

				// ��������
				m_cylinder.erase(cylinder);

				// �����o��
				break;
			}
			else
			{ // �j�����Ȃ��ꍇ

#ifdef _DEBUG

				// ��ݒ�
				cylinder->GetCylinder()->GetTube()->SetColor(XCOL_BLUE);

#endif // _DEBUG

				// �����o��
				break;
			}
		}
		else
		{ // ����O�������ꍇ

#ifdef _DEBUG

			// �ʏ�F��ݒ�
			cylinder->GetCylinder()->GetTube()->SetColor(COL);

#endif // _DEBUG
		}
	}
}

//============================================================
// �X�t�B�A�̏�������
//============================================================
void CEditCollision::DeleteCollSphere(const bool bRelease)
{
	D3DXVECTOR3 posEdit = m_pEditor->GetVec3OffSet();	// �G�f�B�b�g�̈ʒu
	CEffect3D::Create(posEdit, DELETE_RANGE);			// �G�t�F�N�g�𐶐�

	for (auto sphere = m_sphere.begin(); sphere != m_sphere.end(); sphere++)
	{ // �t�B�[���h�����J��Ԃ�

		D3DXVECTOR3 posOther = sphere->GetVec3OffSet();	// �Ώۂ̒n�ʈʒu
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// ���g�̑傫��
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// �Ώۂ̑傫��

		// ���g�̑傫����ݒ�
		sizeThis.x = DELETE_RANGE;		// ����T�C�YX��ݒ�
		sizeThis.y = DELETE_RANGE;		// ����T�C�YY��ݒ�
		sizeThis.z = DELETE_RANGE;		// ����T�C�YZ��ݒ�
		sizeThis *= 0.5f;				// ����T�C�Y�𔼕���

		// �Ώۂ̑傫����ݒ�
		sizeOther.x = sphere->GetSphere()->GetRadius();	// ����T�C�YX��ݒ�
		sizeOther.y = sphere->GetSphere()->GetRadius();	// ����T�C�YY��ݒ�
		sizeOther.z = sphere->GetSphere()->GetRadius();	// ����T�C�YZ��ݒ�
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

			if (bRelease)
			{ // �j������ꍇ

				// �I������
				sphere->Uninit();

				// ��������
				m_sphere.erase(sphere);

				// �����o��
				break;
			}
			else
			{ // �j�����Ȃ��ꍇ

#ifdef _DEBUG

				// ��ݒ�
				sphere->GetSphere()->GetSphere()->SetColor(XCOL_BLUE);

#endif // _DEBUG

				// �����o��
				break;
			}
		}
		else
		{ // ����O�������ꍇ

#ifdef _DEBUG

			// �ʏ�F��ݒ�
			sphere->GetSphere()->GetSphere()->SetColor(COL);

#endif // _DEBUG
		}
	}
}

//============================================================
// �|���S���̏�������
//============================================================
void CEditCollision::DeleteCollPolygon(const bool bRelease)
{
	D3DXVECTOR3 posEdit = m_pEditor->GetVec3OffSet();	// �G�f�B�b�g�̈ʒu
	CEffect3D::Create(posEdit, DELETE_RANGE);			// �G�t�F�N�g�𐶐�

	for (auto polygon = m_polygon.begin(); polygon != m_polygon.end(); polygon++)
	{ // �t�B�[���h�����J��Ԃ�

		D3DXVECTOR3 posOther = polygon->GetVec3OffSet();	// �Ώۂ̒n�ʈʒu
		D3DXVECTOR3 sizeThis = VEC3_ZERO;	// ���g�̑傫��
		D3DXVECTOR3 sizeOther = VEC3_ZERO;	// �Ώۂ̑傫��

		// ���g�̑傫����ݒ�
		sizeThis.x = DELETE_RANGE;		// ����T�C�YX��ݒ�
		sizeThis.y = DELETE_RANGE;		// ����T�C�YY��ݒ�
		sizeThis.z = DELETE_RANGE;		// ����T�C�YZ��ݒ�
		sizeThis *= 0.5f;				// ����T�C�Y�𔼕���

		// �Ώۂ̑傫����ݒ�
		sizeOther = polygon->GetInfo().size * 0.5f;	// ����T�C�Y��ݒ�

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

			if (bRelease)
			{ // �j������ꍇ

				// �I������
				polygon->Uninit();

				// ��������
				m_polygon.erase(polygon);

				// �����o��
				break;
			}
			else
			{ // �j�����Ȃ��ꍇ

#ifdef _DEBUG

				// ��ݒ�
				polygon->GetPolygon()->GetPolygon()->SetColor(XCOL_BLUE);

#endif // _DEBUG

				// �����o��
				break;
			}
		}
		else
		{ // ����O�������ꍇ

#ifdef _DEBUG

			// �ʏ�F��ݒ�
			polygon->GetPolygon()->GetPolygon()->SetColor(COL);

#endif // _DEBUG
		}
	}
}
