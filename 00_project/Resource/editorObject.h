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
	CEditorObject();

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
	static CEditorObject *Create(CEditStage::EType type);	// ����
	static void Release(CEditorObject *&prEditorObject);	// �j��

	// �����o�֐�
	D3DXVECTOR3 GetVec3Position(void) const { return m_pos; }	// �ʒu�擾
	D3DXVECTOR3 GetVec3Rotation(void) const { return m_rot; }	// �����擾

protected:
	// ���z�֐�
	virtual void UpdatePosition(void);	// �ʒu�X�V
	virtual void UpdateRotation(void);	// �����X�V

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_rot;	// ����
	CEditor::EAngle m_angle;		// �p�x
};

#endif	// _EDITOR_OBJECT_H_
