//============================================================
//
//	�T�G�w�b�_�[ [enemyWolf.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_WOLF_H_
#define _ENEMY_WOLF_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"

//************************************************************
//	�N���X��`
//************************************************************
// �T�G�N���X
class CEnemyWolf : public CEnemy
{
public:

	// ���[�V������
	enum EMotion
	{
		MOTION_IDOL = 0,	// �ҋ@���[�V����
		MOTION_RUN,			// ���s���[�V����
		MOTION_FOUND,		// �������[�V����
		MOTION_BITE,		// ���݂����[�V����
		MOTION_TURN,		// �U��������[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyWolf();

	// �f�X�g���N�^
	~CEnemyWolf() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	void SetData(void) override;	// ���̐ݒ菈��

private:

};

#endif	// _ENEMY_CHASE_H_
