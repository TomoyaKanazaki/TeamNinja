//============================================================
//
//	�~���̓����蔻��w�b�_�[ [collisionCylinder.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COLLISION_CYLINDER_H_
#define _COLLISION_CYLINDER_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "collisionModel.h"

#ifdef _DEBUG

//-----------------------------------
// �O���錾
//-----------------------------------
class CObjectMeshTube;		// ���b�V���`���[�u

#endif // _DEBUG

// �N���X��`(�����蔻��N���X)
class CCollisionCylinder : public CCollision
{
public:

	CCollisionCylinder();		// �R���X�g���N�^
	~CCollisionCylinder();		// �f�X�g���N�^

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

	// �ÓI�����o�֐�
	static CCollisionCylinder* Create(const D3DXVECTOR3& rPos, const float fLength, const float fAngle, const float fRadius, const float fHeight);		// ��������

private:

	// �����o�ϐ�
	float m_fRadius;		// ���a
	float m_fHeight;		// ����

#ifdef _DEBUG

	CObjectMeshTube* m_pTube;	// ���b�V���`���[�u

#endif // _DEBUG
};

#endif	// _COLLISION_H_