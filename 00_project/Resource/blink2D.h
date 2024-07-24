//============================================================
//
//	�_�ŃI�u�W�F�N�g2D�w�b�_�[ [blink2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BLINK2D_H_
#define _BLINK2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �_�ŃI�u�W�F�N�g2D�N���X
class CBlink2D : public CObject2D
{
public:
	// �萔
	static constexpr float LEVEL = 1.0f;	// �t�F�[�h���l������

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ�
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_DISP,		// �\��
		STATE_FADEIN,	// �t�F�[�h�C��
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CBlink2D();

	// �f�X�g���N�^
	~CBlink2D() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CBlink2D *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rSize,		// �傫��
		const float fSubIn = LEVEL,		// �C���̃��l������
		const float fAddOut = LEVEL,	// �A�E�g�̃��l������
		const float fMaxWait = 0.0f,	// �]�C����
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_AWHITE		// �F
	);

	// �����o�֐�
	void SetDisp(void);	// �\���ݒ�
	bool IsDisp(void)	{ return (m_state == STATE_DISP); }	// �\���擾
	void SetSubIn(const float fLevel)		{ m_fSubIn = fLevel; }		// ���l�����ʐݒ�
	float GetSubIn(void) const				{ return m_fSubIn; }		// ���l�����ʎ擾
	void SetAddOut(const float fLevel)		{ m_fAddOut = fLevel; }		// ���l�����ʐݒ�
	float GetAddOut(void) const				{ return m_fAddOut; }		// ���l�����ʎ擾
	void SetMaxWait(const float fMaxWait)	{ m_fMaxWait = fMaxWait; }	// �]�C���Ԑݒ�
	float GetMaxWait(void) const			{ return m_fMaxWait; }		// �]�C���Ԏ擾

private:
	// �����o�ϐ�
	EState m_state;		// ���
	float m_fSubIn;		// �C���̃��l������
	float m_fAddOut;	// �A�E�g�̃��l������
	float m_fWaitTime;	// ���݂̗]�C����
	float m_fMaxWait;	// �]�C����
};

#endif	// _BLINK2D_H_
