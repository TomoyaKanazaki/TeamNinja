//============================================================
//
//	�A�C���X�C����ԏ��� [fadeStateIrisIn.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fadeStateIrisIn.h"
#include "fade.h"

//************************************************************
//	�q�N���X [CFadeStateIrisIn] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFadeStateIrisIn::CFadeStateIrisIn(std::function<D3DXVECTOR3(void)> pFuncPos) :
	m_pFuncPos	(pFuncPos),	// �؂蔲���^�ʒu�֐��|�C���^
	m_fInitRad	(0.0f),		// �������a
	m_fCurTime	(0.0f)		// ���݂̌o�ߎ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CFadeStateIrisIn::~CFadeStateIrisIn()
{

}

//============================================================
//	����������
//============================================================
HRESULT CFadeStateIrisIn::Init(void)
{
	// �����o�ϐ���������
	m_fInitRad	= 0.0f;	// �������a
	m_fCurTime	= 0.0f;	// ���݂̌o�ߎ���

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CFadeStateIrisIn::Uninit(void)
{
	// ���g�̔j��
	delete this;
}

//============================================================
//	�X�V����
//============================================================
void CFadeStateIrisIn::Update(const float fDeltaTime)
{
	D3DXVECTOR3 posIris = (m_pFuncPos == nullptr) ? SCREEN_CENT : m_pFuncPos();	// �؂蔲���^�ʒu
	m_pContext->SetCropPosition(posIris);	// �؂蔲���^�̈ʒu��ݒ�

	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	const float fInTime = m_pContext->GetInTime();					// �C������
	const float fEndRadius = m_pContext->CalcCropRadius(posIris);	// �؂蔲���ő唼�a
	const float fDiffRad = fEndRadius - m_fInitRad;					// ���a����
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, fInTime);		// �o�ߎ����̊������v�Z

	// ���a�𔽉f
	m_pContext->SetCropRadius(m_fInitRad + (fDiffRad * fRate));
	if (m_fCurTime >= fInTime)
	{ // �傫���Ȃ������ꍇ

		// ���a��␳
		m_pContext->SetCropRadius(fEndRadius);

		// �؂蔲����̃|���S���𓧖��ɂ���
		m_pContext->SetAlpha(0.0f);

		// �������Ȃ���Ԃɂ���
		m_pContext->ChangeState(new CFadeStateNone);
	}
}
