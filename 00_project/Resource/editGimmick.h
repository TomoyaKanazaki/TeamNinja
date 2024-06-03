#if 1
//============================================================
//
//	�G�f�B�b�g�M�~�b�N�w�b�_�[ [editGimmick.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_GIMMICK_H_
#define _EDIT_GIMMICK_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editorObject.h"
#include "gimmick.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�M�~�b�N�N���X
class CEditGimmick : public CEditorObject
{
public:
	// �R���X�g���N�^
	CEditGimmick();

	// �f�X�g���N�^
	~CEditGimmick() override;

	// �M�~�b�N���\����
	struct SInfo
	{
		CGimmick::EType type;	// ���
		D3DXVECTOR3 size;		// �傫��
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
	void UpdateRotation(void) override;		// �����X�V

	// �����o�֐�
	void UpdateSize(void);		// �傫���X�V
	void ChangeType(void);		// ��ލX�V
	void CreateGimmick(void);		// �M�~�b�N����
	void ReleaseGimmick(void);	// �M�~�b�N�j��
	void DeleteCollisionGimmick(const bool bRelase);	// �M�~�b�N�̍폜����
	void InitAllColorGimmick(void);					// �M�~�b�N�̐F�S������

	// �����o�ϐ�
	CGimmick* m_pGimmick;	// �M�~�b�N���
	SInfo m_infoCreate;		// �M�~�b�N�z�u���
	bool m_bSave;			// �ۑ���
};

#endif	// _EDIT_ACTOR_H_
#endif
