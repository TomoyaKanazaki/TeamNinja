//============================================================
//
//	�^�b�`�A�N�^�[�w�b�_�[ [touchActor.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TOUCH_ACTOR_H_
#define _TOUCH_ACTOR_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectModel.h"
#include "listManager.h"

//************************************************************
//	�N���X��`
//************************************************************
// �^�b�`�A�N�^�[�N���X
class CTouchActor : public CObjectModel
{
public:

	// ��ޗ�
	enum EType
	{
		TYPE_CAN = 0,	// ��
		TYPE_BIRD,		// ��
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CTouchActor();

	// �f�X�g���N�^
	virtual ~CTouchActor() override;

	// �I�[�o�[���C�h�֐�
	virtual HRESULT Init(void) override;	// ������
	virtual void Uninit(void) override;		// �I��
	virtual void Update(const float fDeltaTime) override;	// �X�V
	virtual void Draw(CShader* pShader = nullptr) override;	// �`��

	// �����o�֐�
	virtual bool Collision		// �����蔻��
	(
		D3DXVECTOR3& rPos,
		const D3DXVECTOR3& rPosOld,
		const float fRadius,
		const float fHeight
	) = 0;

	// �Z�b�g�E�Q�b�g�֐�
	void SetType(const EType type) { m_type = type; }		// ��ނ̐ݒ菈��
	EType GetType(void) const { return m_type; }			// ��ނ̎擾����

	// �ÓI�����o�֐�
	static CTouchActor* Create	// ����
	( // ����
		const EType type,						// ���
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �g�嗦
	);
	static CListManager<CTouchActor>* GetList(void);	// ���X�g�\���̎擾����

private:

	// �ÓI�����o�ϐ�
	static CListManager<CTouchActor>* m_pList;			// ���X�g�\��

	// �����o�ϐ�
	CListManager<CTouchActor>::AIterator m_iterator;	// �C�e���[�^�[

	EType m_type;	// ���
};

#endif	// _ACTOR_H_
