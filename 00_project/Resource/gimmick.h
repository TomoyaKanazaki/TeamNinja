//============================================================
//
// �M�~�b�N�w�b�_�[ [gimmick.h]
// Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GIMMICK_H_
#define _GIMMICK_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D.h"
#include "listManager.h"
#include "player_clone.h"

//************************************************************
//	�N���X��`
//************************************************************
// �M�~�b�N�̐e�N���X
class CGimmick : public CObject3D
{
public:

	// ���
	enum EType
	{
		TYPE_JUMPTABLE = 0,		// �W�����v��
		TYPE_MAX				// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CGimmick();

	// �f�X�g���N�^
	virtual ~CGimmick() override;

	// �I�[�o�[���C�h�֐�
	virtual HRESULT Init(void) override;	// ������
	virtual void Uninit(void) override;		// �I��
	virtual void Update(const float fDeltaTime) override;		// �X�V
	virtual void Draw(CShader* pShader = nullptr) override;		// �`��

	// �Z�b�g�E�Q�b�g�֌W
	void SetType(const EType type) { m_type = type; }			// ��ނ̐ݒ菈��
	EType GetType(void) const { return m_type; }				// ��ނ̎擾����
	void SetNumClone(const int nNum) { m_nNumClone = nNum; }	// ���g�̑����̐ݒ菈��
	int GetNumClone(void) { return m_nNumClone; }				// ���g�̑����̎擾����
	void SetEnableActive(const bool bAct) { m_bActive = bAct; }	// ���������̐ݒ菈��
	bool IsActive(void) { return m_bActive; }					// ���������̎擾����

	// �ÓI�����o�֐�
	static CGimmick* Create(const D3DXVECTOR3& rPos, const EType type);	// ����
	static CListManager<CGimmick>* GetList(void);	// ���X�g�擾

private:

	// �����o�ϐ�
	CListManager<CGimmick>::AIterator m_iterator;	// �C�e���[�^�[
	EType m_type;		// ���
	int m_nNumClone;	// �͈͂ɓ����Ă��镪�g�̐�
	bool m_bActive;		// ������

	// �ÓI�����o�ϐ�
	static CListManager<CGimmick>* m_pList;			// �I�u�W�F�N�g���X�g
};

#endif	// _GIMMICK_H_