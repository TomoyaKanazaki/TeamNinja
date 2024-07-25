#if 1
//============================================================
//
//	�G�f�B�b�g�_��w�b�_�[ [editGodItem.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_GODITEM_H_
#define _EDIT_GODITEM_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editorObject.h"
#include "godItem.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�_��N���X
class CEditGodItem : public CEditorObject
{
public:
	// �R���X�g���N�^
	CEditGodItem(CEditStage* pEditor);

	// �f�X�g���N�^
	~CEditGodItem() override;

	// �_����\����
	struct SInfo
	{
		CGodItem::EType type;	// ���
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

	// �����o�֐�
	void ChangeType(void);		// ��ލX�V
	void CreateGodItem(void);	// �_�퐶��
	void ReleaseGodItem(void);	// �_��j��
	void DeleteCollisionGodItem(const bool bRelase);	// �_��̍폜����

	// �����o�ϐ�
	CObjectModel* m_pGodItem;	// �_����
	SInfo m_infoCreate;	// �_��z�u���
	bool m_bSave;		// �ۑ���
};

#endif	// _EDIT_GODITEM_H_
#endif
