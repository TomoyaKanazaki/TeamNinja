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
//	�N���X��`
//************************************************************
// ���U���g�}�l�[�W���[�N���X
class CResultManager
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_MAX		// ���̗񋓌^�̑���
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
	// �����o�ϐ�
	EState m_state;	// ���
};

#endif	// _RESULTMANAGER_H_
