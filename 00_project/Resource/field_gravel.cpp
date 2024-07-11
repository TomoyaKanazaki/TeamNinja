//==========================================
//
//  �������M�~�b�N(field_gravel.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field_gravel.h"
#include "player_clone.h"
#include "player.h"

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
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickGravel::Hit(CPlayerClone* pClone)
{
	// ���g�ɕ������n��
	pClone->AddFrags(GetFlag());
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickGravel::Miss(CPlayerClone* pClone)
{
	// ���g����t���O���폜����
	pClone->SabFrags(GetFlag());
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickGravel::Hit(CPlayer* pPlayer)
{
	// ���g�ɕ������n��
	pPlayer->AddFrags(GetFlag());
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickGravel::Miss(CPlayer* pPlayer)
{
	// ���g����t���O���폜����
	pPlayer->SabFrags(GetFlag());
}
