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

	// ���
	enum EState
	{
		STATE_PLAYER = 0,	// �v���C���[�ǐՏ��
		STATE_CLONE,		// ���g�ǐՏ��
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
	void TargetSelect(void);		// �W�I�I������
	bool Search(const D3DXVECTOR3& posTarget);		// �T������
	void Chase(const D3DXVECTOR3& posTarget);		// �ǐՏ���
	bool Approach(const D3DXVECTOR3& posTarget);	// �ڋߏ���

	// �����o�ϐ�
	EState m_state;			// ���
};

#endif	// _ENEMY_CHASE_H_
