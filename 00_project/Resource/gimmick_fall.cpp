//=========================================
//
//  ���Ƃ����M�~�b�N (gimmick_fall.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_fall.h"
#include "manager.h"
#include "player_clone.h"

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickFall::CGimmickFall() : CField(),
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
	if (FAILED(CField::Init()))
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
	CField::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickFall::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CField::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickFall::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CField::Draw(pShader);
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickFall::Hit(CPlayerClone* pClone)
{
	// ���g�ɕ������n��
	pClone->AddFrags(GetFlag());
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickFall::Miss(CPlayerClone* pClone)
{
	// ���g����t���O���폜����
	pClone->SabFrags(GetFlag());
}
