//=========================================
//
//  ���ݑ�M�~�b�N (gimmick_step.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_step.h"

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickStep::CGimmickStep() : CGimmickAction()
{

}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickStep::~CGimmickStep()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickStep::Init(void)
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
void CGimmickStep::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickAction::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickStep::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CGimmickAction::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickStep::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickAction::Draw(pShader);
}
