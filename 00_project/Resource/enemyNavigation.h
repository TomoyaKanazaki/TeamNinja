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
class CObjectMeshCube;		// ���b�V���L���[�u

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
		D3DXVECTOR3* pPos,		// �ʒu
		D3DXVECTOR3* pRot,		// ����
		D3DXVECTOR3* pMove,		// �ړ���
		D3DXVECTOR3* pRotDest,	// �ړI�̌���
		const float fSpeed,		// ���x
		const float fDeltaTime	// �f���^�^�C��
	);

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const EState state) { m_state = state; }				// ��Ԃ̐ݒ菈��
	EState GetState(void) const { return m_state; }						// ��Ԃ̎擾����
	void SetStateCount(const int nCount) { m_nStateCount = nCount; }	// ��ԃJ�E���g�̐ݒ菈��

protected:

	// �Z�b�g�E�Q�b�g�֌W
	int GetStateCount(void) const { return m_nStateCount; }					// ��ԃJ�E���g�̎擾����
	void SetPosInit(const D3DXVECTOR3& rPosInit) { m_posInit = rPosInit; }	// �����ʒu�̐ݒ菈��
	D3DXVECTOR3 GetPosInit(void) const { return m_posInit; }				// �����ʒu�̎擾����
	void SetPosDest(const D3DXVECTOR3& rPosDest) { m_posDest = rPosDest; }	// �ڕW�ʒu�̐ݒ菈��
	D3DXVECTOR3 GetPosDest(void) const { return m_posDest; }				// �ڕW�ʒu�̎擾����

private:

	// �������z�����o�֐�
	virtual void StopFunc		// ��~��ԏ���
	(
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rRot,		// ����
		D3DXVECTOR3* pRotDest			// �ړI�̌���
	) = 0;
	virtual void TurnFunc		// �^�[����ԏ���
	(
		D3DXVECTOR3* pRot,				// ����
		D3DXVECTOR3* pMove,				// �ړ���
		const D3DXVECTOR3& rRotDest,	// �ړI�̌���
		const float fSpeed,				// ���x
		const float fDeltaTime			// �f���^�^�C��
	) = 0;
	virtual void MoveFunc		// �ړ���ԏ���
	(
		D3DXVECTOR3* pPos,				// �ʒu
		const D3DXVECTOR3& rMove		// �ړ���
	) = 0;

	// �����o�ϐ�
	D3DXVECTOR3 m_posInit;			// �����ʒu
	D3DXVECTOR3 m_posDest;			// �ڕW�ʒu
	EState m_state;					// ���
	int m_nStateCount;				// ��ԃJ�E���g
};

#endif	// _ENEMY_NAVIGATION_H_
