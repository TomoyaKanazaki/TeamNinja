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
class CRenderTexture
{
public:
	// �R���X�g���N�^
	explicit CRenderTexture(const CObject::EScene scene);

	// �f�X�g���N�^
	~CRenderTexture();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Draw(void);	// �`��
	int GetTextureIndex(void) const { return m_nTextureID; }	// �e�N�X�`���C���f�b�N�X�擾

	// �ÓI�����o�֐�
	static CRenderTexture *Create(const CObject::EScene scene);	// ����

private:
	// �����o�ϐ�
	LPDIRECT3DSURFACE9 m_pSurTexture;	// �e�N�X�`���T�[�t�F�C�X�ւ̃|�C���^
	int m_nTextureID;					// �����_�[�e�N�X�`���C���f�b�N�X
	const CObject::EScene m_scene;		// �`��V�[��
};

#endif	// _RENDER_TEXTURE_H_
