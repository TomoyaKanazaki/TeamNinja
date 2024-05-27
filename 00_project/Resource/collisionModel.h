//============================================================
//
//	���f���̓����蔻��w�b�_�[ [collisionModel.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COLLISION_MODEL_H_
#define _COLLISION_MODEL_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

// �N���X��`(�����蔻��N���X)
class CCollision
{
public:

	CCollision();				// �R���X�g���N�^
	~CCollision();				// �f�X�g���N�^

	void Uninit(void);					// �I������

	// �Z�b�g�E�Q�b�g�֌W
	void SetOffset(const D3DXVECTOR3& rOffset) { m_offset = rOffset; }	// �I�t�Z�b�g���W�̐ݒ菈��
	D3DXVECTOR3 GetOffset(void) const { return m_offset; }				// �I�t�Z�b�g���W�̎擾����

	// �ÓI�����o�֐�
	static CCollision* Create(const D3DXVECTOR3& rOffset);	// ��������

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_offset;	// �I�t�Z�b�g���W
};

#endif	// _COLLISION_H_
