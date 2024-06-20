//============================================================
//
//	�������G�w�b�_�[ [enemyStalk.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_STALK_H_
#define _ENEMY_STALK_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"

//************************************************************
//	�N���X��`
//************************************************************
// �������G�N���X
class CEnemyStalk : public CEnemy
{
public:

	// �^�[�Q�b�g
	enum ETarget
	{
		TARGET_PLAYER = 0,	// �v���C���[
		TARGET_CLONE,		// ���g
		TARGET_MAX			// ���̗񋓌^�̑���
	};

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
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	// ���
	enum EState
	{
		STATE_CRAWL = 0,	// ������
		STATE_STALK,		// �ǐՏ��
		STATE_ATTACK,		// �U�����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyStalk();

	// �f�X�g���N�^
	~CEnemyStalk() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	void SetData(void) override;	// ���̐ݒ菈��

private:

	// �I�[�o�[���C�h�֐�
	int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot) override;	// ��Ԃ̍X�V����
	void UpdateMotion(int nMotion, const float fDeltaTime) override;	// ���[�V�����̍X�V����
	void UpdateLanding(D3DXVECTOR3* pPos) override;	// ���n�X�V

	// �����o�֐�
	void Crawl(void);		// ���񏈗�
	void Stalk(void);		// �ǐՏ���
	void Move(void);		// �ړ�����
	bool Approach(void);	// �ڋߏ���

	// �����o�ϐ�
	D3DXVECTOR3 m_posTarget;	// �ڕW�̈ʒu
	ETarget m_target;			// �W�I
	EState m_state;				// ���
	float m_fSpeed;				// ���x
};

#endif	// _ENEMY_CHASE_H_
