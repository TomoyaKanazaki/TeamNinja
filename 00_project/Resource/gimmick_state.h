//============================================================
//
// ��ԃM�~�b�N�w�b�_�[ [gimmick_state.h]
// Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GIMMICK_STATE_H_
#define _GIMMICK_STATE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gimmick.h"

//===========================================
//  �O���錾
//===========================================
class CPlayerClone;

//************************************************************
//	�N���X��`
//************************************************************
// ��ԃM�~�b�N�N���X
class CGimmickState : public CGimmick
{
public:

	// �e�M�~�b�N�̃t���O
	enum EFrag
	{
		GRAVEL = 'g', // ������
		BOOB = 'b', // �h�u
		WATER = 'w', // ����
		CLEEN = 'c', // �|�������Ă̏�
		MAX
	};

	// �R���X�g���N�^
	CGimmickState();

	// �f�X�g���N�^
	virtual ~CGimmickState() override;

	// �I�[�o�[���C�h�֐�
	virtual HRESULT Init(void) override;	// ������
	virtual void Uninit(void) override;		// �I��
	virtual void Update(const float fDeltaTime) override;	// �X�V
	virtual void Draw(CShader* pShader = nullptr) override;	// �`��

	void CollisionClone() override;			// �N���[���Ƃ̓����蔻��

protected:

	// �������z�֐�
	virtual void HitClone(CPlayerClone* pClone) = 0; // ���g�ɓ������Ă������̏���
	virtual void MissClone(CPlayerClone* pClone) = 0; // ���g�ɓ������Ă��Ȃ��ꍇ�̏���

private:

};

#endif	// _GIMMICK_ACTION_H_
