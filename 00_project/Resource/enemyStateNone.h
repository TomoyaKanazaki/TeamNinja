//===================================
//
// �G�̒ʏ��ԃw�b�_�[[enemyStateNone.h]
// Author ��������
//
//===================================
#ifndef _ENEMY_STATE_NONE_H_
#define _ENEMY_STATE_NONE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "enemyState.h"

// �N���X��`(�G�̒ʏ��ԃN���X)
class CEnemyStateNone : public CEnemyState
{
public:

	CEnemyStateNone(CEnemy* pEnemy);	// �R���X�g���N�^
	~CEnemyStateNone();						// �f�X�g���N�^

	void Process(void) override;			// ��ԏ���

private:

};

#endif