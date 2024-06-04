//============================================================
//
// �A�N�V�����M�~�b�N�w�b�_�[ [gimmick_action.h]
// Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GIMMICK_ACTION_H_
#define _GIMMICK_ACTION_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gimmick.h"

//************************************************************
//	�N���X��`
//************************************************************
// �A�N�V�����M�~�b�N�N���X
class CGimmickAction : public CGimmick
{
public:

	// �R���X�g���N�^
	CGimmickAction();

	// �f�X�g���N�^
	virtual ~CGimmickAction() override;

	// �I�[�o�[���C�h�֐�
	virtual HRESULT Init(void) override;	// ������
	virtual void Uninit(void) override;		// �I��
	virtual void Update(const float fDeltaTime) override;	// �X�V
	virtual void Draw(CShader* pShader = nullptr) override;	// �`��

	void CollisionClone() override;			// �N���[���Ƃ̓����蔻��
	bool CollisionPlayer() override;		// �v���C���[�Ƃ̓����蔻��

	// �Z�b�g�E�Q�b�g�֌W
	void SetNumClone(const int nNum) { m_nNumClone = nNum; }	// ���g�̑����̐ݒ菈��
	int GetNumClone(void) const { return m_nNumClone; }			// ���g�̑����̎擾����
	void SetEnableActive(const bool bAct) { m_bActive = bAct; }	// ���������̐ݒ菈��
	bool IsActive(void) const { return m_bActive; }				// ���������̎擾����
	bool GetMoment() const { return m_bMoment; }				// �������t���O�̎擾
	void SetMoment(bool moment) { m_bMoment = moment; }			// �������t���O�̐ݒ�

private:

	// �����o�ϐ�
	int m_nNumClone;	// �͈͂ɓ����Ă��镪�g�̐�
	bool m_bActive;		// ������
	bool m_bMoment;		// �������t���O

};

#endif	// _GIMMICK_ACTION_H_
