//============================================================
//
//	�G�̒ǐՔ͈͏��� [enemyChaseRange.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "enemyChaseRange.h"
#include "collision.h"

#ifdef _DEBUG

#include "objectMeshCube.h"

#endif // _DEBUG


//************************************************************
//	�q�N���X [CEnemyChaseRange] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyChaseRange::CEnemyChaseRange() :
	m_fWidth(0.0f),		// ��
	m_fDepth(0.0f)		// ���s
#ifdef _DEBUG
	, m_pRangeCube(nullptr)		// �L���[�u�̏��
#endif // _DEBUG

{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyChaseRange::~CEnemyChaseRange()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyChaseRange::Init(void)
{
	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyChaseRange::Uninit(void)
{
#ifdef _DEBUG

	// �L���[�u�̔j������
	SAFE_UNINIT(m_pRangeCube);

#endif // _DEBUG

	// ���g��j��
	delete this;
}

//============================================================
//	�X�V����
//============================================================
bool CEnemyChaseRange::ChaseRange(const D3DXVECTOR3& rInitPos, D3DXVECTOR3* pPos)
{
	// �͈͂𒴂������ǂ���
	bool bOver = false;

	if (pPos->x >= rInitPos.x + m_fWidth)
	{ // �E�[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->x = rInitPos.x + m_fWidth;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->x <= rInitPos.x - m_fWidth)
	{ // ���[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->x = rInitPos.x - m_fWidth;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->z >= rInitPos.z + m_fDepth)
	{ // ���[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->z = rInitPos.z + m_fDepth;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->z <= rInitPos.z - m_fDepth)
	{ // ��O�[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->z = rInitPos.z - m_fDepth;

		// �͈͒�����
		bOver = true;
	}

	// �͈͏󋵂�Ԃ�
	return bOver;
}

//============================================================
// �^�[�Q�b�g���͈͓��ɂ��邩�̏���
//============================================================
bool CEnemyChaseRange::InsideTargetPos(const D3DXVECTOR3& rInitPos, const D3DXVECTOR3& rTargetPos)
{
	// ����ݒ�
	D3DXVECTOR3 centerSize = D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth);

	// �����蔻���Ԃ�
	return collision::Box2D(rInitPos, rTargetPos, centerSize, centerSize, VEC3_ZERO, VEC3_ZERO);
}

//============================================================
// ��������
//============================================================
CEnemyChaseRange* CEnemyChaseRange::Create(const D3DXVECTOR3& pos, const float fWidth, const float fDepth)
{
	// �i�r�Q�[�V�����̐���
	CEnemyChaseRange* pNav = new CEnemyChaseRange;

	if (pNav == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �A�N�^�[�̏�����
		if (FAILED(pNav->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �A�N�^�[�̔j��
			SAFE_DELETE(pNav);
			return nullptr;
		}

		// ����ݒ�
		pNav->m_fWidth = fWidth;	// ��
		pNav->m_fDepth = fDepth;	// ���s

#if 1
#ifdef _DEBUG

		// �L���[�u�𐶐�
		pNav->m_pRangeCube = CObjectMeshCube::Create
		(
			pos,
			VEC3_ZERO,
			D3DXVECTOR3(fWidth, 10.0f, fDepth),
			XCOL_CYAN
		);

		// �����蔻�胉�x���ɕς���
		pNav->m_pRangeCube->SetLabel(CObject::LABEL_COLLISION);

#endif // _DEBUG
#endif // 0

		// �m�ۂ����A�h���X��Ԃ�
		return pNav;
	}
}