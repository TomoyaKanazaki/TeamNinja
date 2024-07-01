//============================================================
//
//	�T�G�w�b�_�[ [enemyWolf.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_WOLF_H_
#define _ENEMY_WOLF_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyAttack.h"

//************************************************************
//	�N���X��`
//************************************************************
// �T�G�N���X
class CEnemyWolf : public CEnemyAttack
{
public:

	// ��ԗ�
	enum EState
	{
		STATE_CRAWL = 0,	// ������
		STATE_CAVEAT,		// �x�����
		STATE_FOUND,		// �ǐՏ��
		STATE_ATTACK,		// �U�����
		STATE_UPSET,		// ���h���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// ���[�V������
	enum EMotion
	{
		MOTION_IDOL = 0,	// �ҋ@���[�V����
		MOTION_RUN,			// ���s���[�V����
		MOTION_FOUND,		// �������[�V����
		MOTION_BITE,		// ���݂����[�V����
		MOTION_TURN,		// �U��������[�V����
		MOTION_FALL,		// �������[�V����
		MOTION_LANDING,		// ���n���[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyWolf();

	// �f�X�g���N�^
	~CEnemyWolf() override;

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
	int UpdateCrawl(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);	// �����Ԏ��̍X�V
	int UpdateCaveat(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);	// �x����Ԏ��̍X�V
	int UpdateFound(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);	// �ǐՏ�Ԏ��̍X�V
	int UpdateAttack(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);	// �U����Ԏ��̍X�V
	int UpdateUpset(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);	// ���h��Ԏ��̍X�V
	void UpdateMove(D3DXVECTOR3& rPos, D3DXVECTOR3& rRot, const float fDeltaTime);	// �ړ��ʁE�ڕW�����̍X�V
	void UpdatePosition(D3DXVECTOR3& rPos, const float fDeltaTime);	// �ʒu�̍X�V
	void UpdateRotation(D3DXVECTOR3& rRot, const float fDeltaTime);	// �����̍X�V
	void UpdateRotation(D3DXVECTOR3& rRot, const float fRevRota, const float fDeltaTime);	// �����̍X�V (�␳�ʐݒ�)
	void LookTarget(const D3DXVECTOR3& rPos);	// �ڕW�ʒu�̎��F

	// �����o�ϐ�
	EState m_state;				// ���
};

#endif	// _ENEMY_CHASE_H_
