//============================================================
//
// �d�����w�b�_�[ [gimmick_heavydoor.h]
// Author�F����������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GIMMICK_HEAVYDOOR_H_
#define _GIMMICK_HEAVYDOOR_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gimmick_action.h"

//************************************************************
//	�O���錾
//************************************************************
class CMapModel;		// �}�b�v���f���N���X
class CStage;			// �X�e�[�W�N���X

//************************************************************
//	�N���X��`
//************************************************************
class CGimmickHeavyDoor : public CGimmickAction
{
public:

	// ���̏�ԗ�
	enum EState
	{
		STATE_NONE = 0,		// �������ĂȂ�
		STATE_CLOSE,		// �����Ă�
		STATE_OPEN,			// ���グ�Ă�
		STATE_FULLY,		// ���S�J
		STATE_MAX
	};

	CGimmickHeavyDoor();
	~CGimmickHeavyDoor();

	HRESULT Init(void);	// ������
	void Uninit(void);		// �I��
	void Update(const float fDeltaTime);	// �X�V
	void Draw(CShader* pShader = nullptr);	// �`��

private:

	void OpenTheDoor(void);	// �����グ��
	void CloseTheDoor(void);	// ����������

	// �����o�ϐ�
	CMapModel* m_pMapModel;	// �}�b�v���f���̏��
	CStage* m_pStage;		// �X�e�[�W�̏��

	D3DXVECTOR3 m_move;		// �ړ���
	int m_nDoorCounter;		// ���̊J�J�E���^�[
	int m_nNumBound;		// ���˂���

	EState m_state;			// ���̏��

};

#endif
