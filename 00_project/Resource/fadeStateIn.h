//============================================================
//
//	�t�F�[�h�C����ԃw�b�_�[ [fadeStateIn.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FADE_STATE_IN_H_
#define _FADE_STATE_IN_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fadeState.h"

//************************************************************
//	�N���X��`
//************************************************************
// �t�F�[�h�C����ԃN���X
class CFadeStateIn : public CFadeState
{
public:
	// �R���X�g���N�^
	CFadeStateIn();

	// �f�X�g���N�^
	~CFadeStateIn() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V

private:
	// �����o�ϐ�
	float m_fCurTime;	// ���݂̌o�ߎ���
};

#endif	// _FADE_STATE_IN_H_
