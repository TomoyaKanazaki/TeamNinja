#if 1
//============================================================
//
//	�G�f�B�b�g�R�C���w�b�_�[ [editCoin.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_COIN_H_
#define _EDIT_COIN_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editorObject.h"
#include "coin.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�R�C���N���X
class CEditCoin : public CEditorObject
{
public:
	// �R���X�g���N�^
	CEditCoin(CEditStage* pEditor);

	// �f�X�g���N�^
	~CEditCoin() override;

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
	void CreateCoin(void);		// �R�C������
	void ReleaseCoin(void);		// �R�C���j��
	void DeleteCollisionCoin(const bool bRelase);	// �R�C���̍폜����
	void InitAllColorCoin(void);					// �R�C���̐F�S������

	// �����o�ϐ�
	CCoin* m_pCoin;		// �R�C�����
	bool m_bSave;		// �ۑ���
};

#endif	// _EDIT_COIN_H_
#endif
