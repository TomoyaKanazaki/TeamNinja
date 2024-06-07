//============================================================
//
//	�]�C�Ǘ���ԃw�b�_�[ [fadeStateWait.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FADE_STATE_WAIT_H_
#define _FADE_STATE_WAIT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fadeState.h"

//************************************************************
//	�N���X��`
//************************************************************
// �]�C�Ǘ���ԃN���X
class CFadeStateWait : public CFadeState
{
public:
	// �R���X�g���N�^
	CFadeStateWait(const float fWaitTime, CFadeState* pNextState);

	// �f�X�g���N�^
	~CFadeStateWait() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V

private:
	// �����o�ϐ�
	CFadeState* m_pNextState;	// �J�ڐ�̏��
	float m_fWaitTime;			// ���݂̗]�C����
};

#endif	// _FADE_STATE_WAIT_H_
