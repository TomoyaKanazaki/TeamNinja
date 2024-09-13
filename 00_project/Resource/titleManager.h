//============================================================
//
//	�^�C�g���}�l�[�W���[�w�b�_�[ [titleManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TITLEMANAGER_H_
#define _TITLEMANAGER_H_

//************************************************************
//	�O���錾
//************************************************************
class CTitleLogo2D;	// �^�C�g�����S2D�N���X
class CBlink2D;		// �_�ŃI�u�W�F�N�g2D�N���X
class CLogoManager;	// ���S�}�l�[�W���[

//************************************************************
//	�N���X��`
//************************************************************
// �^�C�g���}�l�[�W���[�N���X
class CTitleManager
{
public:
	// �萔
	static constexpr int NUM_LOGO = 4;	// ������

	// ��ԗ�
	enum EState
	{
		STATE_LOGO = 0,	// ���S�\��
		STATE_TITLE,	// �^�C�g���\��
		STATE_WAIT,		// ����ҋ@
		STATE_RUN,		// �E�Ҏ���
		STATE_END,		// �I��
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CTitleManager();

	// �f�X�g���N�^
	~CTitleManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(const float fDeltaTime);	// �X�V

	// �ÓI�����o�֐�
	static CTitleManager *Create(void);	// ����
	static void Release(CTitleManager *&prTitleManager);	// �j��

private:
	// �����o�֐�
	void UpdateSkip(void);	// �X�L�b�v����X�V
	void SkipStaging(void);	// ���o�X�L�b�v

	// �����o�ϐ�
	CLogoManager* m_pLogoManager;		// ���S�}�l�[�W���[
	CTitleLogo2D* m_apLogo[NUM_LOGO];	// �^�C�g�����
	CBlink2D* m_pStart;	// �J�n������
	EState m_state;		// ���
	float m_fCurTime;	// ���݂̌o�ߎ���
};

#endif	// _TITLEMANAGER_H_
