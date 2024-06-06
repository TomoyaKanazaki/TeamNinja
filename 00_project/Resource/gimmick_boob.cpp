//==========================================
//
//  �h�u�M�~�b�N(gimmick_boob.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_boob.h"
#include "player_clone.h"

//==========================================
//  �R���X�g���N�^
//==========================================
CGimmickBoob::CGimmickBoob() : CGimmickState()
{

}

//==========================================
//  �f�X�g���N�^
//==========================================
CGimmickBoob::~CGimmickBoob()
{

}

//==========================================
//  ����������
//==========================================
HRESULT CGimmickBoob::Init(void)
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
void CGimmickBoob::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickState::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CGimmickBoob::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CGimmickState::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CGimmickBoob::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickState::Draw(pShader);
}

//===========================================
//  ������(�t���O)�̒ǉ�
//===========================================
void CGimmickBoob::HitClone(CPlayerClone* pClone)
{
	// ���g�Ƀt���O��ǉ�����
	pClone->AddFrags(BOOB);
}

//=========================================
//  ������(�t���O)�̍폜
//===========================================
void CGimmickBoob::MissClone(CPlayerClone* pClone)
{

}
