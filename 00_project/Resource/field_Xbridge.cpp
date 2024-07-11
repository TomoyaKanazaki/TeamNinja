//==========================================
//
//  ����M�~�b�N(field_Xbridge.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field_Xbridge.h"
#include "player_clone.h"
#include "player.h"

//==========================================
//  �R���X�g���N�^
//==========================================
CGimmickXBridge::CGimmickXBridge() : CField()
{

}

//==========================================
//  �f�X�g���N�^
//==========================================
CGimmickXBridge::~CGimmickXBridge()
{

}

//==========================================
//  ����������
//==========================================
HRESULT CGimmickXBridge::Init(void)
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
void CGimmickXBridge::Uninit(void)
{
	// �e�N���X�̏I��
	CField::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CGimmickXBridge::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CField::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CGimmickXBridge::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CField::Draw(pShader);
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickXBridge::Hit(CPlayerClone* pClone)
{
	// �t���O��ǉ�����
	pClone->AddFrags(GetFlag());
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickXBridge::Miss(CPlayerClone* pClone)
{
	// �t���O���폜����
	pClone->SabFrags(GetFlag());
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickXBridge::Hit(CPlayer* pPlayer)
{
	// �t���O��ǉ�����
	pPlayer->AddFrags(GetFlag());
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickXBridge::Miss(CPlayer* pPlayer)
{
	// �t���O���폜����
	pPlayer->SabFrags(GetFlag());
}
