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
#include "editCollision.h"
#include "editor.h"

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
	CEditorObject(CEditStage* pEditor);

	// �f�X�g���N�^
	virtual ~CEditorObject();

	// �������z�֐�
	virtual HRESULT Save(void)	= 0;	// �ۑ�
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
	static CEditorObject* Create(CEditStage* pEditor, CEditStage::EType type);	// ����
	static void Release(CEditorObject *&prEditorObject);	// �j��

protected:
	// ���z�֐�
	virtual void UpdatePosition(void);	// �ʒu�X�V
	virtual void UpdateRotation(void);	// �����X�V

	// �����o�֐�
	void SetVec3Position(const D3DXVECTOR3 pos)	{ m_pEditor->SetVec3Position(pos); }		// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const		{ return m_pEditor->GetVec3Position(); }	// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3 rot)	{ m_pEditor->SetVec3Rotation(rot); }		// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const		{ return m_pEditor->GetVec3Rotation(); }	// �����擾
	void SetAngle(const EAngle angle)			{ m_pEditor->SetAngle(angle); }				// �����ݒ�
	EAngle GetAngle(void) const					{ return m_pEditor->GetAngle(); }			// �����擾

private:
	// �����o�ϐ�
	CEditStage* m_pEditor;	// �G�f�B�^�[���
};

#endif	// _EDITOR_OBJECT_H_
