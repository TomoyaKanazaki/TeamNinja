//============================================================
//
//	�^�C�g���J�ڃ|�C���g�w�b�_�[ [transpoint_title.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TRANSPOINT_TITLE_H_
#define _TRANSPOINT_TITLE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "transpoint.h"

//************************************************************
//	�N���X��`
//************************************************************
// �^�C�g���J�ڃ|�C���g�N���X
class CTransPointTitle : public CTransPoint
{
public:
	// �R���X�g���N�^
	explicit CTransPointTitle();

	// �f�X�g���N�^
	~CTransPointTitle() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Update(const float fDeltaTime) override;	// �X�V
};

#endif	// _TRANSPOINT_TITLE_H_
