//=========================================
//
//  �����Ǘ��M�~�b�N (gimmick_malti.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_malti.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickMalti::CGimmickMalti() : CGimmick()
{

}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickMalti::~CGimmickMalti()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickMalti::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CGimmick::Init()))
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
void CGimmickMalti::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmick::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickMalti::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CGimmick::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickMalti::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmick::Draw(pShader);
}
