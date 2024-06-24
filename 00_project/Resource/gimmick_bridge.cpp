//===========================================
//
//  ���̃M�~�b�N(gimmick_bridge.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "gimmick_bridge.h"
#include "manager.h"
#include "player.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const float DISTANCE = 30.0f; // �ҋ@�ʒu�Ƃ̋���
}

//===========================================
//  �R���X�g���N�^
//===========================================
CGimmickBridge::CGimmickBridge() : CGimmickAction(),
m_bSet(false),
m_ConectPoint(),
m_vecToWait(VEC3_ZERO),
m_nIdxWait(0)
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
D3DXVECTOR3 CGimmickBridge::CalcWaitPoint(const int Idx)
{
	// �C���f�b�N�X�ԍ���0�̏ꍇ2�_�̂����v���C���[�ɋ߂�����ҋ@���S�Ƃ���
	if (Idx == 0)
	{
		// �v���C���[���W���擾
		D3DXVECTOR3 posPlayer = GET_PLAYER->GetVec3Position();

		// �v���C���[��2�_�����ԃx�N�g�����Z�o
		D3DXVECTOR3 vecToPlayer[2] = { posPlayer - m_ConectPoint[0], posPlayer - m_ConectPoint[1] };

		// ������2�悪���������̔z��ԍ���ۑ�
		if (vecToPlayer[0].x * vecToPlayer[0].x + vecToPlayer[0].z * vecToPlayer[0].z <=
			vecToPlayer[1].x * vecToPlayer[1].x + vecToPlayer[1].z * vecToPlayer[1].z)
		{
			m_nIdxWait = 0;
		}
		else
		{
			m_nIdxWait = 1;
		}

		// ���S����ҋ@���S�ւ̃x�N�g�����Z�o�����K������
		D3DXVECTOR3 vecToWait = m_ConectPoint[m_nIdxWait] - GetVec3Position();
		D3DXVec3Normalize(&m_vecToWait, &vecToWait);
	}

	// �ҋ@�ʒu��Ԃ�
	return m_ConectPoint[m_nIdxWait] + (m_vecToWait * DISTANCE * (float)Idx);
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
