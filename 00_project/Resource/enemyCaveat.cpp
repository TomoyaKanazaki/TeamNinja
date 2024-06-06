//============================================================
//
//	�x���G���� [enemy_Caveat.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "enemyCaveat.h"
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
	const float MOVE = -300.0f;								// �ړ���
	const float ROT_REV = 0.5f;								// �����̕␳�W��
}

//************************************************************
//	�q�N���X [CEnemyCaveat] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyCaveat::CEnemyCaveat() : CEnemy(),
m_state(STATE_CRAWL)		// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyCaveat::~CEnemyCaveat()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyCaveat::Init(void)
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
void CEnemyCaveat::Uninit(void)
{
	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyCaveat::Update(const float fDeltaTime)
{
	// �ߋ��ʒu�X�V
	UpdateOldPosition();

	// ��ԏ���
	State();

	// �G�̍X�V
	CEnemy::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyCaveat::Draw(CShader* pShader)
{
	// �G�̕`��
	CEnemy::Draw(pShader);
}

//============================================================
// ��ԏ���
//============================================================
void CEnemyCaveat::State(void)
{
	switch (m_state)
	{
	case CEnemyCaveat::STATE_CRAWL:

		if (SearchPlayer() ||
			SearchClone())
		{ // ���E���Ƀv���C���[�����g���������ꍇ

			// �x����Ԃɂ���
			m_state = STATE_CAVEAT;
		}

		break;

	case CEnemyCaveat::STATE_CAVEAT:

		// �x������
		Caveat();

		break;

	default:		// ��O����
		assert(false);
		break;
	}
}

//============================================================
// �x������
//============================================================
void CEnemyCaveat::Caveat(void)
{
	D3DXVECTOR3 pos = GetVec3Position();
	D3DXVECTOR3 rot = GetVec3Rotation();

	pos.x += sinf(rot.y) * 100.0f;
	pos.y += cosf(rot.y) * 100.0f;
	pos.z += cosf(rot.y) * 100.0f;

	rot.y += 0.01f;

	useful::NormalizeRot(rot.y);

	SetVec3Position(pos);
	SetVec3Rotation(rot);
}
