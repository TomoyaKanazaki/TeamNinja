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
	~CEnemyNav();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��
	void Update				// �X�V
	(
		D3DXVECTOR3* pPos,		// �ʒu
		D3DXVECTOR3* pRot,		// ����
		D3DXVECTOR3* pMove,		// �ړ���
		D3DXVECTOR3* pRotDest,	// �ړI�̌���
		const float fRadius,	// ���a
		const float fHeight,	// ����
		const float fSpeed,		// ���x
		const float fDeltaTime	// �f���^�^�C��
	);

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const EState state);		// ��Ԃ̐ݒ菈��
	void SetStateCount(const int nCount);	// ��ԃJ�E���g�̐ݒ菈��

	// �ÓI�����o�֐�
	static CEnemyNav* Create(const D3DXVECTOR3& rPosInit, const float fWidth, const float fDepth);		// ��������

private:

	// �����o�֐�
	void StopFunc(D3DXVECTOR3* pRotDest);		// ��~��ԏ���
	void TurnFunc					// �^�[����ԏ���
	(
		D3DXVECTOR3* pRot,				// ����
		D3DXVECTOR3* pMove,				// �ړ���
		const D3DXVECTOR3& rRotDest,	// �ړI�̌���
		const float fSpeed,				// ���x
		const float fDeltaTime			// �f���^�^�C��
	);
	void MoveFunc(D3DXVECTOR3* pPos, const D3DXVECTOR3& rMove);		// �ړ���ԏ���
	bool CollisionRange(D3DXVECTOR3* pPos);		// �͈͂Ƃ̏Փ�

	// �����o�ϐ�
	CObjectMeshCube* m_pRangeCube;	// �͈͂̃u���b�N
	D3DXVECTOR3 m_posInit;			// �����ʒu
	D3DXVECTOR3 m_posDest;			// �ڕW�ʒu
	D3DXVECTOR2 m_MoveRange;		// �ړ��͈�
	EState m_state;					// ���
	int m_nStateCount;				// ��ԃJ�E���g
};

#endif	// _ACTOR_H_
