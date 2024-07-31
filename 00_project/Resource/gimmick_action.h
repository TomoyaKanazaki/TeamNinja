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
#include "effekseerControl.h"
#include "effekseerManager.h"

//===========================================
//  �O���錾
//===========================================
class CObject3D;

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

	bool CollisionPlayer() override;		// �v���C���[�Ƃ̓����蔻��(��`)
	bool DistancePlayer();					// �v���C���[�Ƃ̓����蔻��(�~�`)

	// �Z�b�g�E�Q�b�g�֌W
	void SetNumClone(const int nNum) { m_nNumClone = nNum; }	// ���g�̑����̐ݒ菈��
	int GetNumClone(void) const { return m_nNumClone; }			// ���g�̑����̎擾����
	void AddNumClone();											// ���g�̑��������Z
	void SabNumClone();											// ���g�̑��������Z
	void SetEnableActive(const bool bAct) { m_bActive = bAct; }	// ���������̐ݒ菈��
	bool IsActive(void) const { return m_bActive; }				// ���������̎擾����
	bool GetMoment() const { return m_bMoment; }				// �������t���O�̎擾
	void SetMoment(bool moment) { m_bMoment = moment; }			// �������t���O�̐ݒ�
	void SetActionPoint(const D3DXVECTOR3& pos);				// ��������ʒu�̐ݒ�
	D3DXVECTOR3 GetActionPoint() const { return m_posAction; }	// ��������ʒu�̎擾

	void SetVec3Sizing(const D3DXVECTOR3& rSize) override; // �T�C�Y�ݒ�

	// �ÓI�����o�֐�
	static CListManager<CGimmickAction>* GetList(void);			// ���X�g�擾

	// ���z�֐�
	virtual D3DXVECTOR3 CalcWaitPoint(const int Idx, const CPlayerClone* pClone)	// �e���g���̑ҋ@�ʒu���Z�o
	{ assert(false); return GetVec3Position(); }
	virtual D3DXVECTOR3 CalcWaitRotation(const int Idx, const CPlayerClone* pClone)	// �e���g���̑ҋ@�������Z�o
	{ assert(false); return GetVec3Rotation(); }

private:

	// �����o�֐�
	void SetEffect(); // �G�t�F�N�g�̐���

	// �����o�ϐ�
	CListManager<CGimmickAction>::AIterator m_iterator;	// �C�e���[�^�[
	int m_nNumClone;			// �͈͂ɓ����Ă��镪�g�̐�
	bool m_bActive;				// ������
	bool m_bOldActive;			// ������
	bool m_bMoment;				// �������t���O
	D3DXVECTOR3 m_posAction;	// �A�N�V�����|�C���g(�ҋ@���W)
	CEffekseer::CEffectData* m_pEffect;	// �G�t�F�N�g�̃|�C���^

	// �ÓI�����o�ϐ�
	static CListManager<CGimmickAction>* m_pList;	// �I�u�W�F�N�g���X�g

};

#endif	// _GIMMICK_ACTION_H_
