//============================================================
//
//	�L���[�u�̓����蔻��w�b�_�[ [collisionCube.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COLLISION_CUBE_H_
#define _COLLISION_CUBE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "collisionModel.h"

#ifdef _DEBUG

//-----------------------------------
// �O���錾
//-----------------------------------
class CObjectMeshCube;		// ���b�V���L���[�u

#endif // _DEBUG

// �N���X��`(�����蔻��N���X)
class CCollisionCube : public CCollision
{
public:

	CCollisionCube();			// �R���X�g���N�^
	~CCollisionCube();			// �f�X�g���N�^

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
	static CCollisionCube* Create(const D3DXVECTOR3& rOffset, const float fWidth, const float fHeight, const float fDepth);		// ��������

private:

	// �����o�ϐ�
	float m_fWidth;		// ��
	float m_fHeight;	// ����
	float m_fDepth;		// ���s

#ifdef _DEBUG

	CObjectMeshCube* m_pCube;	// ���b�V���L���[�u

#endif // _DEBUG
};

#endif	// _COLLISION_CUBE_H_
