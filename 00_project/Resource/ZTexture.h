//============================================================
//
//	Z�e�N�X�`�������w�b�_�[ [ZTexture.h]
//	Author�F�O�� ���V��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ZTexture_H_
#define _ZTexture_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const UINT ZTEX_WIDTH = SCREEN_WIDTH;								//Z�e�N�X�`���̕�
	const UINT ZTEX_HEIGHT = SCREEN_WIDTH;										//Z�e�N�X�`���̍���
	const float VIEWING_ANGLE = 45.0f;										//����p
	const float NEAR_CLIP = 1000.0f;											//�`��ŏ��[�x
	const float FAR_CLIP = 10000.0f;										//�`��ő�[�x
	const int NUM_TEX = 9;
	const D3DXVECTOR3 VIEW_POINT = D3DXVECTOR3(1000.0f, 8000.0f, 1000.0f);		//���_
	const D3DXVECTOR3 FOCUS_POINT = D3DXVECTOR3(1100.0f, -10.0f, 0.0f);		//�����_
	const D3DXVECTOR3 LOOK_UP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				//������̃x�N�g�� *�x��:�����������*
}
//************************************************************
//	�N���X��`
//************************************************************
// �A�j���[�V����2D�N���X
class CZTexture
{
public:
	// �R���X�g���N�^
	CZTexture();
	// �f�X�g���N�^
	~CZTexture();
	// ���������\�b�h
	bool Init();

	// �`��ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s���ݒ�
	void SetWorldMatrix(D3DXMATRIX *pMat);

	// �r���[�s���ݒ�
	void SetViewMatrix(D3DXMATRIX *pMat);

	// �ˉe�ϊ��s���ݒ�
	void SetProjMatrix(D3DXMATRIX *pMat);

	// �`��̊J�n��錾����
	HRESULT Begin();

	// �`��̏I����錾����
	HRESULT End();

	// �p�X�̊J�n��錾����
	HRESULT BeginPass();

	// �p�X�̏I����錾����
	HRESULT EndPass();

	void DrawSprite();

	void Blur();

	// �o�^����Ă���p�����[�^�����G�t�F�N�g�ɃZ�b�g
	bool SetParamToEffect();

	// Z�l�e�N�X�`�����擾����
	bool GetZTex(IDirect3DTexture9 **cpTex);

	bool GetIsBegin(void) { return m_bBegin; }

	// �ÓI�����o�֐�
	static CZTexture* Create(void);		// ����
	static CZTexture* GetInstance(void);	// �擾
	static void Release(void);				// �j��

	D3DXMATRIX GetViewMtx() { return m_matView; }
	D3DXMATRIX GetProjMtx() { return m_matProj; }

private:

	// �ÓI�����o�ϐ�
	static CZTexture* m_pShader;	// �V�F�[�_�[���

	// �����o�ϐ�
	IDirect3DTexture9 * m_cpZTex;				// Z�l�e�N�X�`��
	IDirect3DSurface9 * m_cpZTexSurf;			// Z�l�e�N�X�`���T�[�t�F�C�X
	IDirect3DTexture9* m_cpBlurTex;				// �u���[�e�N�X�`��
	IDirect3DSurface9* m_cpBlurTexSurf;			// �u���[�e�N�X�`���T�[�t�F�C�X
	IDirect3DSurface9 * m_cpDepthBuff;			// �[�x�o�b�t�@
	IDirect3DSurface9 * m_cpDevBuffer;			// �f�o�C�X�o�b�N�o�b�t�@
	IDirect3DSurface9 * m_cpDevDepth;			// �f�o�C�X�[�x�o�b�t�@
	ID3DXEffect * m_cpEffect;					// Z�l�v���b�g�G�t�F�N�g
	ID3DXEffect* m_cpBlur;					// Z�l�v���b�g�G�t�F�N�g
	D3DXMATRIX m_matWorld;						// ���[���h�ϊ��s��
	D3DXMATRIX m_matView;						// �r���[�ϊ��s��
	D3DXMATRIX m_matProj;						// �ˉe�ϊ��s��
	D3DXHANDLE m_hWorldMat;						// ���[���h�ϊ��s��n���h��
	D3DXHANDLE m_hViewMat;						// �r���[�ϊ��s��n���h��
	D3DXHANDLE m_hProjMat;						// �ˉe�ϊ��s��n���h��
	D3DXHANDLE m_hTechnique;					// �e�N�j�b�N�ւ̃n���h��
	D3DXHANDLE m_hTechniqueBlur;					// �e�N�j�b�N�ւ̃n���h��
	ID3DXSprite* m_pSprite;						//�V���h�E�p�X�v���C�g

	bool m_bBegin;
};

#endif	// _ZTexture_H_
