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

#ifdef _DEBUG
#include "objectMeshTube.h"
#endif // _DEBUG

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
// �I������
//============================================================
void CCollisionCylinder::Uninit(void)
{
#ifdef _DEBUG

	// ���b�V���`���[�u�̏I��
	SAFE_UNINIT(m_pTube);

#endif // _DEBUG

	// �I������
	CCollision::Uninit();
}

//============================================================
// �q�b�g����
//============================================================
bool CCollisionCylinder::Hit(D3DXVECTOR3& rPos, const float fRadius)
{
	// �����蔻���Ԃ�
	return collision::CirclePillar(rPos, GetPos(), fRadius, m_fRadius);
}

//============================================================
// ��������
//============================================================
CCollisionCylinder* CCollisionCylinder::Create(const D3DXVECTOR3& rOffset, const float fRadius, const float fHeight)
{
	// �����蔻��̐���
	CCollisionCylinder* pColl = new CCollisionCylinder();

	// �����o���Ă��Ȃ��ꍇ nullptr ��Ԃ�
	if (pColl == nullptr) { return nullptr; }

	// �I�t�Z�b�g�ʒu��ݒ�
	pColl->SetOffset(rOffset);

	// ���a��ݒ�
	pColl->m_fRadius = fRadius;

	// ������ݒ�
	pColl->m_fHeight = fHeight;

#ifdef _DEBUG

	// �`���[�u�𐶐�
	pColl->m_pTube = CObjectMeshTube::Create(rOffset, VEC3_ZERO, XCOL_RED, POSGRID2(8, 2), POSGRID2(8, 2), fRadius, fHeight);

#endif // _DEBUG

	// �����蔻���Ԃ�
	return pColl;
}
