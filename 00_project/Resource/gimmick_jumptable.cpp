//============================================================
//
//	�W�����v�䏈�� [gimmick_jumptable.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "gimmick_jumptable.h"
#include "player.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const float DISTANCE_CENTER = 50.0f; // �ҋ@���S�Ƃ̋���
}

//************************************************************
//	�q�N���X [CGimmickJumpTable] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGimmickJumpTable::CGimmickJumpTable() : CGimmickAction()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGimmickJumpTable::~CGimmickJumpTable()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGimmickJumpTable::Init(void)
{
	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CGimmickAction::Init()))
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
void CGimmickJumpTable::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I��
	CGimmickAction::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGimmickJumpTable::Update(const float fDeltaTime)
{
	// �v���C���[�Ƃ̓����蔻��
	if (DistancePlayer())
	{
		// �v���C���[���W�����v�I
		SetMoment(GET_PLAYER->GimmickHighJump());
	}
	else
	{
		// �W�����v���Ă��Ȃ��ꍇfalse
		SetMoment(false);
	}

	// �e�N���X�̍X�V
	CGimmickAction::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CGimmickJumpTable::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CGimmickAction::Draw(pShader);
}

//===========================================
//  �e���g���̑ҋ@�ʒu���Z�o
//===========================================
D3DXVECTOR3 CGimmickJumpTable::CalcWaitPoint(const int Idx) const
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }

	// �v���C���[�̈ʒu���擾
	D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();

	// �ҋ@�ʒu���擾
	D3DXVECTOR3 posThis = GetActionPoint();

	// �ڕW�����Ƃ̍��������߂�
	D3DXVECTOR3 vecTarget = posPlayer - posThis;

	// �����x�N�g���̌��������߂�
	float fRot = atan2f(vecTarget.x, -vecTarget.z) + (((D3DX_PI * 2.0f) / GetNumActive()) * Idx);
	useful::NormalizeRot(fRot);

	// �����x�N�g�������ɌX���č��W��ݒ�
	D3DXVECTOR3 posWait = D3DXVECTOR3
		(
			posThis.x + cosf(fRot) * DISTANCE_CENTER,
			posThis.y,
			posThis.z + sinf(fRot) * DISTANCE_CENTER
		);

	// �Z�o�������W��Ԃ�
	return posWait;
}
