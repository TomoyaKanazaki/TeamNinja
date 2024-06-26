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

	// ���
	enum EType
	{
		TYPE_STALK = 0,		// �������G
		TYPE_CAVEAT,		// �x���G
		TYPE_WOLF,			// �T�G
		TYPE_MAX			// ���̗񋓌^�̑���
	};

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
	static CEnemy* Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const EType type			// ���
	);
	static CListManager<CEnemy>* GetList(void);			// ���X�g�擾

	// �����o�֐�
	inline D3DXVECTOR3 GetOldPosition(void) const			{ return m_oldPos; }				// �ߋ��ʒu�擾
	inline void SetDestRotation(const D3DXVECTOR3& rRot)	{ m_destRot = rRot; }				// �ړI�̌����ݒ�
	inline D3DXVECTOR3 GetDestRotation(void) const			{ return m_destRot; }				// �ړI�̌����擾
	inline void SetMovePosition(const D3DXVECTOR3& rMove)	{ m_move = rMove; }					// �ʒu�ړ��ʐݒ�
	inline D3DXVECTOR3 GetMovePosition(void) const			{ return m_move; }					// �ʒu�ړ��ʎ擾
	inline void SetItem(CEnemyItem* pItem)					{ m_pItem = pItem; }				// �G�̎������ݒ�
	inline CEnemyItem* GetItem(void) const					{ return m_pItem; }					// �G�̎������擾

protected:

	// �������z�֐�
	virtual int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) = 0;	// ��Ԃ̍X�V����
	virtual void UpdateMotion(int nMotion, const float fDeltaTime) = 0;	// ���[�V�����̍X�V����
	virtual void UpdateLanding(D3DXVECTOR3* pPos) = 0;	// ���n�X�V

	// �����o�֐�
	bool SearchPlayer(D3DXVECTOR3* pPos = nullptr);	// �v���C���[�̒T������
	bool SearchClone(D3DXVECTOR3* pPos = nullptr, CPlayerClone** pClone = nullptr);	// ���g�̒T������

	void UpdateOldPosition(void) { m_oldPos = GetVec3Position(); }	// �ߋ��ʒu�X�V
	void UpdateGravity(void);	// �d�͍X�V

	bool IsJump(void) { return m_bJump; }	// �W�����v�󋵎擾

private:

	// �ÓI�����o�ϐ�
	static CListManager<CEnemy>* m_pList;		// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CEnemy>::AIterator m_iterator;	// �C�e���[�^�[
	CEnemyItem* m_pItem;			// �������̏��
	D3DXVECTOR3	m_oldPos;			// �ߋ��ʒu
	D3DXVECTOR3 m_destRot;			// �ړI�̌���
	D3DXVECTOR3	m_move;				// �ړ���
	EType m_type;					// ���
	bool m_bJump;					// �W�����v��
};

#endif	// _ENEMY_H_
