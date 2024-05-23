//============================================================
//
//	�G�f�B�^�[�I�u�W�F�N�g�w�b�_�[ [editorObject.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDITOR_OBJECT_H_
#define _EDITOR_OBJECT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editStage.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditManager;	// �G�f�B�b�g�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�^�[�I�u�W�F�N�g�N���X
class CEditorObject
{
public:
	// �R���X�g���N�^
	CEditorObject(CEditManager *pEditManager);

	// �f�X�g���N�^
	virtual ~CEditorObject();

	// �������z�֐�
	virtual void Save(void)		= 0;	// �ۑ�
	virtual bool IsSave(void)	= 0;	// �ۑ��󋵎擾
	virtual void SaveInfo(void)	= 0;	// ���ۑ�
	virtual void LoadInfo(void)	= 0;	// ���Ǎ�

	// ���z�֐�
	virtual HRESULT Init(void);	// ������
	virtual void Uninit(void);	// �I��
	virtual void Update(void);	// �X�V
	virtual void DrawDebugControl(void);	// ����\���`��
	virtual void DrawDebugInfo(void);		// ���\���`��

	// �ÓI�����o�֐�
	static CEditorObject *Create(CEditManager *pEditManager, CEditStage::EType type);	// ����
	static void Release(CEditorObject *&prEditorObject);	// �j��

	// �����o�֐�
	CEditManager *GetPtrEditManager(void) const;	// �G�f�B�b�g�}�l�[�W���[�擾
	D3DXVECTOR3 GetVec3Position(void) const { return m_pos; }	// �ʒu�擾

private:
	// �����o�֐�
	void UpdatePosition(void);	// �ʒu�X�V

	// �����o�ϐ�
	CEditManager *m_pEditManager;	// �G�f�B�b�g�}�l�[�W���[
	D3DXVECTOR3 m_pos;	// �ʒu
};

#endif	// _EDITOR_OBJECT_H_
