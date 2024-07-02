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
class CObjectMeshCylinder;		// ���b�V���V�����_�[

//************************************************************
//	�N���X��`
//************************************************************
// �G�̃i�r�Q�[�V�����N���X
class CEnemyNav
{
public:

	// ���
	enum STATE
	{
		STATE_STOP = 0,		// ��~���
		STATE_TURN,			// �^�[�����
		STATE_WALK,			// ���s���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyNav();

	// �f�X�g���N�^
	~CEnemyNav();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);					// ������
	void Uninit(void);					// �I��
	void Update(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosOld, D3DXVECTOR3* pRot, D3DXVECTOR3* pRotDest, D3DXVECTOR3* pMove);		// �X�V

	// �ÓI�����o�֐�
	static CEnemyNav* Create(const D3DXVECTOR3& rPosInit);		// ��������

private:

	// �����o�֐�
	void Stop(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosOld, D3DXVECTOR3* pRotDest);							// ��~��ԏ���
	void Turn(D3DXVECTOR3* pRot, D3DXVECTOR3* pRotDest, D3DXVECTOR3* pMove);		// �^�[����ԏ���
	void Walk(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosOld, D3DXVECTOR3* pRot, D3DXVECTOR3* pMove);		// ���s��ԏ���

	void CollisionActor(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosOld);	// �A�N�^�[�̓����蔻�菈��
	void CollisionWall(D3DXVECTOR3* pPos);	// �ǂ̓����蔻�菈��

	// �����o�ϐ�
	CObjectMeshCylinder* m_pCylinder;		// ���b�V���V�����_�[�̏��
	D3DXVECTOR3 m_posInit;	// �����ʒu
	D3DXVECTOR3 m_posDest;	// �ڕW�n�_
	STATE m_state;			// ���
	int m_nStopCount;		// ��~�J�E���g
	float m_fRotMove;		// �����̈ړ���
};

#endif	// _ACTOR_H_
