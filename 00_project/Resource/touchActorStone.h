//============================================================
//
//	�^�b�`�A�N�^�[(��)�w�b�_�[ [touchActorStone.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TOUCH_ACTOR_STONE_H_
#define _TOUCH_ACTOR_STONE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "touchActor.h"

//************************************************************
//	�N���X��`
//************************************************************
// �^�b�`�A�N�^�[(��)�N���X
class CTouchStone : public CTouchActor
{
public:

	// �R���X�g���N�^
	CTouchStone();

	// �f�X�g���N�^
	~CTouchStone() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	void SetData				// ���̐ݒ菈��
	(
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rScale	// �g�嗦
	) override;

	// �����o�֐�
	bool Collision		// �����蔻��
	(
		const D3DXVECTOR3& rPos,
		const D3DXVECTOR3& rPosOld,
		const float fRadius,
		const float fHeight
	) override;

private:

	// �I�[�o�[���C�h�֐�
	void UpdateNone(const float fDeltaTime) override;		// �ʏ��ԍX�V����
	void UpdateAct(const float fDeltaTime) override;		// �A�N�V������ԍX�V����
};

#endif	// _TOUCH_ACTOR_CAN_H_
