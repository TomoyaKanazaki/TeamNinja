//==========================================
//
//  �|�������Ă̏��M�~�b�N(gimmick_cleen.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_cleen.h"
#include "player_clone.h"

//==========================================
//  �R���X�g���N�^
//==========================================
CGimmickCleen::CGimmickCleen() : CGimmickState()
{

}

//==========================================
//  �f�X�g���N�^
//==========================================
CGimmickCleen::~CGimmickCleen()
{

}

//==========================================
//  ����������
//==========================================
HRESULT CGimmickCleen::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CGimmickState::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//==========================================
//  �I������
//==========================================
void CGimmickCleen::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickState::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CGimmickCleen::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CGimmickState::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CGimmickCleen::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickState::Draw(pShader);
}

//===========================================
//  
//===========================================
void CGimmickCleen::HitClone(CPlayerClone* pClone)
{
	// ���g�ɕ������n��
	pClone->AddFrags(CLEEN);
}

//=========================================
//  ������(�t���O)�̍폜
//===========================================
void CGimmickCleen::MissClone(CPlayerClone* pClone)
{
	// ���g����t���O���폜����
	pClone->SabFrags(CLEEN);
}
