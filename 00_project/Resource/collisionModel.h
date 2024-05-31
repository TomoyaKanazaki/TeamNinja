//============================================================
//
//	���f���̓����蔻��w�b�_�[ [collisionModel.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COLLISION_MODEL_H_
#define _COLLISION_MODEL_H_

// �N���X��`(�����蔻��N���X)
class CCollision
{
public:

	CCollision();				// �R���X�g���N�^
	virtual ~CCollision();		// �f�X�g���N�^

	virtual void Uninit(void);	// �I������

	virtual bool Hit			// �q�b�g����
	(
		D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
		const float fRadius,			// ���a
		const float fHeight,			// ����
		D3DXVECTOR3& rMove,				// �ړ���
		bool& rJump						// �W�����v��
	) = 0;

	void OffSet(const D3DXMATRIX& mtx);	// �I�t�Z�b�g�ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetPos(const D3DXVECTOR3& rPos) { m_pos = rPos; }				// �ʒu�̐ݒ菈��
	D3DXVECTOR3 GetPos(void) const { return m_pos; }					// �ʒu�̎擾����
	void SetOffset(const D3DXVECTOR3& rOffset) { m_offset = rOffset; }	// �I�t�Z�b�g�̐ݒ菈��
	D3DXVECTOR3 GetOffset(void) const { return m_offset; }				// �I�t�Z�b�g�̎擾����

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_offset;	// �I�t�Z�b�g���W
};

#endif	// _COLLISION_MODEL_H_
