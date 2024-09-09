//============================================================
//
//	�u���[2D�w�b�_�[ [blur2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BLUR2D_H_
#define _BLUR2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CAnim2D;	// �A�j���[�V����2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �u���[2D�N���X
class CBlur2D : public CObject
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_VANISH,	// �������
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CBlur2D();

	// �f�X�g���N�^
	~CBlur2D();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CBlur2D *Create	// ����
	( // ����
		CAnim2D *pParent,			// �e�I�u�W�F�N�g
		const D3DXCOLOR& rCol,		// �u���[���f�F
		const float fStartAlpha,	// �u���[�J�n�����x
		const int nMaxLength,		// �ێ��I�u�W�F�N�g�ő吔
		const int nTextureID		// �e�N�X�`���C���f�b�N�X
	);

	// �����o�֐�
	void SetState(const EState state);			// ��Ԑݒ�
	EState GetState(void) const					{ return m_state; }	// ��Ԏ擾
	void SetColor(const D3DXCOLOR& rCol)		{ m_col = rCol; }	// �u���[���f�F�ݒ�
	D3DXCOLOR GetColor(void) const				{ return m_col; }	// �u���[���f�F�擾
	void SetStartAlpha(const float fStartAlpha)	{ m_fStartAlpha = fStartAlpha; }	// �u���[�̊J�n�����x�ݒ�
	float GetStartAlpha(void) const				{ return m_fStartAlpha; }			// �u���[�̊J�n�����x�擾
	void SetMaxLength(const int nMaxLength)		{ m_nMaxLength = nMaxLength; }		// �ێ��I�u�W�F�N�g���ݒ�
	int GetMaxLength(void) const				{ return m_nMaxLength; }			// �ێ��I�u�W�F�N�g���擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override { CObject::Release(); }	// �j��

	// �����o�ϐ�
	std::vector<CAnim2D*> m_oldObject;	// �ߋ��I�u�W�F�N�g���
	CAnim2D *m_pParent;			// �e�I�u�W�F�N�g
	D3DXCOLOR m_col;			// �u���[���f�F
	float	m_fStartAlpha;		// �u���[�̊J�n�����x
	int		m_nTextureID;		// �u���[�e�N�X�`���C���f�b�N�X
	int		m_nMaxLength;		// �ێ�����e�I�u�W�F�N�g�̍ő吔
	EState	m_state;			// ���
	int		m_nCounterState;	// ��ԊǗ��J�E���^�[

};

#endif	// _BLUR2D_H_
