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

	// ���
	enum EState
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_ACT,			// �A�N�V�������
		STATE_MAX			// ���̗񋓌^�̑���
	};

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

	virtual void SetData				// ���̐ݒ菈��
	(
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rScale	// �g�嗦
	);

	// �����o�֐�
	virtual bool Collision		// �����蔻��
	(
		const D3DXVECTOR3& rPos,
		const D3DXVECTOR3& rPosOld,
		const float fRadius,
		const float fHeight
	) = 0;

	// �Z�b�g�E�Q�b�g�֐�
	void SetType(const EType type)		{ m_type = type; }		// ��ނ̐ݒ菈��
	EType GetType(void) const			{ return m_type; }		// ��ނ̎擾����
	void SetState(const EState state)	{ m_state = state; }	// ��Ԃ̐ݒ菈��
	EState GetState(void) const			{ return m_state; }		// ��Ԃ̎擾����

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

	// �I�[�o�[���C�h�֐�
	virtual void UpdateNone(const float fDeltaTime) = 0;	// �ʏ��ԍX�V����
	virtual void UpdateAct(const float fDeltaTime) = 0;		// �A�N�V������ԍX�V����

	// �����o�ϐ�
	CListManager<CTouchActor>::AIterator m_iterator;	// �C�e���[�^�[

	EType m_type;	// ���
	EState m_state;	// ���

	// �ÓI�����o�ϐ�
	static CListManager<CTouchActor>* m_pList;			// ���X�g�\��
};

#endif	// _TOUCH_ACTOR_H_
