//============================================================
//
//	�g�D�[���V���h�E�w�b�_�[ [ToonShadow.h]
//	Author�F�O�엳�V��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TOON_SHADOW_H_
#define _TOON_SHADOW_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shader.h"

//************************************************************
//	�N���X��`
//************************************************************
// �g�D�[���V�F�[�_�[�N���X
class CToonShadow : public CShader
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_TOON = 0,	// �g�D�[���}�b�v�e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CToonShadow();

	// �f�X�g���N�^
	~CToonShadow();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);		// �I��

	// �����o�֐�
	void SetTexture(const CTexture::STexture& rTexture);	// �e�N�X�`���ݒ� (�|�C���^)
	void SetTexture(const int nTextureID);					// �e�N�X�`���ݒ� (�C���f�b�N�X)
	void SetMaterial(const D3DMATERIAL9& rMaterial);		// �}�e���A���ݒ�
	void SetDiffuse(const D3DXCOLOR& rDiffuse);				// �g�U���ݒ�
	void SetAmbient(const D3DXCOLOR& rAmbient);				// �����ݒ�
	void SetEmissive(const D3DXCOLOR& rEmissive);			// ���ˌ��ݒ�
	
	void SetOnlyDiffuse(const D3DXCOLOR& rDiffuse);			// �g�U���̂ݐݒ�

	void SetFog(const D3DXCOLOR& rColor, const float& rNear, const float& rFar);	//�t�H�O�p�����[�^�܂Ƃ߂Đݒ�
	void SetFogColor(const D3DXCOLOR& rColor);				//�t�H�O�F
	void SetFogNear(const float& rNear);					//�t�H�O�J�n����
	void SetFogFar(const float& rFar);						//�t�H�O�I������

	void SetShadowMap();									//�V���h�E�}�b�s���O�p�̃f�[�^�ݒ�

	// �ÓI�����o�֐�
	static CToonShadow* Create(void);		// ����
	static CToonShadow* GetInstance(void);	// �擾
	static void Release(void);				// �j��

private:
	// �����o�֐�
	void SetToonMapTexture(const ETexture texture);	// �g�D�[���}�b�v�e�N�X�`���ݒ�

	// �ÓI�����o�ϐ�
	static CToonShadow* m_pShader;	// �V�F�[�_�[���

	// �����o�ϐ�
	D3DXHANDLE m_hTextureToon;	// �g�D�[���}�b�v�e�N�X�`��
	D3DXHANDLE m_hDirectLight;	// ���s�����̕����x�N�g��
	D3DXHANDLE m_hDiffuse;		// �g�U��
	D3DXHANDLE m_hAmbient;		// ����
	D3DXHANDLE m_hEmissive;		// ���ˌ�

	D3DXHANDLE m_hEyePos;		// ���_���W
	D3DXHANDLE m_hLightViewMat;	// ���C�g�r���[�ϊ��s��n���h��
	D3DXHANDLE m_hLightProjMat;	// ���C�g�ˉe�ϊ��s��n���h��
	D3DXHANDLE m_hShadowMapTex;	// �V���h�E�}�b�v�e�N�X�`���n���h��
	
	D3DXHANDLE m_hFogColor;		//�t�H�O�F
	D3DXHANDLE m_hFogNear;		//�t�H�O�J�n����
	D3DXHANDLE m_hFogFar;		//�t�H�O�I������
};

#endif	// _TOON_SHADOW_H_
