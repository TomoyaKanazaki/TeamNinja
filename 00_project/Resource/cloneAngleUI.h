//============================================================
//
// ���g���o��������UI�w�b�_�[ [cloneAngleUI.h]
// Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _CLONE_ANGLE_UI_H_
#define _CLONE_ANGLE_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���g����UI�N���X
class CCloneAngleUI : public CObject3D
{
public:

	// �R���X�g���N�^
	CCloneAngleUI();

	// �f�X�g���N�^
	~CCloneAngleUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CCloneAngleUI* Create(CObject *pParent);	// ����

private:

	// �����o�ϐ�
	CObject* m_pParentObject;	// �e�I�u�W�F�N�g
};

#endif	// _SHADOW_H_
