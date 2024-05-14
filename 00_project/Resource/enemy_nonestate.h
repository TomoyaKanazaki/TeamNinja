//===================================
//
// �G�̏��(���)�w�b�_�[[enemy_nonestate.h]
// Author ��������
//
//===================================
#ifndef _ENEMY_NONESTATE_H_
#define _ENEMY_NONESTATE_H_
//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "main.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CEnemy;		// �G

// �N���X��`(�G�̏�Ԃ̐e�N���X)
class CEnemyStateNone
{
public:

	explicit CEnemyStateNone(const CEnemy* pEnemy);	// �R���X�g���N�^
	virtual ~CEnemyStateNone();			// �f�X�g���N�^

	virtual void Uninit(void);			// �I������

	virtual void Process(void) = 0;		// ��ԏ���

private:

	// �����o�ϐ�
	const CEnemy* m_pEnemy;				// �{�̂̏��
};

#endif