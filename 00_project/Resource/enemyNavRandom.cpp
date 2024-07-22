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
#include "enemy.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int STOP_COUNT = 100;				// ��~�J�E���g
	const float ROT_CORRECT_DIFF = 0.01f;	// ������␳���鍷��
	const float MIN_DISTANCE = 250.0f;		// �Œ���̋���
}

//************************************************************
//	�q�N���X [CEnemyNavRandom] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyNavRandom::CEnemyNavRandom() : CEnemyNav(),
#ifdef _DEBUG

m_pRangeCube(nullptr),	// �͈�

#endif
m_MoveRange(VEC3_ZERO)	// �ړ��͈�
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
#ifdef _DEBUG

	// �͈͂̏I������
	SAFE_UNINIT(m_pRangeCube);

#endif

	// �i�r�̏I������
	CEnemyNav::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyNavRandom::Update
(
	D3DXVECTOR3* pPos,			// �ʒu
	D3DXVECTOR3* pRot,			// ����
	CEnemy* pEnemy,				// �G�̏��
	const float fSpeed,			// ���x
	const float fDeltaTime		// �f���^�^�C��
)
{
	// �i�r�̍X�V����
	CEnemyNav::Update
	(
		pPos,		// �ʒu
		pRot,		// ����
		pEnemy,		// �G�̏��
		fSpeed,		// ���x
		fDeltaTime	// �f���^�^�C��
	);
}

//============================================================
// ��������
//============================================================
CEnemyNavRandom* CEnemyNavRandom::Create(const D3DXVECTOR3& pos, const float fWidth, const float fDepth)
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

		// �ړ��͈͂�ݒ�
		pNav->m_MoveRange = D3DXVECTOR3(fWidth, 0.0f, fDepth);

		// ��ނ�ݒ�
		pNav->SetType(TYPE_RANDOM);

#if 0
#ifdef _DEBUG

		// �L���[�u�𐶐�
		pNav->m_pRangeCube = CObjectMeshCube::Create
		(
			pos,
			VEC3_ZERO,
			D3DXVECTOR3(fWidth, 10.0f, fDepth),
			XCOL_WHITE
		);

#endif // _DEBUG
#endif // 0


		// �m�ۂ����A�h���X��Ԃ�
		return pNav;
	}
}

//============================================================
// ��~��ԏ���
//============================================================
void CEnemyNavRandom::StopFunc
(
	const D3DXVECTOR3& rPos,	// �ʒu
	CEnemy* pEnemy				// �G�̏��
)
{
	int nStateCount = GetStateCount();					// ��ԃJ�E���g���擾����
	D3DXVECTOR3 posDest = GetPosDest();					// �ړI�̈ʒu���擾����
	D3DXVECTOR3 posInit = pEnemy->GetPosInit();			// �����ʒu���擾����
	D3DXVECTOR3 rotDest = pEnemy->GetDestRotation();	// �ړI�̌������擾����

	// ��ԃJ�E���g�����Z����
	nStateCount++;

	if (nStateCount % STOP_COUNT == 0)
	{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

		// ��ԃJ�E���g��0�ɂ���
		nStateCount = 0;

		// �^�[����Ԃɂ���
		SetState(STATE_TURN);

		// �ړI�̈ʒu��ݒ肷��
		posDest.x = posInit.x + rand() % ((int)m_MoveRange.x + 1) - ((int)m_MoveRange.x * 0.5f);
		posDest.z = posInit.z + rand() % ((int)m_MoveRange.z + 1) - ((int)m_MoveRange.z * 0.5f);

		// ������ݒ肷��
		rotDest.y = atan2f(rPos.x - posDest.x, rPos.z - posDest.z);

		if (sqrtf((rPos.x - posDest.x) * (rPos.x - posDest.x) + (rPos.z - posDest.z) * (rPos.z - posDest.z)) <= MIN_DISTANCE)
		{ // �Œ���̋����ȉ��̏ꍇ

			// �ړI�̈ʒu��������ɉ��΂�
			posDest.x -= sinf(rotDest.y) * (m_MoveRange.x * 0.5f);
			posDest.z -= cosf(rotDest.y) * (m_MoveRange.z * 0.5f);
		}

		// �����̐��K��
		useful::NormalizeRot(rotDest.y);
	}

	SetStateCount(nStateCount);			// ��ԃJ�E���g��K�p
	pEnemy->SetDestRotation(rotDest);	// �ړI�̌�����K�p
	SetPosDest(posDest);				// �ړI�̈ʒu��K�p
}

//============================================================
// �^�[����ԏ���
//============================================================
void CEnemyNavRandom::TurnFunc
(
	D3DXVECTOR3* pRot,			// ����
	CEnemy* pEnemy,				// �G�̏��
	const float fSpeed,			// ���x
	const float fDeltaTime		// �f���^�^�C��
)
{
	D3DXVECTOR3 rotDest = pEnemy->GetDestRotation();	// �ړI�̌������擾
	D3DXVECTOR3 move = pEnemy->GetMovePosition();		// �ړ��ʂ��擾

	if (pRot->y <= rotDest.y + ROT_CORRECT_DIFF &&
		pRot->y >= rotDest.y - ROT_CORRECT_DIFF)
	{ // �������ړI�ɋ߂Â����ꍇ

		// ������␳����
		pRot->y = rotDest.y;

		// �ړ���Ԃɂ���
		SetState(STATE_MOVE);

		// �ړ��ʂ�ݒ肷��
		move.x = sinf(pRot->y) * fSpeed * fDeltaTime;
		move.z = cosf(pRot->y) * fSpeed * fDeltaTime;
	}

	pEnemy->SetMovePosition(move);		// �ړ��ʂ�K�p
}

//============================================================
// �ړ���ԏ���
//============================================================
void CEnemyNavRandom::MoveFunc
(
	D3DXVECTOR3* pPos,			// �ʒu
	CEnemy* pEnemy				// �G�̏��
)
{
	D3DXVECTOR3 posDest = GetPosDest();				// �ړI�̈ʒu���擾
	D3DXVECTOR3 move = pEnemy->GetMovePosition();	// �ړ��ʂ��擾
	D3DXVECTOR3 posInit = pEnemy->GetPosInit();		// �����ʒu���擾

	if (PosCorrect(posDest.x, &pPos->x, move.x) ||
		PosCorrect(posDest.z, &pPos->z, move.z) ||
		CollisionRange(pPos, posInit))
	{ // �����I���邩�A�͈͂𒴂����ꍇ

		// ��ԃJ�E���g��0�ɂ���
		SetStateCount(0);

		// ��~��Ԃɂ���
		SetState(STATE_STOP);
	}
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
bool CEnemyNavRandom::CollisionRange(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosInit)
{
	// �͈͂𒴂������ǂ���
	bool bOver = false;

	if (pPos->x >= rPosInit.x + m_MoveRange.x)
	{ // �E�[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->x = rPosInit.x + m_MoveRange.x;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->x <= rPosInit.x - m_MoveRange.x)
	{ // ���[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->x = rPosInit.x - m_MoveRange.x;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->z >= rPosInit.z + m_MoveRange.z)
	{ // ���[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->z = rPosInit.z + m_MoveRange.z;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->z <= rPosInit.z - m_MoveRange.z)
	{ // ��O�[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->z = rPosInit.z - m_MoveRange.z;

		// �͈͒�����
		bOver = true;
	}

	// �͈͏󋵂�Ԃ�
	return bOver;
}