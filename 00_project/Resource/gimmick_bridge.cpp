//===========================================
//
//  ���̃M�~�b�N(gimmick_bridge.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "gimmick_bridge.h"

//===========================================
//  �R���X�g���N�^
//===========================================
CGimmickBridge::CGimmickBridge()
{
}

//===========================================
//  �f�X�g���N�^
//===========================================
CGimmickBridge::~CGimmickBridge()
{
}

//===========================================
//  ����������
//===========================================
HRESULT CGimmickBridge::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CGimmickAction::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
//  �I������
//===========================================
void CGimmickBridge::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickAction::Uninit();
}

//===========================================
//  �X�V����
//===========================================
void CGimmickBridge::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CGimmickAction::Update(fDeltaTime);
}

//===========================================
//  �`�揈��
//===========================================
void CGimmickBridge::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickAction::Draw(pShader);
}

//===========================================
//  �e���g���̑ҋ@�ʒu���Z�o
//===========================================
D3DXVECTOR3 CGimmickBridge::CalcWaitPoint(const int Idx) const
{
	return D3DXVECTOR3();
}
