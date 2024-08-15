//============================================================
//
//	�Q�[���}�l�[�W���[�w�b�_�[ [gameManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "retentionManager.h"
#include "godItem.h"

//************************************************************
//	�O���錾
//************************************************************
class CGodItemManager;	// �_��l�����o�}�l�[�W���[
class CResultManager;	// ���U���g�}�l�[�W���[

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[���}�l�[�W���[�N���X
class CGameManager
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_START,	// �X�^�[�g���
		STATE_NORMAL,	// �ʏ���
		STATE_GODITEM,	// �_��l�����
		STATE_RESULT,	// ���U���g���
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CGameManager();

	// �f�X�g���N�^
	~CGameManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(const float fDeltaTime);	// �X�V
	void SetState(const EState state)	{ m_state = state; }	// ��Ԑݒ�
	EState GetState(void) const			{ return m_state; }		// ��Ԏ擾
	void SetSave(const int nSave)		{ m_nSave = nSave; }	// �Z�[�u�ݒ�
	void PossessGodItem(const CGodItem::EType typeID);			// ���ʊl��
	void TransitionResult(const CRetentionManager::EWin win);	// ���U���g��ʑJ��

	// �ÓI�����o�֐�
	static CGameManager *Create(void);	// ����
	static void Release(CGameManager *&prGameManager);	// �j��

private:
	// �����o�ϐ�
	CGodItemManager *m_pGodItem;	// �_��l�����o�}�l�[�W���[
	CResultManager *m_pResult;		// ���U���g�}�l�[�W���[
	EState m_state;	// ���
	int m_nSave;	// �Ō�̃Z�[�u�|�C���g
};

#endif	// _GAMEMANAGER_H_
