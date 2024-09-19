//============================================================
//
//	�^�b�`�A�N�^�[(��)�w�b�_�[ [touchActorBird.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TOUCH_ACTOR_BIRD_H_
#define _TOUCH_ACTOR_BIRD_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "touchActor.h"

//************************************************************
//	�N���X��`
//************************************************************
// �^�b�`�A�N�^�[(��)�N���X
class CTouchBird : public CTouchActor
{
public:

	// �R���X�g���N�^
	CTouchBird();

	// �f�X�g���N�^
	~CTouchBird() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

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

	// �����o�ϐ�
	bool m_bArrival;		// �A����
};

#endif	// _TOUCH_ACTOR_BIRD_H_
