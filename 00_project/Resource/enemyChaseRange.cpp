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
	m_pos(VEC3_ZERO),	// �ʒu
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
bool CEnemyChaseRange::ChaseRange(D3DXVECTOR3* pPos)
{
	// �͈͂𒴂������ǂ���
	bool bOver = false;

	if (pPos->x >= m_pos.x + m_fWidth)
	{ // �E�[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->x = m_pos.x + m_fWidth;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->x <= m_pos.x - m_fWidth)
	{ // ���[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->x = m_pos.x - m_fWidth;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->z >= m_pos.z + m_fDepth)
	{ // ���[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->z = m_pos.z + m_fDepth;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->z <= m_pos.z - m_fDepth)
	{ // ��O�[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->z = m_pos.z - m_fDepth;

		// �͈͒�����
		bOver = true;
	}

	// �͈͏󋵂�Ԃ�
	return bOver;
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
		pNav->m_pos = pos;			// �ʒu
		pNav->m_fWidth = fWidth;	// ��
		pNav->m_fDepth = fDepth;	// ���s

		// �L���[�u�𐶐�
		pNav->m_pRangeCube = CObjectMeshCube::Create
		(
			pos,
			VEC3_ZERO,
			D3DXVECTOR3(fWidth, 10.0f, fDepth),
			XCOL_CYAN
		);

		// �m�ۂ����A�h���X��Ԃ�
		return pNav;
	}
}