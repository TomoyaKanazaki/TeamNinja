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
class CEnemyChaseRange;	// �G�̒ǐՔ͈�
class CActor;			// �A�N�^�[

//************************************************************
//	�N���X��`
//************************************************************
// �U���n�G�N���X
class CEnemyAttack : public CEnemy
{
public:

	// ���
	enum EType
	{
		TYPE_STALK = 0,		// �������G
		TYPE_WOLF,			// �T�G
		TYPE_AMBUSH,		// �҂������G
		TYPE_MAX			// ���̗񋓌^�̑���
	};

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

	// �ÓI�����o�֐�
	static CListManager<CEnemyAttack>* GetList(void);			// ���X�g�擾
	static HRESULT LoadSetup(const char* pPass);		// �Z�b�g�A�b�v

	// �Z�b�g�E�Q�b�g�֌W
	void SetChaseRange(CEnemyChaseRange* pChace)	{ assert(m_pChaseRange == nullptr); m_pChaseRange = pChace; }		// �ǐՔ͈͂̏��
	CEnemyChaseRange* GetChaseRange(void) const		{ return m_pChaseRange; }			// �ǐՔ͈͂̏��
	void SetType(const EType type)					{ m_type = type; }					// ��ސݒ�
	EType GetType(void) const						{ return m_type; }					// ��ގ擾
	void SetClone(CPlayerClone* pClone)				{ m_pClone = pClone; }				// ���g�̏��ݒ�
	CPlayerClone* GetClone(void) const				{ return m_pClone; }				// ���g�̏��擾
	void SetTargetPos(const D3DXVECTOR3& pos)		{ m_posTarget = pos; }				// �ڕW�̈ʒu�ݒ�
	D3DXVECTOR3 GetTargetPos(void) const			{ return m_posTarget; }				// �ڕW�̈ʒu�擾
	void SetTarget(const ETarget target)			{ m_target = target; }				// �W�I�ݒ�
	ETarget GetTarget(void) const					{ return m_target; }				// �W�I�擾
	void SetAttackCount(const int nCount)			{ m_nAttackCount = nCount; }		// �U���J�E���g�ݒ�
	int GetAttackCount(void) const					{ return m_nAttackCount; }			// �U���J�E���g�擾
	void SetRegressionCount(const int nCount)		{ m_nRegressionCount = nCount; }	// ��A�J�E���g�ݒ�
	int GetRegressionCount(void) const				{ return m_nRegressionCount; }		// ��A�J�E���g�擾
	void SetEnableDodge(const bool bAttack)			{ m_bDodge = bAttack; }				// �����t�t���O�ݒ�
	bool IsDodge(void) const						{ return m_bDodge; }				// �����t�t���O�擾

	// �����o�֐�
	void Move(D3DXVECTOR3* pPos, const D3DXVECTOR3& rRot, const float fSpeed, const float fDeltaTime);			// �ړ�����
	void RotMove(D3DXVECTOR3& rRot, const float fRevRota, const float fDeltaTime);	// �����̈ړ�����
	void LookTarget(const D3DXVECTOR3& rPos);	// �ڕW�ʒu�̎��F
	bool Approach(const D3DXVECTOR3& rPos, const float fDis = 50.0f);				// �ڋߏ���
	bool JudgePlayer(void);		// �v���C���[�̒T������
	bool JudgeClone(void);		// ���g�̒T������
	bool ShakeOffPlayer(void);	// �v���C���[�̐U��؂菈��
	bool ShakeOffClone(void);	// ���g�̐U��؂菈��

	// TODO�F���Z���J�X���肾�����ŏC��
	bool HitPlayer(const D3DXVECTOR3& rPos);		// �v���C���[�̃q�b�g����
	bool HitClone(const D3DXVECTOR3& rPos);			// ���g�̃q�b�g����

	// �ÓI�����o�֐�
	static D3DXVECTOR3 GetAttackUp();	// �����蔻��̎擾
	static D3DXVECTOR3 GetAttackDown();	// �����蔻��̎擾

protected:

	// �I�[�o�[���C�h�֐�
	virtual bool BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight);		// ���̈ʒu�ɖ߂鏈��

private:

	// �I�[�o�[���C�h�֐�
	virtual int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override = 0;	// ��Ԃ̍X�V����
	virtual void UpdateMotion(int nMotion, const float fDeltaTime) override = 0;	// ���[�V�����̍X�V����
	void CollisionActor(D3DXVECTOR3& rPos, bool& bHit) override;					// �A�N�^�[�̓����蔻�菈��

	// �����o�֐�
	void ActorRegist(void);		// �A�N�^�[�̓o�^����

	// �ÓI�����o�ϐ�
	static CListManager<CEnemyAttack>* m_pList;		// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CEnemyAttack>::AIterator m_iterator;	// �C�e���[�^�[
	CEnemyChaseRange* m_pChaseRange;	// �ǐՔ͈͂̏��
	std::vector<CActor*> m_actor;		// �����蔻������A�N�^�[�̏��
	CPlayerClone* m_pClone;		// ���g�̏��
	D3DXVECTOR3 m_posTarget;	// �ڕW�̈ʒu
	ETarget m_target;			// �W�I
	EType m_type;				// ���
	int m_nAttackCount;			// �U���J�E���g
	int m_nRegressionCount;		// ��A�J�E���g
	bool m_bDodge;				// �����t�t���O
};

#endif	// _ENEMY_CHASE_H_
