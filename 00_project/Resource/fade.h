//============================================================
//
//	�t�F�[�h�w�b�_�[ [fade.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FADE_H_
#define _FADE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scene.h"
#include "object2D.h"
#include "objectCircle2D.h"

//************************************************************
//	�O���錾
//************************************************************
class CFadeState;	// �t�F�[�h��ԃN���X

//************************************************************
//	�N���X��`
//************************************************************
// �t�F�[�h�N���X
class CFade : public CObject2D
{
public:
	// �萔
	static constexpr int	PRIORITY = 7;	// �t�F�[�h�D�揇��
	static constexpr float	LEVEL = 1.0f;	// �t�F�[�h���l������

	// �R���X�g���N�^
	CFade();

	// �f�X�g���N�^
	~CFade();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CFade *Create(void);	// ����

	// �����o�֐�
	HRESULT ChangeState(CFadeState *pState);	// ��ԕύX
	bool SubAlpha(const float fDeltaTime);		// ���l����
	bool AddAlpha(const float fDeltaTime);		// ���l����
	void TransNextMode(void);	// ���V�[���J��
	bool IsFade(void);			// �t�F�[�h�����̔���擾
	bool IsFadeIn(void);		// �t�F�[�h�C�������̔���擾

	void SetFade	// �t�F�[�h�J�n�ݒ�
	( // ����
		const float fAddOut	= LEVEL,		// �A�E�g�̃��l������
		const float fSubIn	= LEVEL,		// �C���̃��l������
		const int nPriority	= PRIORITY,		// �D�揇��
		const D3DXCOLOR& rCol = XCOL_ABLACK	// �F
	);
	void SetModeFade	// ���V�[���ݒ� (�t�F�[�h�̂�)
	( // ����
		const CScene::EMode mode,			// ���V�[��
		const float fWaitTime	= 0.0f,		// �]�C����
		const float fAddOut		= LEVEL,	// �A�E�g�̃��l������
		const float fSubIn		= LEVEL		// �C���̃��l������
	);
	void SetLoadFade	// ���V�[���ݒ� (���[�h��ʕt��)
	( // ����
		const CScene::EMode mode,			// ���V�[��
		const float fWaitTime	= 0.0f,		// �]�C����
		const float fAddOut		= LEVEL,	// �A�E�g�̃��l������
		const float fSubIn		= LEVEL		// �C���̃��l������
	);

	void SetCropPosition(const D3DXVECTOR3& rPos)	{ m_pCrop->SetVec3Position(rPos); }		// �؂蔲���^�ʒu�ݒ�
	D3DXVECTOR3 GetCropPosition(void) const			{ return m_pCrop->GetVec3Position(); }	// �؂蔲���^�ʒu�擾
	void SetCropRadius(const float fRadius)			{ m_pCrop->SetRadius(fRadius); }		// �؂蔲���^���a�ݒ�
	float GetCropRadius(void)						{ return m_pCrop->GetRadius(); }		// �؂蔲���^���a�擾
	float CalcCropRadius(const D3DXVECTOR3& rPos);	// �؂蔲�����a�v�Z

private:
	// �����o�ϐ�
	std::function<HRESULT(CScene::EMode)> m_pFuncSetMode;	// ���[�h�ݒ�֐��|�C���^
	CObjectCircle2D *m_pCrop;	// �؂蔲���^���
	CFadeState *m_pState;		// ���
	CScene::EMode m_modeNext;	// ���V�[��
	float m_fSubIn;		// �C���̃��l������
	float m_fAddOut;	// �A�E�g�̃��l������
};

#endif	// _FADE_H_
