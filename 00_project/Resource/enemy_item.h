//============================================================
//
//	�G�̎������w�b�_�[ [enemy_item.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_ITEM_H_
#define _ENEMY_ITEM_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectModel.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�̎������N���X
class CEnemyItem : public CObjectModel
{
public:

	// ��ޗ�
	enum EType
	{
		TYPE_KATANA = 0,		// ��
		TYPE_MAX				// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyItem();

	// �f�X�g���N�^
	virtual ~CEnemyItem() override;

	// �I�[�o�[���C�h�֐�
	virtual HRESULT Init(void) override;	// ������
	virtual void Uninit(void) override;		// �I��
	virtual void Update(const float fDeltaTime) override;	// �X�V
	virtual void Draw(CShader* pShader = nullptr) override;	// �`��

	// �����o�֐�
	void Offset(const D3DXMATRIX& rMtx);	// �I�t�Z�b�g����

	// �Z�b�g�E�Q�b�g�֐�
	void SetType(const EType type) { m_type = type; }		// ��ނ̐ݒ菈��
	EType GetType(void) const { return m_type; }			// ��ނ̎擾����

	// �ÓI�����o�֐�
	static CEnemyItem* Create			// ����
	( // ����
		const EType type,				// ���
		const D3DXVECTOR3& rOffset,		// �I�t�Z�b�g
		const D3DXMATRIX& rMtx			// �}�g���b�N�X���
	);

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_offset;	// �I�t�Z�b�g
	EType m_type;			// ���
};

#endif	// _ACTOR_H_
