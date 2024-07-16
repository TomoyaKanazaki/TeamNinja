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
#include "enemyAttack.h"

//************************************************************
//	�N���X��`
//************************************************************
// �������G�N���X
class CEnemyStalk : public CEnemyAttack
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
		MOTION_FALL,		// TODO�F�������[�V����
		MOTION_LANDING,		// TODO�F���n���[�V����
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
		STATE_CAUTION,		// �x�����
		STATE_FADEOUT,		// �t�F�[�h�A�E�g���
		STATE_FADEIN,		// �t�F�[�h�C�����
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

	float GetRadius(void) const override;			// ���a�̎擾����
	float GetHeight(void) const override;			// �����̎擾����

private:

	// �I�[�o�[���C�h�֐�
	int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override;	// ��Ԃ̍X�V����
	void UpdateMotion(int nMotion, const float fDeltaTime) override;	// ���[�V�����̍X�V����
	void UpdateLanding(D3DXVECTOR3* pPos) override;	// ���n�X�V
	void NavMotionSet(EMotion* pMotion);			// �i�r�ɂ�郂�[�V�����̐ݒ菈��

	// �����o�֐�
	EMotion Crawl(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);	// ���񏈗�
	EMotion Warning(D3DXVECTOR3* pPos);			// �x������
	EMotion Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime);	// �ǐՏ���
	EMotion Attack(const D3DXVECTOR3& rPos);	// �U������
	EMotion Upset(void);						// ���h����
	EMotion Caution(void);						// �x������
	EMotion FadeOut(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot);		// �t�F�[�h�A�E�g����
	EMotion FadeIn(void);						// �t�F�[�h�C������

	// �����o�ϐ�
	EState m_state;						// ���
	int m_nStateCount;					// ��ԃJ�E���g
	int m_nNumUpsetLoop;				// ���h���[�V�����̃��[�v��
};

#endif	// _ENEMY_CHASE_H_
