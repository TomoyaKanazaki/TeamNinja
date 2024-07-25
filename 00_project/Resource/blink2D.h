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
	static constexpr float ADD_ROT = 2.0f;	// �����̉��Z��
	static constexpr float LEVEL = 1.0f;	// �t�F�[�h���l������

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ�
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_BLINK,	// �_��
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
		const float fMinAlpha = 0.0f,	// �Œᓧ���x
		const float fMaxAlpha = 1.0f,	// �ő哧���x
		const float fCalcAlpha = ADD_ROT,		// �����x�̉�����
		const float fSubIn = LEVEL,				// �_�őO�̃C���̃��l������
		const float fAddOut = LEVEL,			// �_�Ō�̃A�E�g�̃��l������
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_AWHITE		// �F
	);

	// �����o�֐�
	void SetBlink(const bool bBlink);		// �_�Őݒ�
	bool IsBlink(void)	{ return (m_state == STATE_BLINK); }	// �_�Ŏ擾
	void SetCalcAlpha(const float fCalc)	{ m_fAddSinRot = fCalc; }	// ���������̉��Z�ʐݒ�
	float GetCalcAlpha(void) const			{ return m_fAddSinRot; }	// ���������̉��Z�ʎ擾
	void SetMinAlpha(const float fMin)		{ m_fMinAlpha = fMin; }		// �Œᓧ���x�ݒ�
	float GetMinAlpha(void) const			{ return m_fMinAlpha; }		// �Œᓧ���x�擾
	void SetMaxAlpha(const float fMax)		{ m_fMaxAlpha = fMax; }		// �ő哧���x�ݒ�
	float GetMaxAlpha(void) const			{ return m_fMaxAlpha; }		// �ő哧���x�擾
	void SetSubIn(const float fLevel)		{ m_fSubIn = fLevel; }		// ���l�����ʐݒ�
	float GetSubIn(void) const				{ return m_fSubIn; }		// ���l�����ʎ擾
	void SetAddOut(const float fLevel)		{ m_fAddOut = fLevel; }		// ���l�����ʐݒ�
	float GetAddOut(void) const				{ return m_fAddOut; }		// ���l�����ʎ擾

private:
	// �����o�ϐ�
	EState m_state;		// ���
	float m_fSinAlpha;	// ��������
	float m_fAddSinRot;	// ���������̉��Z��
	float m_fMinAlpha;	// �Œᓧ���x
	float m_fMaxAlpha;	// �ő哧���x
	float m_fSubIn;		// �C���̃��l������
	float m_fAddOut;	// �A�E�g�̃��l������
};

#endif	// _BLINK2D_H_
