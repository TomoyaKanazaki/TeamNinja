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
#include "collision.h"

//************************************************************
// �萔��`
//************************************************************
namespace
{
	const int POLYGON_PRIORITY = 5;			// �|���S���̗D�揇��
	const D3DXCOLOR COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// �F
	const float GRAVITY = -250.0f;			// ���������̏d��
	const float ROT_TOLERANCE = 0.02f;		// �����̋��e�͈�
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

	if (m_pPolygon->GetInside(rPos))
	{ // �����ɂ���ꍇ

		if (fPosY > rPos.y &&
			fPosY < rPos.y + fHeight)
		{ // ���ݒn���|���S����荂�������ꍇ

			// Y���̈ʒu��K�p����
			rPos.y = fPosY;

			// �d�͂����ɂ���������
			rMove.y = GRAVITY;

			// �W�����v�󋵂� false �ɂ���
			rJump = false;

			// true ��Ԃ�
			return true;
		}
	}
	else
	{ // ��L�ȊO

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

	// �����ɂ��ϊ�����
	if (pColl->Convert(rSize.x, rSize.z, rRot.y))
	{
		std::string str;

		str += std::to_string(rPos.x);
		str += "�@�@";
		str += std::to_string(rPos.y);
		str += "�@�@";
		str += std::to_string(rPos.z);

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�|���S���̓����蔻�肠��̂Ɋp�x����������B", "�x���I", MB_ICONWARNING);
		MessageBox(nullptr, str.c_str(), "�x���I", MB_ICONWARNING);
	}

#ifndef _DEBUG

	// �`���؂�
	pColl->m_pPolygon->SetEnableDraw(false);

#endif // !_DEBUG

	// �����蔻���Ԃ�
	return pColl;
}

//============================================================
// �����ɂ��ϊ�����
//============================================================
bool CCollisionPolygon::Convert(const float fWidth, const float fDepth, const float fRot)
{
	bool b = false;

#ifdef _DEBUG

	// �����̌x������
	b = RotWarning(fRot);

#endif // _DEBUG

	if ((fRot >= D3DX_PI * -0.75f && fRot <= D3DX_PI * -0.25f) ||
		(fRot >= D3DX_PI * 0.25f && fRot <= D3DX_PI * 0.75f))
	{ // 90�x�A270�x�̏ꍇ

		// ���a��ݒ�
		m_size.x = fDepth;

		// ���s��ݒ�
		m_size.z = fWidth;
	}
	else
	{ // ��L�ȊO

		// ���a��ݒ�
		m_size.x = fWidth;

		// ���s��ݒ�
		m_size.z = fDepth;
	}

	return b;
}


#ifdef _DEBUG
//============================================================
// �����̌x������
//============================================================
bool CCollisionPolygon::RotWarning(const float fRot)
{
	bool bOver = false;		// �͈͊O�̌���

	if (fRot >= -D3DX_PI * 0.25f &&
		fRot <= D3DX_PI * 0.25f)
	{ // 0�x�␳�O��Ō덷���傫������ꍇ

		if (fabsf(fRot) >= ROT_TOLERANCE)
		{ // �����̋��e�͈͂𒴂����ꍇ

			// �͈͊O������o��
			bOver = true;
		}
	}
	else if (fRot >= D3DX_PI * 0.25f &&
		fRot <= D3DX_PI * 0.75f)
	{ // 90�x�␳�O��Ō덷���傫������ꍇ

		if (fabsf(fRot - (D3DX_PI * 0.5f)) >= ROT_TOLERANCE)
		{ // �����̋��e�͈͂𒴂����ꍇ

			// �͈͊O������o��
			bOver = true;
		}
	}
	else if ((fRot >= D3DX_PI * -0.75f &&
		fRot <= D3DX_PI * -0.25f))
	{ // 270�x�␳�O��Ō덷���傫������ꍇ

		if (fabsf(fRot + (D3DX_PI * 0.5f)) >= ROT_TOLERANCE)
		{ // �����̋��e�͈͂𒴂����ꍇ

			// �͈͊O������o��
			bOver = true;
		}
	}
	else
	{
		if (fabsf(fRot - D3DX_PI) >= ROT_TOLERANCE &&
			fabsf(fRot + D3DX_PI) >= ROT_TOLERANCE)
		{  // �����̋��e�͈͂𒴂����ꍇ

			// �͈͊O������o��
			bOver = true;
		}
	}

	// �͈͊O������o��
	return bOver;
}

#endif