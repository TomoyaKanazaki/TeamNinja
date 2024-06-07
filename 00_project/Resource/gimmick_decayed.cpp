//=========================================
//
//  ���������M�~�b�N (gimmick_decayed.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_decayed.h"
#include "manager.h"
#include "player.h"

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickDecaed::CGimmickDecaed() : CGimmickAction()
{

}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickDecaed::~CGimmickDecaed()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickDecaed::Init(void)
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
void CGimmickDecaed::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickAction::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickDecaed::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CGimmickAction::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickDecaed::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickAction::Draw(pShader);
}
