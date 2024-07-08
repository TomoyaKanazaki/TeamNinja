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
m_target(TARGET_NONE),		// �W�I
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
void CEnemyAttack::Move(D3DXVECTOR3* pPos, const D3DXVECTOR3& rRot, const float fSpeed, const float fDeltaTime)
{
	D3DXVECTOR3 move = GetMovePosition();		// �ړ���

	// �ړ��ʂ�ݒ肷��
	move.x = sinf(rRot.y) * fSpeed * fDeltaTime;
	move.z = cosf(rRot.y) * fSpeed * fDeltaTime;

	// �ʒu���ړ�����
	*pPos += move;

	// ����K�p
	SetMovePosition(move);
}

//============================================================
// �����̈ړ�����
//============================================================
void CEnemyAttack::RotMove(D3DXVECTOR3& rRot, const float fRevRota, const float fDeltaTime)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// �ڕW����
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = destRot.y - rRot.y;
	useful::NormalizeRot(fDiffRot);	// ���������̐��K��

	// �����̍X�V
	rRot.y += fDiffRot * fDeltaTime * fRevRota;
	useful::NormalizeRot(rRot.y);	// �����̐��K��
}

//============================================================
//	�ڕW�ʒu�̎��F����
//============================================================
void CEnemyAttack::LookTarget(const D3DXVECTOR3& rPos)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// �ڕW����

	// �ڕW���������߂�
	destRot.y = atan2f(rPos.x - GetTargetPos().x, rPos.z - GetTargetPos().z);

	SetDestRotation(destRot);	// �ڕW�����𔽉f
}

//============================================================
// �ڋߏ���
//============================================================
bool CEnemyAttack::Approach(const D3DXVECTOR3& rPos, const float fDis)
{
	float fDistance = 0.0f;					// ����

	// �����𑪂�
	fDistance = sqrtf((rPos.x - m_posTarget.x) * (rPos.x - m_posTarget.x) + (rPos.z - m_posTarget.z) * (rPos.z - m_posTarget.z));

	if (fDistance <= fDis)
	{ // ���̋����ɓ������ꍇ

		// �ڋ߂���
		return true;
	}

	// �ڋ߂��ĂȂ�
	return false;
}

//============================================================
// �v���C���[�̒T������
//============================================================
bool CEnemyAttack::JudgePlayer(void)
{
	// �v���C���[��������Ȃ������ꍇ false ��Ԃ�
	if (!SearchPlayer(&m_posTarget)) { return false; }

	// �W�I���v���C���[�ɂ���
	m_target = TARGET_PLAYER;

	// true ��Ԃ�
	return true;
}

//============================================================
// ���g�̒T������
//============================================================
bool CEnemyAttack::JudgeClone(void)
{
	// ���g��������Ȃ������ꍇ false ��Ԃ�
	if (!SearchClone(&m_posTarget, &m_pClone)) { return false; }

	// �W�I�𕪐g�ɂ���
	m_target = TARGET_CLONE;

	// true ��Ԃ�
	return true;
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