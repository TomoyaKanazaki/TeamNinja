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
#include "listManager.h"

#ifdef _DEBUG

#include "objectMeshTube.h"

#endif // _DEBUG

// �N���X��`(�����蔻��N���X)
class CCollisionCylinder : public CCollision
{
public:

	CCollisionCylinder();		// �R���X�g���N�^
	~CCollisionCylinder();		// �f�X�g���N�^

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
	void SetHeight(const float fHeight) { m_fHeight = fHeight; }	// �����̐ݒ菈��
	float GetHeight(void) const { return m_fHeight; }				// �����̎擾����

#ifdef _DEBUG

	CObjectMeshTube* GetTube(void) const { return m_pTube; }	// ���b�V���`���[�u�̎擾����

#endif // _DEBUG

	// �ÓI�����o�֐�
	static CCollisionCylinder* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rOffset, const float fRadius, const float fHeight);		// ��������
	static CListManager<CCollisionCylinder>* GetList(void);	// ���X�g�擾

private:

	// �����o�ϐ�
	CListManager<CCollisionCylinder>::AIterator m_iterator;	// �C�e���[�^�[

	float m_fRadius;		// ���a
	float m_fHeight;		// ����

#ifdef _DEBUG

	CObjectMeshTube* m_pTube;	// ���b�V���`���[�u

#endif // _DEBUG

	// �ÓI�����o�ϐ�
	static CListManager<CCollisionCylinder>* m_pList;			// �I�u�W�F�N�g���X�g
};

#endif	// _COLLISION_H_
