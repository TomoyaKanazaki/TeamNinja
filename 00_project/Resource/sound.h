//============================================================
//
//	�T�E���h�w�b�_�[ [sound.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//************************************************************
//	�N���X��`
//************************************************************
// �T�E���h�N���X
class CSound
{
public:
	// �R���X�g���N�^
	CSound();

	// �f�X�g���N�^
	~CSound();

	// �T�E���h��
	enum ELabel
	{
		LABEL_NONE = NONE_IDX,		// ���x������
		LABEL_BGM_GENERAL,			// BGM (�ėp)
		LABEL_SE_SELECT_000,		// �I�𑀍쉹00
		LABEL_SE_SELECT_001,		// �I�𑀍쉹01
		LABEL_SE_DECISION_000,		// ���艹00
		LABEL_SE_DECISION_001,		// ���艹01
		LABEL_SE_DECISION_002,		// ���艹02
		LABEL_SE_LAND_S,			// ���n�� (��)
		LABEL_SE_LAND_B,			// ���n�� (��)
		LABEL_SE_STALKWALK_000,		// �ǐՓG�̕�����
		LABEL_SE_CLONEATTACK_000,	// ���g���U�������Ƃ��̉�
		LABEL_SE_STALKATTACK_000,	// �ǐՓG���U�������Ƃ��̉�
		LABEL_SE_STALKFOUND_000,	// �ǐՓG�����������Ƃ��̉�
		LABEL_SE_STALKUPSET_000,	// �ǐՓG�����h�����Ƃ��̉�
		LABEL_SE_STALKSTANCE_000,	// �ǐՓG���\�����Ƃ��̉�
		LABEL_SE_PLAYERWALK_000,	// �v���C���[�̕��s��
		LABEL_SE_PLAYERJUMP_S,		// �v���C���[�̏��W�����v��
		LABEL_SE_PLAYERSTEP_000,	// �v���C���[�̉����
		LABEL_MAX					// ���̗񋓌^�̑���
	};

	// �T�E���h���\����
	struct SSoundInfo
	{
		const char *pFilename;	// �t�@�C����
		int nCntLoop;			// ���[�v�J�E���g
	};

	// �����o�֐�
	HRESULT Init(HWND hWnd);	// ������
	void Uninit(void);			// �I��
	HRESULT LoadAll(HWND hWnd);	// �S�Ǎ�
	HRESULT Play(ELabel label);	// �Đ�
	void Stop(ELabel label);	// ��~
	void Stop(void);			// �S��~
	void SetVolume(const ELabel label, float fVolume);	// ���ʐݒ�
	float GetVolume(const ELabel label) const;			// ���ʎ擾
	void SetFrequency(const ELabel label, float fFreq);	// ���g���ݒ�
	float GetFrequency(const ELabel label) const;		// ���g���擾

	// �ÓI�����o�֐�
	static CSound *Create(HWND hWnd);		// ����
	static void Release(CSound *&pSound);	// �j��

private:
	// �����o�֐�
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);	// �`�����N�`�F�b�N
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// �`�����N�f�[�^�Ǎ�

	// �����o�ϐ�
	IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[LABEL_MAX];						// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[LABEL_MAX];						// �I�[�f�B�I�f�[�^�T�C�Y
};

#endif	// _SOUND_H_
