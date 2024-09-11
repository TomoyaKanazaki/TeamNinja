//============================================================
//
//	����UI�w�b�_�[ [goditemUI.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GODITEMUI_H_
#define _GODITEMUI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "godItem.h"

//************************************************************
//	�O���錾
//************************************************************
class CAnim2D;	// �A�j��2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �_��UI�N���X
class CGodItemUI : public CObject
{
public:

	// ���ʃ}�[�N�̏��
	struct SBody
	{
		CAnim2D* pMark;	// ���ʖ{�̂̏��
		bool bGet;			// �擾��
	};

	// �R���X�g���N�^
	CGodItemUI();

	// �f�X�g���N�^
	~CGodItemUI();

	// �����o�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	void Get(const CGodItem::EType type);			// �擾����

	// �ÓI�����o�֐�
	static CGodItemUI* Create(void);				// ����
	static CListManager<CGodItemUI>* GetList();		// ���X�g�擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override { CObject::Release(); }	// �j��

	// �����o�ϐ�
	CListManager<CGodItemUI>::AIterator m_iterator;	// �C�e���[�^�[

	CAnim2D* m_apGround[CGodItem::TYPE_MAX];	// ���n�̏��
	SBody m_aBody[CGodItem::TYPE_MAX];			// �{�̂̏��

	// �ÓI�����o�ϐ�
	static CListManager<CGodItemUI>* m_pList;	// �I�u�W�F�N�g���X�g
};

#endif	// _GODITEMUI_H_
