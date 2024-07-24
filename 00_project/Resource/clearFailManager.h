//============================================================
//
//	�N���A���s�}�l�[�W���[�w�b�_�[ [clearFailManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _CLEAR_FAIL_MANAGER_H_
#define _CLEAR_FAIL_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "clearManager.h"

//************************************************************
//	�O���錾
//************************************************************
class CAnim2D;		// �A�j���[�V����2D�N���X
class CString2D;	// ������2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �N���A���s�}�l�[�W���[�N���X
class CClearFailManager : public CClearManager
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_WAIT,		// �ҋ@���
		STATE_END,		// �I�����
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CClearFailManager();

	// �f�X�g���N�^
	~CClearFailManager() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void SetAllMove(const D3DXVECTOR3& rMove) override;	// �SUI�I�u�W�F�N�g�ړ�
	void SkipStaging(void) override;					// ���o�X�L�b�v
	bool IsEndState(void) const override { return (m_state == STATE_END); }	// �I���󋵎擾

private:
	// ��ԍX�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CClearFailManager::*AFuncUpdateState)(const float fDeltaTime);

	// �ÓI�����o�ϐ�
	static AFuncUpdateState m_aFuncUpdateState[];	// ��ԍX�V�֐�

	// �����o�֐�
	void UpdateWait(const float fDeltaTime);	// �ҋ@�X�V

	// �����o�ϐ�
	EState m_state;		// ���
	float m_fCurTime;	// ���݂̑ҋ@����
};

#endif	// _CLEAR_FAIL_MANAGER_H_
