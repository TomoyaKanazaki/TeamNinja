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
// �O���錾
//************************************************************
class CPlayerClone;		// ���g�̏��

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
		STATE_WARNING,		// �x�����
		STATE_STALK,		// �ǐՏ��
		STATE_ATTACK,		// �U�����
		STATE_UPSET,		// ���h���
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
	int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override;	// ��Ԃ̍X�V����
	void UpdateMotion(int nMotion, const float fDeltaTime) override;	// ���[�V�����̍X�V����
	void UpdateLanding(D3DXVECTOR3* pPos) override;	// ���n�X�V

	// �����o�֐�
	EMotion Crawl(void);				// ���񏈗�
	EMotion Warning(void);				// �x������
	EMotion Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot);	// �ǐՏ���
	EMotion Attack(const D3DXVECTOR3& rPos);				// �U������
	EMotion Upset(void);				// ���h����

	void Move(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot);		// �ړ�����
	bool Approach(const D3DXVECTOR3& rPos);	// �ڋߏ���

	// TODO�F���Z���J�X���肾�����ŏC��
	void HitPlayer(const D3DXVECTOR3& rPos);		// �v���C���[�̃q�b�g����
	void HitClone(const D3DXVECTOR3& rPos);		// ���g�̃q�b�g����

	// �����o�ϐ�
	CPlayerClone* m_pClone;		// ���g�̏��
	D3DXVECTOR3 m_posTarget;	// �ڕW�̈ʒu
	ETarget m_target;			// �W�I
	EState m_state;				// ���
	float m_fSpeed;				// ���x
	bool m_bAttack;				// �U����TODO�F��ŕύX
};

#endif	// _ENEMY_CHASE_H_
