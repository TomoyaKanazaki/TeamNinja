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
// �O���錾
//************************************************************
class CObject3D;			// 3D�|���S��

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�M�~�b�N�N���X
class CEditGimmick : public CEditorObject
{
public:
	// �R���X�g���N�^
	CEditGimmick(CEditStage* pEditor);

	// �f�X�g���N�^
	~CEditGimmick() override;

	// �t�B�[���h���\����
	struct SInfo
	{
		CGimmick::EType type;	// ���
		D3DXVECTOR3 size;		// �傫��
		int nNumActive;			// �����ɉ\�Ȑl��
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
	void UpdateRotation(void) override;	// �����X�V

	// �����o�֐�
	void UpdateSize(void);		// �傫���X�V
	void ChangeType(void);		// ��ޕύX
	void UpdateNumActive(void);	// �����\�l���X�V
	void CreateGimmick(void);	// �t�B�[���h����
	void ReleaseGimmick(void);	// �t�B�[���h�j��
	void DeleteCollisionGimmick(const bool bRelase);	// �t�B�[���h�̍폜����
	void InitAllColorGimmick(void);					// �t�B�[���h�̐F�S������

	// �����o�ϐ�
	CGimmick* m_pGimmick;		// �t�B�[���h���
	CObject3D* m_pAngleSign;	// ���̏��
	SInfo m_infoCreate;	// �t�B�[���h�z�u���
	bool m_bSave;		// �ۑ���
};

#endif	// _EDIT_GIMMICK_H_
#endif
