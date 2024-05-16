//============================================================
//
//	�G�w�b�_�[ [enemy.h]
//	Author�F��������
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
class CEnemyState;			// �G�̏�Ԃ̊��

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
		TYPE_CHASE = 0,		// �ǐՓG
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CEnemy(const EType type);

	// �f�X�g���N�^
	virtual ~CEnemy();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void Draw(CShader* pShader = nullptr) override;		// �`��

	// �ÓI�����o�֐�
	static CEnemy* Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const EType type			// ���
	);
	static CListManager<CEnemy>* GetList(void);			// ���X�g�擾

	// �����o�֐�
	inline void UpdateOldPosition(void)						{ m_oldPos = GetVec3Position(); }	// �ߋ��ʒu�X�V
	inline D3DXVECTOR3 GetOldPosition(void) const			{ return m_oldPos; }				// �ߋ��ʒu�擾
	inline void SetDestRotation(const D3DXVECTOR3& rRot)	{ m_destRot = rRot; }				// �ړI�̌����ݒ�
	inline D3DXVECTOR3 GetDestRotation(void) const			{ return m_destRot; }				// �ړI�̌����擾
	inline void SetMovePosition(const D3DXVECTOR3& rMove)	{ m_move = rMove; }					// �ʒu�ړ��ʐݒ�
	inline D3DXVECTOR3 GetMovePosition(void) const			{ return m_move; }					// �ʒu�ړ��ʎ擾

	// �����ǉ��̂��
	CEnemyState* GetState(void) const { return m_pState; }		// ��Ԃ̎擾����
	void ChangeState(CEnemyState* pNext);						// ��Ԃ̐ݒ菈��

protected:

private:

	// �ÓI�����o�ϐ�
	static CListManager<CEnemy>* m_pList;		// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CEnemy>::AIterator m_iterator;	// �C�e���[�^�[
	D3DXVECTOR3	m_oldPos;			// �ߋ��ʒu
	D3DXVECTOR3 m_destRot;			// �ړI�̌���
	D3DXVECTOR3	m_move;				// �ړ���
	EType m_type;					// ���
	CEnemyState* m_pState;			// ���
};

#endif	// _ENEMY_H_
