//============================================================
//
//	�G�̃i�r�Q�[�V����(����)���� [enemyNavStreet.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "enemyNavStreet.h"

#include "effect3D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int STOP_COUNT = 100;				// ��~�J�E���g
	const float ROT_CORRECT_DIFF = 0.01f;	// ������␳���鍷��
}

//************************************************************
//	�q�N���X [CEnemyNavStreet] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyNavStreet::CEnemyNavStreet() : CEnemyNav()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyNavStreet::~CEnemyNavStreet()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyNavStreet::Init(void)
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
void CEnemyNavStreet::Uninit(void)
{
	// �o�H��S��������
	m_Route.clear();

	// �i�r�̏I������
	CEnemyNav::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyNavStreet::Update
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
CEnemyNavStreet* CEnemyNavStreet::Create(const D3DXVECTOR3& rPosInit, const std::vector<D3DXVECTOR3>& rRoute)
{
	// �i�r�Q�[�V�����̐���
	CEnemyNavStreet* pNav = new CEnemyNavStreet;

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

		// �o�H��ݒ�
		pNav->m_Route = rRoute;

		// �m�ۂ����A�h���X��Ԃ�
		return pNav;
	}
}

//============================================================
// ��~��ԏ���
//============================================================
void CEnemyNavStreet::StopFunc
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
void CEnemyNavStreet::TurnFunc
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
void CEnemyNavStreet::MoveFunc
(
	D3DXVECTOR3* pPos,
	const D3DXVECTOR3& rMove
)
{
	D3DXVECTOR3 posDest = GetPosDest();		// �ړI�̈ʒu

	if (PosCorrect(posDest.x, &pPos->x, rMove.x) ||
		PosCorrect(posDest.z, &pPos->z, rMove.z))
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
bool CEnemyNavStreet::PosCorrect(const float fDest, float* fTarget, const float fMove)
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