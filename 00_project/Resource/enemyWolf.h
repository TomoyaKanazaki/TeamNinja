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
// �O���錾
//************************************************************
class CEnemyNav;		// �G�̃i�r�Q�[�V����

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
		STATE_BLANKATTACK,	// �󔒍U�����
		STATE_UPSET,		// ���h���
		STATE_STANCE,		// �\�����
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

	float GetRadius(void) const override;			// ���a�̎擾����
	float GetHeight(void) const override;			// �����̎擾����

	// �ÓI�����o�֐�
	static CEnemyWolf* Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const float fMoveWidth,		// �ړ���
		const float fMoveDepth,		// �ړ����s
		const float fChaseWidth,	// �ǐՕ�
		const float fChaseDepth		// �ǐՉ��s
	);
	static CEnemyWolf* Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rRot,				// ����
		const std::vector<D3DXVECTOR3> route,	// ���[�g�̔z��
		const float fChaseWidth,				// �ǐՕ�
		const float fChaseDepth					// �ǐՉ��s
	);

private:

	// �I�[�o�[���C�h�֐�
	int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override;	// ��Ԃ̍X�V����
	void UpdateMotion(int nMotion, const float fDeltaTime) override;	// ���[�V�����̍X�V����
	void UpdateLanding(D3DXVECTOR3* pPos) override;	// ���n�X�V
	void NavMoitonSet(int* pMotion);				// �i�r�Q�[�V�����ɂ�郂�[�V�����̐ݒ菈��

	// �����o�֐�
	int UpdateCrawl(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);		// �����Ԏ��̍X�V
	int UpdateCaveat(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);		// �x����Ԏ��̍X�V
	int UpdateFound(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);		// �ǐՏ�Ԏ��̍X�V
	int UpdateAttack(const D3DXVECTOR3& rPos);											// �U����Ԏ��̍X�V
	int UpdateBlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime);					// �󔒍U������
	int UpdateUpset(void);		// ���h��Ԏ��̍X�V
	int UpdateStance(void);		// �\������
	bool BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight) override;		// ���̈ʒu�ɖ߂鏈��
	void SetState(const EState state);			// ��Ԃ̐ݒ菈��
	void UpdatePosition(D3DXVECTOR3& rPos, const float fDeltaTime);	// �ʒu�̍X�V

	// �����o�ϐ�
	CEnemyNav* m_pNav;					// �i�r�Q�[�V�����̏��
	EState m_state;						// ���
	int m_nStateCount;					// ��ԃJ�E���g
	int m_nRegressionCount;				// ��A�J�E���g
};

#endif	// _ENEMY_CHASE_H_
