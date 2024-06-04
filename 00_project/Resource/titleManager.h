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
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �^�C�g���}�l�[�W���[�N���X
class CTitleManager
{
public:
	// ���S��
	//enum ELogo
	//{
	//	LOGO_NEVER = 0,	// NEVER���S
	//	LOGO_GIVE,		// GIVE���S
	//	LOGO_UP,		// UP!���S
	//	LOGO_MAX		// ���̗񋓌^�̑���
	//};

	// �X�^�[�g��ԗ�
	enum EStartState
	{
		STARTSTATE_NONE = 0,	// �������Ȃ����
		STARTSTATE_APPEAR,		// �o�����
		STARTSTATE_DISAPPEAR,	// ���ŏ��
		STARTSTATE_SCALE,		// �g����
		STARTSTATE_MAX			// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_FADEOUT,	// �t�F�[�h�A�E�g���
		STATE_SCALE,	// �^�C�g���k�����
		STATE_MOVE,		// �^�C�g���ړ����
		STATE_WAIT,		// �J�ڑҋ@���
		STATE_TRANS,	// �J�ڏ��
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
	void UpdateFade(void);	// �t�F�[�h�A�E�g
	void UpdateScale(void);	// �^�C�g���g��
	void UpdateMove(void);	// �^�C�g���ړ�
	void UpdateStart(void);	// �J�ڌ���
	void UpdateBlink(void);	// �X�^�[�g�_��
	void UpdateTrans(void);	// �J��
	void SkipStaging(void);	// ���o�X�L�b�v

	// �����o�ϐ�
	CObject2D *m_pLogo;		// �^�C�g���\���̏��
	CObject2D *m_pFade;		// �t�F�[�h�̏��
	CObject2D* m_pStart;	// �X�^�[�g�̏��
	EState m_state;		// ���
	EStartState m_startState;	// �X�^�[�g�̏��
	float m_fScale;		// �^�C�g���g�嗦
	float m_fMove;		// �^�C�g�����S�̈ړ���
	int m_nSelect;		// ���݂̑I��
	int m_nOldSelect;	// �O��̑I��
	int m_nWaitCounter;	// �^�C�g���ړ��܂ł̑ҋ@����
};

#endif	// _TITLEMANAGER_H_
