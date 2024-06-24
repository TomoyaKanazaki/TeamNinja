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

//************************************************************
// �O���錾
//************************************************************
class CPlayerClone;		// �v���C���[�̕��g

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
		TYPE_STEP,				// ���ݑ�
		TYPE_POST,				// �ݒu
		TYPE_JUMPOFF,			// ��э~��
		TYPE_HEAVYDOOR,			// �d����
		TYPE_BRIDGE,			// ��
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
	void SetNumActive(const int nNum) { m_nNumActive = nNum; }	// �����\�l���̐ݒ菈��
	int GetNumActive(void) const { return m_nNumActive; }		// �����\�l���̎擾����
	virtual bool IsSet() { return bool(); }						// �ݒu�M�~�b�N�̃t���O�擾�p

	// �����蔻��
	virtual void CollisionClone() {}							// �N���[���Ƃ̓����蔻��
	virtual bool CollisionPlayer() { return false; }			// �v���C���[�Ƃ̓����蔻��

	// �ÓI�����o�֐�
	static CGimmick* Create				// ����
	(
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rSize,		// �T�C�Y
		const EType type,				// ���
		const int nNumActive			// �����\�ȕ��g�̐�
	);
	static CListManager<CGimmick>* GetList(void);	// ���X�g�擾

private:

	// �����o�ϐ�
	CListManager<CGimmick>::AIterator m_iterator;	// �C�e���[�^�[
	EType m_type;		// ���
	int m_nNumActive;	// �����\�Ȑ�

	// �ÓI�����o�ϐ�
	static CListManager<CGimmick>* m_pList;			// �I�u�W�F�N�g���X�g
};

#endif	// _GIMMICK_H_
