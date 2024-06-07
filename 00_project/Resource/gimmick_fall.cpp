//=========================================
//
//  ���Ƃ����M�~�b�N (gimmick_fall.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_fall.h"
#include "manager.h"
#include "player.h"

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickFall::CGimmickFall() : CGimmickAction(),
m_bFall(false) // �����t���O
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
	// �A�N�e�B�u�t���O�𗎉��t���O�ɑ��
	m_bFall = IsActive();

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
