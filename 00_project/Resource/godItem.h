//============================================================
//
//	�_��w�b�_�[ [godItem.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GODITEM_H_
#define _GODITEM_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectModel.h"

//************************************************************
//	�N���X��`
//************************************************************
// �_��N���X
class CGodItem : public CObjectModel
{
public:

	// ���
	enum EType
	{
		TYPE_MIRROR = 0,	// ���@��
		TYPE_SWORD,			// ���㌕
		TYPE_MAGATAMA,		// ����������
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CGodItem();

	// �f�X�g���N�^
	~CGodItem() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	// �����o�֐�
	bool Collision						// �����蔻�菈��
	(
		D3DXVECTOR3& rPos,				// �ʒu
		const float fRadius,			// ���a
		const float fHeight				// ����
	);

	// �Z�b�g�E�Q�b�g�֌W
	void SetType(const EType type);		// ��ނ̐ݒ菈��
	EType GetType(void) const;			// ��ނ̎擾����

	// �ÓI�����o�֐�
	static CGodItem* Create(const D3DXVECTOR3& rPos, const EType type);		// ��������
	static CListManager<CGodItem>* GetList(void);			// ���X�g�\���̎擾����
	static HRESULT LoadSetup(const char* pPass);			// �Z�b�g�A�b�v

private:

	// �ÓI�����o�֐�
	static bool DuplicationCheck(const EType type);		// �d���`�F�b�N����

	// �ÓI�����o�ϐ�
	static CListManager<CGodItem>* m_pList;			// ���X�g�\��

	// �����o�ϐ�
	CListManager<CGodItem>::AIterator m_iterator;	// �C�e���[�^�[
	EType m_type;									// ���
};

#endif	// _COIN_H_
