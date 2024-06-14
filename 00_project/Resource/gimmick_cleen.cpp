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
CGimmickCleen::CGimmickCleen() : CField()
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
	if (FAILED(CField::Init()))
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
	CField::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CGimmickCleen::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CField::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CGimmickCleen::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CField::Draw(pShader);
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickCleen::Hit(CPlayerClone* pClone)
{
	// ���g�ɕ������n��
	pClone->AddFrags(GetFlag());
}

//=========================================
//  �������Ă��Ȃ��ꍇ�̏���
//===========================================
void CGimmickCleen::Miss(CPlayerClone* pClone)
{
	// ���g����t���O���폜����
	pClone->SabFrags(GetFlag());
}
