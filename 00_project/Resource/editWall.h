#if 1
//============================================================
//
//	�G�f�B�b�g�E�H�[���w�b�_�[ [editWall.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_WALL_H_
#define _EDIT_WALL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editorObject.h"
#include "wall.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�E�H�[���N���X
class CEditWall : public CEditorObject
{
public:
	// �R���X�g���N�^
	CEditWall(CEditStage* pEditor);

	// �f�X�g���N�^
	~CEditWall() override;

	// �E�H�[�����\����
	struct SInfo
	{
		CWall::EType type;	// ���
		D3DXVECTOR2 size;	// �傫��
		POSGRID2 part;		// ������
		POSGRID2 texPart;	// �e�N�X�`��������
	};

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
	void UpdateRotation(void) override;	// �����X�V

	// �����o�֐�
	void UpdateSizing(void);	// �傫���X�V
	void UpdateTexPart(void);	// �e�N�X�`�������X�V
	void ChangeType(void);		// ��ޕύX
	void CreateWall(void);		// �E�H�[������
	void ReleaseWall(void);		// �E�H�[���j��
	void DeleteCollisionWall(const bool bRelase);	// �E�H�[���̍폜����
	void InitAllColorWall(void);					// �E�H�[���̐F�S������

	// �����o�ϐ�
	CWall *m_pWall;		// �E�H�[�����
	SInfo m_infoCreate;	// �E�H�[���z�u���
	bool m_bSave;		// �ۑ���
};

#endif	// _EDIT_WALL_H_
#endif
