//============================================================
//
//	�t�F�[�h�A�E�g��ԃw�b�_�[ [fadeStateOut.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FADE_STATE_OUT_H_
#define _FADE_STATE_OUT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fadeState.h"
#include "scene.h"

//************************************************************
//	�N���X��`
//************************************************************
// �t�F�[�h�A�E�g��ԃN���X
class CFadeStateOut : public CFadeState
{
public:
	// �R���X�g���N�^
	CFadeStateOut();

	// �f�X�g���N�^
	~CFadeStateOut() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
};

#endif	// _FADE_STATE_OUT_H_
