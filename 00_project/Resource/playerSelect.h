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
//	�O���錾
//************************************************************
class CTransPoint;	// �J�ڃ|�C���g�N���X

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
	void SetSpawn(void) override;	// �X�|�[���ݒ�
	void SetEnter(void) override;	// ����ݒ�

private:
	// �����o�֐�
	EMotion UpdateSpawn(const float fDeltaTime);	// �X�|�[����Ԏ��̍X�V
	EMotion UpdateNormal(const float fDeltaTime);	// �ʏ��Ԏ��̍X�V
	EMotion UpdateWait(const float fDeltaTime);		// �ҋ@��Ԏ��̍X�V
	EMotion UpdateEnter(const float fDeltaTime);	// �����Ԏ��̍X�V
	EMotion UpdateOpen(const float fDeltaTime);		// �����Ԏ��̍X�V
	void UpdateTrans(D3DXVECTOR3& rPos);			// �X�e�[�W�J�ڂ̍X�V
	void SetWait(CTransPoint* pHit);				// �ҋ@�ݒ�

	// �����o�ϐ�
	std::string m_sSelectPath;	// �I�𒆂̑J�ڐ�}�b�v�p�X
};

#endif	// _PLAYER_SELECT_H_
