//============================================================
//
//	�G�̃i�r�Q�[�V����(����)�w�b�_�[ [enemyNavStreet.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_NAV_STREET_H_
#define _ENEMY_NAV_STREET_H_

//************************************************************
// �C���N���[�h�t�@�C��
//************************************************************
#include "enemyNavigation.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�̃i�r�Q�[�V����(����)�N���X
class CEnemyNavStreet : public CEnemyNav
{
public:

	// �R���X�g���N�^
	CEnemyNavStreet();

	// �f�X�g���N�^
	~CEnemyNavStreet() override;

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
	static CEnemyNavStreet* Create(const D3DXVECTOR3& rPosInit, const std::vector<D3DXVECTOR3>& rRoute);		// ��������

private:

	// �����o�֐�
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

	bool PosCorrect(const float fDest, float* fTarget, const float fMove);	// �ʒu�̕␳����

	// �����o�ϐ�
	std::vector<D3DXVECTOR3> m_Route;	// �o�H�z��
};

#endif	// _ENEMY_NAV_STREET_H_

