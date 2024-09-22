//============================================================
//
//	�~���̓����蔻�菈�� [collisionCylinder.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "collisionCylinder.h"
#include "collision.h"

//************************************************************
// �萔��`
//************************************************************
namespace
{
	const int TUBE_PRIORITY = 5;			// �`���[�u�̗D�揇��
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// �F
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CCollisionCylinder>* CCollisionCylinder::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//============================================================
// �R���X�g���N�^
//============================================================
CCollisionCylinder::CCollisionCylinder() : CCollision(),
m_fRadius(0.0f),		// ���a
m_fHeight(0.0f)			// ����
#ifdef _DEBUG
,m_pTube(nullptr)		// ���b�V���`���[�u
#endif // _DEBUG

{

}

//============================================================
// �f�X�g���N�^
//============================================================
CCollisionCylinder::~CCollisionCylinder()
{

}

//============================================================
// ����������
//============================================================
void CCollisionCylinder::Init(void)
{
	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CCollisionCylinder>::Create();
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
void CCollisionCylinder::Uninit(void)
{
#ifdef _DEBUG

	// ���b�V���`���[�u�̏I��
	SAFE_UNINIT(m_pTube);

#endif // _DEBUG

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I������
	CCollision::Uninit();
}

//============================================================
// �q�b�g����
//============================================================
bool CCollisionCylinder::Hit
(
	D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
	const float fRadius,			// ���a
	const float fHeight,			// ����
	D3DXVECTOR3& rMove,				// �ړ���
	bool& rJump						// �W�����v��
)
{
	// �ϐ���錾
	D3DXVECTOR3 posColl = GetPos();	// �ʒu
	float fLength = 0.0f;			// ����ʒu�Ɣ���ڕW�ʒu�̊Ԃ̋���

	// ����ʒu�Ɣ���ڕW�ʒu�̋��������߂�
	fLength = (rPos.x - posColl.x) * (rPos.x - posColl.x)
		+ (rPos.z - posColl.z) * (rPos.z - posColl.z);

	if (fLength < (fRadius + m_fRadius) * (fRadius + m_fRadius) &&
		rPos.y < posColl.y + m_fHeight &&
		rPos.y + fHeight > posColl.y)
	{ // ������̏ꍇ

		if (rPosOld.y >= posColl.y + m_fHeight &&
			rPos.y < posColl.y + m_fHeight)
		{ // �ォ��̓����蔻��

			// �ʒu��␳����
			rPos.y = posColl.y + m_fHeight;

			// �ړ��ʂ�0�ɂ���
			rMove.y = 0.0f;

			// �W�����v���Ă��Ȃ�
			rJump = false;

			// �^��Ԃ�
			return true;
		}
		else if (rPosOld.y + fHeight <= posColl.y &&
			rPos.y + fHeight > posColl.y)
		{ // �ォ��̓����蔻��

			// �ʒu��␳����
			rPos.y = posColl.y - fHeight;

			// �ړ��ʂ�0�ɂ���
			rMove.y = 0.0f;

			// �^��Ԃ�
			return true;
		}


		// �ϐ���錾
		float fCenterRot = atan2f(rPos.x - posColl.x, rPos.z - posColl.z);	// ����ڕW���猩���������

		// �ʒu��␳
		rPos.x = posColl.x + sinf(fCenterRot) * (fRadius + m_fRadius);
		rPos.z = posColl.z + cosf(fCenterRot) * (fRadius + m_fRadius);

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
// �I�t�Z�b�g�ݒ菈��
//============================================================
void CCollisionCylinder::OffSet(const D3DXMATRIX& mtx)
{
	// �I�t�Z�b�g����
	CCollision::OffSet(mtx);

#ifdef _DEBUG

	// �`���[�u�̈ʒu��ݒ�
	m_pTube->SetVec3Position(GetPos());

#endif // _DEBUG
}

//============================================================
// ��������
//============================================================
CCollisionCylinder* CCollisionCylinder::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rOffset, const float fRadius, const float fHeight)
{
	// �����蔻��̐���
	CCollisionCylinder* pColl = new CCollisionCylinder();

	// �����o���Ă��Ȃ��ꍇ nullptr ��Ԃ�
	if (pColl == nullptr) { return nullptr; }

	// ����������
	pColl->Init();

	// �ʒu��ݒ�
	pColl->SetPos(rPos);

	// �I�t�Z�b�g�̐ݒ�
	pColl->SetOffset(rOffset);

	// ���a��ݒ�
	pColl->m_fRadius = fRadius;

	// ������ݒ�
	pColl->m_fHeight = fHeight;

#ifdef _DEBUG

	// �`���[�u�𐶐�
	pColl->m_pTube = CObjectMeshTube::Create(rPos, VEC3_ZERO, COL, POSGRID2(8, 2), POSGRID2(8, 2), fRadius, fHeight);
	pColl->m_pTube->SetPriority(TUBE_PRIORITY);
	pColl->m_pTube->SetLabel(CObject::LABEL_COLLISION);

#endif // _DEBUG

	// �����蔻���Ԃ�
	return pColl;
}

//============================================================
// ���X�g�擾
//============================================================
CListManager<CCollisionCylinder>* CCollisionCylinder::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_pList;
}
