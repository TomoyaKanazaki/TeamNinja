//============================================================
//
//	�����L���O�}�l�[�W���[�w�b�_�[ [rankingManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RANKING_MANAGER_H_
#define _RANKING_MANAGER_H_

//************************************************************
//	�O���錾
//************************************************************
class CTransPoint;	// �J�ڃ|�C���g�N���X
class CObject2D;	// �I�u�W�F�N�g2D�N���X
class CString2D;	// ������2D�N���X
class CTimeUI;		// �^�C��UI�N���X
class CAnim2D;		// �A�j���[�V����2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �����L���O�}�l�[�W���[�N���X
class CRankingManager
{
public:
	// �萔
	static constexpr int MAX_RANK = 3;	// �����L���O��ʕ\����

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_FADEOUT,	// �t�F�[�h�A�E�g���
		STATE_SPAWN,	// UI�o�����
		STATE_WAIT,		// �ҋ@���
		STATE_FADEIN,	// �t�F�[�h�C�����
		STATE_FALL,		// UI�������
		STATE_END,		// �I�����
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CRankingManager(CTransPoint* pParent);

	// �f�X�g���N�^
	~CRankingManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(const float fDeltaTime);	// �X�V
	void SetState(const EState state)		{ m_state = state; }	// ��Ԑݒ�
	EState GetState(void) const				{ return m_state; }		// ��Ԏ擾
	bool IsEnd(void) const	{ return (m_state == STATE_END); }		// ���o�I���t���O�擾

	// �ÓI�����o�֐�
	static CRankingManager *Create(CTransPoint* pParent);		// ����
	static void Release(CRankingManager *&prRankingManager);	// �j��
	static int SetRank(const float fNewTime);	// �����L���O�ݒ�

private:
	// �I���
	enum ESelect
	{
		SELECT_YES = 0,	// �͂�
		SELECT_NO,		// ������
		SELECT_MAX		// ���̗񋓌^�̑���
	};

	// ��ԍX�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CRankingManager::*AFuncUpdateState)(const float fDeltaTime);

	// �ÓI�����o�֐�
	static int SortRank(const float fNewTime, float* pRankArray);	// �����L���O�\�[�g
	static HRESULT LoadRank(const char* pPath, float* pRankArray);	// �����L���O�Ǎ�
	static HRESULT SaveRank(const char* pPath, float* pRankArray);	// �����L���O�ۑ�

	// �����o�֐�
	void UpdateFadeOut(const float fDeltaTime);	// �t�F�[�h�A�E�g�X�V
	void UpdateSpawn(const float fDeltaTime);	// UI�o���X�V
	void UpdateWait(const float fDeltaTime);	// �ҋ@�X�V
	void UpdateFadeIn(const float fDeltaTime);	// �t�F�[�h�C���X�V
	void UpdateFall(const float fDeltaTime);	// UI�����X�V
	void UpdateEnd(const float fDeltaTime);		// �I���X�V
	void UpdateSkip(void);	// �X�L�b�v����X�V
	void SkipStaging(void);	// ���o�X�L�b�v

	// �ÓI�����o�ϐ�
	static AFuncUpdateState m_aFuncUpdateState[];	// ��ԍX�V�֐�

	// �����o�ϐ�
	CAnim2D* m_apRankValue[MAX_RANK];	// �����L���O���ʏ��
	CTimeUI* m_apRankTime[MAX_RANK];	// �����L���O���ԏ��
	CString2D* m_apSelect[SELECT_MAX];	// �I�������
	CObject2D* m_pFade;		// �t�F�[�h���
	CObject2D* m_pTitle;	// �^�C�g�����
	CObject2D* m_pBalloon;	// �����o�����
	CString2D* m_pShadow;	// �����L���O�^�C�g���̉e���
	CString2D* m_pName;		// �����L���O�^�C�g�����
	CTransPoint* m_pParent;	// �J�ڃ|�C���g���
	EState m_state;			// ���
	float m_fCurTime;		// ���݂̑ҋ@����
	int m_nCurSelect;		// ���݂̑I����
	int m_nOldSelect;		// �O��̑I����
};

#endif	// _RANKING_MANAGER_H_
