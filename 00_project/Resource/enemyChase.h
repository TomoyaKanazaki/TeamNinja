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

	// ���
	enum EState
	{
		STATE_PLAYER = 0,	// �v���C���[�ǐՏ��
		STATE_CLONE,		// ���g�ǐՏ��
		STATE_MAX			// ���̗񋓌^�̑���
	};

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

	// �����o�֐�
	void TargetSelect(void);		// �W�I�I������
	void Chase(const D3DXVECTOR3 posTarget);		// �ǐՏ���

	// �����o�ϐ�
	EState m_state;			// ���
};

#endif	// _ENEMY_CHASE_H_
