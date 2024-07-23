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
//	�C���N���[�h�t�@�C��
//************************************************************
#include "stage.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;		// �I�u�W�F�N�g2D�N���X
class CAnim2D;			// �A�j���[�V����2D�N���X
class CString2D;		// ������2D�N���X
class CScrollText2D;	// ��������e�L�X�g2D�N���X
class CTimeUI;			// �^�C��UI�N���X

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
		STATE_CLEAR_STAMP,		// �N���A�n���R�������
		STATE_TIME_TITLE_WAIT,	// ���s���ԃ^�C�g���ҋ@���
		STATE_TIME_TITLE,		// ���s���ԃ^�C�g���\�����
		STATE_TIME_VALUE_WAIT,	// ���s���ԑҋ@���
		STATE_TIME_VALUE,		// ���s���ԕ\�����
		STATE_ITEM_TITLE_WAIT,	// �_��^�C�g���ҋ@���
		STATE_ITEM_TITLE,		// �_��^�C�g���\�����
		STATE_ITEM_BG_WAIT,		// �_��A�C�R���w�i�ҋ@���
		STATE_ITEM_BG,			// �_��A�C�R���w�i�\�����
		STATE_ITEM_ICON_WAIT,	// �_��A�C�R���ҋ@���
		STATE_ITEM_ICON,		// �_��A�C�R���\�����
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
	void UpdateStamp(const float fDeltaTime);			// �N���A�n���R�����X�V
	void UpdateTimeTitleWait(const float fDeltaTime);	// ���s���ԃ^�C�g���ҋ@�X�V
	void UpdateTimeTitle(const float fDeltaTime);		// ���s���ԃ^�C�g���\���X�V
	void UpdateTimeValueWait(const float fDeltaTime);	// ���s���ԑҋ@�X�V
	void UpdateTimeValue(const float fDeltaTime);		// ���s���ԕ\���X�V
	void UpdateItemTitleWait(const float fDeltaTime);	// �_��^�C�g���ҋ@�X�V
	void UpdateItemTitle(const float fDeltaTime);		// �_��^�C�g���\���X�V
	void UpdateItemIconBgWait(const float fDeltaTime);	// �_��A�C�R���w�i�ҋ@�X�V
	void UpdateItemIconBg(const float fDeltaTime);		// �_��A�C�R���w�i�\���X�V
	void UpdateItemIconWait(const float fDeltaTime);	// �_��A�C�R���ҋ@�X�V
	void UpdateItemIcon(const float fDeltaTime);		// �_��A�C�R���\���X�V
	void UpdateWait(const float fDeltaTime);			// �ҋ@�X�V
	void UpdateFadeOut(const float fDeltaTime);			// �t�F�[�h�A�E�g�X�V
	void UpdateFadeOutWait(const float fDeltaTime);		// �t�F�[�h�A�E�g�ҋ@�X�V
	void UpdateFadeOutAccel(const float fDeltaTime);	// �t�F�[�h�A�E�g�����X�V
	void UpdateEnd(const float fDeltaTime);				// �I���X�V

	// �����o�ϐ�
	CAnim2D* m_apGodItemBG[CStage::GOD_MAX];	// �_��A�C�R���w�i���
	CAnim2D* m_apGodItemIcon[CStage::GOD_MAX];	// �_��A�C�R�����
	CObject2D* m_pFade;			// �t�F�[�h���
	CScrollText2D* m_pTitle;	// �^�C�g�����
	CObject2D* m_pStamp;		// �n���R���
	CString2D* m_pTime;			// ���s���ԃ^�C�g�����
	CTimeUI* m_pTimeVal;		// ���s���ԏ��
	CString2D* m_pGodItem;		// �_��^�C�g�����
	EState m_state;				// ���
	float m_fMoveY;				// �c�ړ���
	float m_fCurTime;			// ���݂̑ҋ@����
};

#endif	// _RESULTMANAGER_H_
