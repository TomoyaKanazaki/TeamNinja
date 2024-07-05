//============================================================
//
//	�U���n�G�w�b�_�[ [enemyAttack.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_ATTACK_H_
#define _ENEMY_ATTACK_H_

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
// �U���n�G�N���X
class CEnemyAttack : public CEnemy
{
public:

	// �^�[�Q�b�g
	enum ETarget
	{
		TARGET_NONE = 0,	// ���Ώ�
		TARGET_PLAYER,		// �v���C���[
		TARGET_CLONE,		// ���g
		TARGET_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyAttack();

	// �f�X�g���N�^
	~CEnemyAttack() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	void SetData(void) override;	// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetClone(CPlayerClone* pClone) { m_pClone = pClone; }			// ���g�̏��ݒ�
	CPlayerClone* GetClone(void) const { return m_pClone; }				// ���g�̏��擾
	void SetTargetPos(const D3DXVECTOR3& pos) { m_posTarget = pos; }	// �ڕW�̈ʒu�ݒ�
	D3DXVECTOR3 GetTargetPos(void) const { return m_posTarget; }		// �ڕW�̈ʒu�擾
	void SetTarget(const ETarget target) { m_target = target; }			// �W�I�ݒ�
	ETarget GetTarget(void) const { return m_target; }					// �W�I�擾
	int GetAttackCount(void) const { return m_nAttackCount; }			// �U���J�E���g�擾
	void SetEnableAttack(const bool bAttack) { m_bAttack = bAttack; }	// �U���󋵐ݒ�
	bool IsAttack(void) const { return m_bAttack; }						// �U���󋵎擾

	// �����o�֐�
	void Move(D3DXVECTOR3* pPos, const D3DXVECTOR3& rRot, const float fSpeed, const float fDeltaTime);			// �ړ�����
	void RotMove(D3DXVECTOR3& rRot, const float fRevRota, const float fDeltaTime);	// �����̈ړ�����
	bool Approach(const D3DXVECTOR3& rPos, const float fDis = 50.0f);				// �ڋߏ���
	bool JudgePlayer(void);		// �v���C���[�̒T������
	bool JudgeClone(void);		// ���g�̒T������

	// TODO�F���Z���J�X���肾�����ŏC��
	void HitPlayer(const D3DXVECTOR3& rPos);		// �v���C���[�̃q�b�g����
	void HitClone(const D3DXVECTOR3& rPos);			// ���g�̃q�b�g����

private:

	// �I�[�o�[���C�h�֐�
	virtual int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override = 0;	// ��Ԃ̍X�V����
	virtual void UpdateMotion(int nMotion, const float fDeltaTime) override = 0;	// ���[�V�����̍X�V����

	// �����o�ϐ�
	CPlayerClone* m_pClone;		// ���g�̏��
	D3DXVECTOR3 m_posTarget;	// �ڕW�̈ʒu
	ETarget m_target;			// �W�I
	int m_nAttackCount;			// �U���J�E���g
	bool m_bAttack;				// �U����TODO�F��ŕύX
};

#endif	// _ENEMY_CHASE_H_
