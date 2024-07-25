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
class CClearManager;	// �N���A�}�l�[�W���[�N���X
class CObject2D;		// �I�u�W�F�N�g2D�N���X
class CScrollText2D;	// ��������e�L�X�g2D�N���X

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
		STATE_FADE_WAIT,		// �t�F�[�h�ҋ@���
		STATE_FADEIN,			// �t�F�[�h�C�����
		STATE_FADEIN_WAIT,		// �t�F�[�h�C���ҋ@���
		STATE_FADEIN_ACCEL,		// �t�F�[�h�C���������
		STATE_STAGE_WAIT,		// �X�e�[�W�\���ҋ@���
		STATE_STAGE_TITLE,		// �X�e�[�W�^�C�g�����
		STATE_STAMP,			// �n���R�������
		STATE_CLEAR,			// �N���A�}�l�[�W���[�X�V���
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
	// ��ԍX�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CResultManager::*AFuncUpdateState)(const float fDeltaTime);

	// �ÓI�����o�ϐ�
	static AFuncUpdateState m_aFuncUpdateState[];	// ��ԍX�V�֐�

	// �����o�֐�
	void UpdateFadeWait(const float fDeltaTime);		// �t�F�[�h�ҋ@�X�V
	void UpdateFadeIn(const float fDeltaTime);			// �t�F�[�h�C���X�V
	void UpdateFadeInWait(const float fDeltaTime);		// �t�F�[�h�C���ҋ@�X�V
	void UpdateFadeInAccel(const float fDeltaTime);		// �t�F�[�h�C�������X�V
	void UpdateStageWait(const float fDeltaTime);		// �X�e�[�W�\���ҋ@�X�V
	void UpdateStageTitle(const float fDeltaTime);		// �X�e�[�W�^�C�g���X�V
	void UpdateStamp(const float fDeltaTime);			// �n���R�����X�V
	void UpdateClear(const float fDeltaTime);			// �N���A�}�l�[�W���[�X�V
	void UpdateFadeOut(const float fDeltaTime);			// �t�F�[�h�A�E�g�X�V
	void UpdateFadeOutWait(const float fDeltaTime);		// �t�F�[�h�A�E�g�ҋ@�X�V
	void UpdateFadeOutAccel(const float fDeltaTime);	// �t�F�[�h�A�E�g�����X�V
	void UpdateEnd(const float fDeltaTime);				// �I���X�V
	void SetAllMove(const D3DXVECTOR3& rMove);	// �SUI�I�u�W�F�N�g�ړ�
	void SkipStaging(void);	// ���o�X�L�b�v

	// �����o�ϐ�
	CClearManager* m_pClear;	// �N���A�}�l�[�W���[���
	CObject2D* m_pFade;			// �t�F�[�h���
	CScrollText2D* m_pTitle;	// �^�C�g�����
	CObject2D* m_pStamp;		// �n���R���
	EState m_state;				// ���
	float m_fMoveY;				// �c�ړ���
	float m_fCurTime;			// ���݂̑ҋ@����
};

#endif	// _RESULTMANAGER_H_
