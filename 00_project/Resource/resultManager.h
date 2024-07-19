//============================================================
//
//	���U���g�}�l�[�W���[�w�b�_�[ [resultManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���U���g�}�l�[�W���[�N���X
class CResultManager
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,			// �������Ȃ����
		STATE_FADEWAIT,			// �t�F�[�h�ҋ@���
		STATE_FADEIN,			// �t�F�[�h�C�����
		STATE_FADEIN_WAIT,		// �t�F�[�h�C���ҋ@���
		STATE_FADEIN_ACCEL,		// �t�F�[�h�C���������
		STATE_WAIT,				// �ҋ@���
		STATE_FADEOUT,			// �t�F�[�h�A�E�g���
		STATE_FADEOUT_WAIT,		// �t�F�[�h�A�E�g�ҋ@���
		STATE_FADEOUT_ACCEL,	// �t�F�[�h�A�E�g�������
		STATE_END,				// �I�����
		STATE_MAX				// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CResultManager();

	// �f�X�g���N�^
	~CResultManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(const float fDeltaTime);	// �X�V
	void SetState(const EState state)	{ m_state = state; }	// ��Ԑݒ�
	EState GetState(void) const			{ return m_state; }		// ��Ԏ擾

	// �ÓI�����o�֐�
	static CResultManager *Create(void);	// ����
	static void Release(CResultManager *&prResultManager);	// �j��

private:
	// �����o�֐�
	void UpdateFadeWait(const float fDeltaTime);		// �t�F�[�h�ҋ@�X�V
	void UpdateFadeIn(const float fDeltaTime);			// �t�F�[�h�C���X�V
	void UpdateFadeInWait(const float fDeltaTime);		// �t�F�[�h�C���ҋ@�X�V
	void UpdateFadeInAccel(const float fDeltaTime);		// �t�F�[�h�C�������X�V
	void UpdateWait(const float fDeltaTime);			// �ҋ@�X�V
	void UpdateFadeOut(const float fDeltaTime);			// �t�F�[�h�A�E�g�X�V
	void UpdateFadeOutWait(const float fDeltaTime);		// �t�F�[�h�A�E�g�ҋ@�X�V
	void UpdateFadeOutAccel(const float fDeltaTime);	// �t�F�[�h�A�E�g�����X�V

	// �����o�ϐ�
	CObject2D *m_pFade;	// �t�F�[�h���
	EState m_state;		// ���
	float m_fMoveY;		// �c�ړ���
	float m_nCurTime;	// ���݂̑ҋ@����
};

#endif	// _RESULTMANAGER_H_
