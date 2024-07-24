//============================================================
//
//	�N���A�����}�l�[�W���[�w�b�_�[ [clearSuccessManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _CLEAR_SUCCESS_MANAGER_H_
#define _CLEAR_SUCCESS_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "clearManager.h"
#include "stage.h"

//************************************************************
//	�O���錾
//************************************************************
class CAnim2D;			// �A�j���[�V����2D�N���X
class CString2D;		// ������2D�N���X
class CScrollText2D;	// ��������e�L�X�g2D�N���X
class CTimeUI;			// �^�C��UI�N���X
class CBlink2D;			// �_�ŃI�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �N���A�����}�l�[�W���[�N���X
class CClearSuccessManager : public CClearManager
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,			// �������Ȃ����
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
		STATE_END,				// �I�����
		STATE_MAX				// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CClearSuccessManager();

	// �f�X�g���N�^
	~CClearSuccessManager() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void SetAllMove(const D3DXVECTOR3& rMove) override;	// �SUI�I�u�W�F�N�g�ړ�
	void SkipStaging(void) override;					// ���o�X�L�b�v
	bool IsEndState(void) const override { return (m_state == STATE_END); }	// �I���󋵎擾

private:
	// ��ԍX�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CClearSuccessManager::*AFuncUpdateState)(const float fDeltaTime);

	// �ÓI�����o�ϐ�
	static AFuncUpdateState m_aFuncUpdateState[];	// ��ԍX�V�֐�

	// �����o�֐�
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

	// �����o�ϐ�
	CAnim2D* m_apGodItemBG[CStage::GOD_MAX];	// �_��A�C�R���w�i���
	CAnim2D* m_apGodItemIcon[CStage::GOD_MAX];	// �_��A�C�R�����
	CString2D* m_pTime;		// ���s���ԃ^�C�g�����
	CTimeUI* m_pTimeVal;	// ���s���ԏ��
	CString2D* m_pGodItem;	// �_��^�C�g�����
	CBlink2D* m_pControl;	// ������
	EState m_state;			// ���
	float m_fCurTime;		// ���݂̑ҋ@����
};

#endif	// _CLEAR_SUCCESS_MANAGER_H_
