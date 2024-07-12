//==========================================
//
//  Z�����̋��M�~�b�N(field_Zbridge.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field_Zbridge.h"
#include "player_clone.h"
#include "player.h"

//==========================================
//  �R���X�g���N�^
//==========================================
CGimmickZBridge::CGimmickZBridge() : CField()
{

}

//==========================================
//  �f�X�g���N�^
//==========================================
CGimmickZBridge::~CGimmickZBridge()
{

}

//==========================================
//  ����������
//==========================================
HRESULT CGimmickZBridge::Init(void)
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
void CGimmickZBridge::Uninit(void)
{
	// �e�N���X�̏I��
	CField::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CGimmickZBridge::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CField::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CGimmickZBridge::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CField::Draw(pShader);
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickZBridge::Hit(CPlayerClone* pClone)
{
	// �t���O��ǉ�����
	pClone->AddFrags(GetFlag());
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickZBridge::Miss(CPlayerClone* pClone)
{
	// �t���O���폜����
	pClone->SabFrags(GetFlag());
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickZBridge::Hit(CPlayer* pPlayer)
{
	// �t���O��ǉ�����
	pPlayer->AddFrags(GetFlag());
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickZBridge::Miss(CPlayer* pPlayer)
{
	// �t���O���폜����
	pPlayer->SabFrags(GetFlag());
}
