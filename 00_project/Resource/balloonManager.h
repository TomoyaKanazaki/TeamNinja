//============================================================
//
//	�����o���}�l�[�W���[�w�b�_�[ [balloonManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BALLOON_MANAGER_H_
#define _BALLOON_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �����o���}�l�[�W���[�N���X
class CBalloonManager : public CObject
{
public:
	// �R���X�g���N�^
	CBalloonManager();

	// �f�X�g���N�^
	~CBalloonManager();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CBalloonManager *Create(void);	// ����

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override { CObject::Release(); }	// �j��
};

#endif	// _BALLOON_MANAGER_H_
