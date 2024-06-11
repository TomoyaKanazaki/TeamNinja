//============================================================
//
//	�A�N�^�[�w�b�_�[ [actor.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ACTOR_H_
#define _ACTOR_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectModel.h"
#include "listManager.h"

//************************************************************
// �O���錾
//************************************************************
class CCollisionCube;		// �L���[�u�̓����蔻��
class CCollisionCylinder;	// �V�����_�[�̓����蔻��
class CCollisionSphere;		// �X�t�B�A�̓����蔻��

//************************************************************
//	�N���X��`
//************************************************************
// �A�N�^�[�N���X
class CActor : public CObjectModel
{
public:

	// ��ޗ�
	enum EType
	{
		TYPE_ROCK_S = 0,		// ��(��)
		TYPE_MAX				// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CActor();

	// �f�X�g���N�^
	virtual ~CActor() override;

	// �I�[�o�[���C�h�֐�
	virtual HRESULT Init(void) override;	// ������
	virtual void Uninit(void) override;		// �I��
	virtual void Update(const float fDeltaTime) override;	// �X�V
	virtual void Draw(CShader* pShader = nullptr) override;	// �`��

	// �����o�֐�
	void Collision						// �����蔻�菈��
	(
		D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
		const float fRadius,			// ���a
		const float fHeight,			// ����
		D3DXVECTOR3& rMove,				// �ړ���
		bool& bJump,					// �W�����v��
		bool* pHit = nullptr			// �����������ǂ����̔���
	);
	void ClearCollision(void);			// �����蔻��̏�������

	// �Z�b�g�E�Q�b�g�֐�
	void SetType(const EType type) { m_type = type; }		// ��ނ̐ݒ菈��
	EType GetType(void) const { return m_type; }			// ��ނ̎擾����
	std::vector<CCollisionCube*> GetCube(void) const { return m_cube; }					// �L���[�u�̏��̎擾����
	std::vector<CCollisionCylinder*> GetCylinder (void) const { return m_cylinder; }	// �V�����_�[�̏��̎擾����
	std::vector<CCollisionSphere*> GetSphere(void) const { return m_sphere; }			// �X�t�B�A�̏��̎擾����

	// �ÓI�����o�֐�
	static CActor* Create	// ����
	( // ����
		const EType type,						// ���
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �g�嗦
	);
	static CListManager<CActor>* GetList(void);	// ���X�g�\���̎擾����
	static HRESULT LoadSetup(void);	// �Z�b�g�A�b�v

private:

	// �����o�֐�
	void BindCollision(void);						// �����蔻��̊��蓖�ď���

	// �ÓI�����o�ϐ�
	static CListManager<CActor>* m_pList;			// ���X�g�\��

	// �����o�ϐ�
	CListManager<CActor>::AIterator m_iterator;		// �C�e���[�^�[
	EType m_type;									// ���
	std::vector<CCollisionCube*> m_cube;			// �L���[�u�̉ϒ��z��
	std::vector<CCollisionCylinder*> m_cylinder;	// �V�����_�[�̉ϒ��z��
	std::vector<CCollisionSphere*> m_sphere;		// �X�t�B�A�̉ϒ��z��
};

#endif	// _ACTOR_H_
