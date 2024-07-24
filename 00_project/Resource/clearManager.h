#if 1
//============================================================
//
//	�N���A�}�l�[�W���[�w�b�_�[ [clearManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _CLEARMANAGER_H_
#define _CLEARMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "retentionManager.h"

//************************************************************
//	�N���X��`
//************************************************************
// �N���A�}�l�[�W���[�N���X
class CClearManager
{
public:
	// �R���X�g���N�^
	CClearManager();

	// �f�X�g���N�^
	virtual ~CClearManager();

	// �������z�֐�
	virtual HRESULT Init(void)	= 0;	// ������
	virtual void Uninit(void)	= 0;	// �I��
	virtual void Update(const float fDeltaTime)	= 0;	// �X�V

	// �ÓI�����o�֐�
	static CClearManager *Create(const CRetentionManager::EWin win);	// ����
	static void Release(CClearManager *&prClearManager);	// �j��
};

#endif	// _CLEARMANAGER_H_
#endif
