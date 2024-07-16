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
class CEnemyNav;		// �G�̃i�r�Q�[�V����
class CEnemyChaseRange;	// �G�̒ǐՔ͈�

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

	// �Z�b�g�E�Q�b�g�֌W
	CEnemyNav* GetNavigation(void) const		{ return m_pNav; }				// �i�r�Q�[�V�����̏��擾
	CEnemyChaseRange* GetChaseRange(void) const { return m_pChaseRange; }		// �ǐՔ͈͂̏��
	void SetClone(CPlayerClone* pClone)			{ m_pClone = pClone; }			// ���g�̏��ݒ�
	CPlayerClone* GetClone(void) const			{ return m_pClone; }			// ���g�̏��擾
	void SetTargetPos(const D3DXVECTOR3& pos)	{ m_posTarget = pos; }			// �ڕW�̈ʒu�ݒ�
	D3DXVECTOR3 GetTargetPos(void) const		{ return m_posTarget; }			// �ڕW�̈ʒu�擾
	void SetTarget(const ETarget target)		{ m_target = target; }			// �W�I�ݒ�
	ETarget GetTarget(void) const				{ return m_target; }			// �W�I�擾
	void SetAttackCount(const int nCount)		{ m_nAttackCount = nCount; }	// �U���J�E���g�ݒ�
	int GetAttackCount(void) const				{ return m_nAttackCount; }		// �U���J�E���g�擾
	void SetAlpha(const float fAlpha)			{ m_fAlpha = fAlpha; }			// �����x�ݒ�
	float GetAlpha(void) const					{ return m_fAlpha; }			// �����x�擾
	void SetEnableDodge(const bool bAttack)		{ m_bDodge = bAttack; }			// �����t�t���O�ݒ�
	bool IsDodge(void) const					{ return m_bDodge; }			// �����t�t���O�擾

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
	void HitPlayer(const D3DXVECTOR3& rPos);		// �v���C���[�̃q�b�g����
	void HitClone(const D3DXVECTOR3& rPos);			// ���g�̃q�b�g����

	// �ÓI�����o�֐�
	static CEnemyAttack* Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const EType type,			// ���
		const float fMoveWidth,		// �ړ���
		const float fMoveDepth,		// �ړ����s
		const float fChaseWidth,	// �ǐՕ�
		const float fChaseDepth		// �ǐՉ��s
	);
	static CEnemyAttack* Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rRot,				// ����
		const EType type,						// ���
		const std::vector<D3DXVECTOR3> route,	// ���[�g�̔z��
		const float fChaseWidth,				// �ǐՕ�
		const float fChaseDepth					// �ǐՉ��s
	);
	static D3DXVECTOR3 GetAttackUp();	// �����蔻��̎擾
	static D3DXVECTOR3 GetAttackDown();	// �����蔻��̎擾

private:

	// �I�[�o�[���C�h�֐�
	virtual int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override = 0;	// ��Ԃ̍X�V����
	virtual void UpdateMotion(int nMotion, const float fDeltaTime) override = 0;	// ���[�V�����̍X�V����

	// �ÓI�����o�ϐ�
	static CListManager<CEnemyAttack>* m_pList;		// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CEnemyAttack>::AIterator m_iterator;	// �C�e���[�^�[
	CEnemyNav* m_pNav;					// �i�r�Q�[�V�����̏��
	CEnemyChaseRange* m_pChaseRange;	// �ǐՔ͈͂̏��
	CPlayerClone* m_pClone;		// ���g�̏��
	D3DXVECTOR3 m_posTarget;	// �ڕW�̈ʒu
	ETarget m_target;			// �W�I
	EType m_type;				// ���
	int m_nAttackCount;			// �U���J�E���g
	float m_fAlpha;				// �����x
	bool m_bDodge;				// �����t�t���O
};

#endif	// _ENEMY_CHASE_H_
