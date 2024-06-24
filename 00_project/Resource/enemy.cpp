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

#include "player.h"
#include "player_clone.h"
#include "stage.h"

#include "enemy_item.h"
#include "enemyStalk.h"
#include "enemyCaveat.h"
#include "enemyWolf.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY = 3;			// �G�̗D�揇��
	const float	GRAVITY = 60.0f;		// �d��
	const float VIEW_RANGE = 400.0f;	// ���E�͈̔�
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
CEnemy::CEnemy() : CObjectChara(CObject::LABEL_ENEMY, CObject::DIM_3D, PRIORITY),
m_pItem(nullptr),			// �������̏��
m_oldPos(VEC3_ZERO),		// �ߋ��ʒu
m_destRot(VEC3_ZERO),		// �ړI�̌���
m_move(VEC3_ZERO),			// �ړ���
m_type(TYPE_STALK),			// ���
m_bJump(false)				// ���n��
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
	D3DXVECTOR3 posEnemy = GetVec3Position();	// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// �G����

	// �ߋ��ʒu�X�V
	UpdateOldPosition();

	// ��ԍX�V
	int nCurMotion = UpdateState(&posEnemy, &rotEnemy, fDeltaTime);	// ���݂̃��[�V�������擾

	SetVec3Position(posEnemy);	// �ʒu�𔽉f
	SetVec3Rotation(rotEnemy);	// �����𔽉f

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�X�V
	UpdateMotion(nCurMotion, fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CEnemy::Draw(CShader* pShader)
{
	CToonShader	*pToonShader = CToonShader::GetInstance();	// �g�D�[���V�F�[�_�[���
	if (pToonShader->IsEffectOK())
	{ // �G�t�F�N�g���g�p�\�ȏꍇ

		// �I�u�W�F�N�g�L�����N�^�[�̕`��
		CObjectChara::Draw(pToonShader);
	}
	else
	{ // �G�t�F�N�g���g�p�s�\�ȏꍇ

		// �G�t�F�N�g�G���[
		assert(false);

		// �I�u�W�F�N�g�L�����N�^�[�̕`��
		CObjectChara::Draw(pShader);
	}
}

//============================================================
//	��������
//============================================================
CEnemy* CEnemy::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const EType type)
{
	// �|�C���^��錾
	CEnemy* pEnemy = nullptr;	// �G���

	switch (type)
	{
	case TYPE_STALK:

		// �ǐՓG�𐶐�
		pEnemy = new CEnemyStalk;

		break;

	case TYPE_CAVEAT:

		// �x���G�𐶐�
		pEnemy = new CEnemyCaveat;

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

		// ���̐ݒ菈��
		pEnemy->SetData();

		// �ʒu��ݒ�
		pEnemy->SetVec3Position(rPos);

		// ������ݒ�
		pEnemy->SetVec3Rotation(rRot);

		// ��ނ�ݒ�
		pEnemy->m_type = type;

		// �m�ۂ����A�h���X��Ԃ�
		return pEnemy;
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
// �v���C���[�̒T������
//============================================================
bool CEnemy::SearchPlayer(D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 pos = VEC3_ZERO;				// �ʒu
	float fRot = GetVec3Rotation().y + D3DX_PI;	// ����

	// �����𐳋K��
	useful::NormalizeRot(fRot);

	// �ʒu���擾����
	pos = CScene::GetPlayer()->GetVec3Position();

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
bool CEnemy::SearchClone(D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 pos = VEC3_ZERO;					// �ʒu
	D3DXVECTOR3 posEnemy = GetVec3Position();		// �G�̈ʒu
	float fRot = GetVec3Rotation().y + D3DX_PI;		// ����
	float fLength = FLT_MAX;						// ����
	float fLengthComp = FLT_MAX;					// ��r���鋗��
	int nIdx = INT_MAX;								// �ǂ킹��C���f�b�N�X

	// �����𐳋K��
	useful::NormalizeRot(fRot);

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

		// �����𑪂�
		fLengthComp = sqrtf((posEnemy.x - pos.x) * (posEnemy.x - pos.x) + (posEnemy.z - pos.z) * (posEnemy.z - pos.z));

		// ��r���������̕��������ꍇ�A���ɐi��
		if (fLength < fLengthComp) { continue; }

		// �������X�V����
		fLength = fLengthComp;

		// �C���f�b�N�X��ݒ肷��
		nIdx = nCnt;
	}

	// �ʒu��ݒ肷��
	pos = (*CPlayerClone::GetList()->GetIndex(nIdx))->GetVec3Position();

	// ���E���ɋ��Ȃ������ꍇ false ��Ԃ�
	if (!collision::Sector(GetVec3Position(), pos, fRot, VIEW_RANGE, D3DX_PI)) { return false; }

	// �v���C���[�̈ʒu���擾����
	if (pPos != nullptr) { *pPos = pos; }

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
	CStage *pStage = CScene::GetStage();	// �X�e�[�W���

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
