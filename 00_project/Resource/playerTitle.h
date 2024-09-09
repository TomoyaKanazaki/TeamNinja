//============================================================
//
//	�^�C�g����ʃv���C���[�w�b�_�[ [playerTitle.h]
//	Author�F���c�E��
// 
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_TITLE_H_
#define _PLAYER_TITLE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player.h"

//************************************************************
//	�N���X��`
//************************************************************
// �^�C�g����ʃv���C���[�N���X
class CPlayerTitle : public CPlayer
{
public:
	// �R���X�g���N�^
	CPlayerTitle();

	// �f�X�g���N�^
	~CPlayerTitle() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;			// �X�V
	EMotion UpdateState(const float fDeltaTime) override;	// ��ԍX�V
	void SetSpawn(void) override;	// �X�|�[���ݒ�
	void SetStart(void) override;	// ���o�J�n�ݒ�

private:
	// �����o�֐�
	EMotion UpdateIdol(const float fDeltaTime);	// �m��������Ԏ��̍X�V
	EMotion UpdateMove(const float fDeltaTime);	// �ړ���Ԏ��̍X�V
};

#endif	// _PLAYER_TITLE_H_
