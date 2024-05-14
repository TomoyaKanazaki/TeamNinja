//===================================
//
// �G�̏��(���)�w�b�_�[[enemyState.h]
// Author ��������
//
//===================================
#ifndef _ENEMY_STATE_H_
#define _ENEMY_STATE_H_
//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "main.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CEnemy;		// �G

// �N���X��`(�G�̏�Ԃ̐e�N���X)
class CEnemyState
{
public:

	explicit CEnemyState(CEnemy* pEnemy);	// �R���X�g���N�^
	virtual ~CEnemyState();			// �f�X�g���N�^

	virtual void Uninit(void);			// �I������

	// �����o�֐�
	virtual void Process(void) = 0;		// ��ԏ���
	CEnemy* GetEnemy(void) { return m_pEnemy; }		// �G�̏��̎擾����

private:

	// �����o�ϐ�
	CEnemy* m_pEnemy;				// �{�̂̏��
};

#endif