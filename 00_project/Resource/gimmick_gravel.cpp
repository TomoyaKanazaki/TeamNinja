//==========================================
//
//  �������M�~�b�N(gimmick_gravel.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_gravel.h"
#include "player_clone.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const char GRAVEL_FRAG = 'g'; // �������̃t���O
}

//==========================================
//  �R���X�g���N�^
//==========================================
CGimmickGravel::CGimmickGravel() : CGimmickState()
{

}

//==========================================
//  �f�X�g���N�^
//==========================================
CGimmickGravel::~CGimmickGravel()
{

}

//==========================================
//  ����������
//==========================================
HRESULT CGimmickGravel::Init(void)
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
void CGimmickGravel::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickState::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CGimmickGravel::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CGimmickState::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CGimmickGravel::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickState::Draw(pShader);
}

//===========================================
//  ������(�t���O)�̒ǉ�
//===========================================
void CGimmickGravel::SetChar(CPlayerClone* pClone)
{
	// ���g�ɕ������n��
	pClone->AddFrag(GRAVEL_FRAG);
}

//=========================================
//  ������(�t���O)�̍폜
//===========================================
void CGimmickGravel::DeleteChar(CPlayerClone* pClone)
{
	// ���g����t���O���폜����
	pClone->SabFrag(GRAVEL_FRAG);
}
