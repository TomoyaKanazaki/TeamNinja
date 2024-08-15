//============================================================
//
//	�_��l�����o�}�l�[�W���[�w�b�_�[ [godItemManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GODITEM_MANAGER_H_
#define _GODITEM_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;		// �I�u�W�F�N�g2D�N���X
class CScrollString2D;	// �������蕶����2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �_��l�����o�}�l�[�W���[�N���X
class CGodItemManager : public CObject
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_FADEOUT,	// �t�F�[�h�A�E�g���
		STATE_LINE,		// �����o�����
		STATE_TITLE,	// �^�C�g���o�����
		STATE_NAME,		// ���O�o�����
		STATE_WAIT,		// �ҋ@���
		STATE_FADEIN,	// �t�F�[�h�C�����
		STATE_FALL,		// UI�������
		STATE_END,		// �I�����
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CGodItemManager();

	// �f�X�g���N�^
	~CGodItemManager() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �����o�֐�
	void SetState(const EState state)	{ m_state = state; }	// ��Ԑݒ�
	EState GetState(void) const			{ return m_state; }		// ��Ԏ擾

	// �ÓI�����o�֐�
	static CGodItemManager *Create(void);	// ����

private:
	// ��ԍX�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CGodItemManager::*AFuncUpdateState)(const float fDeltaTime);

	// �ÓI�����o�ϐ�
	static AFuncUpdateState m_aFuncUpdateState[];	// ��ԍX�V�֐�
	static CGodItemManager* m_pThisClass;			// ���g�̃C���X�^���X

	// �I�[�o�[���C�h�֐�
	void Release(void) override { CObject::Release(); }	// �j��

	// �����o�֐�
	void UpdateFadeOut(const float fDeltaTime);	// �t�F�[�h�A�E�g�X�V
	void UpdateLine(const float fDeltaTime);	// �����o���X�V
	void UpdateTitle(const float fDeltaTime);	// �^�C�g���o���X�V
	void UpdateName(const float fDeltaTime);	// ���O�o���X�V
	void UpdateWait(const float fDeltaTime);	// �ҋ@�X�V
	void UpdateFadeIn(const float fDeltaTime);	// �t�F�[�h�C���X�V
	void UpdateFall(const float fDeltaTime);	// UI�����X�V
	void UpdateEnd(const float fDeltaTime);		// �I���X�V
	void UpdateSkip(void);	// �X�L�b�v����X�V
	void SkipStaging(void);	// ���o�X�L�b�v

	// �����o�ϐ�
	CObject2D* m_pFade;			// �t�F�[�h���
	CObject2D* m_pTitle;		// �^�C�g�����
	CObject2D* m_pLine;			// �������
	CScrollString2D* m_pName;	// ���O���
	EState m_state;				// ���
	float m_fCurTime;			// ���݂̑ҋ@����
};

#endif	// _GODITEM_MANAGER_H_
