//============================================================
//
//	�G�̃i�r�Q�[�V�����w�b�_�[ [enemyNavigation.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_NAVIGATION_H_
#define _ENEMY_NAVIGATION_H_

//************************************************************
// �O���錾
//************************************************************
class CEnemy;				// �G�̏��

//************************************************************
//	�N���X��`
//************************************************************
// �G�̃i�r�Q�[�V�����N���X
class CEnemyNav
{
public:

	// ���
	enum EState
	{
		STATE_STOP = 0,		// ��~���
		STATE_TURN,			// �^�[�����
		STATE_MOVE,			// �ړ����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyNav();

	// �f�X�g���N�^
	virtual ~CEnemyNav();

	// �I�[�o�[���C�h�֐�
	virtual HRESULT Init(void);		// ������
	virtual void Uninit(void);		// �I��
	virtual void Update				// �X�V
	(
		D3DXVECTOR3* pPos,			// �ʒu
		D3DXVECTOR3* pRot,			// ����
		CEnemy* pEnemy,				// �G�̏��
		const float fSpeed,			// ���x
		const float fDeltaTime		// �f���^�^�C��
	);

	virtual void NavReset(void);	// �i�r�Q�[�V�����̃��Z�b�g����

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const EState state) { m_state = state; }				// ��Ԃ̐ݒ菈��
	EState GetState(void) const { return m_state; }						// ��Ԃ̎擾����
	void SetStateCount(const int nCount) { m_nStateCount = nCount; }	// ��ԃJ�E���g�̐ݒ菈��

protected:

	// �Z�b�g�E�Q�b�g�֌W
	int GetStateCount(void) const { return m_nStateCount; }					// ��ԃJ�E���g�̎擾����
	void SetPosDest(const D3DXVECTOR3& rPosDest) { m_posDest = rPosDest; }	// �ڕW�ʒu�̐ݒ菈��
	D3DXVECTOR3 GetPosDest(void) const { return m_posDest; }				// �ڕW�ʒu�̎擾����

private:

	// �������z�����o�֐�
	virtual void StopFunc	// ��~��ԏ���
	(
		const D3DXVECTOR3& rPos,	// �ʒu
		CEnemy* pEnemy				// �G�̏��
	) = 0;

	virtual void TurnFunc	// �^�[����ԏ���
	(
		D3DXVECTOR3* pRot,			// ����
		CEnemy* pEnemy,				// �G�̏��
		const float fSpeed,			// ���x
		const float fDeltaTime		// �f���^�^�C��
	) = 0;

	virtual void MoveFunc	// �ړ���ԏ���
	(
		D3DXVECTOR3* pPos,			// �ʒu
		CEnemy* pEnemy				// �G�̏��
	) = 0;

	// �����o�ϐ�
	D3DXVECTOR3 m_posDest;			// �ڕW�ʒu
	EState m_state;					// ���
	int m_nStateCount;				// ��ԃJ�E���g
};

#endif	// _ENEMY_NAVIGATION_H_
