//============================================================
//
//	�G�w�b�_�[ [enemy.h]
//	Author�F��������
//	Adder �F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectChara.h"
#include "motion.h"

//************************************************************
// �O���錾
//************************************************************
class CEnemyItem;			// �G�̎�����
class CPlayerClone;			// �v���C���[�̕��g

//************************************************************
// �N���X��`
//************************************************************
// �G�N���X
class CEnemy : public CObjectChara
{
public:

	// �R���X�g���N�^
	CEnemy();

	// �f�X�g���N�^
	virtual ~CEnemy();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void Draw(CShader* pShader = nullptr) override;		// �`��

	virtual void SetData(void) = 0;	// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CListManager<CEnemy>* GetList(void);			// ���X�g�擾
	static void VanishAll(void);						// �S�G�̏��ŏ���

	// �����o�֐�
	inline void SetOldPosition(const D3DXVECTOR3& rPosOld)	{ m_oldPos = rPosOld; }				// �ߋ��ʒu�ݒ�
	inline D3DXVECTOR3 GetOldPosition(void) const			{ return m_oldPos; }				// �ߋ��ʒu�擾
	inline D3DXVECTOR3 GetPosInit(void) const				{ return m_posInit; }				// �����ʒu�擾
	inline D3DXVECTOR3 GetRotInit(void) const				{ return m_rotInit; }				// ���������擾
	inline void SetDestRotation(D3DXVECTOR3& rRot)			{ useful::NormalizeRot(rRot); m_destRot = rRot; }	// �ړI�̌����ݒ�
	inline D3DXVECTOR3 GetDestRotation(void) const			{ return m_destRot; }				// �ړI�̌����擾
	inline void SetMovePosition(const D3DXVECTOR3& rMove)	{ m_move = rMove; }					// �ʒu�ړ��ʐݒ�
	inline D3DXVECTOR3 GetMovePosition(void) const			{ return m_move; }					// �ʒu�ړ��ʎ擾
	inline void SetItem(CEnemyItem* pItem)					{ m_pItem = pItem; }				// �G�̎������ݒ�
	inline CEnemyItem* GetItem(void) const					{ return m_pItem; }					// �G�̎������擾
	virtual float GetRadius(void) const = 0;			// ���a�̎擾����
	virtual float GetHeight(void) const = 0;			// �����̎擾����

protected:

	// �������z�֐�
	virtual int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) = 0;	// ��Ԃ̍X�V����
	virtual void UpdateMotion(int nMotion, const float fDeltaTime) = 0;	// ���[�V�����̍X�V����
	virtual void UpdateLanding(D3DXVECTOR3* pPos) = 0;	// ���n�X�V

	// �����o�֐�
	bool Collision(D3DXVECTOR3& rPos);		// �����蔻�菈��
	bool SearchPlayer(D3DXVECTOR3* pPos = nullptr);	// �v���C���[�̒T������
	bool SearchClone(D3DXVECTOR3* pPos = nullptr, CPlayerClone** pClone = nullptr);	// ���g�̒T������

	void UpdateOldPosition(void) { m_oldPos = GetVec3Position(); }	// �ߋ��ʒu�X�V
	void UpdateGravity(void);	// �d�͍X�V

	inline void SetPosInit(const D3DXVECTOR3& rPosInit) { m_posInit = rPosInit; }	// �����ʒu�ݒ�(�������̂ݐݒ�)
	inline void SetRotInit(const D3DXVECTOR3& rRotInit) { m_rotInit = rRotInit; }	// ���������ݒ�(�������̂ݐݒ�)
	inline void SetEnableJump(const bool bJump)			{ m_bJump = bJump; }		// �W�����v�󋵐ݒ�
	inline bool IsJump(void)							{ return m_bJump; }			// �W�����v�󋵎擾

private:

	// �����o�֐�
	virtual void CollisionActor(D3DXVECTOR3& rPos, bool& bHit);		// �A�N�^�[�̓����蔻�菈��
	void CollisionPlayer(const D3DXVECTOR3& rPos, const float fRadius, const float fHeight);		// �v���C���[�Ƃ̓����蔻�菈��

	// �ÓI�����o�ϐ�
	static CListManager<CEnemy>* m_pList;		// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CEnemy>::AIterator m_iterator;	// �C�e���[�^�[
	CEnemyItem* m_pItem;			// �������̏��
	D3DXVECTOR3	m_oldPos;			// �ߋ��ʒu
	D3DXVECTOR3 m_posInit;			// �����ʒu
	D3DXVECTOR3 m_destRot;			// �ړI�̌���
	D3DXVECTOR3	m_move;				// �ړ���
	D3DXVECTOR3 m_rotInit;			// ��������
	bool m_bJump;					// �W�����v��
	bool m_bVanish;					// ���ŏ�
};

#endif	// _ENEMY_H_
