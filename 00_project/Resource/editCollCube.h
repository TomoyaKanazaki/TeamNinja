#if 1
//============================================================
//
//	�G�f�B�b�g�R���W�����L���[�u�w�b�_�[ [editCollCube.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_COLL_CUBE_H_
#define _EDIT_COLL_CUBE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editorCollShape.h"
#include "collisionCube.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�R���W�����L���[�u�N���X
class CEditCollCube : public CEditorCollShape
{
public:
	// �R���X�g���N�^
	CEditCollCube(const int nIdx);

	// �f�X�g���N�^
	~CEditCollCube() override;

	// �L���[�u���\����
	struct SInfo
	{
		float fWidth;		// ��
		float fHeight;		// ����
		float fDepth;		// ���s
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
	CCollisionCube* GetCube(void) const { return m_pCube; }		// �L���[�u�̎擾����
	SInfo GetInfo(void) const { return m_infoCreate; }			// ���̎擾����

private:
	// �I�[�o�[���C�h�֐�
	void UpdateOffset(void) override;		// �I�t�Z�b�g���W�X�V

	// �����o�֐�
	void UpdateSize(void);	// �傫���X�V

	// �����o�ϐ�
	CCollisionCube* m_pCube;	// �L���[�u���
	SInfo m_infoCreate;	// �L���[�u�z�u���
	bool m_bSave;		// �ۑ���
};

#endif	// _EDIT_COLL_CUBE_H_
#endif
