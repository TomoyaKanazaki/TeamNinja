//===================================
//
// �G�̍U����ԃw�b�_�[[enemyStateAttack.h]
// Author ��������
//
//===================================
#ifndef _ENEMY_STATE_ATTACK_H_
#define _ENEMY_STATE_ATTACK_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "enemyState.h"

// �N���X��`(�G�̍U����ԃN���X)
class CEnemyStateAttack : public CEnemyState
{
public:

	CEnemyStateAttack(const CEnemy* pEnemy);	// �R���X�g���N�^
	~CEnemyStateAttack();						// �f�X�g���N�^

	void Process(void) override;				// ��ԏ���

private:


};

#endif