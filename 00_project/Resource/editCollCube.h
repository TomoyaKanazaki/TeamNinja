#if 1
//============================================================
//
//	�G�f�B�b�g�R���W�����L���[�u�w�b�_�[ [editCollisionCube.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_COLLISION_CUBE_H_
#define _EDIT_COLLISION_CUBE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editor.h"
#include "editorObject.h"
#include "collisionCube.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�L���[�u�N���X
class CEditCollisionCube : public CEditorObject
{
public:

	// �L���[�u���\����
	struct SInfo
	{
		CEditor::EAngle angle;	// �����̎��
		D3DXVECTOR3 size;		// �傫��
		float fLength;			// ����
		float fAngle;			// ����
	};

	// �R���X�g���N�^
	CEditCollisionCube();

	// �f�X�g���N�^
	~CEditCollisionCube() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	HRESULT Save(void) override;	// �ۑ�
	bool IsSave(void) override;		// �ۑ��󋵎擾
	void SaveInfo(void) override;	// ���ۑ�
	void LoadInfo(void) override;	// ���Ǎ�
	void DrawDebugControl(void) override;	// ����\���`��
	void DrawDebugInfo(void) override;		// ���\���`��

private:
	// �I�[�o�[���C�h�֐�
	void UpdatePosition(void) override;		// �ʒu�X�V
	void UpdateRotation(void) override;		// �����X�V

	// �����o�֐�
	void UpdateSizing(void);	// �傫���X�V
	void CreateCube(void);		// �L���[�u����
	void ReleaseCube(void);		// �L���[�u�j��
	void DeleteCollisionCube(const bool bRelase);	// �L���[�u�̍폜����
	void InitAllColorCube(void);					// �L���[�u�̐F�S������

	// �����o�ϐ�
	CCollisionCube* m_pCube;	// �L���[�u���
	SInfo m_infoCreate;			// �L���[�u�z�u���
	bool m_bSave;				// �ۑ���
};

#endif	// _EDIT_FIELD_H_
#endif
