//============================================================
//
//	�G�f�B�b�g�R���W�����w�b�_�[ [editCollision.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_COLLISION_H_
#define _EDIT_COLLISION_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editor.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditManager;		// �G�f�B�b�g�}�l�[�W���[�N���X
class CEditorObject;	// �G�f�B�^�[�I�u�W�F�N�g�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�X�e�[�W�N���X
class CEditCollision : public CEditor
{
public:

	// ��ޗ�
	enum EType
	{
		TYPE_CUBE = 0,	// �L���[�u
		TYPE_CYLINDER,	// �~��
		TYPE_SPHERE,	// ��
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEditCollision(CEditManager* pEditManager);

	// �f�X�g���N�^
	virtual ~CEditCollision();

	// �����o�֐�
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
	// �����o�֐�
	void ChangeObjectType(void);	// �I�u�W�F�N�g�^�C�v�ύX

	// �����o�ϐ�
	CEditorObject* m_pEditor;		// �G�f�B�^�[���
	CEditCollision::EType m_type;	// �I�u�W�F�N�g�^�C�v
};

#endif	// _EDIT_COLLISION_H_
