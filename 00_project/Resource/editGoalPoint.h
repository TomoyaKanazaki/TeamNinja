#if 1
//============================================================
//
//	�G�f�B�b�g�S�[���|�C���g�w�b�_�[ [editGoalPoint.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_GOALPOINT_H_
#define _EDIT_GOALPOINT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editorObject.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�S�[���|�C���g�N���X
class CEditGoalPoint : public CEditorObject
{
public:
	// �R���X�g���N�^
	CEditGoalPoint();

	// �f�X�g���N�^
	~CEditGoalPoint() override;

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
	void CreateCheckPoint(void);		// �`�F�b�N�|�C���g����
	void ReleaseCheckPoint(void);		// �`�F�b�N�|�C���g�j��
	void DeleteCollisionCheckPoint(const bool bRelase);	// �`�F�b�N�|�C���g�̍폜����

	// �����o�ϐ�
	CObjectModel* m_pPoint;		// �|�C���g���(�`�F�b�N�|�C���g�̃��X�g�ɓ���Ȃ��悤��)
	bool m_bSave;				// �ۑ���
};

#endif	// _EDIT_ACTOR_H_
#endif
