//============================================================
//
//	���S�}�l�[�W���[�w�b�_�[ [logoManager.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LOGOMANAGER_H_
#define _LOGOMANAGER_H_

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;		// �I�u�W�F�N�g2D

//************************************************************
//	�N���X��`
//************************************************************
// ���S�}�l�[�W���[�N���X
class CLogoManager
{
public:

	// �|���S���̎��
	enum EPoly
	{
		POLYGON_BACK = 0,	// �w�i
		POLYGON_LOGO,		// ���S
		POLYGON_MAX			// ���̗񋓌^�̑���
	};

	// ���
	enum EState
	{
		STATE_APPEAR = 0,	// �o�����
		STATE_WAIT,			// �ҋ@���
		STATE_DISAPPEAR,	// ���ŏ��
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CLogoManager();

	// �f�X�g���N�^
	~CLogoManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(const float fDeltaTime);	// �X�V

	// �ÓI�����o�֐�
	static CLogoManager* Create(void);	// ����
	static void Release(CLogoManager*& prResultManager);	// �j��

private:

	// �����o�֐�
	void UpdateAppear(void);	// �o����ԏ���
	void UpdateWait(void);		// �ҋ@��ԏ���
	void UpdateSkip(void);		// �X�L�b�v����X�V

	// �����o�ϐ�
	CObject2D* m_apLogo[POLYGON_MAX];	// ���S���
	EState m_state;						// ���
	int m_nStateCount;					// ��ԃJ�E���g
};

#endif	// _RESULTMANAGER_H_
