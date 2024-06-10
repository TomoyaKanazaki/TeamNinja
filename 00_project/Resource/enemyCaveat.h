//============================================================
//
//	�x���G�w�b�_�[ [enemyCaveat.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_CAVEAT_H_
#define _ENEMY_CAVEAT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"

//************************************************************
//	�N���X��`
//************************************************************
// �x���G�N���X
class CEnemyCaveat : public CEnemy
{
public:

	// ���
	enum EState
	{
		STATE_CRAWL = 0,	// ������
		STATE_CAVEAT,		// �x�����
		STATE_MAX			// ���̗񋓌^�̑���
	};

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
	CEnemyCaveat();

	// �f�X�g���N�^
	~CEnemyCaveat() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

private:

	// �����o�֐�
	void State(void);		// ��ԏ���
	void Caveat(void);		// �x������

	// �����o�ϐ�
	EState m_state;			// ���
};

#endif	// _ENEMY_CHASE_H_
