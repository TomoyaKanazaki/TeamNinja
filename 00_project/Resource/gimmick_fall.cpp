//=========================================
//
//  ���Ƃ����M�~�b�N (gimmick_fall.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_fall.h"
#include "manager.h"
#include "player.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const float CLIMB_MAX = 300.0f; // �o�����E�̍���
	const float CLIMB_SPEED = 200.0f; // 1�b�Ԃɓo�鑬�x
}

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickFall::CGimmickFall() : CGimmickAction()
{

}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickFall::~CGimmickFall()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickFall::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CGimmickAction::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//=========================================
//  �I������
//=========================================
void CGimmickFall::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickAction::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickFall::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CGimmickAction::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickFall::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickAction::Draw(pShader);
}
