//============================================================
//
//	�N���A���s�}�l�[�W���[���� [clearFailManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "clearFailManager.h"
#include "manager.h"
#include "anim2D.h"
#include "string2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// �N���A���s�}�l�[�W���[�̗D�揇��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CClearFailManager::AFuncUpdateState CClearFailManager::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,						// �������Ȃ��X�V
	&CClearFailManager::UpdateWait,	// �ҋ@�X�V
	nullptr,						// �I���X�V
};

//************************************************************
//	�e�N���X [CClearFailManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CClearFailManager::CClearFailManager() :
	m_state		(STATE_NONE),	// ���
	m_fCurTime	(0.0f)			// ���݂̑ҋ@����
{
	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CClearFailManager::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	�f�X�g���N�^
//============================================================
CClearFailManager::~CClearFailManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CClearFailManager::Init(void)
{
	// �����o�ϐ���������
	//m_state		= STATE_;	// ���	// TODO
	m_fCurTime	= 0.0f;		// ���݂̑ҋ@����

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CClearFailManager::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CClearFailManager::Update(const float fDeltaTime)
{
	assert(m_state > NONE_IDX && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ

		// �e��Ԃ��Ƃ̍X�V
		(this->*(m_aFuncUpdateState[m_state]))(fDeltaTime);
	}
}

//============================================================
//	���o�̃X�L�b�v����
//============================================================
void CClearFailManager::SkipStaging(void)
{
	// �ҋ@��Ԃɂ���
	m_state = STATE_WAIT;
}

//============================================================
//	�SUI�I�u�W�F�N�g�̈ړ�����
//============================================================
void CClearFailManager::SetAllMove(const D3DXVECTOR3& rMove)
{

}

//============================================================
//	�ҋ@�̍X�V����
//============================================================
void CClearFailManager::UpdateWait(const float fDeltaTime)
{
	if (GET_INPUTPAD->IsAnyTrigger()
	||  GET_INPUTKEY->IsTrigger(DIK_SPACE))
	{
		// �I����Ԃɂ���
		m_state = STATE_END;
	}
}
