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

	// ���
	enum EState
	{
		STATE_AMBUSH = 0,	// �҂��������
		STATE_WARNING,		// �x�����
		STATE_STALK,		// �ǐՏ��
		STATE_ATTACK,		// �U�����
		STATE_BLANKATTACK,	// �󔒍U�����
		STATE_UPSET,		// ���h���
		STATE_STANCE,		// �\�����
		STATE_MAX			// ���̗񋓌^�̑���
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

	// �����o�֐�
	EMotion Ambush(D3DXVECTOR3* pPos, const float fDeltaTime);			// �҂���������
	EMotion Warning(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);	// �x������
	EMotion Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);	// �ǐՏ���
	EMotion Attack(const D3DXVECTOR3& rPos);	// �U������
	EMotion BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime);		// �󔒍U������
	EMotion Upset(void);						// ���h����
	EMotion Stance(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot);				// �\������
	bool BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot);			// ���̈ʒu�ɖ߂鏈��
	void SetState(const EState state);			// ��Ԃ̐ݒ菈��
	void WalkSound(void);						// ���s������

	// �����o�ϐ�
	EState m_state;						// ���
	int m_nStateCount;					// ��ԃJ�E���g
	int m_nRegressionCount;				// ��A�J�E���g
};

#endif	// _ENEMY_CHASE_H_
