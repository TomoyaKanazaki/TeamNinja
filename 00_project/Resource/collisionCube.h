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
#include "listManager.h"

#ifdef _DEBUG

#include "objectMeshCube.h"

#endif // _DEBUG

// �N���X��`(�����蔻��N���X)
class CCollisionCube : public CCollision
{
public:

	CCollisionCube();			// �R���X�g���N�^
	~CCollisionCube();			// �f�X�g���N�^

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
	void SetWidth(const float fWidth) { m_fWidth = fWidth; }		// ���̐ݒ菈��
	float GetWidth(void) const { return m_fWidth; }					// ���̎擾����
	void SetHeight(const float fHeight) { m_fHeight = fHeight; }	// �����̐ݒ菈��
	float GetHeight(void) const { return m_fHeight; }				// �����̎擾����
	void SetDepth(const float fDepth) { m_fDepth = fDepth; }		// ���s�̐ݒ菈��
	float GetDepth(void) const { return m_fDepth; }					// ���s�̎擾����

#ifdef _DEBUG

	// �Z�b�g�E�Q�b�g�֌W
	CObjectMeshCube* GetCube(void) const { return m_pCube; }	// �L���[�u�̎擾����

#endif // _DEBUG

	// �ÓI�����o�֐�
	static CCollisionCube* Create			// ��������
	(
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rOffset,		// �I�t�Z�b�g
		const float fWidth,				// ��
		const float fHeight,			// ����
		const float fDepth,				// ���s
		const float fRot				// ����
	);
	static CListManager<CCollisionCube>* GetList(void);	// ���X�g�擾

private:

	// �����o�֐�
	bool Convert(const float fWidth, const float fDepth, const float fRot);		// �����ɂ��ϊ�����

#ifdef _DEBUG

	bool RotWarning(const float fRot);		// �����̌x������

#endif


	// �����o�ϐ�
	CListManager<CCollisionCube>::AIterator m_iterator;	// �C�e���[�^�[

	float m_fWidth;		// ��
	float m_fHeight;	// ����
	float m_fDepth;		// ���s

#ifdef _DEBUG

	CObjectMeshCube* m_pCube;	// ���b�V���L���[�u

#endif // _DEBUG

	// �ÓI�����o�ϐ�
	static CListManager<CCollisionCube>* m_pList;			// �I�u�W�F�N�g���X�g
};

#endif	// _COLLISION_CUBE_H_
