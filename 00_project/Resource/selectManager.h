//============================================================
//
//	�Z���N�g�}�l�[�W���[�w�b�_�[ [selectManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SELECTMANAGER_H_
#define _SELECTMANAGER_H_

//************************************************************
//	�N���X��`
//************************************************************
// �Z���N�g�}�l�[�W���[�N���X
class CSelectManager
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CSelectManager();

	// �f�X�g���N�^
	~CSelectManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(const float fDeltaTime);	// �X�V
	void SetState(const EState state);		// ��Ԑݒ�
	EState GetState(void) const;			// ��Ԏ擾

	// �ÓI�����o�֐�
	static CSelectManager *Create(void);	// ����
	static void Release(CSelectManager *&prSelectManager);		// �j��

private:
	// �����o�ϐ�
	EState m_state;	// ���
};

#endif	// _SELECTMANAGER_H_
