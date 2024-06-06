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

#include "enemyStalk.h"
#include "stage.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY = 3;		// �G�̗D�揇��
	const float	GRAVITY = 1.0f;		// �d��
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
	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	//CObjectChara::Update(fDeltaTime);

	// �d�͏���
	Gravity();

	// ���n����
	Landing();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemy::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw(pShader);
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
// �d�͏���
//============================================================
void CEnemy::Gravity(void)
{
	// �d�͂����Z����
	m_move.y -= GRAVITY;
}

//============================================================
// ���n����
//============================================================
void CEnemy::Landing(void)
{
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu
	CStage* pStage = CScene::GetStage();	// �X�e�[�W���

	// �W�����v���Ă����Ԃɂ���
	m_bJump = true;

	// �n�ʁE�����ʒu�̒��n����
	if (pStage->LandFieldPosition(pos, m_move)
		|| pStage->LandLimitPosition(pos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}

	// �ʒu��K�p
	SetVec3Position(pos);
}
