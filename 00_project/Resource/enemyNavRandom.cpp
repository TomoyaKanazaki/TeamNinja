//============================================================
//
//	�G�̃i�r�Q�[�V����(�����_�����W)���� [enemyNavRandom.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "enemyNavRandom.h"

#include "objectMeshCube.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int STOP_COUNT = 100;				// ��~�J�E���g
	const float ROT_CORRECT_DIFF = 0.01f;	// ������␳���鍷��
}

//************************************************************
//	�q�N���X [CEnemyNavRandom] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyNavRandom::CEnemyNavRandom() : CEnemyNav(),
m_pRangeCube(nullptr)	// �͈�
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyNavRandom::~CEnemyNavRandom()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyNavRandom::Init(void)
{
	if (FAILED(CEnemyNav::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyNavRandom::Uninit(void)
{
	if (m_pRangeCube != nullptr)
	{ // �͈͂� NULL ����Ȃ��ꍇ

		// �͈͂̏I������
		m_pRangeCube->Uninit();
		m_pRangeCube = nullptr;
	}

	// �i�r�̏I������
	CEnemyNav::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyNavRandom::Update
(
	D3DXVECTOR3* pPos,		// �ʒu
	D3DXVECTOR3* pRot,		// ����
	D3DXVECTOR3* pMove,		// �ړ���
	D3DXVECTOR3* pRotDest,	// �ړI�̌���
	const float fSpeed,		// ���x
	const float fDeltaTime	// �f���^�^�C��
)
{
	// �i�r�̍X�V����
	CEnemyNav::Update
	(
		pPos,		// �ʒu
		pRot,		// ����
		pMove,		// �ړ���
		pRotDest,	// �ړI�̌���
		fSpeed,		// ���x
		fDeltaTime	// �f���^�^�C��
	);
}

//============================================================
// ��������
//============================================================
CEnemyNavRandom* CEnemyNavRandom::Create(const D3DXVECTOR3& rPosInit, const float fWidth, const float fDepth)
{
	// �i�r�Q�[�V�����̐���
	CEnemyNavRandom* pNav = new CEnemyNavRandom;

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

		// �����ʒu��ݒ�
		pNav->SetPosInit(rPosInit);

		// �ړ��͈͂�ݒ�
		pNav->m_MoveRange = D3DXVECTOR3(fWidth, 0.0f, fDepth);

		// �~���𐶐�
		pNav->m_pRangeCube = CObjectMeshCube::Create
		(
			rPosInit,
			VEC3_ZERO,
			D3DXVECTOR3(fWidth, 10.0f, fDepth),
			XCOL_WHITE
		);

		// �m�ۂ����A�h���X��Ԃ�
		return pNav;
	}
}

//============================================================
// ��~��ԏ���
//============================================================
void CEnemyNavRandom::StopFunc
(
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rRot,		// ����
	D3DXVECTOR3* pRotDest			// �ړI�̌���
)
{
	// ��ԃJ�E���g���擾����
	int nStateCount = GetStateCount();

	// ��ԃJ�E���g�����Z����
	nStateCount++;

	if (nStateCount % STOP_COUNT == 0)
	{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

		// ��ԃJ�E���g��0�ɂ���
		nStateCount = 0;

		// �^�[����Ԃɂ���
		SetState(STATE_TURN);

		// �ړI�ʒu�̃����_������
		DestPosRandom();

		// ������ݒ肷��
		pRotDest->y = atan2f(rPos.x - GetPosDest().x, rPos.z - GetPosDest().z);

		// �����̐��K��
		useful::NormalizeRot(pRotDest->y);
	}

	// ��ԃJ�E���g��K�p����
	SetStateCount(nStateCount);
}

//============================================================
// �^�[����ԏ���
//============================================================
void CEnemyNavRandom::TurnFunc
(
	D3DXVECTOR3* pRot,				// ����
	D3DXVECTOR3* pMove,				// �ړ���
	const D3DXVECTOR3& rRotDest,	// �ړI�̌���
	const float fSpeed,				// ���x
	const float fDeltaTime			// �f���^�^�C��
)
{
	if (pRot->y <= rRotDest.y + ROT_CORRECT_DIFF &&
		pRot->y >= rRotDest.y - ROT_CORRECT_DIFF)
	{ // �������ړI�ɋ߂Â����ꍇ

		// ������␳����
		pRot->y = rRotDest.y;

		// �ړ���Ԃɂ���
		SetState(STATE_MOVE);

		// �ړ��ʂ�ݒ肷��
		pMove->x = sinf(pRot->y) * fSpeed * fDeltaTime;
		pMove->z = cosf(pRot->y) * fSpeed * fDeltaTime;
	}
}

//============================================================
// �ړ���ԏ���
//============================================================
void CEnemyNavRandom::MoveFunc
(
	D3DXVECTOR3* pPos, 
	const D3DXVECTOR3& rMove
)
{
	D3DXVECTOR3 posDest = GetPosDest();		// �ړI�̈ʒu

	if (PosCorrect(posDest.x,&pPos->x,rMove.x) ||
		PosCorrect(posDest.z, &pPos->z, rMove.z) ||
		CollisionRange(pPos))
	{ // �����I���邩�A�͈͂𒴂����ꍇ

		// ��ԃJ�E���g��0�ɂ���
		SetStateCount(0);

		// ��~��Ԃɂ���
		SetState(STATE_STOP);
	}
}

//============================================================
// �ʒu�̃����_������
//============================================================
void CEnemyNavRandom::DestPosRandom(void)
{
	D3DXVECTOR3 posDest = VEC3_ZERO;	// �ړI�̈ʒu

	// �ړI�̈ʒu��ݒ肷��
	posDest.x = GetPosInit().x + rand() % ((int)m_MoveRange.x + 1) - ((int)m_MoveRange.x * 0.5f);
	posDest.z = GetPosInit().z + rand() % ((int)m_MoveRange.z + 1) - ((int)m_MoveRange.z * 0.5f);

	// �ړI�ʒu��K�p����
	SetPosDest(posDest);
}

//============================================================
// �ʒu�̕␳����
//============================================================
bool CEnemyNavRandom::PosCorrect(const float fDest, float* fTarget, const float fMove)
{
	if (*fTarget > fDest)
	{ // �T�C�Y���ڕW���߂������ꍇ

		// �T�C�Y�����Z����
		*fTarget += fMove;

		if (*fTarget <= fDest)
		{ // �ڕW�̃T�C�Y�ȉ��ɂȂ����ꍇ

			// �T�C�Y��␳����
			*fTarget = fDest;

			// true ��Ԃ�
			return true;
		}
	}

	if (*fTarget < fDest)
	{ // �T�C�Y���ڕW�����������ꍇ

		// �T�C�Y�����Z����
		*fTarget += fMove;

		if (*fTarget >= fDest)
		{ // �ڕW�̃T�C�Y�ȏ�ɂȂ����ꍇ

			// �T�C�Y��␳����
			*fTarget = fDest;

			// true ��Ԃ�
			return true;
		}
	}

	// false ��Ԃ�
	return false;
}

//============================================================
// �͈͂Ƃ̏Փ�
//============================================================
bool CEnemyNavRandom::CollisionRange(D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 posInit = GetPosInit();		// �����ʒu

	// �͈͂𒴂������ǂ���
	bool bOver = false;

	if (pPos->x >= posInit.x + m_MoveRange.x)
	{ // �E�[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->x = posInit.x + m_MoveRange.x;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->x <= posInit.x - m_MoveRange.x)
	{ // ���[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->x = posInit.x - m_MoveRange.x;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->z >= posInit.z + m_MoveRange.z)
	{ // ���[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->z = posInit.z + m_MoveRange.z;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->z <= posInit.z - m_MoveRange.z)
	{ // ��O�[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->z = posInit.z - m_MoveRange.z;

		// �͈͒�����
		bOver = true;
	}

	// �͈͏󋵂�Ԃ�
	return bOver;
}