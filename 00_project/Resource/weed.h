//============================================================
//
//	���w�b�_�[ [weed.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _WEED_H_
#define _WEED_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���N���X
class CWeed : public CObject3D
{
public:
	// �R���X�g���N�^
	CWeed();

	// �f�X�g���N�^
	~CWeed() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CWeed *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);

	// �����o�֐�
	void CollisionPlayer(void);	// �v���C���[�Ƃ̓����蔻��

private:
	// �����o�ϐ�
	float m_fCurLength;		// ���݂̋���
	float m_fDestLength;	// �ڕW�̋���
	float m_fCurAngle;		// ���݂̊p�x
	float m_fDestAngle;		// �ڕW�̊p�x
};

#endif	// _WEED_H_
