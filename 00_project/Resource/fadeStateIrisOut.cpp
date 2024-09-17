//============================================================
//
//	�A�C���X�A�E�g��ԏ��� [fadeStateIrisOut.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fadeStateIrisOut.h"
#include "fade.h"

//************************************************************
//	�q�N���X [CFadeStateIrisOut] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFadeStateIrisOut::CFadeStateIrisOut(std::function<D3DXVECTOR3(void)> pFuncPos) :
	m_pFuncPos	(pFuncPos),	// �؂蔲���^�ʒu�֐��|�C���^
	m_fInitRad	(0.0f),		// �������a
	m_fCurTime	(0.0f)		// ���݂̌o�ߎ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CFadeStateIrisOut::~CFadeStateIrisOut()
{

}

//============================================================
//	����������
//============================================================
HRESULT CFadeStateIrisOut::Init(void)
{
	D3DXVECTOR3 posIris = (m_pFuncPos == nullptr) ? SCREEN_CENT : m_pFuncPos();	// �؂蔲���^�ʒu

	// �����o�ϐ���������
	m_fInitRad	= m_pContext->CalcCropRadius(posIris);	// �������a
	m_fCurTime	= 0.0f;	// ���݂̌o�ߎ���

	// �؂蔲���^�̈ʒu��������
	m_pContext->SetCropPosition(posIris);

	// �؂蔲���^�̔��a��������
	m_pContext->SetCropRadius(m_fInitRad);

	// �؂蔲����̃|���S����s�����ɂ���
	m_pContext->SetAlpha(1.0f);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CFadeStateIrisOut::Uninit(void)
{
	// ���g�̔j��
	delete this;
}

//============================================================
//	�X�V����
//============================================================
void CFadeStateIrisOut::Update(const float fDeltaTime)
{
	D3DXVECTOR3 posIris = (m_pFuncPos == nullptr) ? SCREEN_CENT : m_pFuncPos();	// �؂蔲���^�ʒu
	m_pContext->SetCropPosition(posIris);	// �؂蔲���^�̈ʒu��ݒ�

	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	const float fDiffRad = 0.0f - m_fInitRad;	// ���a����
	float fRate = easeing::OutQuad(m_fCurTime, 0.0f, 1.0f);	// �o�ߎ����̊������v�Z

	// ���a�𔽉f
	m_pContext->SetCropRadius(m_fInitRad + (fDiffRad * fRate));

	if (m_fCurTime >= 1.0f)
	{ // �������Ȃ肫�����ꍇ

		// ���V�[���֑J�ڂ���
		m_pContext->TransNextMode();

		// ���a�𔽉f
		m_pContext->SetCropRadius(0.0f);

		// �A�C���X�C����Ԃɂ���
		m_pContext->ChangeState(new CFadeStateIrisIn(m_pFuncPos));
	}
}
