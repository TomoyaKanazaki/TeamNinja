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

#ifdef _DEBUG
#include "objectMeshSphere.h"
#endif // _DEBUG

//************************************************************
// �萔��`
//************************************************************
namespace
{
	const int SPHERE_PRIORITY = 5;			// ���̗D�揇��
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// �F
}

//============================================================
// �R���X�g���N�^
//============================================================
CCollisionSphere::CCollisionSphere() : CCollision(),
m_fRadius(0.0f)			// ���a
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
// �I������
//============================================================
void CCollisionSphere::Uninit(void)
{
#ifdef _DEBUG

	// ���b�V���X�t�B�A�̏I��
	SAFE_UNINIT(m_pSphere);

#endif // _DEBUG

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
// ��������
//============================================================
CCollisionSphere* CCollisionSphere::Create(const D3DXVECTOR3& rPos, const float fLength, const float fAngle, const float fRadius)
{
	// �����蔻��̐���
	CCollisionSphere* pColl = new CCollisionSphere();

	// �����o���Ă��Ȃ��ꍇ nullptr ��Ԃ�
	if (pColl == nullptr) { return nullptr; }

	// �ʒu
	D3DXVECTOR3 pos = VEC3_ZERO;

	// �ʒu���쐬����
	pos.x = rPos.x + sinf(fAngle) * fLength;
	pos.y = rPos.y;
	pos.z = rPos.z + cosf(fAngle) * fLength;

	// �ʒu��ݒ肷��
	pColl->SetPos(pos);

	// ���a��ݒ�
	pColl->m_fRadius = fRadius;

#ifdef _DEBUG

	// �X�t�B�A�𐶐�
	pColl->m_pSphere = CObjectMeshSphere::Create(pos, VEC3_ZERO, COL, POSGRID2(8, 2), POSGRID2(8, 2), fRadius);
	pColl->m_pSphere->SetPriority(SPHERE_PRIORITY);

#endif // _DEBUG

	// �����蔻���Ԃ�
	return pColl;
}