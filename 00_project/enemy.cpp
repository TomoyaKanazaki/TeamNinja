//============================================================
//
//	�G���� [enemy.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "stage.h"
#include "enemyBossDragon.h"
#include "enemyMiniDragon.h"
#include "multiModel.h"
#include "collSphere.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	// ��{���
	const char *SETUP_TXT[] =	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	{
		"data\\TXT\\enemyBossDragon.txt",	// �{�X�h���S��
		"data\\TXT\\enemyMiniDragon.txt",	// �~�j�h���S��
	};

	const int	PRIORITY	= 3;		// �G�̗D�揇��
	const float	GRAVITY		= 1.0f;		// �d��
	const float	REV_ROTA	= 0.15f;	// ������]�̕␳�W��
	const float	MAX_ROTA	= 0.1f;		// ������]�̍ō����x
	const float	JUMP_REV	= 0.16f;	// �󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV	= 0.16f;	// �n��̈ړ��ʂ̌����W��
	const float	ADD_ALPHA	= 0.03f;	// �����x�̉��Z��

	const int	DAMAGE_FRAME	= 4;		// �_���[�W��Ԃ̈ێ��t���[��
	const int	INVULN_FRAME	= 12;		// ���G��Ԃ̈ێ��t���[��
	const int	STAN_FRAME		= 640;		// �X�^����Ԃ̈ێ��t���[��
	const float	STAN_ALPHA		= 0.75f;	// ���G��Ԃ̊�b�����x
	const float	ADD_SINROT		= 0.1f;		// �����x���ӂ�ӂ킳����ۂ̃T�C���J�[�u�������Z��
	const float	MAX_ADD_ALPHA	= 0.15f;	// �����x�̍ő���Z��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CEnemy> *CEnemy::m_pList = nullptr;			// �I�u�W�F�N�g���X�g
CMotion::SInfo		CEnemy::m_aMotionInfo[TYPE_MAX]	= {};	// ���[�V�������
CEnemy::SPartsInfo	CEnemy::m_aPartsInfo[TYPE_MAX]	= {};	// �p�[�c���
CEnemy::SStatusInfo	CEnemy::m_aStatusInfo[TYPE_MAX]	= {};	// �X�e�[�^�X���
CEnemy::SCollInfo	CEnemy::m_aCollInfo[TYPE_MAX]	= {};	// �����蔻����
CEnemy::AFuncUpdateState CEnemy::m_aFuncUpdateState[] =		// ��ԍX�V�֐�
{
	&CEnemy::UpdateNone,		// �Ȃɂ����Ȃ���Ԏ��̍X�V
	&CEnemy::UpdateSpawn,		// �X�|�[����Ԏ��̍X�V
	&CEnemy::UpdateNormal,		// �ʏ��Ԏ��̍X�V
	&CEnemy::UpdateDamage,		// �_���[�W��Ԏ��̍X�V
	&CEnemy::UpdateInvuln,		// ���G��Ԏ��̍X�V
	&CEnemy::UpdateKnock,		// �m�b�N�o�b�N��Ԏ��̍X�V
	&CEnemy::UpdateStan,		// �X�^����Ԏ��̍X�V
	&CEnemy::UpdateRideFlyUp,	// ���C�h��яオ���Ԏ��̍X�V
	&CEnemy::UpdateRideRotate,	// ���C�h�����Ԏ��̍X�V
	&CEnemy::UpdateRideEnd,		// ���C�h�I����Ԏ��̍X�V
	&CEnemy::UpdateDeath,		// ���S��Ԏ��̍X�V
};

//************************************************************
//	�q�N���X [CEnemy] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemy::CEnemy(const EType type) : CObjectChara(CObject::LABEL_ENEMY, CObject::DIM_3D, PRIORITY),
	m_type			(type),					// ��ޒ萔
	m_status		(m_aStatusInfo[type]),	// �X�e�[�^�X�萔
	m_parts			(m_aPartsInfo[type]),	// �p�[�c�萔
	m_motion		(m_aMotionInfo[type]),	// ���[�V�����萔
	m_oldPos		(VEC3_ZERO),			// �ߋ��ʒu
	m_destRot		(VEC3_ZERO),			// �ڕW����
	m_move			(VEC3_ZERO),			// �ړ���
	m_state			(STATE_NONE),			// ���
	m_prevState		(STATE_NONE),			// �_���[�W���󂯂�O�̏��
	m_fSinAlpha		(0.0f),					// ��������
	m_bJump			(false),				// �W�����v��
	m_nCounterState	(0)						// ��ԊǗ��J�E���^�[
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
	// �����o�ϐ���������
	m_oldPos	= VEC3_ZERO;	// �ߋ��ʒu
	m_destRot	= VEC3_ZERO;	// �ڕW����
	m_move		= VEC3_ZERO;	// �ړ���
	m_state		= STATE_NONE;	// ���
	m_prevState	= STATE_NONE;	// �_���[�W���󂯂�O�̏��
	m_fSinAlpha	= 0.0f;			// ��������
	m_bJump		= false;		// �W�����v��
	m_nCounterState	= 0;		// ��ԊǗ��J�E���^�[

	// �I�u�W�F�N�g�L�����N�^�[�̏�����
	if (FAILED(CObjectChara::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	for (int nCntEnemy = 0; nCntEnemy < m_motion.nNumType; nCntEnemy++)
	{ // �ǂݍ��񂾃��[�V���������J��Ԃ�

		// ���[�V�������̐ݒ�
		CObjectChara::SetMotionInfo(m_motion.aMotionInfo[nCntEnemy]);
	}

	for (int nCntEnemy = 0; nCntEnemy < m_parts.nNumParts; nCntEnemy++)
	{ // �p�[�c�����J��Ԃ�

		// �p�[�c���̐ݒ�
		CObjectChara::SetPartsInfo
		( // ����
			nCntEnemy,							// �p�[�c�C���f�b�N�X
			m_parts.aInfo[nCntEnemy].nParentID,	// �e�C���f�b�N�X
			m_parts.aInfo[nCntEnemy].pos,		// �ʒu
			m_parts.aInfo[nCntEnemy].rot,		// ����
			GetModelFileName(nCntEnemy)			// �t�@�C����
		);

		// ������̐ݒ�
		CCollSphere *pColl = GetCollision(nCntEnemy);	// �~������
		std::vector<CCollSphere::SInfo> vector = m_aCollInfo[m_type].apInfo[nCntEnemy]->GetVector();	// �~����z��
		for (const auto& rInfo : vector)
		{ // �z��̗v�f�����J��Ԃ�

			// �����ǉ�
			pColl->AddColl(rInfo.offset, rInfo.fRadius);
		}
	}

	// ���f�����̐ݒ�
	SetModelInfo();

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
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DeleteList(m_iterator);

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
void CEnemy::Update(void)
{
	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update();

	// �e��Ԃ��Ƃ̍X�V
	assert(m_state > NONE_IDX && m_state < STATE_MAX);
	(this->*(m_aFuncUpdateState[m_state]))();

	// ���[�V�����̍X�V
	UpdateMotion();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemy::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw(pShader);
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CEnemy::SetState(const int nState)
{
	// �����̏�Ԃ�ݒ�
	m_state = (EState)nState;

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �Ȃɂ����Ȃ����
	case STATE_NORMAL:	// �ʏ���
	case STATE_DAMAGE:	// �_���[�W���
		break;

	case STATE_SPAWN:	// �X�|�[�����

		// �X�|�[����Ԃ̐ݒ�
		SetSpawn();
		break;

	case STATE_INVULN:	// ���G���

		// ���G��Ԃ̐ݒ�
		SetInvuln();
		break;

	case STATE_KNOCK:	// �m�b�N�o�b�N���

		// �m�b�N�o�b�N��Ԃ̐ݒ�
		SetKnock();
		break;

	case STATE_STAN:	// �X�^�����

		// �X�^����Ԃ̐ݒ�
		SetStan();
		break;

	case STATE_RIDE_FLYUP:	// ���C�h��яオ����

		// ���C�h��яオ���Ԃ̐ݒ�
		SetRideFlyUp();
		break;

	case STATE_RIDE_ROTATE:	// ���C�h������

		// ���C�h�����Ԃ̐ݒ�
		SetRideRotate();
		break;

	case STATE_RIDE_END:	// ���C�h�I�����

		// ���C�h�I����Ԃ̐ݒ�
		SetRideEnd();
		break;

	case STATE_DEATH:	// ���S���

		// ���S��Ԃ̐ݒ�
		SetDeath();
		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	��Ԏ擾����
//============================================================
int CEnemy::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	��ގ擾����
//============================================================
int CEnemy::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//============================================================
//	���a�擾����
//============================================================
float CEnemy::GetRadius(void) const
{
	// ���a��Ԃ�
	return m_status.fRadius;
}

//============================================================
//	�c���擾����
//============================================================
float CEnemy::GetHeight(void) const
{
	// �c����Ԃ�
	return m_status.fHeight;
}

//============================================================
//	�ʏ��Ԃ̏���������
//============================================================
void CEnemy::InitNormal(void)
{
	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �ړ��ʂ�������
	m_move = VEC3_ZERO;
}

//============================================================
//	�؂蔲���p�̕`�揈��
//============================================================
void CEnemy::DrawCrop(void)
{
	assert(false);
}

//============================================================
//	�q�b�g����
//============================================================
bool CEnemy::Hit(const int /*nDamage*/)
{
	assert(false);
	return false;
}

//============================================================
//	UI�`��̐ݒ菈��
//============================================================
void CEnemy::SetEnableDrawUI(const bool /*bDraw*/)
{

}

//============================================================
//	�̗̓Q�[�W�̗D�揇�ʂ̐ݒ菈��
//============================================================
void CEnemy::SetLifePriority(const int /*nPrio*/)
{
	assert(false);
}

//============================================================
//	���C�h�\���̎擾����
//============================================================
bool CEnemy::IsRideOK(const D3DXVECTOR3& /*rPos*/) const
{
	assert(false);
	return false;
}

//============================================================
//	��������
//============================================================
CEnemy *CEnemy::Create(const EType type, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// �|�C���^��錾
	CEnemy *pEnemy = nullptr;	// �G���

	// �G�̐���
	switch (type)
	{ // ��ނ��Ƃ̏���
	case TYPE_BOSS_DRAGON:

		// �{�X�h���S���𐶐�
		pEnemy = new CEnemyBossDragon(type);

		break;

	case TYPE_MINI_DRAGON:

		// �~�j�h���S���𐶐�
		pEnemy = new CEnemyMiniDragon(type);

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

		// �m�ۂ����A�h���X��Ԃ�
		return pEnemy;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CEnemy> *CEnemy::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	�X�e�[�^�X���擾����
//============================================================
CEnemy::SStatusInfo CEnemy::GetStatusInfo(const int nType)
{
	// �����̎�ނ̃X�e�[�^�X����Ԃ�
	return m_aStatusInfo[nType];
}

//============================================================
//	�Z�b�g�A�b�v�j������
//============================================================
void CEnemy::ReleaseSetup(void)
{
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{ // ��ނ̑������J��Ԃ�

		for (int nCntParts = 0; nCntParts < motion::MAX_PARTS; nCntParts++)
		{ // �p�[�c�̍ő吔���J��Ԃ�

			// �������j��
			SAFE_REF_RELEASE(m_aCollInfo[nCntType].apInfo[nCntParts]);
		}
	}
}

//============================================================
//	�ߋ��ʒu�̍X�V����
//============================================================
void CEnemy::UpdateOldPosition(void)
{
	// �ߋ��ʒu���X�V
	m_oldPos = GetVec3Position();
}

//============================================================
//	�����̍X�V����
//============================================================
void CEnemy::UpdateRotation(D3DXVECTOR3 *pRot, D3DXVECTOR3 *pDestRot)
{
	// �ϐ���錾
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����̐��K��
	useful::NormalizeRot(pDestRot->y);

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = pDestRot->y - pRot->y;

	// ���������̐��K��
	useful::NormalizeRot(fDiffRot);

	// ������]�ʂ����߂�
	float fMoveRot = fDiffRot * REV_ROTA;
	useful::LimitNum(fMoveRot, -MAX_ROTA, MAX_ROTA);	// ��]�ʂ�␳

	// �����̍X�V
	pRot->y += fMoveRot;

	// �����̐��K��
	useful::NormalizeRot(pRot->y);
}

//============================================================
//	�ߋ��ʒu�擾����
//============================================================
D3DXVECTOR3 CEnemy::GetOldPosition(void) const
{
	// �ߋ��ʒu��Ԃ�
	return m_oldPos;
}

//============================================================
//	�ڕW�����̐ݒ菈��
//============================================================
void CEnemy::SetDestRotation(const D3DXVECTOR3& rRot)
{
	// �ڕW������ݒ�
	m_destRot = rRot;
}

//============================================================
//	�ڕW�����擾����
//============================================================
D3DXVECTOR3 CEnemy::GetDestRotation(void) const
{
	// �ڕW������Ԃ�
	return m_destRot;
}

//============================================================
//	�ړ��ʂ̐ݒ菈��
//============================================================
void CEnemy::SetMovePosition(const D3DXVECTOR3 &rMove)
{
	// �����̈ړ��ʂ�ݒ�
	m_move = rMove;
}

//============================================================
//	�ړ��ʎ擾����
//============================================================
D3DXVECTOR3 CEnemy::GetMovePosition(void) const
{
	// �ړ��ʂ�Ԃ�
	return m_move;
}

//============================================================
//	�X�e�[�^�X���擾����
//============================================================
CEnemy::SStatusInfo CEnemy::GetStatusInfo(void) const
{
	// �X�e�[�^�X����Ԃ�
	return m_status;
}

//============================================================
//	�p�[�c���擾����
//============================================================
CEnemy::SPartsInfo CEnemy::GetPartsInfo(void) const
{
	// �p�[�c����Ԃ�
	return m_parts;
}

//============================================================
//	���[�V�������擾����
//============================================================
CMotion::SInfo CEnemy::GetMotionInfo(void) const
{
	// ���[�V��������Ԃ�
	return m_motion;
}

//============================================================
//	�X�|�[����Ԃ̐ݒ菈��
//============================================================
void CEnemy::SetSpawn(void)
{
	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �ړ��ʂ�������
	m_move = VEC3_ZERO;

	// �`����ĊJ
	SetEnableDraw(true);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �����x�𓧖��ɍĐݒ�
	SetAlpha(0.0f);
}

//============================================================
//	���G��Ԃ̐ݒ菈��
//============================================================
void CEnemy::SetInvuln(void)
{
	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �`����ĊJ
	SetEnableDraw(true);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �����x��s�����ɍĐݒ�
	SetAlpha(1.0f);
}

//============================================================
//	�m�b�N�o�b�N��Ԃ̐ݒ菈��
//============================================================
void CEnemy::SetKnock(void)
{
	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �`����ĊJ
	SetEnableDraw(true);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �����x��s�����ɍĐݒ�
	SetAlpha(1.0f);
}

//============================================================
//	�X�^����Ԃ̐ݒ菈��
//============================================================
void CEnemy::SetStan(void)
{
	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// ����������������
	m_fSinAlpha = 0.0f;

	// �`����ĊJ
	SetEnableDraw(true);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// ���G�̊�b�����x��ݒ�
	SetAlpha(STAN_ALPHA);
}

//============================================================
//	���C�h��яオ���Ԃ̐ݒ菈��
//============================================================
void CEnemy::SetRideFlyUp(void)
{
	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �`����ĊJ
	SetEnableDraw(true);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �����x���Đݒ�
	SetAlpha(1.0f);
}

//============================================================
//	���C�h�����Ԃ̐ݒ菈��
//============================================================
void CEnemy::SetRideRotate(void)
{
	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �`����ĊJ
	SetEnableDraw(true);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �����x���Đݒ�
	SetAlpha(1.0f);
}

//============================================================
//	���C�h�I����Ԃ̐ݒ菈��
//============================================================
void CEnemy::SetRideEnd(void)
{
	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �`����ĊJ
	SetEnableDraw(true);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �����x���Đݒ�
	SetAlpha(1.0f);
}

//============================================================
//	���S��Ԃ̐ݒ菈��
//============================================================
void CEnemy::SetDeath(void)
{

}

//============================================================
//	�Ȃɂ����Ȃ���Ԏ��̍X�V����
//============================================================
void CEnemy::UpdateNone(void)
{

}

//============================================================
//	�X�|�[����Ԏ��̍X�V����
//============================================================
void CEnemy::UpdateSpawn(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetVec3Position();	// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// �G����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	if (pStage == nullptr) { assert(false); return; }	// �X�e�[�W��g�p��

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(&posEnemy);

	// ���n����
	UpdateLanding(&posEnemy);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posEnemy, m_status.fRadius);

	// �ʒu�𔽉f
	SetVec3Position(posEnemy);

	// �����𔽉f
	SetVec3Rotation(rotEnemy);

	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	if (UpdateFadeOut(ADD_ALPHA))
	{ // �s�����ɂȂ�؂����ꍇ

		// �ʏ��Ԃɂ���
		SetState(STATE_NORMAL);
	}
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
void CEnemy::UpdateNormal(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetVec3Position();	// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetVec3Rotation();	// �G����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	if (pStage == nullptr) { assert(false); return; }	// �X�e�[�W��g�p��

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(&posEnemy);

	// ���n����
	UpdateLanding(&posEnemy);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posEnemy, m_status.fRadius);

	// �ʒu�𔽉f
	SetVec3Position(posEnemy);

	// �����𔽉f
	SetVec3Rotation(rotEnemy);
}

//============================================================
//	�_���[�W��Ԏ��̍X�V����
//============================================================
void CEnemy::UpdateDamage(void)
{
	// �g�̂̐F��Ԃ�����
	SetAllMaterial(material::DamageRed());

	// �_���[�W���󂯂�O�̏�ԍX�V
	assert(m_prevState > NONE_IDX && m_prevState < STATE_MAX);
	(this->*(m_aFuncUpdateState[m_prevState]))();

	// �J�E���^�[�����Z
	m_nCounterState++;
	if (m_nCounterState > DAMAGE_FRAME)
	{ // �_���[�W��Ԃ̏I���t���[���ɂȂ����ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �g�̂̐F�����ɖ߂�
		ResetMaterial();

		// ���G��Ԃɂ���
		SetState(STATE_INVULN);
	}
}

//============================================================
//	���G��Ԏ��̍X�V����
//============================================================
void CEnemy::UpdateInvuln(void)
{
	// �_���[�W���󂯂�O�̏�ԍX�V
	assert(m_prevState > NONE_IDX && m_prevState < STATE_MAX);
	(this->*(m_aFuncUpdateState[m_prevState]))();

	// �J�E���^�[�����Z
	m_nCounterState++;
	if (m_nCounterState > INVULN_FRAME)
	{ // ���G��Ԃ̏I���t���[���ɂȂ����ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �s�����ɂ���
		SetAlpha(1.0f);

		// �_���[�W���󂯂�O�̏�Ԃ�ݒ�
		assert(m_prevState != STATE_NONE);
		SetState(m_prevState);
	}
}

//============================================================
//	�m�b�N�o�b�N��Ԏ��̍X�V����
//============================================================
void CEnemy::UpdateKnock(void)
{

}

//============================================================
//	�X�^����Ԏ��̍X�V����
//============================================================
void CEnemy::UpdateStan(void)
{
	float fAddAlpha = 0.0f;	// �����x�̉��Z��

	// �T�C���J�[�u��������]
	m_fSinAlpha += ADD_SINROT;
	useful::NormalizeRot(m_fSinAlpha);	// �������K��

	// �����x���Z�ʂ����߂�
	fAddAlpha = (MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinAlpha) - 1.0f);

	// �����x��ݒ�
	SetAlpha(STAN_ALPHA + fAddAlpha);

	// �J�E���^�[�����Z
	m_nCounterState++;
	if (m_nCounterState > STAN_FRAME)
	{ // �X�^����Ԃ̏I���t���[���ɂȂ����ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �s�����ɂ���
		SetAlpha(1.0f);

		// �ʏ��Ԃ�ݒ�
		SetState(STATE_NORMAL);
	}

	D3DXVECTOR3 posEnemy = GetVec3Position();			// �G�ʒu
	CStage *pStage = CScene::GetStage();				// �X�e�[�W���
	if (pStage == nullptr) { assert(false); return; }	// �X�e�[�W��g�p��

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(&posEnemy);

	// ���n����
	UpdateLanding(&posEnemy);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posEnemy, m_status.fRadius);

	// �ʒu�𔽉f
	SetVec3Position(posEnemy);
}

//============================================================
//	���C�h��яオ���Ԏ��̍X�V����
//============================================================
void CEnemy::UpdateRideFlyUp(void)
{

}

//============================================================
//	���C�h�����Ԏ��̍X�V����
//============================================================
void CEnemy::UpdateRideRotate(void)
{

}

//============================================================
//	���C�h�I����Ԏ��̍X�V����
//============================================================
void CEnemy::UpdateRideEnd(void)
{

}

//============================================================
//	���S��Ԏ��̍X�V����
//============================================================
void CEnemy::UpdateDeath(void)
{
	// ���g�̏I��
	Uninit();
}

//============================================================
//	�Ώۂ̎��F����
//============================================================
void CEnemy::UpdateLook(const D3DXVECTOR3& rPosLook, const D3DXVECTOR3& rPosEnemy, D3DXVECTOR3 *pRotEnemy)
{
	// �ϐ���錾
	float fDestRot = 0.0f;	// �ڕW����
	float fDiffRot = 0.0f;	// ����

	// ���F�Ώۂ̕�������
	fDestRot = atan2f(rPosEnemy.x - rPosLook.x, rPosEnemy.z - rPosLook.z);	// �ڕW����

	// �������������߂�
	fDiffRot = fDestRot - pRotEnemy->y;
	useful::NormalizeRot(fDiffRot);		// ���������̐��K��

	// �����̍X�V
	pRotEnemy->y += fDiffRot * m_status.fLookRev;
	useful::NormalizeRot(pRotEnemy->y);	// �����̐��K��
}

//============================================================
//	�d�͂̍X�V����
//============================================================
void CEnemy::UpdateGravity(void)
{
	// �d�͂����Z
	m_move.y -= GRAVITY;
}

//============================================================
//	���n�󋵂̍X�V����
//============================================================
bool CEnemy::UpdateLanding(D3DXVECTOR3 *pPos)
{
	// �ϐ���錾
	bool bLand = false;	// ���n��

	// �W�����v���Ă����Ԃɂ���
	m_bJump = true;

	// �n�ʁE�����ʒu�̒��n����
	if (CScene::GetStage()->LandFieldPosition(*pPos, m_move)
	||  CScene::GetStage()->LandLimitPosition(*pPos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

		// ���n���Ă����Ԃɂ���
		bLand = true;

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}

	// ���n�󋵂�Ԃ�
	return bLand;
}

//============================================================
//	�ʒu�̍X�V����
//============================================================
void CEnemy::UpdatePosition(D3DXVECTOR3 *pPos)
{
	// �ʒu���ړ�
	*pPos += m_move;

	// �ړ��ʂ�����
	if (m_bJump)
	{ // �󒆂̏ꍇ

		m_move.x += (0.0f - m_move.x) * JUMP_REV;
		m_move.z += (0.0f - m_move.z) * JUMP_REV;
	}
	else
	{ // �n��̏ꍇ

		m_move.x += (0.0f - m_move.x) * LAND_REV;
		m_move.z += (0.0f - m_move.z) * LAND_REV;
	}
}

//============================================================
//	�t�F�[�h�A�E�g��Ԏ��̍X�V����
//============================================================
bool CEnemy::UpdateFadeOut(const float fAdd)
{
	// �ϐ���錾
	bool bAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �v���C���[���g�̕`����ĊJ
	CObject::SetEnableDraw(true);

	// �����x���グ��
	fAlpha += fAdd;

	if (fAlpha >= GetMaxAlpha())
	{ // �����x���オ��؂����ꍇ

		// �����x��␳
		fAlpha = GetMaxAlpha();

		// �s�����ɂȂ�؂�����Ԃɂ���
		bAlpha = true;
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bAlpha;
}

//============================================================
//	�t�F�[�h�C����Ԏ��̍X�V����
//============================================================
bool CEnemy::UpdateFadeIn(const float fSub)
{
	// �ϐ���錾
	bool bAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �����x��������
	fAlpha -= fSub;

	if (fAlpha <= 0.0f)
	{ // �����x��������؂����ꍇ

		// �����x��␳
		fAlpha = 0.0f;

		// �����ɂȂ�؂�����Ԃɂ���
		bAlpha = true;

		// �v���C���[���g�̕`����~
		CObject::SetEnableDraw(false);
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bAlpha;
}

//============================================================
//	�S�Z�b�g�A�b�v����
//============================================================
void CEnemy::LoadAllSetup(void)
{
	for (int nCntEnemy = 0; nCntEnemy < TYPE_MAX; nCntEnemy++)
	{ // �G�̎�ޕ��J��Ԃ�

		// �Z�b�g�A�b�v
		CEnemy::LoadSetup((EType)nCntEnemy);
	}
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CEnemy::LoadSetup(const EType typeID)
{
	// �ϐ���錾
	std::vector<CCollSphere::SInfo> vector;	// �~����z��
	CMotion::SMotionInfo infoKey;	// �L�[�̑���p
	CCollSphere::SInfo infoColl;	// ������
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	int nID			= 0;			// �C���f�b�N�X�̑���p
	int nParts		= 0;			// �p�[�c�ԍ�
	int nNowPose	= 0;			// ���݂̃|�[�Y�ԍ�
	int nNowKey		= 0;			// ���݂̃L�[�ԍ�
	int nWeapon		= 0;			// ����\����ON/OFF�̕ϊ��p
	int nLoop		= 0;			// ���[�v��ON/OFF�̕ϊ��p
	int nEnd		= 0;			// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �����̐ÓI�����o�ϐ��̏����N���A
	memset(&m_aMotionInfo[typeID],	0, sizeof(m_aMotionInfo[typeID]));	// ���[�V�������
	memset(&m_aPartsInfo[typeID],	0, sizeof(m_aPartsInfo[typeID]));	// �p�[�c���
	memset(&m_aStatusInfo[typeID],	0, sizeof(m_aStatusInfo[typeID]));	// �X�e�[�^�X���
	memset(&m_aCollInfo[typeID],	0, sizeof(m_aCollInfo[typeID]));	// �����蔻����

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SETUP_TXT[typeID], "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �X�e�[�^�X�̐ݒ�
			if (strcmp(&aString[0], "STATUSSET") == 0)
			{ // �ǂݍ��񂾕����� STATUSSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LIFE") == 0)
					{ // �ǂݍ��񂾕����� LIFE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_aStatusInfo[typeID].nMaxLife);	// �ő�̗͂�ǂݍ���
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &m_aStatusInfo[typeID].fRadius);	// ���a��ǂݍ���
					}
					else if (strcmp(&aString[0], "HEIGHT") == 0)
					{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &m_aStatusInfo[typeID].fHeight);	// �c����ǂݍ���
					}
					else if (strcmp(&aString[0], "LOOK_REV") == 0)
					{ // �ǂݍ��񂾕����� LOOK_REV �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &m_aStatusInfo[typeID].fLookRev);	// ���F�̕␳�W����ǂݍ���
					}
					else if (strcmp(&aString[0], "COLL_RADIUS") == 0)
					{ // �ǂݍ��񂾕����� COLL_RADIUS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &m_aStatusInfo[typeID].fCollRadius);	// �����蔻��̔��a��ǂݍ���
					}
				} while (strcmp(&aString[0], "END_STATUSSET") != 0);	// �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v
			}

			// �L�����N�^�[�̐ݒ�
			else if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // �ǂݍ��񂾕����� CHARACTERSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // �ǂݍ��񂾕����� PARTSSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // �ǂݍ��񂾕����� INDEX �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nID);			// ���f���̃C���f�b�N�X��ǂݍ���

								// �p�[�c�������Z
								m_aPartsInfo[typeID].nNumParts++;
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // �ǂݍ��񂾕����� PARENT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aPartsInfo[typeID].aInfo[nID].nParentID);	// ���f���̐e�̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aPartsInfo[typeID].aInfo[nID].pos.x);	// X���W��ǂݍ���
								fscanf(pFile, "%f", &m_aPartsInfo[typeID].aInfo[nID].pos.y);	// Y���W��ǂݍ���
								fscanf(pFile, "%f", &m_aPartsInfo[typeID].aInfo[nID].pos.z);	// Z���W��ǂݍ���
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // �ǂݍ��񂾕����� ROT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aPartsInfo[typeID].aInfo[nID].rot.x);	// X������ǂݍ���
								fscanf(pFile, "%f", &m_aPartsInfo[typeID].aInfo[nID].rot.y);	// Y������ǂݍ���
								fscanf(pFile, "%f", &m_aPartsInfo[typeID].aInfo[nID].rot.z);	// Z������ǂݍ���
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v
			}

			// �����蔻��̐ݒ�
			else if (strcmp(&aString[0], "COLLISIONSET") == 0)
			{ // �ǂݍ��񂾕����� COLLISIONSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_COLLISIONSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "COLLSET") == 0)
					{ // �ǂݍ��񂾕����� COLLSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_COLLSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "PARTS") == 0)
							{ // �ǂݍ��񂾕����� PARTS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nParts);		// �p�[�c�ԍ���ǂݍ���

								// �~�������ݒ�
								m_aCollInfo->apInfo[nParts] = CCollSphere::Create(nullptr);

								// �~����z���ݒ�
								vector = m_aCollInfo->apInfo[nParts]->GetVector();
							}
							else if (strcmp(&aString[0], "COLL") == 0)
							{ // �ǂݍ��񂾕����� COLL �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_COLL �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "OFFSET") == 0)
									{ // �ǂݍ��񂾕����� OFFSET �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &infoColl.offset.x);	// X�I�t�Z�b�g��ǂݍ���
										fscanf(pFile, "%f", &infoColl.offset.y);	// Y�I�t�Z�b�g��ǂݍ���
										fscanf(pFile, "%f", &infoColl.offset.z);	// Z�I�t�Z�b�g��ǂݍ���
									}
									else if (strcmp(&aString[0], "RADIUS") == 0)
									{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &infoColl.fRadius);	// ���a��ǂݍ���
									}
								} while (strcmp(&aString[0], "END_COLL") != 0);	// �ǂݍ��񂾕����� END_COLL �ł͂Ȃ��ꍇ���[�v

								// �������z��ɒǉ�
								vector.push_back(infoColl);
							}
						} while (strcmp(&aString[0], "END_COLLSET") != 0);	// �ǂݍ��񂾕����� END_COLLSET �ł͂Ȃ��ꍇ���[�v

						// ������̔z���ݒ�
						m_aCollInfo->apInfo[nParts]->SetVector(vector);
					}
				} while (strcmp(&aString[0], "END_COLLISIONSET") != 0);		// �ǂݍ��񂾕����� END_COLLISIONSET �ł͂Ȃ��ꍇ���[�v
			}

			// ���[�V�����̐ݒ�
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // �ǂݍ��񂾕����� MOTIONSET �̏ꍇ

				// ���݂̃|�[�Y�ԍ���������
				nNowPose = 0;

				// �|�[�Y����p�̕ϐ���������
				memset(&infoKey, 0, sizeof(infoKey));

				// �L�����Z���E�R���{�t���[�����Ȃ��ɂ���
				infoKey.nCancelFrame = NONE_IDX;
				infoKey.nComboFrame  = NONE_IDX;

				// �U���������������
				infoKey.collLeft.nMin  = NONE_IDX;
				infoKey.collLeft.nMax  = NONE_IDX;
				infoKey.collRight.nMin = NONE_IDX;
				infoKey.collRight.nMax = NONE_IDX;

				// ����\����OFF�ɂ���
				infoKey.bWeaponDisp = false;

				do
				{ // �ǂݍ��񂾕����� END_MOTION �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "WEAPON") == 0)
					{ // �ǂݍ��񂾕����� WEAPON �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nWeapon);		// ����\����ON/OFF��ǂݍ���

						// �ǂݍ��񂾒l��bool�^�ɕϊ�
						infoKey.bWeaponDisp = (nWeapon == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "LOOP") == 0)
					{ // �ǂݍ��񂾕����� LOOP �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nLoop);		// ���[�v��ON/OFF��ǂݍ���

						// �ǂݍ��񂾒l��bool�^�ɕϊ�
						infoKey.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // �ǂݍ��񂾕����� NUM_KEY �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &infoKey.nNumKey);	// �L�[�̑�����ǂݍ���
					}
					else if (strcmp(&aString[0], "CANCEL") == 0)
					{ // �ǂݍ��񂾕����� CANCEL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &infoKey.nCancelFrame);	// �L�����Z���\�t���[����ǂݍ���
					}
					else if (strcmp(&aString[0], "COMBO") == 0)
					{ // �ǂݍ��񂾕����� COMBO �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &infoKey.nComboFrame);	// �R���{�\�t���[����ǂݍ���
					}
					else if (strcmp(&aString[0], "LEFT_COLL") == 0)
					{ // �ǂݍ��񂾕����� LEFT_COLL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &infoKey.collLeft.nMin);	// ������o���J�n�t���[����ǂݍ���
						fscanf(pFile, "%d", &infoKey.collLeft.nMax);	// ����������I���t���[����ǂݍ���
					}
					else if (strcmp(&aString[0], "RIGHT_COLL") == 0)
					{ // �ǂݍ��񂾕����� RIGHT_COLL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &infoKey.collRight.nMin);	// ������o���J�n�t���[����ǂݍ���
						fscanf(pFile, "%d", &infoKey.collRight.nMax);	// ����������I���t���[����ǂݍ���
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // �ǂݍ��񂾕����� KEYSET �̏ꍇ

						// ���݂̃L�[�ԍ���������
						nNowKey = 0;

						do
						{ // �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // �ǂݍ��񂾕����� FRAME �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &infoKey.aKeyInfo[nNowPose].nFrame);	// �L�[���؂�ւ��܂ł̃t���[������ǂݍ���
							}
							else if (strcmp(&aString[0], "MOVE") == 0)
							{ // �ǂݍ��񂾕����� MOVE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &infoKey.aKeyInfo[nNowPose].move.x);	// �L�[���؂�ւ��܂ł̈ړ��ʂ�ǂݍ���
								fscanf(pFile, "%f", &infoKey.aKeyInfo[nNowPose].move.y);	// �L�[���؂�ւ��܂ł̈ړ��ʂ�ǂݍ���
								fscanf(pFile, "%f", &infoKey.aKeyInfo[nNowPose].move.z);	// �L�[���؂�ւ��܂ł̈ړ��ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // �ǂݍ��񂾕����� KEY �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // �ǂݍ��񂾕����� POS �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);										// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &infoKey.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X�ʒu��ǂݍ���
										fscanf(pFile, "%f", &infoKey.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y�ʒu��ǂݍ���
										fscanf(pFile, "%f", &infoKey.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z�ʒu��ǂݍ���

										// �ǂݍ��񂾈ʒu�Ƀp�[�c�̏����ʒu�����Z
										infoKey.aKeyInfo[nNowPose].aKey[nNowKey].pos += m_aPartsInfo[typeID].aInfo[nNowKey].pos;
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // �ǂݍ��񂾕����� ROT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);										// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &infoKey.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X������ǂݍ���
										fscanf(pFile, "%f", &infoKey.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y������ǂݍ���
										fscanf(pFile, "%f", &infoKey.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z������ǂݍ���

										// �ǂݍ��񂾌����Ƀp�[�c�̏������������Z
										infoKey.aKeyInfo[nNowPose].aKey[nNowKey].rot += m_aPartsInfo[typeID].aInfo[nNowKey].rot;

										// ���������𐳋K��
										useful::NormalizeRot(infoKey.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);
										useful::NormalizeRot(infoKey.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);
										useful::NormalizeRot(infoKey.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

								// ���݂̃L�[�ԍ������Z
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

						// ���݂̃|�[�Y�ԍ������Z
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

				// ���[�V�������̐ݒ�
				m_aMotionInfo[typeID].aMotionInfo[m_aMotionInfo[typeID].nNumType] = infoKey;

				// ���[�V�����������Z
				m_aMotionInfo[typeID].nNumType++;
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�G�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}