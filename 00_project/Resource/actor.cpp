//============================================================
//
//	�A�N�^�[���� [actor.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "actor.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"

#include "sceneGame.h"
#include "collManager.h"
#include "collisionCube.h"
#include "collisionCylinder.h"
#include "collisionSphere.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* MODEL[] =	// ���f���̃p�X
	{
		"data\\MODEL\\Rock\\Moss-Rock000.x",		// ��(��)
	};
	const int PRIORITY = 4;	// �A�N�^�[�̗D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(MODEL) == CActor::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
// �ÓI�����o�ϐ��錾
//************************************************************
CListManager<CActor>* CActor::m_pList = nullptr;		// ���X�g�\��

//************************************************************
//	�q�N���X [CActor] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CActor::CActor() : CObjectModel(CObject::LABEL_ACTOR, CObject::DIM_3D, PRIORITY),
m_type(TYPE_ROCK_S)		// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CActor::~CActor()
{

}

//============================================================
//	����������
//============================================================
HRESULT CActor::Init(void)
{
	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CActor>::Create();

		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CActor::Uninit(void)
{
	for (auto cube : m_cube)
	{
		// �I������
		cube->Uninit();
	}

	for (auto cylinder : m_cylinder)
	{
		// �I������
		cylinder->Uninit();
	}

	for (auto sphere : m_sphere)
	{
		// �I������
		sphere->Uninit();
	}

	// �N���A����
	m_cube.clear();
	m_cylinder.clear();
	m_sphere.clear();

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CActor::Update(const float fDeltaTime)
{
	for (auto cube : m_cube)
	{
		// �I�t�Z�b�g����
		cube->OffSet(GetMtxWorld());
	}

	for (auto cylinder : m_cylinder)
	{
		// �I������
		cylinder->OffSet(GetMtxWorld());
	}

	for (auto sphere : m_sphere)
	{
		// �I������
		sphere->OffSet(GetMtxWorld());
	}

	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CActor::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CActor* CActor::Create
( // ����
	const EType type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rScale	// �傫��
)
{
	// ���f��UI�̐���
	CActor* pActor = new CActor;

	if (pActor == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �A�N�^�[�̏�����
		if (FAILED(pActor->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �A�N�^�[�̔j��
			SAFE_DELETE(pActor);
			return nullptr;
		}

		// �ʒu��ݒ�
		pActor->SetVec3Position(rPos);

		// ������ݒ�
		pActor->SetVec3Rotation(rRot);

		// �g�嗦��ݒ�
		pActor->SetVec3Scaling(rScale);

		// ���f���̊��蓖�ď���
		pActor->BindModel(MODEL[type]);

		// ��ނ�ݒ�
		pActor->m_type = type;

		// �����蔻��̊��蓖�ď���
		pActor->BindCollision();

		// �m�ۂ����A�h���X��Ԃ�
		return pActor;
	}
}

//============================================================
// ���X�g�\���̎擾����
//============================================================
CListManager<CActor>* CActor::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_pList;
}

//============================================================
// �����蔻�菈��
//============================================================
void CActor::Collision
(
	D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
	const float fRadius,			// ���a
	const float fHeight,			// ����
	D3DXVECTOR3& rMove,				// �ړ���
	bool& bJump,					// �W�����v��
	bool* pHit						// �����������ǂ����̔���
)
{
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu

	for (auto cube : m_cube)
	{
		// �q�b�g����
		cube->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);

		if (pHit != nullptr)
		{ // �q�b�g�󋵂� NULL ����Ȃ��ꍇ

			// �q�b�g�󋵂� true �ɂ���
			*pHit = true;
		}
	}

	for (auto cylinder : m_cylinder)
	{
		// �q�b�g����
		cylinder->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);

		if (pHit != nullptr)
		{ // �q�b�g�󋵂� NULL ����Ȃ��ꍇ

			// �q�b�g�󋵂� true �ɂ���
			*pHit = true;
		}
	}

	for (auto sphere : m_sphere)
	{
		// �q�b�g����
		sphere->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);

		if (pHit != nullptr)
		{ // �q�b�g�󋵂� NULL ����Ȃ��ꍇ

			// �q�b�g�󋵂� true �ɂ���
			*pHit = true;
		}
	}
}

//============================================================
// �����蔻��̊��蓖�ď���
//============================================================
void CActor::BindCollision(void)
{
	// �����蔻����擾
	CCollManager::SCollision coll = CSceneGame::GetCollManager()->GetCollInfo(m_type);

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_cube.size()); nCnt++)
	{
		// �L���[�u�̏���ǉ�����
		m_cube.push_back(CCollisionCube::Create
		(
			GetVec3Position(),				// �ʒu
			coll.m_cube[nCnt].offset,		// �I�t�Z�b�g���W
			coll.m_cube[nCnt].fWidth,		// ��
			coll.m_cube[nCnt].fHeight,		// ����
			coll.m_cube[nCnt].fDepth,		// ���s
			GetVec3Rotation().y)			// ����
		);
	}

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_cylinder.size()); nCnt++)
	{
		// �V�����_�[�̏���ǉ�����
		m_cylinder.push_back(CCollisionCylinder::Create
		(
			GetVec3Position(),				// �ʒu
			coll.m_cylinder[nCnt].offset,	// �I�t�Z�b�g���W
			coll.m_cylinder[nCnt].fRadius,	// ���a
			coll.m_cylinder[nCnt].fHeight	// ����
		));
	}

	for (int nCnt = 0; nCnt < static_cast<int>(coll.m_sphere.size()); nCnt++)
	{
		// �X�t�B�A�̏���ǉ�����
		m_sphere.push_back(CCollisionSphere::Create
		(
			GetVec3Position(),				// �ʒu
			coll.m_sphere[nCnt].offset,		// �I�t�Z�b�g���W
			coll.m_sphere[nCnt].fRadius		// ���a
		));
	}
}
