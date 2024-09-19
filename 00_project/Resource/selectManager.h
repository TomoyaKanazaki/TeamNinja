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
//	�O���錾
//************************************************************
class CRankingManager;	// �����L���O�}�l�[�W���[�N���X
class CTransPoint;		// �J�ڃ|�C���g�N���X

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
	void Update(const float fDeltaTime);		// �X�V
	void SetState(const EState state);			// ��Ԑݒ�
	EState GetState(void) const;				// ��Ԏ擾
	HRESULT SetDispRanking(CTransPoint* pHit);	// �����L���O�\���ݒ�

	// �ÓI�����o�֐�
	static CSelectManager *Create(void);	// ����
	static void Release(CSelectManager *&prSelectManager);	// �j��

private:
	// �����o�ϐ�
	CRankingManager* m_pRanking;	// �����L���O�}�l�[�W���[
	EState m_state;	// ���
};

#endif	// _SELECTMANAGER_H_
