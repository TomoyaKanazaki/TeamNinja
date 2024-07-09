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
class CCollisionPolygon;	// �|���S���̓����蔻��

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
		TYPE_ROCK_M,			// ��(��)
		TYPE_ROCK_B,			// ��(��)
		TYPE_MOSS_ROCK_S,		// �ۊ�(��)
		TYPE_MOSS_ROCK_M,		// �ۊ�(��)
		TYPE_MOSS_ROCK_B,		// �ۊ�(��)
		TYPE_BIRCHTREE_B,		// �����̖�(��)
		TYPE_BIRCHTREE_M,		// �����̖�(��)
		TYPE_COM_TREE_B,		// ���ʂ̖�(��)
		TYPE_COM_TREE_M,		// ���ʂ̖�(��)
		TYPE_WILLOW_B,			// ���̖�(��)
		TYPE_WILLOW_M,			// ���̖�(��)
		TYPE_WOODLOG,			// ����
		TYPE_TREESTOMP,			// �؂̊�
		TYPE_LILYPAD,			// �@�̗t
		TYPE_PLANT1,			// 4�����Ȃ����Ă��Ȃ���
		TYPE_PLANT2,			// �����Ȃ����Ă��鑐
		TYPE_BUSH,				// ���ނ�
		TYPE_HOUSE1,			// ��1
		TYPE_HOUSE2,			// ��2
		TYPE_HOUSE3,			// ��3
		TYPE_HOUSE4,			// ��4
		TYPE_HOUSE5,			// ��5
		TYPE_HOUSE6,			// ��6
		TYPE_HOUSE7,			// ��7
		TYPE_HOUSE8,			// ��8
		TYPE_HOUSE9,			// ��9
		TYPE_HOUSE10,			// ��10
		TYPE_LANTERN,			// �����^��
		TYPE_GATE,				// ��(�g����)
		TYPE_DOOR,				// ��(������)
		TYPE_KAKEJIKU,			// �|����
		TYPE_PIER,				// ��
		TYPE_PILLER1,			// ��ɔz�u����`�̒�
		TYPE_PILLER2,			// �����l�p�������̒�
		TYPE_STAIRS,			// �K�i
		TYPE_B_LATTICE,			// �|�̊i�q
		TYPE_ROOFFLOOR1,		// ���g�݂݂����ȉ������̖�
		TYPE_ROOFFLOOR2,		// �����K�����̉������̖�
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
		bool& bJump						// �W�����v��
	);
	void Collision						// �����蔻�菈��(�����Ԃ��I�[�o�[���C�h)
	(
		D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
		const float fRadius,			// ���a
		const float fHeight,			// ����
		D3DXVECTOR3& rMove,				// �ړ���
		bool& bJump,					// �W�����v��
		bool& bHit						// �Փ˔���
	);
	void ClearCollision(void);			// �����蔻��̏�������
	void SetModel(const EType type);	// ���f���̊��蓖�ď���

	// �Z�b�g�E�Q�b�g�֐�
	void SetType(const EType type) { m_type = type; }		// ��ނ̐ݒ菈��
	EType GetType(void) const { return m_type; }			// ��ނ̎擾����
	std::vector<CCollisionCube*> GetCube(void) const { return m_cube; }					// �L���[�u�̏��̎擾����
	std::vector<CCollisionCylinder*> GetCylinder (void) const { return m_cylinder; }	// �V�����_�[�̏��̎擾����
	std::vector<CCollisionSphere*> GetSphere(void) const { return m_sphere; }			// �X�t�B�A�̏��̎擾����
	std::vector<CCollisionPolygon*> GetPolygon(void) const { return m_polygon; }		// �|���S���̏��̎擾����

	// �ÓI�����o�֐�
	static CActor* Create	// ����
	( // ����
		const EType type,						// ���
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �g�嗦
	);
	static CListManager<CActor>* GetList(void);		// ���X�g�\���̎擾����
	static HRESULT LoadSetup(const char* pPass);	// �Z�b�g�A�b�v

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
	std::vector<CCollisionPolygon*> m_polygon;		// �|���S���̉ϒ��z��
};

#endif	// _ACTOR_H_
