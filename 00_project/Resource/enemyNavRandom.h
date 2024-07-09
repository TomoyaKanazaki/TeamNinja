//============================================================
//
//	�G�̃i�r�Q�[�V����(�����_�����W)�w�b�_�[ [enemyNavRandom.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_NAV_RANDOM_H_
#define _ENEMY_NAV_RANDOM_H_

//************************************************************
// �C���N���[�h�t�@�C��
//************************************************************
#include "enemyNavigation.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�̃i�r�Q�[�V����(�����_�����W)�N���X
class CEnemyNavRandom : public CEnemyNav
{
public:

	// �R���X�g���N�^
	CEnemyNavRandom();

	// �f�X�g���N�^
	~CEnemyNavRandom() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update						// �X�V
	(
		D3DXVECTOR3* pPos,			// �ʒu
		D3DXVECTOR3* pRot,			// ����
		CEnemy* pEnemy,				// �G�̏��
		const float fSpeed,			// ���x
		const float fDeltaTime		// �f���^�^�C��
	) override;

	// �ÓI�����o�֐�
	static CEnemyNavRandom* Create(const D3DXVECTOR3& rPos, const float fWidth, const float fDepth);		// ��������

private:

	// �����o�֐�
	void StopFunc			// ��~��ԏ���
	(
		const D3DXVECTOR3& rPos,	// �ʒu
		CEnemy* pEnemy				// �G�̏��
	) override;

	void TurnFunc			// �^�[����ԏ���
	(
		D3DXVECTOR3* pRot,			// ����
		CEnemy* pEnemy,				// �G�̏��
		const float fSpeed,			// ���x
		const float fDeltaTime		// �f���^�^�C��
	) override;

	void MoveFunc			// �ړ���ԏ���
	(
		D3DXVECTOR3* pPos,			// �ʒu
		CEnemy* pEnemy				// �G�̏��
	) override;

	bool PosCorrect(const float fDest, float* fTarget, const float fMove);			// �ʒu�̕␳����
	bool CollisionRange(D3DXVECTOR3* pPos, const D3DXVECTOR3& rPosInit);			// �͈͂Ƃ̏Փ�

	// �����o�ϐ�
	CObjectMeshCube* m_pRangeCube;	// �͈͂̃u���b�N
	D3DXVECTOR3 m_MoveRange;		// �ړ��͈�
};

#endif	// _ENEMY_NAV_RANDOM_H_
