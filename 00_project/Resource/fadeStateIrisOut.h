//============================================================
//
//	�A�C���X�A�E�g��ԃw�b�_�[ [fadeStateIrisOut.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FADE_STATE_IRISOUT_H_
#define _FADE_STATE_IRISOUT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fadeState.h"

//************************************************************
//	�N���X��`
//************************************************************
// �A�C���X�A�E�g��ԃN���X
class CFadeStateIrisOut : public CFadeState
{
public:
	// �R���X�g���N�^
	CFadeStateIrisOut(std::function<D3DXVECTOR3(void)> pFuncPos);

	// �f�X�g���N�^
	~CFadeStateIrisOut() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void SetContext(CFade* pContext) override;		// �R���e�L�X�g�ݒ�

private:
	// �����o�ϐ�
	std::function<D3DXVECTOR3(void)> m_pFuncPos;	// �؂蔲���^�ʒu�֐��|�C���^
};

#endif	// _FADE_STATE_IRISOUT_H_
