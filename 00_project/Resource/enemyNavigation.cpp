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
	const int STOP_COUNT = 300;				// ��~�J�E���g
	const float ROT_CORRECT_DIFF = 0.01f;	// ������␳���鍷��
}

//************************************************************
//	�q�N���X [CEnemyNav] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyNav::CEnemyNav() :
	m_pRange(nullptr),		// �͈�
	m_posInit(VEC3_ZERO),	// �����ʒu
	m_posDest(VEC3_ZERO),	// �ڕW�ʒu
	m_state(STATE_STOP),	// ���
	m_nStopCount(0)			// ��~�J�E���g
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
	if (m_pRange != nullptr)
	{ // �͈͂� NULL ����Ȃ��ꍇ

		// �͈͂̏I������
		m_pRange->Uninit();
		m_pRange = nullptr;
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
		StopFunc(pRotDest);

		break;

	case CEnemyNav::STATE_TURN:

		// �^�[����ԏ���
		TurnFunc(pRot, pMove, *pRotDest, fSpeed, fDeltaTime);

		break;

	case CEnemyNav::STATE_MOVE:

		// �ړ���ԏ���
		MoveFunc(*pPos);

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	//D3DXVECTOR3 posDest = m_posDest;
	//D3DXVECTOR3 move = VEC3_ZERO;
	//bool bJump = false;
	//bool bHit = false;

	//// �A�N�^�[�̃��X�g�\����������Δ�����
	//if (CActor::GetList() == nullptr) { return; }

	//// ���X�g���擾
	//std::list<CActor*> list = CActor::GetList()->GetList();

	//for (auto actor : list)
	//{
	//	// �����蔻�菈��
	//	actor->Collision
	//	(
	//		posDest,	// �ʒu
	//		rPos,		// �O��̈ʒu
	//		fRadius,	// ���a
	//		fHeight,	// ����
	//		move,		// �ړ���
	//		bJump,		// �W�����v��
	//		bHit		// �q�b�g��
	//	);
	//}
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

		// �~���𐶐�
		pNav->m_pRange = CObjectMeshCube::Create
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
void CEnemyNav::StopFunc(D3DXVECTOR3* pRotDest)
{
	// ��~�J�E���g�����Z����
	m_nStopCount++;

	if (m_nStopCount % STOP_COUNT == 0)
	{ // ��~�J�E���g����萔�ɂȂ����ꍇ

		// ��~�J�E���g��0�ɂ���
		m_nStopCount = 0;

		// �^�[����Ԃɂ���
		m_state = STATE_TURN;

		// ������ݒ肷��
		pRotDest->y = useful::RandomRot();
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
	}
}

//============================================================
// �ړ���ԏ���
//============================================================
void CEnemyNav::MoveFunc(const D3DXVECTOR3& rPos)
{

}
