//==========================================
//
//  ����M�~�b�N(gimmick_water.cpp)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_water.h"
#include "player_clone.h"

//==========================================
//  �R���X�g���N�^
//==========================================
CGimmickWater::CGimmickWater() : CGimmickState()
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
void CGimmickWater::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickState::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CGimmickWater::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CGimmickState::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CGimmickWater::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickState::Draw(pShader);
}

//===========================================
//  ������(�t���O)�̒ǉ�
//===========================================
void CGimmickWater::HitClone(CPlayerClone* pClone)
{
	// ���g�Ƀt���O��ǉ�����
	pClone->AddFrags(WATER);

	// �h�u�t���O���폜����
	pClone->SabFrags(BOOB);
}

//=========================================
//  ������(�t���O)�̍폜
//===========================================
void CGimmickWater::MissClone(CPlayerClone* pClone)
{
	// ���g����t���O���폜����
	pClone->SabFrags(WATER);
}
