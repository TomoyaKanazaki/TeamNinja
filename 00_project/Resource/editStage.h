//============================================================
//
//	�G�f�B�b�g�X�e�[�W�w�b�_�[ [editStage.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_STAGE_H_
#define _EDIT_STAGE_H_

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
//	�萔�錾
//************************************************************
namespace editstage
{
	const float SIZE = 50.0f;	// ��u���b�N�̑傫��
}

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�X�e�[�W�N���X
class CEditStage : public CEditor
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_FIELD = 0,	// �n��
		TYPE_WALL,		// ��
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEditStage(CEditManager *pEditManager);

	// �f�X�g���N�^
	virtual ~CEditStage();

	// �����o�֐�
	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��
	void Update(void);		// �X�V
	void SaveInfo(void);	// ���ۑ�
	void LoadInfo(void);	// ���Ǎ�
	void Save(FILE *pFile);	// �ۑ�
	void DrawDebugControl(void);	// ����\���`��
	void DrawDebugInfo(void);		// ���\���`��

private:
	// �����o�֐�
	void ChangeObjectType(void);	// �I�u�W�F�N�g�^�C�v�ύX

	// �����o�ϐ�
	CEditorObject *m_pEditor;	// �G�f�B�^�[���
	CEditStage::EType m_type;	// �I�u�W�F�N�g�^�C�v
};

#endif	// _EDIT_STAGE_H_
