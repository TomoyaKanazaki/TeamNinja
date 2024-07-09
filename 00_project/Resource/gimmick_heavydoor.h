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
class CActor;	// �A�N�^�[�N���X

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
	void Uninit(void);	// �I��
	void Update(const float fDeltaTime);	// �X�V
	void Draw(CShader* pShader = nullptr);	// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// �傫���ݒ�
	D3DXVECTOR3 CalcWaitPoint(const int Idx, const CPlayerClone* pClone) override;		// �e���g���̑ҋ@�ʒu���Z�o
	D3DXVECTOR3 CalcWaitRotation(const int Idx, const CPlayerClone* pClone) override;	// �e���g���̑ҋ@�������Z�o

private:

	void OpenTheDoor(D3DXVECTOR3& rPos);	// �����グ��
	void CloseTheDoor(D3DXVECTOR3& rPos);	// ����������

	// �����o�ϐ�
	CActor* m_pGateModel;	// �A�N�^�[(�g)�̏��
	CActor* m_pDoorModel;	// �A�N�^�[(��)�̏��
	D3DXVECTOR3	m_oldPosDoor;	// �ߋ��ʒu
	D3DXVECTOR3 m_move;	// �ړ���
	EState m_state;		// ���̏��

};

#endif
