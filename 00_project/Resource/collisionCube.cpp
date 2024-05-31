//============================================================
//
//	�L���[�u�̓����蔻�菈�� [collisionCube.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "collisionCube.h"
#include "collision.h"

#ifdef _DEBUG
#include "objectMeshCube.h"
#endif // _DEBUG

//************************************************************
// �萔��`
//************************************************************
namespace
{
	const int CUBE_PRIORITY = 5;			// �L���[�u�̗D�揇��
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// �F
}

//============================================================
// �R���X�g���N�^
//============================================================
CCollisionCube::CCollisionCube() : CCollision(),
m_fWidth(0.0f),		// ��
m_fHeight(0.0f),	// ����
m_fDepth(0.0f)		// ���s
#ifdef _DEBUG
, m_pCube(nullptr)	// ���b�V���L���[�u
#endif // _DEBUG

{

}

//============================================================
// �f�X�g���N�^
//============================================================
CCollisionCube::~CCollisionCube()
{

}

//============================================================
// �I������
//============================================================
void CCollisionCube::Uninit(void)
{
#ifdef _DEBUG

	// ���b�V���L���[�u�̏I��
	SAFE_UNINIT(m_pCube);

#endif // _DEBUG

	// �I������
	CCollision::Uninit();
}

//============================================================
// �q�b�g����
//============================================================
bool CCollisionCube::Hit
(
	D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
	const float fRadius,			// ���a
	const float fHeight,			// ����
	D3DXVECTOR3& rMove,				// �ړ���
	bool& rJump						// �W�����v��
)
{
	// �T�C�Y
	D3DXVECTOR3 centerSizeUp = D3DXVECTOR3(fRadius, fHeight, fRadius);
	D3DXVECTOR3 centerSizeDown = D3DXVECTOR3(fRadius, 0.0f, fRadius);
	D3DXVECTOR3 targetSizeUp = D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth);
	D3DXVECTOR3 targetSizeDown = D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth);
	D3DXVECTOR3 posOld = rPosOld;
	D3DXVECTOR3 move = rMove;
	bool bJump = rJump;

	if (collision::ResponseBox3D(rPos, posOld, GetPos(), centerSizeUp, centerSizeDown, targetSizeUp, targetSizeDown, &move, &bJump))
	{ // ���������ꍇ

		// �ړ��ʂƃW�����v�󋵂�ݒ肷��
		rMove = move;
		rJump = bJump;

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}

//============================================================
// ��������
//============================================================
CCollisionCube* CCollisionCube::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rOffset, const float fWidth, const float fHeight, const float fDepth)
{
	// �����蔻��̐���
	CCollisionCube* pColl = new CCollisionCube();

	// �����o���Ă��Ȃ��ꍇ nullptr ��Ԃ�
	if (pColl == nullptr) { return nullptr; }

	// �ʒu��ݒ�
	pColl->SetPos(rPos);

	// �I�t�Z�b�g�̐ݒ�
	pColl->SetOffset(rOffset);

	// ���a��ݒ�
	pColl->m_fWidth = fWidth;

	// ������ݒ�
	pColl->m_fHeight = fHeight;

	// ���s��ݒ�
	pColl->m_fDepth = fDepth;

#ifdef _DEBUG

	// �L���[�u�𐶐�
	pColl->m_pCube = CObjectMeshCube::Create
	(
		rPos,
		VEC3_ZERO,
		D3DXVECTOR3(pColl->m_fWidth, pColl->m_fHeight / 2, pColl->m_fDepth),
		COL,
		XCOL_BLACK,
		CObjectMeshCube::BORDER_OFF,
		0.0f,
		CObjectMeshCube::TEXSTATE_ONE,
		CObjectMeshCube::SFaceTex(NONE_IDX),
		CObjectMeshCube::ORIGIN_DOWN
	);
	pColl->m_pCube->SetPriority(CUBE_PRIORITY);

#endif // _DEBUG

	// �����蔻���Ԃ�
	return pColl;
}
