//============================================================
//
//	�G�t�F�N�V�A�}�l�[�W���[�w�b�_�[ [effekseerManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EFFEKSEER_MANAGER_H_
#define _EFFEKSEER_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�t�F�N�V�A�}�l�[�W���[�N���X
class CEffekseerManager : public CObject
{
public:
	// �R���X�g���N�^
	CEffekseerManager();

	// �f�X�g���N�^
	~CEffekseerManager() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CEffekseerManager *Create(void);	// ����

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override { CObject::Release(); }	// �j��
};

#endif	// _EFFEKSEER_MANAGER_H_
