//==========================================
//
//  ����M�~�b�N(field_water.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field_water.h"
#include "player_clone.h"
#include "player.h"

//==========================================
//  �R���X�g���N�^
//==========================================
CGimmickWater::CGimmickWater() : CField()
{

}

//==========================================
//  �f�X�g���N�^
//==========================================
CGimmickWater::~CGimmickWater()
{

}

//==========================================
//  ����������
//==========================================
HRESULT CGimmickWater::Init(void)
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
void CGimmickWater::Uninit(void)
{
	// �e�N���X�̏I��
	CField::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CGimmickWater::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CField::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CGimmickWater::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CField::Draw(pShader);
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickWater::Hit(CPlayerClone* pClone)
{
	// ���g�Ƀt���O��ǉ�����
	pClone->AddFrags(GetFlag());

	// �D�t���O���폜����
	pClone->SabFrags(GetFlag(TYPE_BOOB));
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickWater::Miss(CPlayerClone* pClone)
{
	// ���g����t���O���폜����
	pClone->SabFrags(GetFlag());
}

//===========================================
//  �������Ă����ꍇ�̏���
//===========================================
void CGimmickWater::Hit(CPlayer* pPlayer)
{
	// ���g�Ƀt���O��ǉ�����
	pPlayer->AddFrags(GetFlag());

	// �D�t���O���폜����
	pPlayer->SabFrags(GetFlag(TYPE_BOOB));
}

//==========================================
//  �������Ă��Ȃ��ꍇ�̏���
//==========================================
void CGimmickWater::Miss(CPlayer* pPlayer)
{
	// ���g����t���O���폜����
	pPlayer->SabFrags(GetFlag());
}
