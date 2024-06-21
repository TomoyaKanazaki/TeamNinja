//============================================================
//
//	�|���S���̓����蔻�菈�� [collisionPolygon.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "collisionPolygon.h"

//************************************************************
// �萔��`
//************************************************************
namespace
{
	const int POLYGON_PRIORITY = 5;			// �|���S���̗D�揇��
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// �F
}

//============================================================
// �R���X�g���N�^
//============================================================
CCollisionPolygon::CCollisionPolygon() : CCollision(),
m_rot(VEC3_ZERO),		// ��
m_size(VEC3_ZERO)		// ����
#ifdef _DEBUG
, m_pPolygon(nullptr)	// ���b�V���|���S��
#endif // _DEBUG

{

}

//============================================================
// �f�X�g���N�^
//============================================================
CCollisionPolygon::~CCollisionPolygon()
{

}

//============================================================
// �I������
//============================================================
void CCollisionPolygon::Uninit(void)
{
#ifdef _DEBUG

	// ���b�V���|���S���̏I��
	SAFE_UNINIT(m_pPolygon);

#endif // _DEBUG

	// �I������
	CCollision::Uninit();
}

//============================================================
// �q�b�g����
//============================================================
bool CCollisionPolygon::Hit
(
	D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
	const float fRadius,			// ���a
	const float fHeight,			// ����
	D3DXVECTOR3& rMove,				// �ړ���
	bool& rJump						// �W�����v��
)
{

	// false ��Ԃ�
	return false;
}

//============================================================
// �I�t�Z�b�g�ݒ菈��
//============================================================
void CCollisionPolygon::OffSet(const D3DXMATRIX& mtx)
{
	// �I�t�Z�b�g����
	CCollision::OffSet(mtx);

#ifdef _DEBUG

	// �|���S���̈ʒu��ݒ�
	m_pPolygon->SetVec3Position(GetPos());

#endif // _DEBUG
}

//============================================================
// ��������
//============================================================
CCollisionPolygon* CCollisionPolygon::Create
(
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rOffset,		// �I�t�Z�b�g
	const D3DXVECTOR3& rRot,		// ����
	const D3DXVECTOR3& rSize		// �T�C�Y
)
{
	// �����蔻��̐���
	CCollisionPolygon* pColl = new CCollisionPolygon();

	// �����o���Ă��Ȃ��ꍇ nullptr ��Ԃ�
	if (pColl == nullptr) { return nullptr; }

	// �ʒu��ݒ�
	pColl->SetPos(rPos);

	// �I�t�Z�b�g�̐ݒ�
	pColl->SetOffset(rOffset);

	// ������ݒ�
	pColl->m_rot = rRot;

	// �T�C�Y��ݒ�
	pColl->m_size = rSize;

#ifdef _DEBUG

	// �|���S���𐶐�
	pColl->m_pPolygon = CObject3D::Create
	(
		rPos,
		pColl->m_size,
		pColl->m_rot,
		COL
	);
	pColl->m_pPolygon->SetPriority(POLYGON_PRIORITY);
	pColl->m_pPolygon->SetLabel(CObject::LABEL_COLLISION);

#endif // _DEBUG

	// �����蔻���Ԃ�
	return pColl;
}