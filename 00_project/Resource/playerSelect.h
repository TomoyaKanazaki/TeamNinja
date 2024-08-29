//============================================================
//
//	�Z���N�g��ʃv���C���[�w�b�_�[ [playerSelect.h]
//	Author�F���c�E��
// 
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_SELECT_H_
#define _PLAYER_SELECT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player.h"

//************************************************************
//	�N���X��`
//************************************************************
// �Z���N�g��ʃv���C���[�N���X
class CPlayerSelect : public CPlayer
{
public:
	// �R���X�g���N�^
	CPlayerSelect();

	// �f�X�g���N�^
	~CPlayerSelect() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;			// �X�V
	EMotion UpdateState(const float fDeltaTime) override;	// ��ԍX�V

private:
	// �����o�֐�
	EMotion UpdateSpawn(const float fDeltaTime);	// �X�|�[����Ԏ��̍X�V
	EMotion UpdateNormal(const float fDeltaTime);	// �ʏ��Ԏ��̍X�V
	void UpdateTrans(D3DXVECTOR3& rPos);			// �X�e�[�W�J�ڂ̍X�V
};

#endif	// _PLAYER_SELECT_H_
