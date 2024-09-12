//============================================================
//
//	�G���� [enemy.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "camera.h"

#include "player.h"
#include "player_clone.h"
#include "stage.h"
#include "actor.h"

#include "enemy_item.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY = 3;				// �G�̗D�揇��
	const float	GRAVITY = 60.0f;			// �d��
	const float VIEW_RANGE = 700.0f;		// ���E�͈̔�
	const float SUB_VANISH_ALPHA = 0.02f;	// ���Ŏ��̓����x�̌��Z��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CEnemy>* CEnemy::m_pList = nullptr;			// �I�u�W�F�N�g���X�g

//************************************************************
//	�e�N���X [CEnemy] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemy::CEnemy() : CObjectChara(CObject::LABEL_ENEMY, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
m_pItem(nullptr),			// �������̏��
m_oldPos(VEC3_ZERO),		// �ߋ��ʒu
m_posInit(VEC3_ZERO),		// �����ʒu
m_destRot(VEC3_ZERO),		// �ړI�̌���
m_move(VEC3_ZERO),			// �ړ���
m_rotInit(VEC3_ZERO),		// ��������
m_bJump(false),				// ���n��
m_bVanish(false),			// ���ŏ�
m_bHitToPlayer(false)		// �v���C���[�Ƃ̏Փ˔���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemy::~CEnemy()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemy::Init(void)
{
	// �I�u�W�F�N�g�L�����N�^�[�̏�����
	if (FAILED(CObjectChara::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CEnemy>::Create();
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
void CEnemy::Uninit(void)
{
	if (m_pItem != nullptr)
	{ // �������� NULL ����Ȃ��ꍇ

		// �������̏I������
		m_pItem->Uninit();
		m_pItem = nullptr;
	}

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	CObjectChara::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemy::Update(const float fDeltaTime)
{
	// �����x���擾
	float fAlpha = GetAlpha();

	if (m_bVanish)
	{ // ���ŏ󋵂� true �̏ꍇ

		// ���Ŏ��̓����x�����Z����
		fAlpha -= SUB_VANISH_ALPHA;

		// �����x��K�p����
		SetAlpha(fAlpha);

		if (m_pItem != nullptr)
		{ // �A�C�e���������Ă���ꍇ

			// �A�C�e���̃I�t�Z�b�g����
			m_pItem->Update(fDeltaTime);

			// �����x��ݒ肷��
			m_pItem->SetAlpha(fAlpha);
		}

		// �����x�� 0.0f ���߂̏ꍇ�A������
		if (fAlpha > 0.0f) { return; }

		// �j������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}

	D3DXVECTOR3 posEnemy = GetVec3Position();	// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// �G����

	// �ߋ��ʒu�X�V
	UpdateOldPosition();

	// ��ԍX�V
	int nCurMotion = UpdateState(&posEnemy, &rotEnemy, fDeltaTime);	// ���݂̃��[�V�������擾

	SetVec3Position(posEnemy);	// �ʒu�𔽉f
	SetVec3Rotation(rotEnemy);	// �����𔽉f

	// ����������
	bool bFar = useful::IsNearPosR(GetVec3Position());
	if (!bFar) { return; }

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�X�V
	UpdateMotion(nCurMotion, fDeltaTime);

	if (m_pItem != nullptr)
	{ // �A�C�e���������Ă���ꍇ

		// �A�C�e���̃I�t�Z�b�g����
		m_pItem->Update(fDeltaTime);

		// �����x��ݒ肷��
		m_pItem->SetAlpha(fAlpha);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CEnemy::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw(pShader);

	if (m_pItem != nullptr)
	{ // �A�C�e���������Ă���ꍇ

		// �A�C�e���̃I�t�Z�b�g����
		m_pItem->Draw(pShader);
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CEnemy>* CEnemy::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
// �S�G�̏��ŏ���
//============================================================
void CEnemy::VanishAll(void)
{
	// �G�̃��X�g�� NULL �������ꍇ�A������
	if (m_pList == nullptr) { return; }

	// �S�G�̏��ŏ󋵂� true �ɂ���
	for (auto& rEnemy : m_pList->GetList())
	{
		rEnemy->m_bVanish = true;
	}
}

//============================================================
// �����蔻�菈��
//============================================================
bool CEnemy::Collision(D3DXVECTOR3& rPos)
{
	bool bHit = false;		// �q�b�g��

	// �A�N�^�[�̓����蔻�菈��
	CollisionActor(rPos, bHit);

	// �q�b�g�󋵂�Ԃ�
	return bHit;
}

//============================================================
// �v���C���[�̒T������
//============================================================
bool CEnemy::SearchPlayer(D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 pos = VEC3_ZERO;				// �ʒu
	float fRot = GetVec3Rotation().y + D3DX_PI;	// ����

	// �����𐳋K��
	useful::NormalizeRot(fRot);

	// �v���C���[���̎擾
	CPlayer* pPlayer = GET_PLAYER;

	// �v���C���[�����Ȃ��ꍇ������
	if (pPlayer == nullptr) { return false; }

	// �ʒu���擾����
	pos = pPlayer->GetVec3Position();

	// ���E���ɋ��Ȃ������ꍇ false ��Ԃ�
	if (!collision::Sector(GetVec3Position(), pos, fRot, VIEW_RANGE, D3DX_PI)) { return false; }

	// �v���C���[�̈ʒu���擾����
	if (pPos != nullptr) { *pPos = pos; }

	// true ��Ԃ�
	return true;
}

//============================================================
// ���g�̒T������
//============================================================
bool CEnemy::SearchClone(D3DXVECTOR3* pPos, CPlayerClone** pClone)
{
	D3DXVECTOR3 pos = VEC3_ZERO;					// �ʒu
	D3DXVECTOR3 posEnemy = GetVec3Position();		// �G�̈ʒu
	float fRot = GetVec3Rotation().y + D3DX_PI;		// ����
	float fLength = FLT_MAX;						// ����
	float fLengthComp = FLT_MAX;					// ��r���鋗��
	CPlayerClone* pChaseClone = nullptr;			// �ǐՂ��镪�g

	// �����𐳋K��
	useful::NormalizeRot(fRot);

	if (CPlayerClone::GetList() == nullptr ||
		*CPlayerClone::GetList()->GetBegin() == nullptr)
	{ // ���g�̃��X�g�������ꍇ

		// false��Ԃ�
		return false;
	}

	for (auto& clone : CPlayerClone::GetList()->GetList())
	{
		// ���s��Ԃ���Ȃ��ꍇ�A���ɐi��
		if (clone->GetAction() != CPlayerClone::ACTION_MOVE) { continue; }

		// ���g�̈ʒu���擾����
		pos = clone->GetVec3Position();

		// �����𑪂�
		fLengthComp = sqrtf((posEnemy.x - pos.x) * (posEnemy.x - pos.x) + (posEnemy.z - pos.z) * (posEnemy.z - pos.z));

		// ��r���������̕��������ꍇ�A���ɐi��
		if (fLength < fLengthComp) { continue; }

		// �������X�V����
		fLength = fLengthComp;

		// �ǐՂ��镪�g��ݒ�
		pChaseClone = clone;
	}

	// �ǐՂ��镪�g�����Ȃ��ꍇ�A������
	if (pChaseClone == nullptr) { return false; }

	// �ʒu��ݒ肷��
	pos = pChaseClone->GetVec3Position();

	// ���E���ɋ��Ȃ������ꍇ false ��Ԃ�
	if (!collision::Sector(GetVec3Position(), pos, fRot, VIEW_RANGE, D3DX_PI)) { return false; }

	// �v���C���[�̈ʒu���擾����
	if (pPos != nullptr) { *pPos = pos; }

	// ���g�̏����擾����
	if (pClone != nullptr) { *pClone = pChaseClone; }

	// true ��Ԃ�
	return true;
}

//============================================================
// �d�͏���
//============================================================
void CEnemy::UpdateGravity(void)
{
	// �d�͂����Z����
	m_move.y -= GRAVITY;
}

//============================================================
// ���n����
//============================================================
void CEnemy::UpdateLanding(D3DXVECTOR3* pPos)
{
	CStage *pStage = GET_STAGE;	// �X�e�[�W���

	// �W�����v���Ă����Ԃɂ���
	m_bJump = true;

	// �n�ʁE�����ʒu�̒��n����
	if (pStage->LandFieldPosition(*pPos, m_oldPos, m_move)
	||  pStage->LandLimitPosition(*pPos, m_move, 0.0f))
	{ // ���n���Ă����ꍇ

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}
}

//============================================================
// �A�N�^�[�̓����蔻�菈��
//============================================================
void CEnemy::CollisionActor(D3DXVECTOR3& rPos, bool& bHit)
{
	// �A�N�^�[�̃��X�g�\����������Δ�����
	if (CActor::GetList() == nullptr) { return; }

	// ���X�g���擾
	std::list<CActor*> list = CActor::GetList()->GetList();

	for (auto actor : list)
	{
		// �����蔻�菈��
		actor->Collision
		(
			rPos,			// �ʒu
			m_oldPos,		// �O��̈ʒu
			GetRadius(),	// ���a
			GetHeight(),	// ����
			m_move,			// �ړ���
			m_bJump,		// �W�����v��
			bHit,			// �q�b�g��
			false			// �������
		);
	}
}

//============================================================
// �v���C���[�Ƃ̓����蔻�菈��
//============================================================
void CEnemy::CollisionToPlayer(D3DXVECTOR3& rPos, const float fRadius, const float fHeight)
{
	D3DXVECTOR3 pos = GetVec3Position();
	float fHeightEnemy = GetHeight();

	// �����������Ă��Ȃ��ꍇ�A������
	if (pos.y + fHeightEnemy < rPos.y || pos.y > rPos.y + fHeight) { m_bHitToPlayer = false; return; }

	// �G�ɓ������Ă��Ȃ��ꍇ�A������
	if (!collision::CirclePillar(rPos, pos, fRadius, GetRadius())) { m_bHitToPlayer = false; return; }

	// �v���C���[�Ƃ̏Փ˔���� true �ɂ���
	m_bHitToPlayer = true;
}
