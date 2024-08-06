//============================================================
//
//	��]�I�u�W�F�N�g2D�w�b�_�[ [roll2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ROLL2D_H_
#define _ROLL2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// ��]�I�u�W�F�N�g2D�N���X
class CRoll2D : public CObject2D
{
public:
	// �萔
	static constexpr float ADD_ROT = 1.0f;	// �����̉��Z��

	// �R���X�g���N�^
	CRoll2D();

	// �f�X�g���N�^
	~CRoll2D() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CRoll2D *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rSize,		// �傫��
		const float fAddRot = ADD_ROT,			// ��]��
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE		// �F
	);

	// �����o�֐�
	void SetRoll(const bool bRoll)		{ m_bRoll = bRoll; }	// ��]�ݒ�
	bool IsRoll(void) const				{ return m_bRoll; }		// ��]�擾
	void  SetAddRot(const float fAdd)	{ m_fAddRot = fAdd; }	// �������Z�ʐݒ�
	float GetAddRot(void) const			{ return m_fAddRot; }	// �������Z�ʎ擾

private:
	// �����o�ϐ�
	float m_fAddRot;	// �����̉��Z��
	bool m_bRoll;		// ��]��
};

#endif	// _ROLL2D_H_
