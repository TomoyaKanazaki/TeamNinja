#if 0
//============================================================
//
//	�g�k�I�u�W�F�N�g2D�w�b�_�[ [scale2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCALE2D_H_
#define _SCALE2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �g�k�I�u�W�F�N�g2D�N���X
class CScale2D : public CObject2D
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
		STATE_SCALE,	// �g�k
		STATE_FADEIN,	// �t�F�[�h�C��
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CScale2D();

	// �f�X�g���N�^
	~CScale2D() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CScale2D *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rOriginSize,	// ���_�傫��
		const float fMinScale = 0.0f,	// �Œ�g�嗦
		const float fMaxScale = 1.0f,	// �ő�g�嗦
		const float fInitScale = 0.0f,	// �����g�嗦
		const float fCalcScale = ADD_ROT,		// �g�嗦�̉�����
		const float fSubIn = LEVEL,				// �g�k�O�̃C���̊g�嗦������
		const float fAddOut = LEVEL,			// �g�k��̃A�E�g�̊g�嗦������
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_AWHITE		// �F
	);

	// �����o�֐�
	void SetScale(const bool bScale);	// �g�k�ݒ�
	bool IsScale(void) const { return (m_state == STATE_SCALE); }	// �g�k�擾

	void SetOriginSizing(const D3DXVECTOR3& rSize)	{ m_sizeOrigin = rSize; }	// ���_�傫���ݒ�
	D3DXVECTOR3 GetOriginSizing(void) const			{ return m_sizeOrigin; }	// ���_�傫���擾
	void SetCalcScale(const float fCalc)	{ m_fAddSinRot = fCalc; }	// �g�k�����̉��Z�ʐݒ�
	float GetCalcScale(void) const			{ return m_fAddSinRot; }	// �g�k�����̉��Z�ʎ擾
	void SetMinScale(const float fMin)		{ m_fMinScale = fMin; }		// �Œ�g�嗦�ݒ�
	float GetMinScale(void) const			{ return m_fMinScale; }		// �Œ�g�嗦�擾
	void SetMaxScale(const float fMax)		{ m_fMaxScale = fMax; }		// �ő�g�嗦�ݒ�
	float GetMaxScale(void) const			{ return m_fMaxScale; }		// �ő�g�嗦�擾
	void SetInitScale(const float fInit)	{ m_fInitScale = fInit; }	// �����g�嗦�ݒ�
	float GetInitScale(void) const			{ return m_fInitScale; }	// �����g�嗦�擾
	void SetSubIn(const float fLevel)		{ m_fSubIn = fLevel; }		// ���l�����ʐݒ�
	float GetSubIn(void) const				{ return m_fSubIn; }		// ���l�����ʎ擾
	void SetAddOut(const float fLevel)		{ m_fAddOut = fLevel; }		// ���l�����ʐݒ�
	float GetAddOut(void) const				{ return m_fAddOut; }		// ���l�����ʎ擾

private:
	// �����o�ϐ�
	D3DXVECTOR3 m_sizeOrigin;	// ���_�傫��
	EState m_state;		// ���
	float m_fSinScale;	// �g�k����
	float m_fAddSinRot;	// �g�k�����̉��Z��
	float m_fCurScale;	// ���݊g�嗦
	float m_fMinScale;	// �Œ�g�嗦
	float m_fMaxScale;	// �ő�g�嗦
	float m_fInitScale;	// �����g�嗦
	float m_fSubIn;		// �C���̊g�嗦������
	float m_fAddOut;	// �A�E�g�̊g�嗦������
};

#endif	// _SCALE2D_H_
#endif
