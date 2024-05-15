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
#include "enemyChase.h"
#include "renderer.h"
#include "deltaTime.h"

#include "player.h"
#include "player_clone.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\player.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const float MOVE = -420.0f;								// �ړ���
	const float ROT_REV = 0.5f;								// �����̕␳�W��
}

//************************************************************
//	�q�N���X [CEnemyChase] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyChase::CEnemyChase(const EType type) : CEnemy(type),
m_state(STATE_PLAYER)			// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyChase::~CEnemyChase()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyChase::Init(void)
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
void CEnemyChase::Uninit(void)
{
	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyChase::Update(const float fDeltaTime)
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
void CEnemyChase::Draw(CShader* pShader)
{
	// �G�̕`��
	CEnemy::Draw(pShader);
}

//============================================================
// �W�I�I������
//============================================================
void CEnemyChase::TargetSelect(void)
{
	D3DXVECTOR3 posTarget = VEC3_ZERO;		// �W�I�̈ʒu

	switch (m_state)
	{
	case CEnemyChase::STATE_PLAYER:

		// �v���C���[�̈ʒu���擾����
		posTarget = CScene::GetPlayer()->GetVec3Position();

		break;

	case CEnemyChase::STATE_CLONE:

		// ���g�̃��X�g�������ꍇ�����o��
		if (CPlayerClone::GetList() == nullptr ||
			*CPlayerClone::GetList()->GetBegin() == nullptr) { return; }

		// ���g�̈ʒu���擾����
		posTarget = (*CPlayerClone::GetList()->GetBegin())->GetVec3Position();

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// �ǐՏ���
	Chase(posTarget);
}

//============================================================
// �ǐՏ���
//============================================================
void CEnemyChase::Chase(const D3DXVECTOR3 posTarget)
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