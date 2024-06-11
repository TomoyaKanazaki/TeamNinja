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

#ifdef _DEBUG

#include "objectMeshSphere.h"

#endif // _DEBUG

// �N���X��`(�����蔻��N���X)
class CCollisionSphere : public CCollision
{
public:

	CCollisionSphere();			// �R���X�g���N�^
	~CCollisionSphere();		// �f�X�g���N�^

	void Uninit(void);			// �I������

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
	float GetRadius(void) const { return m_fRadius; }	//���a�̎擾����

#ifdef _DEBUG

	CObjectMeshSphere* GetSphere(void) const { return m_pSphere; }	// ���b�V���X�t�B�A�̎擾����

#endif // _DEBUG

	// �ÓI�����o�֐�
	static CCollisionSphere* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rOffset, const float fRadius);		// ��������

private:

	// �����o�ϐ�
	float m_fRadius;	// ���a

#ifdef _DEBUG

	CObjectMeshSphere* m_pSphere;	// ���b�V���X�t�B�A

#endif // _DEBUG
};

#endif	// _COLLISION_SPHERE_H_
