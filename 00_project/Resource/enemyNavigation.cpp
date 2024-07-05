//============================================================
//
//	�G�̃i�r�Q�[�V�������� [enemyNavigation.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "enemyNavigation.h"

#include "actor.h"
#include "objectMeshCube.h"
#include "stage.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int STOP_COUNT = 100;				// ��~�J�E���g
	const float ROT_CORRECT_DIFF = 0.01f;	// ������␳���鍷��
	const int DEST_ROT_RAND = 101;			// �ړI�̌����̃����_����
	const int DEST_ROT_MIN = 50;			// �ړI�̌����̍Œ�ۏ�
	const int MOVE_COUNT_RAND = 30;			// �ړ��J�E���g�̃����_����
	const int MOVE_COUNT_MIN = 70;			// �ړ��J�E���g�̍Œ�ۏ�
}

//************************************************************
//	�q�N���X [CEnemyNav] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyNav::CEnemyNav() :
	m_pRangeCube(nullptr),	// �͈�
	m_posInit(VEC3_ZERO),	// �����ʒu
	m_posDest(VEC3_ZERO),	// �ڕW�ʒu
	m_MoveRange(VEC2_ZERO),	// �ړ��͈�
	m_state(STATE_STOP),	// ���
	m_nStateCount(0)		// ��ԃJ�E���g
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyNav::~CEnemyNav()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyNav::Init(void)
{
	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyNav::Uninit(void)
{
	if (m_pRangeCube != nullptr)
	{ // �͈͂� NULL ����Ȃ��ꍇ

		// �͈͂̏I������
		m_pRangeCube->Uninit();
		m_pRangeCube = nullptr;
	}

	// ���g��j��
	delete this;
}

//============================================================
//	�X�V����
//============================================================
void CEnemyNav::Update
(
	D3DXVECTOR3* pPos,		// �ʒu
	D3DXVECTOR3* pRot,		// ����
	D3DXVECTOR3* pMove,		// �ړ���
	D3DXVECTOR3* pRotDest,	// �ړI�̌���
	const float fRadius,	// ���a
	const float fHeight,	// ����
	const float fSpeed,		// ���x
	const float fDeltaTime	// �f���^�^�C��
)
{
	switch (m_state)
	{
	case CEnemyNav::STATE_STOP:

		// ��~��ԏ���
		StopFunc(*pPos, *pRot, pRotDest);

		break;

	case CEnemyNav::STATE_TURN:

		// �^�[����ԏ���
		TurnFunc(pRot, pMove, *pRotDest, fSpeed, fDeltaTime);

		break;

	case CEnemyNav::STATE_MOVE:

		// �ړ���ԏ���
		MoveFunc(pPos, *pMove);

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//============================================================
// ��Ԃ̐ݒ菈��
//============================================================
void CEnemyNav::SetState(const EState state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//============================================================
// ��ԃJ�E���g�̐ݒ菈��
//============================================================
void CEnemyNav::SetStateCount(const int nCount)
{
	// ��ԃJ�E���g��ݒ肷��
	m_nStateCount = nCount;
}

//============================================================
// ��������
//============================================================
CEnemyNav* CEnemyNav::Create(const D3DXVECTOR3& rPosInit, const float fWidth, const float fDepth)
{
	// �i�r�Q�[�V�����̐���
	CEnemyNav* pNav = new CEnemyNav;

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
		pNav->m_posInit = rPosInit;

		// �ړ��͈͂�ݒ�
		pNav->m_MoveRange = D3DXVECTOR2(fWidth, fDepth);

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
void CEnemyNav::StopFunc
(
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rRot,		// ����
	D3DXVECTOR3* pRotDest			// �ړI�̌���
)
{
	float fRotInit = 0.0f;		// �����n�_�ւ̌���

	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	if (m_nStateCount % STOP_COUNT == 0)
	{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

		// ��ԃJ�E���g��0�ɂ���
		m_nStateCount = 0;

		// �^�[����Ԃɂ���
		m_state = STATE_TURN;

		// �����n�_�ւ̌�����ݒ肷��
		fRotInit = atan2f(rPos.x - m_posInit.x, rPos.z - m_posInit.z);

		// ������ݒ肷��
		pRotDest->y = fRotInit + ((float)(rand() % DEST_ROT_RAND - DEST_ROT_MIN) * 0.01f);

		// �����̐��K��
		useful::NormalizeRot(pRotDest->y);
	}
}

//============================================================
// �^�[����ԏ���
//============================================================
void CEnemyNav::TurnFunc
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
		m_state = STATE_MOVE;

		// �ړ��ʂ�ݒ肷��
		pMove->x = sinf(pRot->y) * fSpeed * fDeltaTime;
		pMove->z = cosf(pRot->y) * fSpeed * fDeltaTime;

		// ��ԃJ�E���g��ݒ肷��
		m_nStateCount = rand() % MOVE_COUNT_RAND + MOVE_COUNT_MIN;
	}
}

//============================================================
// �ړ���ԏ���
//============================================================
void CEnemyNav::MoveFunc(D3DXVECTOR3* pPos, const D3DXVECTOR3& rMove)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount--;

	// �ړ�����
	*pPos += rMove;

	if (m_nStateCount <= 0 ||
		CollisionRange(pPos))
	{ // �����I���邩�A�͈͂𒴂����ꍇ

		// ��ԃJ�E���g��0�ɂ���
		m_nStateCount = 0;

		// ��~��Ԃɂ���
		m_state = STATE_STOP;
	}
}

//============================================================
// �͈͂Ƃ̏Փ�
//============================================================
bool CEnemyNav::CollisionRange(D3DXVECTOR3* pPos)
{
	// �͈͂𒴂������ǂ���
	bool bOver = false;

	if (pPos->x >= m_posInit.x + m_MoveRange.x)
	{ // �E�[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->x = m_posInit.x + m_MoveRange.x;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->x <= m_posInit.x - m_MoveRange.x)
	{ // ���[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->x = m_posInit.x - m_MoveRange.x;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->z >= m_posInit.z + m_MoveRange.y)
	{ // ���[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->z = m_posInit.z + m_MoveRange.y;

		// �͈͒�����
		bOver = true;
	}

	if (pPos->z <= m_posInit.z - m_MoveRange.y)
	{ // ��O�[�𒴂����ꍇ

		// �ʒu��␳����
		pPos->z = m_posInit.z - m_MoveRange.y;

		// �͈͒�����
		bOver = true;
	}

	// �͈͏󋵂�Ԃ�
	return bOver;
}
