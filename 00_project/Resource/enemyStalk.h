//============================================================
//
//	�������G�w�b�_�[ [enemyStalk.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_STALK_H_
#define _ENEMY_STALK_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"

//************************************************************
//	�N���X��`
//************************************************************
// �������G�N���X
class CEnemyStalk : public CEnemy
{
public:

	// �^�[�Q�b�g
	enum ETarget
	{
		TARGET_PLAYER = 0,	// �v���C���[�^�[�Q�b�g
		TARGET_CLONE,		// ���g�^�[�Q�b�g
		TARGET_MAX			// ���̗񋓌^�̑���
	};

	// ���
	enum EState
	{
		STATE_CRAWL = 0,	// ������
		STATE_STALK,		// �ǐՏ��
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEnemyStalk();

	// �f�X�g���N�^
	~CEnemyStalk() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

private:

	// �����o�֐�
	void State(void);		// ��ԏ���
	void Crawl(void);		// ���񏈗�
	void Stalk(void);		// �ǐՏ���
	void Move(void);		// �ړ�����
	bool Approach(void);	// �ڋߏ���

	// �����o�ϐ�
	D3DXVECTOR3 m_posTarget;	// �ڕW�̈ʒu
	ETarget m_target;			// �ڕW
	EState m_state;				// ���
};

#endif	// _ENEMY_CHASE_H_
