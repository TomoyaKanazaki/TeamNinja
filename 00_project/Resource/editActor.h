#if 1
//============================================================
//
//	�G�f�B�b�g�A�N�^�[�w�b�_�[ [editActor.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_ACTOR_H_
#define _EDIT_ACTOR_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editorObject.h"
#include "actor.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�A�N�^�[�N���X
class CEditActor : public CEditorObject
{
public:
	// �R���X�g���N�^
	CEditActor(CEditStage* pEditor);

	// �f�X�g���N�^
	~CEditActor() override;

	// �A�N�^�[���\����
	struct SInfo
	{
		CActor::EType type;	// ���
		D3DXVECTOR3 scale;	// �傫��
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
	void UpdatePosition(void) override;		// �ʒu�X�V
	void UpdateRotation(void) override;		// �����X�V

	// �����o�֐�
	void UpdateScaling(void);	// �傫���X�V
	void ChangeType(void);		// ��ލX�V
	void CreateActor(void);		// �A�N�^�[����
	void ReleaseActor(void);	// �A�N�^�[�j��
	void DeleteCollisionActor(const bool bRelase);	// �A�N�^�[�̍폜����
	void InitAllColorActor(void);					// �A�N�^�[�̐F�S������

	// �����o�ϐ�
	CActor* m_pActor;	// �A�N�^�[���
	SInfo m_infoCreate;	// �A�N�^�[�z�u���
	bool m_bSave;		// �ۑ���
};

#endif	// _EDIT_ACTOR_H_
#endif
