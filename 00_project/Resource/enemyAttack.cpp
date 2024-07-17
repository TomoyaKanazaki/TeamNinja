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
#include "enemyNavigation.h"
#include "enemyNavStreet.h"
#include "enemyNavRandom.h"
#include "enemyChaseRange.h"

#include "enemyStalk.h"
#include "enemyWolf.h"
#include "enemyAmbush.h"
#include "effekseerControl.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 ATTACK_COLLUP = D3DXVECTOR3(30.0f, 100.0f, 30.0f);	// �U������(��)
	const D3DXVECTOR3 ATTACK_COLLDOWN = D3DXVECTOR3(30.0f, 0.0f, 30.0f);	// �U������(��)
	const int DODGE_COUNT = 17;					// ����J�E���g��
	const float SHAKEOFF_RANGE = 1000.0f;		// �U��؂�鋗��
	const float DIVERSION_EFFECT_SCALE = 10.0f;	// ���g�Ƃ̐퓬�G�t�F�N�g�̑傫��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CEnemyAttack>* CEnemyAttack::m_pList = nullptr;			// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CEnemyAttack] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAttack::CEnemyAttack() : CEnemy(),
m_pNav(nullptr),			// �i�r�Q�[�V�����̏��
m_pChaseRange(nullptr),		// �ǐՔ͈͂̏��
m_pClone(nullptr),			// ���g�̏��
m_posTarget(VEC3_ZERO),		// �ڕW�̈ʒu
m_target(TARGET_NONE),		// �W�I
m_nAttackCount(0),			// �U���J�E���g
m_type(TYPE_STALK),			// ���
m_fAlpha(1.0f),				// �����x
m_bDodge(false)				// �����t�t���O
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

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CEnemyAttack>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyAttack::Uninit(void)
{
	// �i�r�Q�[�V�����̏I������
	SAFE_UNINIT(m_pNav);

	// �ǐՔ͈͂̏I������
	SAFE_UNINIT(m_pChaseRange);

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

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

	if (m_bDodge)
	{
		SetAllMaterial(material::Blue());
	}
	else
	{
		ResetMaterial();
	}
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
//	���X�g�擾����
//============================================================
CListManager<CEnemyAttack>* CEnemyAttack::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
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

//============================================================
// �v���C���[�̐U��؂菈��
//============================================================
bool CEnemyAttack::ShakeOffPlayer(void)
{
	// �ʒu���擾����
	m_posTarget = CScene::GetPlayer()->GetVec3Position();

	if (collision::Circle2D(GetVec3Position(), m_posTarget, GetRadius(), SHAKEOFF_RANGE) == true)
	{ // �͈͓��ɓ����Ă���ꍇ

		// �v���C���[��W�I�ɂ���
		m_target = TARGET_PLAYER;

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}

//============================================================
// ���g�̐U��؂菈��
//============================================================
bool CEnemyAttack::ShakeOffClone(void)
{
	D3DXVECTOR3 pos = VEC3_ZERO;					// �ʒu
	D3DXVECTOR3 posEnemy = GetVec3Position();		// �G�̈ʒu

	if (CPlayerClone::GetList() == nullptr ||
		*CPlayerClone::GetList()->GetBegin() == nullptr)
	{ // ���g�̃��X�g�������ꍇ

		// false��Ԃ�
		return false;
	}

	for (int nCnt = 0; nCnt < CPlayerClone::GetList()->GetNumAll(); nCnt++)
	{
		// ���g�̈ʒu���擾����
		pos = (*CPlayerClone::GetList()->GetIndex(nCnt))->GetVec3Position();

		if (!collision::Circle2D(GetVec3Position(), pos, GetRadius(), SHAKEOFF_RANGE)) { continue; }

		// �ʒu��ݒ肷��
		m_posTarget = (*CPlayerClone::GetList()->GetIndex(nCnt))->GetVec3Position();

		// ���g�̏���ݒ肷��
		m_pClone = *CPlayerClone::GetList()->GetIndex(nCnt);

		// ���g��W�I�ɂ���
		m_target = TARGET_CLONE;

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
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
	// �����t�t���O�� false �̏ꍇ�A������
	if (m_bDodge == false) { return; }

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

	// ����J�E���g�����Z����
	m_nAttackCount++;

	if (m_nAttackCount >= DODGE_COUNT)
	{ // ����J�E���g���߂����ꍇ

		// �{�b�N�X�̓����蔻��
		if (collision::Box3D
		(
			rPos,				// ����ʒu
			posPlayer,			// ����ڕW�ʒu
			ATTACK_COLLUP,		// ����T�C�Y(�E�E��E��)
			ATTACK_COLLDOWN,	// ����T�C�Y(���E���E�O)
			sizeUpPlayer,		// ����ڕW�T�C�Y(�E�E��E��)
			sizeDownPlayer		// ����ڕW�T�C�Y(���E���E�O)
		))
		{ // �������ĂȂ������ꍇ

			// ����J�E���g������������
			m_nAttackCount = 0;

			// �q�b�g����
			CScene::GetPlayer()->Hit(500);
		}

		// �����t�t���O�� false �ɂ���
		m_bDodge = false;
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
		m_pClone == nullptr)
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

	// ���g�Ƃ̐퓬�G�t�F�N�g���o��
	GET_EFFECT->Create("data\\EFFEKSEER\\diversion.efkefc", GetVec3Position(), GetVec3Rotation(), VEC3_ZERO, DIVERSION_EFFECT_SCALE);

	// �����t�t���O�� false �ɂ���
	m_bDodge = false;
}

//============================================================
//	��������(���͈͈ړ��G)
//============================================================
CEnemyAttack* CEnemyAttack::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const EType type,			// ���
	const float fMoveWidth,		// �ړ���
	const float fMoveDepth,		// �ړ����s
	const float fChaseWidth,	// �ǐՕ�
	const float fChaseDepth		// �ǐՉ��s
)
{
	// �|�C���^��錾
	CEnemyAttack* pEnemy = nullptr;	// �G���

	switch (type)
	{
	case TYPE_STALK:

		// �ǐՓG�𐶐�
		pEnemy = new CEnemyStalk;

		break;

	case TYPE_WOLF:

		// ���G�𐶐�
		pEnemy = new CEnemyWolf;

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (pEnemy == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�̏�����
		if (FAILED(pEnemy->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�̔j��
			SAFE_DELETE(pEnemy);
			return nullptr;
		}

		// �ʒu��ݒ�
		pEnemy->SetVec3Position(rPos);

		// ������ݒ�
		pEnemy->SetVec3Rotation(rRot);

		// ��ނ�ݒ�
		pEnemy->m_type = type;

		// �����ʒu��ݒ�
		pEnemy->SetPosInit(rPos);

		// ���̐ݒ菈��
		pEnemy->SetData();

		// �i�r�Q�[�V�����𐶐�
		pEnemy->m_pNav = CEnemyNavRandom::Create(rPos, fMoveWidth, fMoveDepth);

		// �ǐՔ͈͂𐶐�
		pEnemy->m_pChaseRange = CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth);

		// �m�ۂ����A�h���X��Ԃ�
		return pEnemy;
	}
}

//============================================================
//	��������(���[�g����ړ��G)
//============================================================
CEnemyAttack* CEnemyAttack::Create
(
	const D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rRot,				// ����
	const EType type,						// ���
	const std::vector<D3DXVECTOR3> route,	// ���[�g�̔z��
	const float fChaseWidth,				// �ǐՕ�
	const float fChaseDepth					// �ǐՉ��s
)
{
	// �|�C���^��錾
	CEnemyAttack* pEnemy = nullptr;	// �G���

	switch (type)
	{
	case TYPE_STALK:

		// �ǐՓG�𐶐�
		pEnemy = new CEnemyStalk;

		break;

	case TYPE_WOLF:

		// ���G�𐶐�
		pEnemy = new CEnemyWolf;

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (pEnemy == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�̏�����
		if (FAILED(pEnemy->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�̔j��
			SAFE_DELETE(pEnemy);
			return nullptr;
		}

		// �ʒu��ݒ�
		pEnemy->SetVec3Position(rPos);

		// ������ݒ�
		pEnemy->SetVec3Rotation(rRot);

		// ��ނ�ݒ�
		pEnemy->m_type = type;

		// �����ʒu��ݒ�
		pEnemy->SetPosInit(rPos);

		// ���̐ݒ菈��
		pEnemy->SetData();

		// �i�r�Q�[�V�����𐶐�
		pEnemy->m_pNav = CEnemyNavStreet::Create(route);

		// �ǐՔ͈͂𐶐�
		pEnemy->m_pChaseRange = CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth);

		// �m�ۂ����A�h���X��Ԃ�
		return pEnemy;
	}
}

//===========================================
//  �����蔻��̎擾
//===========================================
D3DXVECTOR3 CEnemyAttack::GetAttackUp()
{
	return ATTACK_COLLUP;
}

//===========================================
//  �����蔻��̎擾
//===========================================
D3DXVECTOR3 CEnemyAttack::GetAttackDown()
{
	return ATTACK_COLLDOWN;
}
