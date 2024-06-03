#if 1
//============================================================
//
//	�G�f�B�b�g�R���W�����V�����_�[�w�b�_�[ [editCollCylinder.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_COLL_CYLINDER_H_
#define _EDIT_COLL_CYLINDER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editorCollShape.h"
#include "collisionCylinder.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�R���W�����V�����_�[�N���X
class CEditCollCylinder : public CEditorCollShape
{
public:
	// �R���X�g���N�^
	CEditCollCylinder(const int nIdx);

	// �f�X�g���N�^
	~CEditCollCylinder() override;

	// �V�����_�[���\����
	struct SInfo
	{
		float fRadius;		// ���a
		float fHeight;		// ����
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	bool IsSave(void) override;		// �ۑ��󋵎擾
	void SaveInfo(void) override;	// ���ۑ�
	void LoadInfo(void) override;	// ���Ǎ�
	void DrawDebugControl(void) override;	// ����\���`��
	void DrawDebugInfo(void) override;		// ���\���`��
	void Create(void) override;		// ��������

	// �Z�b�g�E�Q�b�g�֌W
	CCollisionCylinder* GetCylinder(void) const { return m_pCylinder; }		// �V�����_�[�̎擾����

private:
	// �I�[�o�[���C�h�֐�
	void UpdateOffset(void) override;		// �I�t�Z�b�g���W�X�V

	// �����o�֐�
	void UpdateSizing(void);	// �傫���X�V

	// �����o�ϐ�
	CCollisionCylinder* m_pCylinder;	// �V�����_�[���
	SInfo m_infoCreate;	// �V�����_�[�z�u���
	bool m_bSave;		// �ۑ���
};

#endif	// _EDIT_COLL_CYLINDER_H_
#endif
