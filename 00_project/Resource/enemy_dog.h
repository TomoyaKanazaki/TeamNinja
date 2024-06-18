//============================================================
//
//	���G�w�b�_�[ [enemyDog.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_DOG_H_
#define _ENEMY_DOG_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���G�N���X
class CEnemyDog : public CEnemy
{
public:

	// ���[�V������
	enum EMotion
	{
		MOTION_IDOL = 0,	// �ҋ@���[�V����
		MOTION_WALK,		// ���s���[�V����
		MOTION_FOUND,		// �������[�V����
		MOTION_ATTACK,		// �U�����[�V����
		MOTION_UPSET,		// ���h���[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyDog();

	// �f�X�g���N�^
	~CEnemyDog() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	void SetData(void) override;	// ���̐ݒ菈��

private:

};

#endif	// _ENEMY_CHASE_H_
