//=========================================
//
//  �ݒu�M�~�b�N (gimmick_post.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_post.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickPost::CGimmickPost() : CGimmickAction()
{

}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickPost::~CGimmickPost()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickPost::Init(void)
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
void CGimmickPost::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickAction::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickPost::Update(const float fDeltaTime)
{
	// �A�N�e�B�u��ԂɂȂ��Ă�����ݒu�t���O��on
	m_bPost = IsActive();

	// �e�N���X�̍X�V
	CGimmickAction::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickPost::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickAction::Draw(pShader);
}
