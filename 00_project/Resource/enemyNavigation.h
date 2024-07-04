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
	void MoveFunc(const D3DXVECTOR3& rPos);		// �ړ���ԏ���

	// �����o�ϐ�
	CObjectMeshCube* m_pRange;		// �͈�
	D3DXVECTOR3 m_posInit;			// �����ʒu
	D3DXVECTOR3 m_posDest;			// �ڕW�ʒu
	EState m_state;					// ���
	int m_nStopCount;				// ��~�J�E���g
};

#endif	// _ACTOR_H_
