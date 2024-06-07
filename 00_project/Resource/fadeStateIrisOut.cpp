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
#include "objectCircle2D.h"	// TODO

//************************************************************
//	�q�N���X [CFadeStateIrisOut] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFadeStateIrisOut::CFadeStateIrisOut(std::function<D3DXVECTOR3(void)> pFuncPos) :
	m_pFuncPos	(pFuncPos)	// �؂蔲���^�ʒu�֐��|�C���^
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

	// �؂蔲���^�̈ʒu��ݒ�
	m_pContext->m_pCrop->SetVec3Position(posIris);

	float fRadiusIris = m_pContext->m_pCrop->GetRadius();	// �؂蔲���^���a

	// TODO�F�����Ђǂ�����
	// �~�����������Ă���
	fRadiusIris -= 450.0f * fDeltaTime;
	if (fRadiusIris <= 0.0f)
	{ // �����ɂȂ����ꍇ

		// ���a��␳
		fRadiusIris = 0.0f;

		// ���V�[���֑J�ڂ���
		m_pContext->TransNextMode();

		// ���a�𔽉f
		m_pContext->m_pCrop->SetRadius(fRadiusIris);

		m_pContext->ChangeState(new CFadeStateIn);

		return;
	}

	// ���a�𔽉f
	m_pContext->m_pCrop->SetRadius(fRadiusIris);
}

//============================================================
//	�R���e�L�X�g�ݒ菈��
//============================================================
void CFadeStateIrisOut::SetContext(CFade* pContext)
{
	D3DXVECTOR3 posIris = (m_pFuncPos == nullptr) ? SCREEN_CENT : m_pFuncPos();	// �؂蔲���^�ʒu

	// �R���e�L�X�g�̐ݒ�
	CFadeState::SetContext(pContext);

	// TODO�F�����Ő؂蔲���^�ʒu�ݒ�
	m_pContext->m_pCrop->SetVec3Position(posIris);

	// TODO�F�����œ����x�C��
	D3DXCOLOR col = m_pContext->GetColor();
	col.a = 1.0f;
	m_pContext->SetColor(col);

	// TODO�F�����Ŕ��a�C��
	m_pContext->m_pCrop->SetRadius(640.0f);
}
