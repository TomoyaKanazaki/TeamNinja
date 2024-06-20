//============================================================
//
// ��э~�� [gimmick_jumpoff.cpp]
// Author�F����������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gimmick_jumpoff.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"

//============================================================
//	�R���X�g���N�^
//============================================================
CGimmickJumpOff::CGimmickJumpOff() : CGimmickAction()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGimmickJumpOff::~CGimmickJumpOff()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGimmickJumpOff::Init(void)
{
	// �M�~�b�N�A�N�V�����̏�����
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
void CGimmickJumpOff::Uninit(void)
{
	// �M�~�b�N�A�N�V�����̏I��
	CGimmickAction::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGimmickJumpOff::Update(const float fDeltaTime)
{
	if (GET_PLAYER->IsJump())
	{ // �v���C���[���W�����v���Ă���ꍇ

		// �v���C���[�Ƃ̓����蔻��
		if (CollisionPlayer())
		{
			if (GetMoment() == false)
			{ // �M�~�b�N�������ĂȂ��ꍇ

				D3DXVECTOR3 pos = GetVec3Position();		// �����ʒu

				if ((GET_PLAYER->GetVec3Position().y - (pos.y + CPlayerClone::GetHeight())) <= 10.0f)
				{ // ����̍����܂ŉ��������ꍇ

					// ������
					SetMoment(GET_PLAYER->GimmickLand());
				}
			}
		}
		else
		{ // �͈͓�����Ȃ��ꍇ

			// �������Ȃ�
			SetMoment(false);
		}
	}
	
	// �M�~�b�N�A�N�V�����̍X�V
	CGimmickAction::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CGimmickJumpOff::Draw(CShader* pShader)
{
	// �M�~�b�N�A�N�V�����̕`��
	CGimmickAction::Draw(pShader);
}

//===========================================
//  �e���g���̑ҋ@�ʒu���Z�o
//===========================================
D3DXVECTOR3 CGimmickJumpOff::CalcWaitPoint(const int Idx) const
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }

	// TODO : ��э~��̑ҋ@�ʒu
	return GetVec3Position();
}
