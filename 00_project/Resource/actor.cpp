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

#include "collisionCylinder.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* MODEL = "data\\MODEL\\Rock\\Moss-Rock000.x";	// ���f���̃p�X
	const int PRIORITY = 4;	// �A�N�^�[�̗D�揇��
}

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
m_pCollisionList(nullptr)
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

	if (m_pCollisionList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pCollisionList = CListManager<CCollision>::Create();

		if (m_pCollisionList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
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
	// �������擾
	int nNumColl = m_pCollisionList->GetNumAll();

	for (int nCnt = 0; nCnt < nNumColl; nCnt++)
	{
		// �I������
		(*m_pCollisionList->GetBegin())->Uninit();
	}

	// ���X�g�}�l�[�W���[�̔j��
	m_pCollisionList->Release(m_pCollisionList);

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
		pActor->BindModel(MODEL);

		// TODO�F���̓����蔻�����ǉ�
		pActor->m_pCollisionList->AddList(CCollisionCylinder::Create(rPos, 60.0f, 30.0f));

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
	bool& bJump						// �W�����v��
)
{
	// �����蔻��̃��X�g�\����������Δ�����
	if (m_pCollisionList == nullptr) { return; }

	std::list<CCollision*> list = m_pCollisionList->GetList();	// ���X�g���擾
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu

	for (auto collision : list)
	{
		// �q�b�g����
		collision->Hit(rPos, rPosOld, fRadius, fHeight, rMove, bJump);
	}
}
