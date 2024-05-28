//============================================================
//
//	�A�N�^�[�w�b�_�[ [actor.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ACTOR_H_
#define _ACTOR_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectModel.h"
#include "listManager.h"
#include "collisionModel.h"

//************************************************************
//	�N���X��`
//************************************************************
// �A�N�^�[�N���X
class CActor : public CObjectModel
{
public:

	// �R���X�g���N�^
	CActor();

	// �f�X�g���N�^
	virtual ~CActor() override;

	// �I�[�o�[���C�h�֐�
	virtual HRESULT Init(void) override;	// ������
	virtual void Uninit(void) override;		// �I��
	virtual void Update(const float fDeltaTime) override;	// �X�V
	virtual void Draw(CShader* pShader = nullptr) override;	// �`��

	// �����o�֐�
	void Collision						// �����蔻�菈��
	(
		D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
		const float fRadius,			// ���a
		const float fHeight,			// ����
		D3DXVECTOR3& rMove,				// �ړ���
		bool& bJump						// �W�����v��
	);

	// �ÓI�����o�֐�
	static CActor* Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �傫��
	);
	static CListManager<CActor>* GetList(void);	// ���X�g�\���̎擾����

private:

	// �ÓI�����o�ϐ�
	static CListManager<CActor>* m_pList;		// ���X�g�\��

	// �����o�ϐ�
	CListManager<CActor>::AIterator m_iterator;	// �C�e���[�^�[
	std::vector<CCollision*> m_collision;		// �����蔻��̃��X�g
};

#endif	// _ACTOR_H_
