//============================================================
//
//	�W�����v�䏈�� [gimmick_jumptable.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "gimmick_jumptable.h"

//************************************************************
//	�q�N���X [CGimmickJumpTable] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGimmickJumpTable::CGimmickJumpTable() : CGimmick()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGimmickJumpTable::~CGimmickJumpTable()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGimmickJumpTable::Init(void)
{
	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CGimmick::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGimmickJumpTable::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I��
	CGimmick::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGimmickJumpTable::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g3D�̍X�V
	CGimmick::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CGimmickJumpTable::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CGimmick::Draw(pShader);
}