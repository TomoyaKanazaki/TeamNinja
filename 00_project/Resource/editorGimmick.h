#if 0

//============================================================
//
//	�G�f�B�^�[�M�~�b�N�w�b�_�[ [editorGimmick.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDITOR_GIMMICK_H_
#define _EDITOR_GIMMICK_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editStage.h"
#include "editor.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�^�[�M�~�b�N�N���X
class CEditorGimmick
{
public:
	// �R���X�g���N�^
	CEditorGimmick();

	// �f�X�g���N�^
	virtual ~CEditorGimmick();

	// �������z�֐�
	virtual HRESULT Save(void) = 0;		// �ۑ�
	virtual bool IsSave(void) = 0;		// �ۑ��󋵎擾
	virtual void SaveInfo(void) = 0;	// ���ۑ�
	virtual void LoadInfo(void) = 0;	// ���Ǎ�

	// ���z�֐�
	virtual HRESULT Init(void);	// ������
	virtual void Uninit(void);	// �I��
	virtual void Update(void);	// �X�V
	virtual void DrawDebugControl(void);	// ����\���`��
	virtual void DrawDebugInfo(void);		// ���\���`��

	// �ÓI�����o�֐�
	static CEditorGimmick* Create(CEditStage::EType type);		// ����
	static void Release(CEditorGimmick*& prEditorObject);		// �j��

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

#endif // 0
