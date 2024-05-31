//============================================================
//
//	�����蔻��G�f�B�^�[�w�b�_�[ [editorCollShape.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDITOR_COLL_SHAPE_H_
#define _EDITOR_COLL_SHAPE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editStage.h"
#include "editor.h"

//************************************************************
//	�O���錾
//************************************************************
class CEditManager;	// �G�f�B�b�g�}�l�[�W���[�N���X
class CActor;		// �A�N�^�[

//************************************************************
//	�N���X��`
//************************************************************
// �����蔻��G�f�B�^�[�N���X
class CEditorCollShape
{
public:
	// �R���X�g���N�^
	CEditorCollShape();

	// �f�X�g���N�^
	virtual ~CEditorCollShape();

	// �������z�֐�
	virtual HRESULT Save(void) = 0;	// �ۑ�
	virtual bool IsSave(void) = 0;	// �ۑ��󋵎擾
	virtual void SaveInfo(void) = 0;	// ���ۑ�
	virtual void LoadInfo(void) = 0;	// ���Ǎ�

	// ���z�֐�
	virtual HRESULT Init(void);	// ������
	virtual void Uninit(void);	// �I��
	virtual void Update(void);	// �X�V
	virtual void DrawDebugControl(void);	// ����\���`��
	virtual void DrawDebugInfo(void);		// ���\���`��

	// �ÓI�����o�֐�
	static CEditorCollShape* Create(CEditStage::EType type);	// ����
	static void Release(CEditorCollShape*& prEditorObject);		// �j��

	// �����o�֐�
	CActor* GetActor(void) const { return m_pActor; }		// �A�N�^�[�̎擾����

protected:
	// ���z�֐�
	virtual void UpdatePosition(void);	// �ʒu�X�V
	virtual void UpdateRotation(void);	// �����X�V

private:

	// �����o�ϐ�
	CActor* m_pActor;				// �A�N�^�[
	CEditor::EAngle m_angle;		// �p�x
};

#endif	// _EDITOR_OBJECT_H_
