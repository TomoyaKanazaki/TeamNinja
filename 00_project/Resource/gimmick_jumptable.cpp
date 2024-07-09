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
#include "player_clone.h"

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
	if (HitPlayer())
	{
		// �v���C���[���W�����v�I
		SetMoment(GET_PLAYER->GimmickHighJump(GetNumActive()));
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
D3DXVECTOR3 CGimmickJumpTable::CalcWaitPoint(const int Idx, const CPlayerClone* pClone)
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }

	// TODO�F�����ɑł��グ���[�V�������Ȃ甲���鏈���L�q
	//if ()

	D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 posAction = GetActionPoint();				// ��������ʒu

	// �ڕW�����Ƃ̍��������߂�
	D3DXVECTOR3 vecTarget = posPlayer - posAction;

	// �����x�N�g���̌��������߂�
	float fRot = atan2f(vecTarget.x, -vecTarget.z) + (((D3DX_PI * 2.0f) / GetNumActive()) * Idx);
	useful::NormalizeRot(fRot);

	// �����x�N�g�������ɌX���č��W��ݒ�
	D3DXVECTOR3 posWait = D3DXVECTOR3
	(
		posAction.x + cosf(fRot) * DISTANCE_CENTER,
		posAction.y,
		posAction.z + sinf(fRot) * DISTANCE_CENTER
	);

	// �Z�o�������W��Ԃ�
	return posWait;
}

//===========================================
//  �e���g���̑ҋ@�������Z�o
//===========================================
D3DXVECTOR3 CGimmickJumpTable::CalcWaitRotation(const int Idx, const CPlayerClone* pClone)
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }

	// �ҋ@���S�Ƃ̍��������߂�
	D3DXVECTOR3 vecCenter = GetActionPoint() - pClone->GetVec3Position();

	// ���������߂�
	D3DXVECTOR3 rot = VEC3_ZERO;
	rot.y = -atan2f(vecCenter.x, -vecCenter.z);

	// �Z�o����������Ԃ�
	return rot;
}

//===========================================
//  �v���C���[�Ƃ̔���
//===========================================
bool CGimmickJumpTable::HitPlayer()
{
	// �v���C���[��y���W���擾
	float fPosYPlayer = GET_PLAYER->GetVec3Position().y;

	// ���g�̐g�����擾
	float fHeightClone = CPlayerClone::GetHeight();

	// ���g��y���W���擾
	float fPosYThis = GetVec3Position().y;

	// ���g�̍��� + ���g�̐g�� ���v���C���[���Ⴂ�ʒu�̏ꍇfalse��Ԃ�
	if (fPosYThis + fHeightClone < fPosYPlayer) { return false; }
	
	return DistancePlayer();
}
