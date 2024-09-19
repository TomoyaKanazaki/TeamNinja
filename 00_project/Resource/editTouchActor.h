#if 1
//============================================================
//
//	�G�f�B�b�g�^�b�`�A�N�^�[�w�b�_�[ [editTouchActor.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_TOUCH_ACTOR_H_
#define _EDIT_TOUCH_ACTOR_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editorObject.h"
#include "touchActor.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�^�b�`�A�N�^�[�N���X
class CEditTouchActor : public CEditorObject
{
public:

	// �R���X�g���N�^
	CEditTouchActor(CEditStage* pEditor);

	// �f�X�g���N�^
	~CEditTouchActor() override;

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
	void ChangeType(void);		// ��ޕύX
	void CreateCoin(void);		// �R�C������
	void ReleaseCoin(void);		// �R�C���j��
	void DeleteCollisionCoin(const bool bRelase);	// �R�C���̍폜����
	void InitAllColorCoin(void);					// �R�C���̐F�S������

	// �����o�ϐ�
	CTouchActor* m_pActor;		// �R�C�����
	CTouchActor::EType m_type;	// ���
	bool m_bSave;				// �ۑ���
};

#endif	// _EDIT_COIN_H_
#endif
