//============================================================
//
//	���̓����蔻��w�b�_�[ [collisionSphere.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COLLISION_SPHERE_H_
#define _COLLISION_SPHERE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "collisionModel.h"
#include "listManager.h"

#ifdef _DEBUG

#include "objectMeshSphere.h"

#endif // _DEBUG

// �N���X��`(�����蔻��N���X)
class CCollisionSphere : public CCollision
{
public:

	CCollisionSphere();			// �R���X�g���N�^
	~CCollisionSphere();		// �f�X�g���N�^

	void Init(void);			// ����������
	void Uninit(void) override;	// �I������

	bool Hit					// �q�b�g����
	(
		D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
		const float fRadius,			// ���a
		const float fHeight,			// ����
		D3DXVECTOR3& rMove,				// �ړ���
		bool& rJump						// �W�����v��
	) override;

	void OffSet(const D3DXMATRIX& mtx) override;		// �I�t�Z�b�g�ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetRadius(const float fRadius) { m_fRadius = fRadius; }	// ���a�̐ݒ菈��
	float GetRadius(void) const { return m_fRadius; }				// ���a�̎擾����

#ifdef _DEBUG

	CObjectMeshSphere* GetSphere(void) const { return m_pSphere; }	// ���b�V���X�t�B�A�̎擾����

#endif // _DEBUG

	// �ÓI�����o�֐�
	static CCollisionSphere* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rOffset, const float fRadius, const bool bList);		// ��������
	static CListManager<CCollisionSphere>* GetList(void);	// ���X�g�擾

private:

	// �����o�ϐ�
	CListManager<CCollisionSphere>::AIterator m_iterator;	// �C�e���[�^�[

	float m_fRadius;	// ���a
	bool m_bList;		// ���X�g��

#ifdef _DEBUG

	CObjectMeshSphere* m_pSphere;	// ���b�V���X�t�B�A

#endif // _DEBUG

	// �ÓI�����o�ϐ�
	static CListManager<CCollisionSphere>* m_pList;			// �I�u�W�F�N�g���X�g
};

#endif	// _COLLISION_SPHERE_H_
