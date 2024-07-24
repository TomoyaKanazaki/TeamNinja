//============================================================
//
//	�R�C���w�b�_�[ [coin.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COIN_H_
#define _COIN_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectModel.h"

//************************************************************
//	�N���X��`
//************************************************************
// �R�C���N���X
class CCoin : public CObjectModel
{
public:

	// �R���X�g���N�^
	CCoin();

	// �f�X�g���N�^
	~CCoin() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	// �����o�֐�
	void Collision						// �����蔻�菈��
	(
		D3DXVECTOR3& rPos,				// �ʒu
		const float fRadius,			// ���a
		const float fHeight			// ����
	);

	// �ÓI�����o�֐�
	static CCoin* Create(const D3DXVECTOR3& rPos);		// ��������
	static CListManager<CCoin>* GetList(void);			// ���X�g�\���̎擾����
	static HRESULT LoadSetup(const char* pPass);		// �Z�b�g�A�b�v

private:

	// �ÓI�����o�ϐ�
	static CListManager<CCoin>* m_pList;			// ���X�g�\��

	// �����o�ϐ�
	CListManager<CCoin>::AIterator m_iterator;		// �C�e���[�^�[
};

#endif	// _COIN_H_
