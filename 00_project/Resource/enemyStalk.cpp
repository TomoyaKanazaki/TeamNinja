//============================================================
//
//	�ǐՓG���� [enemy_chase.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "enemyStalk.h"
#include "renderer.h"
#include "deltaTime.h"
#include "collision.h"

#include "player.h"
#include "player_clone.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\player.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const float MOVE = -300.0f;								// �ړ���
	const float ROT_REV = 0.5f;								// �����̕␳�W��
	const float ATTACK_DISTANCE = 50.0f;					// �U������ɓ��鋗��
	const float VIEW_RANGE = 400.0f;						// ���E�͈̔�
}

//************************************************************
//	�q�N���X [CEnemyStalk] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyStalk::CEnemyStalk() : CEnemy(),
m_posTarget(VEC3_ZERO),		// �ڕW�̈ʒu
m_target(TARGET_PLAYER),	// �ڕW
m_state(STATE_CRAWL)		// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyStalk::~CEnemyStalk()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyStalk::Init(void)
{
	// �G�̏�����
	if (FAILED(CEnemy::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �L�����N�^�[���̊���
	BindCharaData(SETUP_TXT);

	// �}�e���A���̐ݒ菈��
	SetAllMaterial(material::DamageRed());

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyStalk::Uninit(void)
{
	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyStalk::Update(const float fDeltaTime)
{
	// �ߋ��ʒu�X�V
	UpdateOldPosition();

	// ���񏈗�
	Crawl();

	// ��ԏ���
	State();

	// �G�̍X�V
	CEnemy::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyStalk::Draw(CShader* pShader)
{
	// �G�̕`��
	CEnemy::Draw(pShader);
}

//============================================================
// ��ԏ���
//============================================================
void CEnemyStalk::State(void)
{
	switch (m_state)
	{
	case CEnemyStalk::STATE_CRAWL:

		break;

	case CEnemyStalk::STATE_STALK:

		// �ǐՏ���
		Stalk();

		break;

	default:		// ��O����
		assert(false);
		break;
	}
}

//============================================================
// ���񏈗�
//============================================================
void CEnemyStalk::Crawl(void)
{
	float fRot = GetVec3Rotation().y + D3DX_PI;

	switch (m_target)
	{
	case CEnemyStalk::TARGET_PLAYER:

		// �v���C���[�̈ʒu���擾����
		m_posTarget = CScene::GetPlayer()->GetVec3Position();

		break;

	case CEnemyStalk::TARGET_CLONE:

		// ���g�̃��X�g�������ꍇ�����o��
		if (CPlayerClone::GetList() == nullptr ||
			*CPlayerClone::GetList()->GetBegin() == nullptr)
		{
			return;
		}

		// ���g�̈ʒu���擾����
		m_posTarget = (*CPlayerClone::GetList()->GetBegin())->GetVec3Position();

		break;

	default:		// ��O����
		assert(false);
		break;
	}

	// �����𐳋K��
	useful::NormalizeRot(fRot);

	if (collision::Sector(GetVec3Position(), m_posTarget, fRot, VIEW_RANGE, D3DX_PI))
	{ // ���E���ɓ������ꍇ

		// �ǐՏ�Ԃɂ���
		m_state = STATE_STALK;

		// �֐��𔲂���
		return;
	}

	// �����Ԃɂ���
	m_state = STATE_CRAWL;
}

//============================================================
// �ǐՏ���
//============================================================
void CEnemyStalk::Stalk(void)
{
	// �ړ�����
	Move();

	if (Approach())
	{ // �ڋ߂����ꍇ

		switch (m_target)
		{
		case CEnemyStalk::TARGET_PLAYER:

			// �q�b�g����
			CScene::GetPlayer()->Hit(1);

			break;

		case CEnemyStalk::TARGET_CLONE:

			// �q�b�g����
			(*CPlayerClone::GetList()->GetBegin())->Hit(1);

			break;

		default:		// ��O����
			assert(false);
			break;
		}
	}

	// �f�o�b�O
	DebugProc::Print(DebugProc::POINT_RIGHT, "����!!");
}

//============================================================
// �ړ�����
//============================================================
void CEnemyStalk::Move(void)
{
	D3DXVECTOR3 pos = GetVec3Position();		// �ʒu
	D3DXVECTOR3 destRot = GetDestRotation();	// �ړI�̌���
	D3DXVECTOR3 rot = GetVec3Rotation();		// ����
	D3DXVECTOR3 move = GetMovePosition();		// �ړ���
	float fDiff;

	// �ړI�̌������擾
	destRot.y = atan2f(pos.x - m_posTarget.x, pos.z - m_posTarget.z);

	// �����̍���
	fDiff = destRot.y - rot.y;

	// �����̐��K��
	useful::NormalizeRot(fDiff);

	// ������␳
	rot.y += fDiff * ROT_REV;

	// �����̐��K��
	useful::NormalizeRot(rot.y);

	// �ړ��ʂ�ݒ肷��
	move.x = sinf(rot.y) * MOVE * GET_MANAGER->GetDeltaTime()->GetTime();
	move.z = cosf(rot.y) * MOVE * GET_MANAGER->GetDeltaTime()->GetTime();

	// �ʒu���ړ�����
	pos += move;

	// ����K�p
	SetVec3Position(pos);
	SetDestRotation(destRot);
	SetVec3Rotation(rot);
	SetMovePosition(move);
}

//============================================================
// �ڋߏ���
//============================================================
bool CEnemyStalk::Approach(void)
{
	float fDistance = 0.0f;					// ����
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu

	// �����𑪂�
	fDistance = sqrtf((m_posTarget.x - pos.x) * (m_posTarget.x - pos.x) + (m_posTarget.z - pos.z) * (m_posTarget.z - pos.z));

	if (fDistance <= ATTACK_DISTANCE)
	{ // ���̋����ɓ������ꍇ

		// �ڋ߂���
		return true;
	}

	// �ڋ߂��ĂȂ�
	return false;
}
