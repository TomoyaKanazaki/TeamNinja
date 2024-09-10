//============================================================
//
//	�^�C�g����ʃv���C���[���� [playerTitle.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "playerTitle.h"
#include "player_clone.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	NUM_CLONE	= 2;		// �������镪�g��
	const float	MOVE_VALUE	= 500.0f;	// �ړ��X�J���[
	const float	OFFSET_SIDE	= 100.0f;	// ���g�������I�t�Z�b�g
	const float	OFFSET_BACK	= 100.0f;	// ���g�������I�t�Z�b�g
}

//************************************************************
//	�q�N���X [CPlayerTitle] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerTitle::CPlayerTitle()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayerTitle::~CPlayerTitle()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayerTitle::Init(void)
{
	// �v���C���[�̏�����
	if (FAILED(CPlayer::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�|�[���̐ݒ�
	SetSpawn();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayerTitle::Uninit(void)
{
	// �v���C���[�̏I��
	CPlayer::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayerTitle::Update(const float fDeltaTime)
{
	// �v���C���[�̍X�V
	CPlayer::Update(fDeltaTime);
}

//============================================================
//	��Ԃ̍X�V����
//============================================================
CPlayer::EMotion CPlayerTitle::UpdateState(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����
	switch (GetState())
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:

		// �������Ȃ���Ԏ��̍X�V
		currentMotion = UpdateNone(fDeltaTime);
		break;

	case STATE_TITLE_IDOL:

		// �m��������Ԏ��̍X�V
		currentMotion = UpdateIdol(fDeltaTime);
		break;

	case STATE_TITLE_MOVE:

		// �ړ���Ԏ��̍X�V
		currentMotion = UpdateMove(fDeltaTime);
		break;

	default:
		assert(false);
		break;
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	�m��������Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayerTitle::UpdateIdol(const float fDeltaTime)
{
	// �m���������[�V������Ԃ�
	return MOTION_STAND;
}

//============================================================
//	�ړ���Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayerTitle::UpdateMove(const float fDeltaTime)
{
	// �ʒu���ړ��ʕ��ړ�������
	SetVec3Position(GetVec3Position() + GetMove() * fDeltaTime);

	// �ړ����[�V������Ԃ�
	return MOTION_DASH;
}

//============================================================
//	�X�|�[���̐ݒ菈��
//============================================================
void CPlayerTitle::SetSpawn(void)
{
	// ����������
	SetState(STATE_TITLE_IDOL);	// �m��������Ԃ̐ݒ�
	SetMotion(MOTION_STAND);	// �m���������[�V������ݒ�

	// �ړ��ʂ�������
	SetMove(VEC3_ZERO);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �����x��s�����ɐݒ�
	SetAlpha(1.0f);

	// �`����ĊJ
	SetEnableDraw(true);
}

//============================================================
//	���o�J�n�̐ݒ菈��
//============================================================
void CPlayerTitle::SetStart(void)
{
	const D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	const D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����
	const D3DXVECTOR3 moveChara = D3DXVECTOR3(sinf(rotPlayer.y), 0.0f, cosf(rotPlayer.y)) * -MOVE_VALUE;	// �L�����ړ���

	// ����������
	SetState(STATE_TITLE_MOVE);	// �ړ���Ԃ̐ݒ�
	SetMotion(MOTION_DASH);		// �ړ����[�V������ݒ�

	for (int i = 0; i < NUM_CLONE; i++)
	{ // �������镪�g�����J��Ԃ�

		D3DXVECTOR3 posClone = posPlayer;			// ���g�����ʒu
		float fRotSide = rotPlayer.y + HALF_PI;		// �v���C���[����������
		float fSide = (1.0f - ((i % 2) * 2.0f));	// ���������W��

		// �����ʒu�����炷
		posClone += D3DXVECTOR3(sinf(fRotSide), 0.0f, cosf(fRotSide)) * OFFSET_SIDE * fSide;	// ���ɂ��炷
		posClone += D3DXVECTOR3(sinf(rotPlayer.y), 0.0f, cosf(rotPlayer.y)) * OFFSET_BACK;		// ���ɂ��炷

		// ���g�̐���
		CPlayerClone::Create(posClone, moveChara);
	}

	// �ړ��ʂ̐ݒ�
	SetMove(moveChara);
}
