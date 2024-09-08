//============================================================
//
//	�҂������G�w�b�_�[ [enemyAmbush.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_AMBUSH_H_
#define _ENEMY_AMBUSH_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	�N���X��`
//************************************************************
// �҂������G�N���X
class CEnemyAmbush : public CEnemyAttack
{
public:

	// ���[�V������
	enum EMotion
	{
		MOTION_IDOL = 0,	// �ҋ@���[�V����
		MOTION_WALK,		// ���s���[�V����
		MOTION_FOUND,		// �������[�V����
		MOTION_ATTACK,		// �U�����[�V����
		MOTION_UPSET,		// ���h���[�V����
		MOTION_FALL,		// �������[�V����
		MOTION_LANDING,		// ���n���[�V����
		MOTION_STANDBY,		// �\�����[�V����
		MOTION_BATTLE,		// ���g�ɑ΂���U�����[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyAmbush();

	// �f�X�g���N�^
	~CEnemyAmbush() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	void SetData(void) override;	// ���̐ݒ菈��

	float GetRadius(void) const override;			// ���a�̎擾����
	float GetHeight(void) const override;			// �����̎擾����
	float GetSpeed(void) const override;			// ���x�̎擾����

	// �ÓI�����o�֐�
	static CEnemyAmbush* Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rRot,				// ����
		const float fChaseWidth,				// �ǐՕ�
		const float fChaseDepth					// �ǐՉ��s
	);

private:

	// �I�[�o�[���C�h�֐�
	int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override;	// ��Ԃ̍X�V����
	void UpdateMotion(int nMotion, const float fDeltaTime) override;	// ���[�V�����̍X�V����
	void UpdateLanding(D3DXVECTOR3* pPos) override;	// ���n�X�V
	bool BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight) override;		// ���̈ʒu�ɖ߂鏈��

	// �����o�֐�
	int Original(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override;	// ���ꂼ��̓Ǝ�����
	int Warning				// �x������
	(
		D3DXVECTOR3* pPos,		// �ʒu
		D3DXVECTOR3* pRot,		// ����
		const float fDeltaTime,	// �f���^�^�C��
		const float fRotRev		// �����̕␳��
	) override;
	int Stalk				// �ǐՏ���
	(
		D3DXVECTOR3* pPos,		// �ʒu
		D3DXVECTOR3* pRot, 		// ����
		const float fDeltaTime,	// �f���^�^�C��
		const float fRotRev		// �����̕␳��
	) override;
	int Attack(const D3DXVECTOR3& rPos) override;	// �U������
	int BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime, const float fRotRev) override;		// �󔒍U������
	int Upset(void) override;						// ���h����
	int Stance(void) override;						// �\������

};

#endif	// _ENEMY_CHASE_H_
