//============================================================
//
//	�t�F�[�h��ԃw�b�_�[ [fadeState.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FADE_STATE_H_
#define _FADE_STATE_H_

//************************************************************
//	�O���錾
//************************************************************
class CFade;	// �t�F�[�h�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �t�F�[�h��ԃN���X
class CFadeState
{
public:
	// �R���X�g���N�^
	CFadeState();

	// �f�X�g���N�^
	virtual ~CFadeState();

	// �������z�֐�
	virtual HRESULT Init(void)	= 0;	// ������
	virtual void Uninit(void)	= 0;	// �I��
	virtual void Update(const float fDeltaTime) = 0;	// �X�V

	// ���z�֐�
	virtual void SetContext(CFade *pContext) { m_pContext = pContext; }	// �R���e�L�X�g�ݒ�

protected:
	// �����o�ϐ�
	CFade *m_pContext;	// �R���e�L�X�g
};

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fadeStateNone.h"
#include "fadeStateWait.h"
#include "fadeStateOut.h"
#include "fadeStateIn.h"

// TODO�F���܂ł�Fade���ł�����FadeIrisIn, FadeIrisOut���쐬����
//		 InOut�ł͈ʒu�擾�֐��|�C���^���R���X�g���N�^�œ������悤�ɂ��āA
//		 nullptr�Ȃ琢�E�̒��S�A�|�C���^������Ȃ�擾�����ʒu�ŉ~���k�ނ悤�ɂ���

#endif	// _FADE_STATE_H_
