#if 1
//============================================================
//
//	�G�f�B�b�g�R���W�����X�t�B�A�w�b�_�[ [editCollSphere.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_COLL_SPHERE_H_
#define _EDIT_COLL_SPHERE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editorCollShape.h"
#include "collisionSphere.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�R���W�����X�t�B�A�N���X
class CEditCollSphere : public CEditorCollShape
{
public:
	// �R���X�g���N�^
	CEditCollSphere(const int nIdx);

	// �f�X�g���N�^
	~CEditCollSphere() override;

	// �X�t�B�A���\����
	struct SInfo
	{
		float fRadius;		// ���a
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
	void Create(void) override;		//��������

	// �Z�b�g�E�Q�b�g�֌W
	CCollisionSphere* GetSphere(void) const { return m_pSphere; }		// �X�t�B�A�̎擾����
	SInfo GetInfo(void) const { return m_infoCreate; }					// ���̎擾����

private:
	// �I�[�o�[���C�h�֐�
	void UpdateOffset(void) override;		// �I�t�Z�b�g���W�X�V

	// �����o�֐�
	void UpdateSize(void);		// �傫���X�V

	// �����o�ϐ�
	CCollisionSphere* m_pSphere;	// �X�t�B�A���
	SInfo m_infoCreate;	// �X�t�B�A�z�u���
	bool m_bSave;		// �ۑ���
};

#endif	// _EDIT_COLL_SPHERE_H_
#endif
