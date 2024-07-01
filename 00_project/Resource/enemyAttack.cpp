//============================================================
//
//	�U���n�G���� [enemyAttack.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "enemyAttack.h"
#include "renderer.h"
#include "deltaTime.h"

#include "player.h"
#include "player_clone.h"
#include "multiModel.h"

#include "collision.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\enemy.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const float MOVE = -250.0f;				// �ړ���
	const float ROT_REV = 0.5f;				// �����̕␳�W��
	const float ATTACK_DISTANCE = 50.0f;	// �U������ɓ��鋗��
	const int	BLEND_FRAME_OTHER = 5;		// ���[�V�����̊�{�I�ȃu�����h�t���[��
	const int	BLEND_FRAME_LAND = 15;		// ���[�V�������n�̃u�����h�t���[��
	const int	CAUTIOUS_TRANS_LOOP = 7;	// �x�����[�V�����ɑJ�ڂ���ҋ@���[�v��
	const D3DXVECTOR3 ATTACK_COLLUP = D3DXVECTOR3(30.0f, 100.0f, 30.0f);	// �U������(��)
	const D3DXVECTOR3 ATTACK_COLLDOWN = D3DXVECTOR3(30.0f, 0.0f, 30.0f);	// �U������(��)
	const int DODGE_COUNT = 17;				// ����J�E���g��
}

//************************************************************
//	�q�N���X [CEnemyAttack] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack::CEnemyAttack() : CEnemy(),
m_pClone(nullptr),			// ���g�̏��
m_posTarget(VEC3_ZERO),		// �ڕW�̈ʒu
m_target(TARGET_PLAYER),	// �W�I
m_nAttackCount(0),			// �U���J�E���g
m_bAttack(false)			// �U����
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyAttack::~CEnemyAttack()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyAttack::Init(void)
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

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyAttack::Uninit(void)
{
	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyAttack::Update(const float fDeltaTime)
{
	// �G�̍X�V
	CEnemy::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyAttack::Draw(CShader* pShader)
{
	// �G�̕`��
	CEnemy::Draw(pShader);
}

//============================================================
// ���̐ݒ菈��
//============================================================
void CEnemyAttack::SetData(void)
{

}

//============================================================
// �ړ�����
//============================================================
void CEnemyAttack::Move(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// �ړI�̌���
	D3DXVECTOR3 move = GetMovePosition();		// �ړ���
	float fDiff;

	// �ړI�̌������擾
	destRot.y = atan2f(pPos->x - m_posTarget.x, pPos->z - m_posTarget.z);

	// �����̍���
	fDiff = destRot.y - pRot->y;

	// �����̐��K��
	useful::NormalizeRot(fDiff);

	// ������␳
	pRot->y += fDiff * ROT_REV;

	// �����̐��K��
	useful::NormalizeRot(pRot->y);

	// �ړ��ʂ�ݒ肷��
	move.x = sinf(pRot->y) * MOVE * GET_MANAGER->GetDeltaTime()->GetTime();
	move.z = cosf(pRot->y) * MOVE * GET_MANAGER->GetDeltaTime()->GetTime();

	// �ʒu���ړ�����
	*pPos += move;

	// ����K�p
	SetDestRotation(destRot);
	SetMovePosition(move);
}

//============================================================
// �ڋߏ���
//============================================================
bool CEnemyAttack::Approach(const D3DXVECTOR3& rPos)
{
	float fDistance = 0.0f;					// ����

	// �����𑪂�
	fDistance = sqrtf((rPos.x - m_posTarget.x) * (rPos.x - m_posTarget.x) + (rPos.z - m_posTarget.z) * (rPos.z - m_posTarget.z));

	if (fDistance <= ATTACK_DISTANCE)
	{ // ���̋����ɓ������ꍇ

		// �ڋ߂���
		return true;
	}

	// �ڋ߂��ĂȂ�
	return false;
}

//====================================================================================================================================================================================
// TODO�F�������牺�͂��Z���J�X���肾�����ŏC��
//====================================================================================================================================================================================

//============================================================
// �v���C���[�̃q�b�g����
//============================================================
void CEnemyAttack::HitPlayer(const D3DXVECTOR3& rPos)
{
	// �U�����肪 true �̏ꍇ������
	if (m_bAttack == true) { return; }

	// �q�b�g����
	D3DXVECTOR3 posPlayer = CScene::GetPlayer()->GetVec3Position();
	D3DXVECTOR3 sizeUpPlayer =				// �v���C���[�̔���(�E�E��E��)
	{
		CScene::GetPlayer()->GetRadius(),
		CScene::GetPlayer()->GetHeight(),
		CScene::GetPlayer()->GetRadius()
	};
	D3DXVECTOR3 sizeDownPlayer =			// �v���C���[�̔���(���E���E�O)
	{
		CScene::GetPlayer()->GetRadius(),
		0.0f,
		CScene::GetPlayer()->GetRadius()
	};

	// �{�b�N�X�̓����蔻��
	if (!collision::Box3D
	(
		rPos,				// ����ʒu
		posPlayer,			// ����ڕW�ʒu
		ATTACK_COLLUP,		// ����T�C�Y(�E�E��E��)
		ATTACK_COLLDOWN,	// ����T�C�Y(���E���E�O)
		sizeUpPlayer,		// ����ڕW�T�C�Y(�E�E��E��)
		sizeDownPlayer		// ����ڕW�T�C�Y(���E���E�O)
	))
	{ // �������ĂȂ������ꍇ

		return;
	}

	if (m_nAttackCount <= DODGE_COUNT)
	{ // ����J�E���g��

		// �F�ɕς��Ă���
		SetAllMaterial(material::Blue());

		// ����J�E���g�����Z����
		m_nAttackCount++;
	}
	else
	{ // ��L�ȊO

		// ����J�E���g������������
		m_nAttackCount = 0;

		// �}�e���A�������Z�b�g
		ResetMaterial();

		// �q�b�g����
		CScene::GetPlayer()->Hit(500);

		// �U���󋵂� true �ɂ���
		m_bAttack = true;
	}
}

//============================================================
// ���g�̃q�b�g����
//============================================================
void CEnemyAttack::HitClone(const D3DXVECTOR3& rPos)
{
	// ���g�̏�񂪑��݂��Ȃ��ꍇ������
	if (CPlayerClone::GetList() == nullptr ||
		*CPlayerClone::GetList()->GetBegin() == nullptr ||
		m_pClone == nullptr ||
		m_bAttack == true)
	{
		return;
	}

	CPlayerClone* pClone = nullptr;	// ���g�̏��

	for (auto& rClone : CPlayerClone::GetList()->GetList())
	{
		// ���g�̏�񂶂ᖳ�������ꍇ����
		if (m_pClone != rClone) { continue; }

		// ���g�̏���ݒ�
		pClone = rClone;

		// for���𔲂���
		break;
	}

	// ���g�� NULL �̏ꍇ������
	if (pClone == nullptr) { return; }

	// �q�b�g����
	D3DXVECTOR3 sizeUpClone =
	{
		pClone->GetRadius(),
		pClone->GetHeight(),
		pClone->GetRadius()
	};
	D3DXVECTOR3 sizeDownClone =
	{
		pClone->GetRadius(),
		0.0f,
		pClone->GetRadius()
	};

	// �{�b�N�X�̓����蔻��
	if (!collision::Box3D
	(
		rPos,						// ����ʒu
		pClone->GetVec3Position(),	// ����ڕW�ʒu
		ATTACK_COLLUP,				// ����T�C�Y(�E�E��E��)
		ATTACK_COLLDOWN,			// ����T�C�Y(���E���E�O)
		sizeUpClone,				// ����ڕW�T�C�Y(�E�E��E��)
		sizeDownClone				// ����ڕW�T�C�Y(���E���E�O)
	))
	{ // �������ĂȂ������甲����

		return;
	}

	// �q�b�g����
	pClone->Hit(20);

	// �U���󋵂� true �ɂ���
	m_bAttack = true;
}