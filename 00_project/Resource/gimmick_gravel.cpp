//==========================================
//
//  �������M�~�b�N(gimmick_gravel.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_gravel.h"
#include "player_clone.h"

//==========================================
//  �R���X�g���N�^
//==========================================
CGimmickGravel::CGimmickGravel()
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
void CGimmickGravel::Uninit(void)
{
	// �e�N���X�̏I��
	CField::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CGimmickGravel::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CField::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CGimmickGravel::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CField::Draw(pShader);
}

//===========================================
//  ������(�t���O)�̒ǉ�
//===========================================
void CGimmickGravel::Hit(CPlayerClone* pClone)
{
	// ���g�ɕ������n��
	pClone->AddFrags(GetFlag());
}

//=========================================
//  ������(�t���O)�̍폜
//===========================================
void CGimmickGravel::Miss(CPlayerClone* pClone)
{
	// ���g����t���O���폜����
	pClone->SabFrags(GetFlag());
}
