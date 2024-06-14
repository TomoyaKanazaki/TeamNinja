//=========================================
//
//  ���������M�~�b�N (gimmick_decayed.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_decayed.h"
#include "manager.h"
#include "player.h"

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickDecaed::CGimmickDecaed() : CField(),
m_bFall(false) // ������t���O
{

}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickDecaed::~CGimmickDecaed()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickDecaed::Init(void)
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
void CGimmickDecaed::Uninit(void)
{
	// �e�N���X�̏I��
	CField::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickDecaed::Update(const float fDeltaTime)
{
	if (m_bFall) { DebugProc::Print(DebugProc::POINT_CENTER, "������on\n"); }

	// �e�N���X�̍X�V
	CField::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickDecaed::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CField::Draw(pShader);
}
