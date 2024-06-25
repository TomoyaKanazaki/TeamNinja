#if 1
//============================================================
//
//	�G�f�B�b�g�`�F�b�N�|�C���g�w�b�_�[ [editCheckPoint.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_CHECKPOINT_H_
#define _EDIT_CHECKPOINT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editorObject.h"
#include "checkpoint.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�`�F�b�N�|�C���g�N���X
class CEditCheckPoint : public CEditorObject
{
public:
	// �R���X�g���N�^
	CEditCheckPoint(CEditStage* pEditor);

	// �f�X�g���N�^
	~CEditCheckPoint() override;

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
	void UpdatePosition(void) override;		// �ʒu�X�V

	// �����o�֐�
	void CreateCheckPoint(void);		// �`�F�b�N�|�C���g����
	void ReleaseCheckPoint(void);		// �`�F�b�N�|�C���g�j��
	void DeleteCollisionCheckPoint(const bool bRelase);	// �`�F�b�N�|�C���g�̍폜����

	// �����o�ϐ�
	CCheckPoint* m_pPoint;	// �|�C���g���
	bool m_bSave;			// �ۑ���
};

#endif	// _EDIT_ACTOR_H_
#endif
