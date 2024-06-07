//=========================================
//
//  �����Ǘ��M�~�b�N (gimmick_malti.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_malti.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const int NUM_GIMMICK = 3; // �Ǘ�����M�~�b�N�̐�
}

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickMalti::CGimmickMalti() : CGimmick(),
m_pGimmick (nullptr), // �Ǘ�����M�~�b�N
m_bActive (true) // �A�N�e�B�u�t���O
{

}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickMalti::~CGimmickMalti()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickMalti::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CGimmick::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �ݒu�M�~�b�N�𐶐�
	if (FAILED(Create()))
	{
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
void CGimmickMalti::Uninit(void)
{
	// ���X�g�}�l�[�W���[�̔j��
	m_pGimmick->Release(m_pGimmick);

	// �e�N���X�̏I��
	CGimmick::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickMalti::Update(const float fDeltaTime)
{
	// �t���O�����Z�b�g
	m_bActive = true;

	// �Ǘ����Ă���M�~�b�N�̐ݒu�t���O���擾
	for (int i = 0; i < NUM_GIMMICK; ++i)
	{
		// �M�~�b�N���擾
		CGimmick* pGimmick = *m_pGimmick->GetIndex(i);

		// �M�~�b�N�̃t���O�������Ă����玟�ɐi��
		if (pGimmick->IsSet()) { continue; }

		// 1�ł��t���O��off�Ȃ�t���O��false�ɂ���
		m_bActive = false;
		break;
	}

	if (m_bActive) { DebugProc::Print(DebugProc::POINT_CENTER, "�ݒuOK\n"); }

	// �e�N���X�̍X�V
	CGimmick::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickMalti::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmick::Draw(pShader);
}

//===========================================
//  �ݒu�M�~�b�N�̐���
//===========================================
HRESULT CGimmickMalti::Create()
{
	if (m_pGimmick == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pGimmick = CListManager<CGimmick>::Create();
		if (m_pGimmick == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// �M�~�b�N�Ǘ����X�g�ɃI�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	for (int i = 0; i < NUM_GIMMICK; ++i)
	{
		m_iterator = m_pGimmick->AddList(CGimmick::Create(D3DXVECTOR3(0.0f, 0.0f, -500 + -500.0f * i), D3DXVECTOR3(100.0f, 0.0f, 50.0f), CGimmick::TYPE_POST, 1));
	}

	return S_OK;
}
