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
CGimmickBridge::CGimmickBridge() : CGimmickAction(),
m_bSet(false),
m_ConectPoint()
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
	// ���̒[�̐ݒ�
	if (!m_bSet) { CalcConectPoint(); }

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

//===========================================
//  ���̒[�̌v�Z����
//===========================================
void CGimmickBridge::CalcConectPoint()
{
	// �ݒ�ς݃t���O�𗧂Ă�
	m_bSet = true;

	// ���g�̈ʒu���擾
	D3DXVECTOR3 pos = GetVec3Position();

	// ���g�̃T�C�Y���擾
	D3DXVECTOR3 size = GetVec3Sizing();

	// �v�Z���s��
	if (size.x < size.z) // z�����ɉ˂���ꍇ
	{
		// ���S���W�ɃT�C�Y * 0.5�����Z����
		m_ConectPoint[0] = pos + D3DXVECTOR3(0.0f, 0.0f, size.z * 0.5f);
		m_ConectPoint[1] = pos - D3DXVECTOR3(0.0f, 0.0f, size.z * 0.5f);
	}
	else if (size.x > size.z) // x�����ɉ˂���ꍇ
	{
		// ���S���W�ɃT�C�Y * 0.5�����Z����
		m_ConectPoint[0] = pos + D3DXVECTOR3(size.x * 0.5f, 0.0f, 0.0f);
		m_ConectPoint[1] = pos - D3DXVECTOR3(size.x * 0.5f, 0.0f, 0.0f);
	}
	else // xz�̃T�C�Y����v���Ă���ꍇ
	{
		// �{���͂�߂Ăق����B
		assert(false);
		
		// ���S���W�ɃT�C�Y * 0.5�����Z����
		m_ConectPoint[0] = pos + D3DXVECTOR3(size.x * 0.5f, 0.0f, size.z * 0.5f);
		m_ConectPoint[1] = pos - D3DXVECTOR3(size.x * 0.5f, 0.0f, size.z * 0.5f);
	}
}
