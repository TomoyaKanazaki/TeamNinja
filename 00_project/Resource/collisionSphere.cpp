//============================================================
//
//	���̓����蔻�菈�� [collisionSphere.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "collisionSphere.h"
#include "collision.h"

//************************************************************
// �萔��`
//************************************************************
namespace
{
	const int SPHERE_PRIORITY = 5;			// ���̗D�揇��
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// �F
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CCollisionSphere>* CCollisionSphere::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//============================================================
// �R���X�g���N�^
//============================================================
CCollisionSphere::CCollisionSphere() : CCollision(),
m_fRadius(0.0f),			// ���a
m_bList(false)				// ���X�g��
#ifdef _DEBUG
, m_pSphere(nullptr)	// ���b�V���X�t�B�A
#endif // _DEBUG

{

}

//============================================================
// �f�X�g���N�^
//============================================================
CCollisionSphere::~CCollisionSphere()
{

}

//============================================================
// ����������
//============================================================
void CCollisionSphere::Init(void)
{
	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CCollisionSphere>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);
}

//============================================================
// �I������
//============================================================
void CCollisionSphere::Uninit(void)
{
#ifdef _DEBUG

	// ���b�V���X�t�B�A�̏I��
	SAFE_UNINIT(m_pSphere);

#endif // _DEBUG

	if (m_bList == true)
	{ // ���X�g���肵�Ă���ꍇ

		// ���X�g���玩�g�̃I�u�W�F�N�g���폜
		m_pList->DelList(m_iterator);

		if (m_pList->GetNumAll() == 0)
		{ // �I�u�W�F�N�g������Ȃ��ꍇ

			// ���X�g�}�l�[�W���[�̔j��
			m_pList->Release(m_pList);
		}
	}

	// �I������
	CCollision::Uninit();
}

//============================================================
// �q�b�g����
//============================================================
bool CCollisionSphere::Hit
(
	D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
	const float fRadius,			// ���a
	const float fHeight,			// ����
	D3DXVECTOR3& rMove,				// �ړ���
	bool& rJump						// �W�����v��
)
{
	// �����蔻��̌��ʂ�Ԃ�
	return collision::ResponseCircle3D(rPos, GetPos(), fRadius, m_fRadius);
}

//============================================================
// �I�t�Z�b�g�ݒ菈��
//============================================================
void CCollisionSphere::OffSet(const D3DXMATRIX& mtx)
{
	// �I�t�Z�b�g����
	CCollision::OffSet(mtx);

#ifdef _DEBUG

	// �X�t�B�A�̈ʒu��ݒ�
	m_pSphere->SetVec3Position(GetPos());

#endif // _DEBUG
}

//============================================================
// ��������
//============================================================
CCollisionSphere* CCollisionSphere::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rOffset, const float fRadius, const bool bList)
{
	// �����蔻��̐���
	CCollisionSphere* pColl = new CCollisionSphere();

	// �����o���Ă��Ȃ��ꍇ nullptr ��Ԃ�
	if (pColl == nullptr) { return nullptr; }

	// ���X�g�󋵂�ݒ肷��
	pColl->m_bList = bList;

	if (bList == true)
	{ // ���X�g�󋵂� true �̏ꍇ

		// ����������
		pColl->Init();
	}

	// �ʒu��ݒ肷��
	pColl->SetPos(rPos);

	// �I�t�Z�b�g�̐ݒ�
	pColl->SetOffset(rOffset);

	// ���a��ݒ�
	pColl->m_fRadius = fRadius;

#ifdef _DEBUG

	// �X�t�B�A�𐶐�
	pColl->m_pSphere = CObjectMeshSphere::Create(rPos, VEC3_ZERO, COL, POSGRID2(8, 2), POSGRID2(8, 2), fRadius);
	pColl->m_pSphere->SetPriority(SPHERE_PRIORITY);
	pColl->m_pSphere->SetLabel(CObject::LABEL_COLLISION);

#endif // _DEBUG

	// �����蔻���Ԃ�
	return pColl;
}

//============================================================
// ���X�g�擾
//============================================================
CListManager<CCollisionSphere>* CCollisionSphere::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_pList;
}
