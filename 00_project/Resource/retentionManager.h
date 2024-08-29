//============================================================
//
//	�f�[�^�ۑ��}�l�[�W���[�w�b�_�[ [retentionManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RETENTION_MANAGER_H_
#define _RETENTION_MANAGER_H_

//************************************************************
//	�N���X��`
//************************************************************
// �f�[�^�ۑ��}�l�[�W���[�N���X
class CRetentionManager
{
public:
	// ������
	enum EWin
	{
		WIN_NONE,		// ����
		WIN_FAIL,		// �������s
		WIN_SUCCESS,	// ��������
		WIN_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CRetentionManager();

	// �f�X�g���N�^
	~CRetentionManager();

	// ���U���g���\����
	struct SResult
	{
		EWin win;		// ������
		float fTime;	// �o�߃^�C��
		int nSave;		// �Ō�̃Z�[�u�|�C���g
	};

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��

	// �ÓI�����o�֐�
	static CRetentionManager *Create(void);	// ����
	static void Release(CRetentionManager *&prRetentionManager);	// �j��

	// �����o�֐�
	void SetResult(const EWin win, const float fTime, const int nSave);	// ���U���g���ݒ�
	void SetTransIdx(const int nIdx) { m_nTransIdx = nIdx; }	// �J�ڃ|�C���g�C���f�b�N�X�ݒ�
	int GetTransIdx(void) const		 { return m_nTransIdx; }	// �J�ڃ|�C���g�C���f�b�N�X�擾
	EWin GetWin(void) const		{ return m_result.win; }		// �����󋵎擾
	float GetTime(void) const	{ return m_result.fTime; }		// �o�߃^�C���擾
	int GetSave(void) const		{ return m_result.nSave; }		// �Z�[�u�|�C���g�擾
	void InitSave(void)			{ m_result.nSave = NONE_IDX; }	// �Z�[�u�|�C���g������

private:
	
	// �����o�ϐ�
	SResult m_result;	// ���U���g���
	int m_nTransIdx;	// �J�ڃ|�C���g�C���f�b�N�X

};

#endif	// _RETENTION_MANAGER_H_
