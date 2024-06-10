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

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_CREATE			(DIK_0)	// �����L�[
#define NAME_CREATE			("0")	// �����\��
#define KEY_RELEASE			(DIK_9)	// �j���L�[
#define NAME_RELEASE		("9")	// �j���\��
#define KEY_CHANGE_OBJECT	(DIK_2)	// �I�u�W�F�N�g�^�C�v�ύX�L�[
#define NAME_CHANGE_OBJECT	("2")	// �I�u�W�F�N�g�^�C�v�ύX�\��

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

	// �z���S��������
	m_cube.clear();
	m_cylinder.clear();
	m_sphere.clear();

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
	file << "# ���̍s���牺���R�s�[�� [stage.txt] �ɒ���t��\n" << std::endl;

	// �����_�����o���̕��@���w��
	file << std::fixed << std::setprecision(DIGIT_FLOAT);

	// �ǂݍ��݊J�n������������o��
	file << "STAGE_COLLISIONSET\n" << std::endl;

	// �A�N�^�[�̎�ނ������o��
	file << "ACTOR_TYPE = " << m_actorType << std::endl;

//============================================================
//	�L���[�u�̓����蔻��
//============================================================

	// �ǂݍ��݊J�n������������o��
	file << "CUBE_COLLISIONSET" << std::endl;

	// �����蔻��̐��������o��
	file << "\nCUBE_NUM	= " << m_cube.size() << "\n" << std::endl;

	for (const auto& rCube : m_cube)
	{ // �L���[�u�̑������J��Ԃ�

		// ���������o��
		file << "	COLLISIONSET" << std::endl;
		file << "		OFFSET	= " << rCube.GetVec3OffSet().x << " " << rCube.GetVec3OffSet().y << " " << rCube.GetVec3OffSet().z << std::endl;
		file << "		WIDTH	= " << rCube.GetCube()->GetWidth() << std::endl;
		file << "		HEIGHT	= " << rCube.GetCube()->GetHeight() << std::endl;
		file << "		DEPTH	= " << rCube.GetCube()->GetDepth() << std::endl;
		file << "	END_COLLISIONSET\n" << std::endl;
	}

	// �ǂݍ��ݏI��������������o��
	file << "END_CUBE_COLLISIONSET\n" << std::endl;

//============================================================
//	�V�����_�[�̓����蔻��
//============================================================

	// �ǂݍ��݊J�n������������o��
	file << "CYLINDER_COLLISIONSET" << std::endl;

	// �����蔻��̐��������o��
	file << "\nCYLINDER_NUM	= " << m_cylinder.size() << "\n" << std::endl;

	for (const auto& rCylinder : m_cylinder)
	{ // �V�����_�[�̑������J��Ԃ�

		// ���������o��
		file << "	COLLISIONSET" << std::endl;
		file << "		OFFSET	= " << rCylinder.GetVec3OffSet().x << " " << rCylinder.GetVec3OffSet().y << " " << rCylinder.GetVec3OffSet().z << std::endl;
		file << "		RADIUS	= " << rCylinder.GetCylinder()->GetRadius() << std::endl;
		file << "		HEIGHT	= " << rCylinder.GetCylinder()->GetHeight() << std::endl;
		file << "	END_COLLISIONSET\n" << std::endl;
	}

	// �ǂݍ��ݏI��������������o��
	file << "END_CYLINDER_COLLISIONSET\n" << std::endl;

//============================================================
//	�X�t�B�A�̓����蔻��
//============================================================

	// �ǂݍ��݊J�n������������o��
	file << "SPHERE_COLLISIONSET" << std::endl;

	// �����蔻��̐��������o��
	file << "\nSPHERE_NUM	= " << m_sphere.size() << "\n" << std::endl;

	for (const auto& rSphere : m_sphere)
	{ // �X�t�B�A�̑������J��Ԃ�

		// ���������o��
		file << "	COLLISIONSET" << std::endl;
		file << "		OFFSET	= " << rSphere.GetVec3OffSet().x << " " << rSphere.GetVec3OffSet().y << " " << rSphere.GetVec3OffSet().z << std::endl;
		file << "		RADIUS	= " << rSphere.GetSphere()->GetRadius() << std::endl;
		file << "	END_COLLISIONSET\n" << std::endl;
	}

	// �ǂݍ��ݏI��������������o��
	file << "END_SPHERE_COLLISIONSET\n" << std::endl;

	// �ǂݍ��ݏI��������������o��
	file << "END_STAGE_COLLISIONSET" << std::endl;

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

	DebugProc::Print(DebugProc::POINT_RIGHT, "�G�f�B�b�g�X�e�[�W�^�C�v�ύX�F[%s]\n", NAME_CHANGE_OBJECT);

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

				if (pObject->GetLabel() != CObject::LABEL_DEBUG &&
					pObject->GetLabel() != CObject::LABEL_FIELD)
				{ // �f�o�b�O���x���A�t�B�[���h���x���ȊO

					// �I�u�W�F�N�g�������Ȃ�����
					pObject->SetEnableDebugDisp(bDisp);
				}

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
	if (pKeyboard->IsTrigger(KEY_CHANGE_OBJECT))
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
// ��������
//============================================================
void CEditCollision::Create(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_CREATE))
	{ // �����L�[���������ꍇ

		CEditCollCube* pCube = nullptr;
		CEditCollCylinder* pCylinder = nullptr;
		CEditCollSphere* pSphere = nullptr;

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
			{ // �L���[�u�� NULL ����Ȃ��ꍇ

				// �ǉ�����
				m_cylinder.push_back(*pCylinder);
			}

			break;
		case CEditCollision::TYPE_SPHERE:

			pSphere = dynamic_cast<CEditCollSphere*>(m_pEditor);

			if (pSphere != nullptr)
			{ // �L���[�u�� NULL ����Ȃ��ꍇ

				// �ǉ�����
				m_sphere.push_back(*pSphere);
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

	default:		// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�R���W�����L���[�u�̐F�̑S����������
//============================================================
void CEditCollision::InitAllColorCollCube(void)
{
	for (auto& rCube : m_cube)
	{ // �R���W�����L���[�u�����J��Ԃ�

#ifdef _DEBUG

		// �ʏ�F��ݒ�
		rCube.GetCube()->GetCube()->SetCubeColor(COL);

#endif // _DEBUG
	}
}

//============================================================
// �V�����_�[�̐F�S������
//============================================================
void CEditCollision::InitAllColorCollCylinder(void)
{
	for (auto& rCylinder : m_cylinder)
	{ // �R���W�����V�����_�[�����J��Ԃ�

#ifdef _DEBUG

		// �ʏ�F��ݒ�
		rCylinder.GetCylinder()->GetTube()->SetColor(COL);

#endif // _DEBUG
	}
}

//============================================================
// �X�t�B�A�̐F�S������
//============================================================
void CEditCollision::InitAllColorCollSphere(void)
{
	for (auto& rSphere : m_sphere)
	{ // �R���W�����X�t�B�A�����J��Ԃ�

#ifdef _DEBUG

		// �ʏ�F��ݒ�
		rSphere.GetSphere()->GetSphere()->SetColor(COL);

#endif // _DEBUG
	}
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
