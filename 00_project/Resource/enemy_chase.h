//============================================================
//
//	�ǐՓG�w�b�_�[ [enemy_chase.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_CHASE_H_
#define _ENEMY_CHASE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"

//************************************************************
//	�N���X��`
//************************************************************
// �ǐՓG�N���X
class CEnemyChase : public CEnemy
{
public:
	// �R���X�g���N�^
	explicit CEnemyChase(const EType type);

	// �f�X�g���N�^
	~CEnemyChase() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

private:

};

#endif	// _ENEMY_CHASE_H_