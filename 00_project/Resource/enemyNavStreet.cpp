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

#include "enemy.h"

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
CEnemyNavStreet::CEnemyNavStreet() : CEnemyNav(),
m_nNumRoute(0)	// ���݌��������[�g�̔ԍ�
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
CEnemyNavStreet* CEnemyNavStreet::Create(const std::vector<D3DXVECTOR3>& rRoute)
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
	const D3DXVECTOR3& rPos,	// �ʒu
	CEnemy* pEnemy				// �G�̏��
)
{
	int nStateCount = GetStateCount();					// ��ԃJ�E���g���擾����
	D3DXVECTOR3 posDest = GetPosDest();					// �ړI�̈ʒu���擾����
	D3DXVECTOR3 rotDest = pEnemy->GetDestRotation();	// �ړI�̌������擾����

	// ��ԃJ�E���g�����Z����
	nStateCount++;

	if (nStateCount % STOP_COUNT == 0)
	{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

		// ��ԃJ�E���g��0�ɂ���
		nStateCount = 0;

		// �^�[����Ԃɂ���
		SetState(STATE_TURN);

		// �ړI�̈ʒu��ݒ�
		posDest = m_Route.at(m_nNumRoute);

		// ������ݒ肷��
		rotDest.y = atan2f(rPos.x - posDest.x, rPos.z - posDest.z);

		// �����̐��K��
		useful::NormalizeRot(rotDest.y);
	}

	SetStateCount(nStateCount);			// ��ԃJ�E���g��K�p
	SetPosDest(posDest);				// �ړI�̈ʒu��K�p
	pEnemy->SetDestRotation(rotDest);	// �ړI�̌�����K�p
}

//============================================================
// �^�[����ԏ���
//============================================================
void CEnemyNavStreet::TurnFunc
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
void CEnemyNavStreet::MoveFunc
(
	D3DXVECTOR3* pPos,			// �ʒu
	CEnemy* pEnemy				// �G�̏��
)
{
	D3DXVECTOR3 posDest = GetPosDest();				// �ړI�̈ʒu
	D3DXVECTOR3 move = pEnemy->GetMovePosition();	// �ړ��ʂ��擾

	if (PosCorrect(posDest.x, &pPos->x, move.x) ||
		PosCorrect(posDest.z, &pPos->z, move.z))
	{ // �����I���邩�A�͈͂𒴂����ꍇ

		int nNumAll = m_Route.size();	// �ꏊ�̑���

		// �������ꏊ�����ɐi�߂�
		m_nNumRoute = (m_nNumRoute + 1) % nNumAll;

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