//============================================================
//
//	�|���S���̓����蔻��w�b�_�[ [collisionPolygon.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COLLISION_POLYGON_H_
#define _COLLISION_POLYGON_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "collisionModel.h"

#ifdef _DEBUG

#include "object2D.h"

#endif // _DEBUG

// �N���X��`(�����蔻��N���X)
class CCollisionPolygon : public CCollision
{
public:

	CCollisionPolygon();		// �R���X�g���N�^
	~CCollisionPolygon();		// �f�X�g���N�^

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
	void SetRot(const D3DXVECTOR3& rRot) { m_rot = rRot; }		// �����̐ݒ菈��
	D3DXVECTOR3 GetRot(void) const { return m_rot; }			// �����̎擾����
	void SetSize(const D3DXVECTOR3& rSize) { m_size = rSize; }	// �T�C�Y�̐ݒ菈��
	D3DXVECTOR3 GetSize(void) const { return m_size; }			// �T�C�Y�̎擾����

#ifdef _DEBUG

	CObject2D* GetSphere(void) const { return m_pPolygon; }	// ���b�V���X�t�B�A�̎擾����

#endif // _DEBUG

	// �ÓI�����o�֐�
	static CCollisionPolygon* Create			// ��������
	(
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rOffset,		// �I�t�Z�b�g
		const D3DXVECTOR3& rRot,		// ����
		const D3DXVECTOR3& rSize		// �T�C�Y
	);

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_rot;	// ����
	D3DXVECTOR3 m_size;	// �T�C�Y

#ifdef _DEBUG

	CObject2D* m_pPolygon;	// �|���S��

#endif // _DEBUG
};

#endif	// _COLLISION_POLYGON_H_