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
#include "deltaTime.h"

#include "actor.h"
#include "objectMeshCylinder.h"
#include "stage.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float MOVE_DISTANCE = -700.0f;	// �ړ�����͈�
	const int STOP_COUNT = 300;				// ��~�J�E���g
}

//************************************************************
//	�q�N���X [CEnemyNav] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyNav::CEnemyNav() :
	m_pCylinder(nullptr),
	m_posInit(VEC3_ZERO),	// �����ʒu
	m_posDest(VEC3_ZERO),	// �ڕW�n�_
	m_state(STATE_STOP),	// ���
	m_nStopCount(0),		// ��~�J�E���g
	m_fRotMove(0.0f)		// �����̈ړ���
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
	if (m_pCylinder != nullptr)
	{ // �V�����_�[�� NULL ����Ȃ��ꍇ

		// �V�����_�[�̏I������
		m_pCylinder->Uninit();
		m_pCylinder = nullptr;
	}

	// ���g��j��
	delete this;
}

//============================================================
//	�X�V����
//============================================================
void CEnemyNav::Update(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosOld, D3DXVECTOR3* pRot, D3DXVECTOR3* pRotDest, D3DXVECTOR3* pMove)
{
	switch (m_state)
	{
	case CEnemyNav::STATE_STOP:

		// ��~��ԏ���
		Stop(pPos,rPosOld, pRotDest);

		break;

	case CEnemyNav::STATE_TURN:

		// �^�[����ԏ���
		Turn(pRot, pRotDest, pMove);

		break;

	case CEnemyNav::STATE_WALK:

		// ���s��ԏ���
		Walk(pPos, rPosOld, pRot, pMove);

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//============================================================
// ��������
//============================================================
CEnemyNav* CEnemyNav::Create(const D3DXVECTOR3& rPosInit)
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

		pNav->m_pCylinder->Create
		(
			rPosInit,
			VEC3_ZERO,
			XCOL_WHITE,
			POSGRID2(32, 8),
			POSGRID2(4, 4),
			MOVE_DISTANCE,
			30.0f
		);

		// �m�ۂ����A�h���X��Ԃ�
		return pNav;
	}
}

//============================================================
// ��~��ԏ���
//============================================================
void CEnemyNav::Stop(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosOld, D3DXVECTOR3* pRotDest)
{
	// ��~������
	m_nStopCount++;

	if (m_nStopCount % STOP_COUNT == 0)
	{ // ��~�J�E���g����萔�̏ꍇ

		float fRotToDestPos = useful::RandomRot();

		// �ړI�ʒu��ݒ�
		m_posDest.x = m_posInit.x + sinf(fRotToDestPos) * MOVE_DISTANCE;
		m_posDest.z = m_posInit.z + cosf(fRotToDestPos) * MOVE_DISTANCE;

		// �A�N�^�[�̓����蔻�菈��
		CollisionActor(pPos, rPosOld);

		// �ǂ̓����蔻�菈��
		//CollisionWall(pPos);

		// ������ݒ肷��
		pRotDest->y = atan2f(pPos->x - m_posDest.x, pPos->z - m_posDest.z);

		// ��Ԃ��N���A����
		m_state = STATE_TURN;
	}
}

//============================================================
// �^�[����ԏ���
//============================================================
void CEnemyNav::Turn(D3DXVECTOR3* pRot, D3DXVECTOR3* pRotDest, D3DXVECTOR3* pMove)
{
	if (pRot->y <= pRotDest->y + 0.01f &&
		pRot->y >= pRotDest->y - 0.01f)
	{ // �������ړI�ɋ߂Â����ꍇ

		// ������␳����
		pRot->y = pRotDest->y;

		// ���s��Ԃɂ���
		m_state = STATE_WALK;

		// �ړ��ʂ�ݒ肷��
		pMove->x = sinf(pRot->y) * -290.0f * GET_MANAGER->GetDeltaTime()->GetTime();
		pMove->z = cosf(pRot->y) * -290.0f * GET_MANAGER->GetDeltaTime()->GetTime();
	}
}

//============================================================
// ���s��ԏ���
//============================================================
void CEnemyNav::Walk(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosOld, D3DXVECTOR3* pRot, D3DXVECTOR3* pMove)
{
	bool bX = false, bZ = false;

	// �ړ��ʂ�ݒ肷��
	pMove->x = sinf(pRot->y) * -290.0f * GET_MANAGER->GetDeltaTime()->GetTime();
	pMove->z = cosf(pRot->y) * -290.0f * GET_MANAGER->GetDeltaTime()->GetTime();

	// �ړ�����
	pPos->x += pMove->x;
	pPos->z += pMove->z;

	// ���E�̓����蔻��
	if (pPos->x >= m_posDest.x &&
		rPosOld.x <= m_posDest.x)
	{ // ������̓����蔻��

		// �ʒu��␳
		pPos->x = m_posDest.x;

		bX = true;
	}
	else if (pPos->x <= m_posDest.x &&
		rPosOld.x >= m_posDest.x)
	{ // �E����̓����蔻��

		// �ʒu��␳
		pPos->x = m_posDest.x;

		bX = true;
	}

	// �O��̓����蔻��
	if (pPos->z >= m_posDest.z &&
		rPosOld.z <= m_posDest.z)
	{ // �O����̓����蔻��

		// �ʒu��␳
		pPos->z = m_posDest.z;

		bZ = true;
	}
	else if (pPos->z <= m_posDest.z &&
		rPosOld.z >= m_posDest.z)
	{ // �ォ��̓����蔻��

		// �ʒu��␳
		pPos->z = m_posDest.z;

		bZ = true;
	}

	if (bX && bZ)
	{
		// ��~��Ԃɂ���
		m_state = STATE_STOP;
	}
}

//============================================================
// �A�N�^�[�̓����蔻�菈��
//============================================================
void CEnemyNav::CollisionActor(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosOld)
{
	D3DXVECTOR3 move = VEC3_ZERO;
	bool bJump = false;

	// �A�N�^�[�̃��X�g�\����������Δ�����
	if (CActor::GetList() == nullptr) { return; }

	// ���X�g���擾
	std::list<CActor*> list = CActor::GetList()->GetList();

	for (auto actor : list)
	{
		// �����蔻�菈��
		actor->Collision
		(
			m_posDest,	// �ʒu
			*pPos,		// �O��̈ʒu
			40.0f,		// ���a
			40.0f,		// ����
			move,		// �ړ���
			bJump		// �W�����v��
		);
	}
}

//============================================================
// �ǂ̓����蔻�菈��
//============================================================
void CEnemyNav::CollisionWall(D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 move = VEC3_ZERO;

	// �ǂ̓����蔻��
	CScene::GetStage()->CollisionWall(m_posDest, *pPos, 40.0f, 40.0f, move);
}
