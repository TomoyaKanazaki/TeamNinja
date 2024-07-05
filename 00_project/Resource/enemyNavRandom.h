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
		D3DXVECTOR3* pPos,		// �ʒu
		D3DXVECTOR3* pRot,		// ����
		D3DXVECTOR3* pMove,		// �ړ���
		D3DXVECTOR3* pRotDest,	// �ړI�̌���
		const float fSpeed,		// ���x
		const float fDeltaTime	// �f���^�^�C��
	) override;

	// �ÓI�����o�֐�
	static CEnemyNavRandom* Create(const D3DXVECTOR3& rPosInit, const float fWidth, const float fDepth);		// ��������

private:

	// �������z�����o�֐�
	void StopFunc		// ��~��ԏ���
	(
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rRot,		// ����
		D3DXVECTOR3* pRotDest			// �ړI�̌���
	) override;
	void TurnFunc		// �^�[����ԏ���
	(
		D3DXVECTOR3* pRot,				// ����
		D3DXVECTOR3* pMove,				// �ړ���
		const D3DXVECTOR3& rRotDest,	// �ړI�̌���
		const float fSpeed,				// ���x
		const float fDeltaTime			// �f���^�^�C��
	) override;
	void MoveFunc		// �ړ���ԏ���
	(
		D3DXVECTOR3* pPos,				// �ʒu
		const D3DXVECTOR3& rMove		// �ړ���
	) override;

	// �����o�֐�
	void DestPosRandom(void);			// �ʒu�̃����_������
	bool PosCorrect(const float fDest, float* fTarget, const float fMove);	// �ʒu�̕␳����

	// �����o�ϐ�
	CObjectMeshCube* m_pRangeCube;	// �͈͂̃u���b�N
};

#endif	// _ENEMY_NAV_RANDOM_H_
