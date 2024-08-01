//============================================================
//
//	�����o���w�b�_�[ [balloon.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BALLOON_H_
#define _BALLOON_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectBillboard.h"

//************************************************************
//	�N���X��`
//************************************************************
// �����o���N���X
class CBalloon : public CObjectBillboard
{
public:
	// �R���X�g���N�^
	CBalloon();

	// �f�X�g���N�^
	~CBalloon();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�

	// �ÓI�����o�֐�
	static CBalloon *Create(const D3DXVECTOR3& rPosParent);	// ����

	// �����o�֐�
	void SetDisp(const bool bDisp);	// �����o���\���ݒ�

private:
	// �����o�ϐ�
	float m_fScale;		// �g�嗦
	float m_fDestScale;	// �ڕW�g�嗦
};

#endif	// _BALLOON_H_
