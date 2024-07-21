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
m_size(VEC3_ZERO),		// ����
m_pPolygon(nullptr)		// �����蔻��|���S��
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
	// ���b�V���|���S���̏I��
	SAFE_UNINIT(m_pPolygon);

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
	// �������擾����
	float fPosY = m_pPolygon->GetPositionHeight(rPos);

	if (fPosY > rPos.y &&
		fPosY < rPos.y + fHeight)
	{ // ���ݒn���|���S����荂�������ꍇ

		// Y���̈ʒu��K�p����
		rPos.y = fPosY;

		// �d�͂�0�ɂ���
		rMove.y = 0.0f;

		// �W�����v�󋵂� false �ɂ���
		rJump = false;
	}

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

	// �|���S���̈ʒu��ݒ�
	m_pPolygon->SetVec3Position(GetPos());
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

#ifndef _DEBUG

	// �`���؂�
	pColl->m_pPolygon->SetEnableDraw(false);

#endif // !_DEBUG

	// �����蔻���Ԃ�
	return pColl;
}