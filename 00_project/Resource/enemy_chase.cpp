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
#include "enemy_chase.h"
#include "renderer.h"
#include "deltaTime.h"

#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\TXT\\player.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const float MOVE = -420.0f;							// �ړ���
	const float ROT_REV = 0.5f;							// �����̕␳�W��
}

//************************************************************
//	�q�N���X [CEnemyChase] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyChase::CEnemyChase(const EType type) : CEnemy(type)
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

	// �ǐՏ���
	Chase();

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
// �ǐՏ���
//============================================================
void CEnemyChase::Chase(void)
{
	D3DXVECTOR3 pos = GetVec3Position();		// �ʒu
	D3DXVECTOR3 destRot = GetDestRotation();	// �ړI�̌���
	D3DXVECTOR3 rot = GetVec3Rotation();		// ����
	D3DXVECTOR3 posPlayer = CScene::GetPlayer()->GetVec3Position();		// �v���C���[�̈ʒu
	D3DXVECTOR3 move = GetMovePosition();		// �ړ���
	float fDiff;

	// �ړI�̌������擾
	destRot.y = atan2f(pos.x - posPlayer.x, pos.z - posPlayer.z);

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