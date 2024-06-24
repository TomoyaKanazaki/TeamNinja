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
#include "multiModel.h"
#include "enemy_item.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\enemy.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const float MOVE = -300.0f;								// �ړ���
	const float ROT_REV = 0.5f;								// �����̕␳�W��

	const D3DXVECTOR3 OFFSET = D3DXVECTOR3(-3.0f, -15.0f, 0.0f);	// �I�t�Z�b�g���W
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
	// �G�̍X�V
	CEnemy::Update(fDeltaTime);

	if (GetItem() != nullptr)
	{ // �A�C�e���������Ă���ꍇ

		// �A�C�e���̃I�t�Z�b�g����
		GetItem()->Offset(GetParts(8)->GetMtxWorld());
	}
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
// ���̐ݒ菈��
//============================================================
void CEnemyCaveat::SetData(void)
{
	// �A�C�e����ݒ肷��
	SetItem(CEnemyItem::Create
	(
		CEnemyItem::TYPE_KATANA,
		OFFSET,
		GetParts(8)->GetMtxWorld()
	));
}

//============================================================
// ��Ԃ̍X�V����
//============================================================
int CEnemyCaveat::UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// ���݂̃��[�V����
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

	// ���݂̃��[�V������Ԃ�
	return nCurMotion;
}

//============================================================
// ���[�V�����̍X�V����
//============================================================
void CEnemyCaveat::UpdateMotion(int nMotion, const float fDeltaTime)
{

}

//============================================================
// ���n�̍X�V����
//============================================================
void CEnemyCaveat::UpdateLanding(D3DXVECTOR3* pPos)
{
	// �e�N���X�̒��n�X�V
	CEnemy::UpdateLanding(pPos);

	// ���݂̃��[�V������ނ��擾
	int nCurMotion = GetMotionType();

	// �������[�V�����̃t���O��ݒ�
	bool bTypeFall = nCurMotion == MOTION_FALL;

	if (!IsJump())
	{ // �󒆂ɂ��Ȃ��ꍇ

		if (bTypeFall)
		{ // ���[�V�������������̏ꍇ

			// ���n���[�V�������w��
			SetMotion(MOTION_LANDING);
		}
	}
	else
	{ // �󒆂ɂ���ꍇ

		// �������[�V�������w��
		SetMotion(MOTION_FALL);
	}
}

//============================================================
// �x������
//============================================================
void CEnemyCaveat::Caveat(void)
{

}
