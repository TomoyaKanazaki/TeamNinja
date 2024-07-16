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
		STATE_FADEOUT,		// �t�F�[�h�A�E�g���
		STATE_FADEIN,		// �t�F�[�h�C�����
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
	int UpdateAttack(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);		// �U����Ԏ��̍X�V
	int UpdateUpset(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);		// ���h��Ԏ��̍X�V
	int UpdateFadeOut(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	int UpdateFadeIn(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);		// �t�F�[�h�C����Ԏ��̍X�V
	void UpdatePosition(D3DXVECTOR3& rPos, const float fDeltaTime);	// �ʒu�̍X�V
	void UpdateRotation(D3DXVECTOR3& rRot, const float fDeltaTime);	// �����̍X�V

	// �����o�ϐ�
	EState m_state;						// ���
};

#endif	// _ENEMY_CHASE_H_
