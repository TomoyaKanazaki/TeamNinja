//==========================================
//
//  �D�M�~�b�N(field_boob.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field_boob.h"
#include "player_clone.h"
#include "player.h"

//==========================================
//  �R���X�g���N�^
//==========================================
CGimmickBoob::CGimmickBoob() : CField()
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
void CGimmickBoob::Uninit(void)
{
	// �e�N���X�̏I��
	CField::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CGimmickBoob::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CField::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CGimmickBoob::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CField::Draw(pShader);
}

//===========================================
//  ������(�t���O)�̒ǉ�
//===========================================
void CGimmickBoob::Hit(CPlayerClone* pClone)
{
	// ���g�Ƀt���O��ǉ�����
	pClone->AddFrags(GetFlag());
}

//=========================================
//  ������(�t���O)�̍폜
//===========================================
void CGimmickBoob::Miss(CPlayerClone* pClone)
{

}
//===
// ========================================
//  ������(�t���O)�̒ǉ�
//===========================================
void CGimmickBoob::Hit(CPlayer* pPlayer)
{
	// ���g�Ƀt���O��ǉ�����
	pPlayer->AddFrags(GetFlag());
}

//=========================================
//  ������(�t���O)�̍폜
//===========================================
void CGimmickBoob::Miss(CPlayer* pPlayer)
{

}
