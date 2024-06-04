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
	const float MOVE = -350.0f;								// �ړ���
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
m_state(STATE_PLAYER)			// ���
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

	// �W�I�I������
	TargetSelect();

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
// �W�I�I������
//============================================================
void CEnemyStalk::TargetSelect(void)
{
	D3DXVECTOR3 posTarget = VEC3_ZERO;		// �W�I�̈ʒu

	switch (m_state)
	{
	case CEnemyStalk::STATE_PLAYER:

		// �v���C���[�̈ʒu���擾����
		posTarget = CScene::GetPlayer()->GetVec3Position();

		if (Search(posTarget))
		{ // �T���͈͂Ɉ������������ꍇ

			// �ǐՏ���
			Chase(posTarget);

			// �f�o�b�O
			DebugProc::Print(DebugProc::POINT_RIGHT, "����!!");
		}

		if (Approach(posTarget))
		{ // �ڋ߂����ꍇ

			// �q�b�g����
			CScene::GetPlayer()->Hit(1);
		}

		break;

	case CEnemyStalk::STATE_CLONE:

		// ���g�̃��X�g�������ꍇ�����o��
		if (CPlayerClone::GetList() == nullptr ||
			*CPlayerClone::GetList()->GetBegin() == nullptr) { return; }

		// ���g�̈ʒu���擾����
		posTarget = (*CPlayerClone::GetList()->GetBegin())->GetVec3Position();

		if (Search(posTarget))
		{ // �T���͈͂Ɉ������������ꍇ

			// �ǐՏ���
			Chase(posTarget);

			// �f�o�b�O
			DebugProc::Print(DebugProc::POINT_RIGHT, "����!!");
		}

		if (Approach(posTarget))
		{ // �ڋ߂����ꍇ

			// �q�b�g����
			(*CPlayerClone::GetList()->GetBegin())->Hit(1);
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//============================================================
// �T������
//============================================================
bool CEnemyStalk::Search(const D3DXVECTOR3& posTarget)
{
	float fRot = GetVec3Rotation().y + D3DX_PI;

	// �����𐳋K��
	useful::NormalizeRot(fRot);

	// �����蔻��̌��ʂ�Ԃ�
	return collision::Sector(GetVec3Position(), posTarget, fRot, VIEW_RANGE, D3DX_PI);
}

//============================================================
// �ǐՏ���
//============================================================
void CEnemyStalk::Chase(const D3DXVECTOR3& posTarget)
{
	D3DXVECTOR3 pos = GetVec3Position();		// �ʒu
	D3DXVECTOR3 destRot = GetDestRotation();	// �ړI�̌���
	D3DXVECTOR3 rot = GetVec3Rotation();		// ����
	D3DXVECTOR3 move = GetMovePosition();		// �ړ���
	float fDiff;

	// �ړI�̌������擾
	destRot.y = atan2f(pos.x - posTarget.x, pos.z - posTarget.z);

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
bool CEnemyStalk::Approach(const D3DXVECTOR3& posTarget)
{
	float fDistance = 0.0f;					// ����
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu

	// �����𑪂�
	fDistance = sqrtf((posTarget.x - pos.x) * (posTarget.x - pos.x) + (posTarget.z - pos.z) * (posTarget.z - pos.z));

	if (fDistance <= ATTACK_DISTANCE)
	{ // ���̋����ɓ������ꍇ

		// �ڋ߂���
		return true;
	}

	// �ڋ߂��ĂȂ�
	return false;
}
