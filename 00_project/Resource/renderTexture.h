//============================================================
//
//	�����_�[�e�N�X�`���w�b�_�[ [renderTexture.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �����_�[�e�N�X�`���N���X
class CRenderTexture : public CObject
{
public:
	// �R���X�g���N�^
	CRenderTexture();

	// �f�X�g���N�^
	~CRenderTexture();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CRenderTexture *Create(void);	// ����

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override { CObject::Release(); }	// �j��
};

#endif	// _RENDER_TEXTURE_H_
