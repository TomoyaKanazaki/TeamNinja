//============================================================
//
//	�^�C�g�����S2D�w�b�_�[ [titleLogo2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TITLE_LOGO2D_H_
#define _TITLE_LOGO2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "anim2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �^�C�g�����S2D�N���X
class CTitleLogo2D : public CAnim2D
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,		// �������Ȃ�
		STATE_MOVE_WAIT,	// �ړ��ҋ@
		STATE_MOVE,			// �ړ�
		STATE_AURA_WAIT,	// �I�[���ҋ@
		STATE_AURA,			// �I�[��
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CTitleLogo2D();

	// �f�X�g���N�^
	~CTitleLogo2D() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// �傫���ݒ�
	void SetPattern(const int nPattern) override;			// �p�^�[���̐ݒ�
	void SetWidthPattern(const int nWidthPtrn) override;	// �e�N�X�`���̉��������̐ݒ�
	void SetHeightPattern(const int nHeightPtrn) override;	// �e�N�X�`���̏c�������̐ݒ�

	// �ÓI�����o�֐�
	static CTitleLogo2D *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rOffset,		// �I�t�Z�b�g
		const D3DXVECTOR3& rSize,		// �傫��
		const float fMoveTime = 1.0f,	// �ړ�����
		const float fWaitTime = 1.0f	// �ҋ@����
	);

	// �����o�֐�
	void SetStag(void)		{ m_state = STATE_MOVE_WAIT; }		// ���o�J�n�ݒ�
	bool IsStag(void) const	{ return (m_state != STATE_NONE); }	// ���o���t���O�擾
	void BindAuraTexture(const char *pTexturePass)	{ m_pAura->BindTexture(pTexturePass); }	// �e�N�X�`������ (�C���f�b�N�X)
	void BindAuraTexture(const int nTextureID)		{ m_pAura->BindTexture(nTextureID); }	// �e�N�X�`������ (�p�X)

private:
	// �����o�֐�
	void UpdateMoveWait(const float fDeltaTime);	// �ړ��ҋ@�X�V
	void UpdateMove(const float fDeltaTime);		// �ړ��X�V
	void UpdateAuraWait(const float fDeltaTime);	// �I�[���ҋ@�X�V
	void UpdateAura(const float fDeltaTime);		// �I�[���X�V

	// �����o�ϐ�
	CAnim2D* m_pAura;		// �I�[�����
	EState m_state;			// ���
	float m_fMoveTime;		// �ړ�����
	float m_fWaitTime;		// �ҋ@����
	float m_fCurTime;		// ���݂̑ҋ@����
	D3DXVECTOR3 m_offset;	// �����ʒu�I�t�Z�b�g
	D3DXVECTOR3 m_initPos;	// �����ʒu
	D3DXVECTOR3 m_destPos;	// �ڕW�ʒu
};

#endif	// _TITLE_LOGO2D_H_
