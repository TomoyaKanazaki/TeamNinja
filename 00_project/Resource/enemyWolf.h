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
#include "enemyAttack.h"

//************************************************************
// �O���錾
//************************************************************
class CEnemyNav;		// �G�̃i�r�Q�[�V����

//************************************************************
//	�N���X��`
//************************************************************
// �T�G�N���X
class CEnemyWolf : public CEnemyAttack
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
		MOTION_FALL,		// �������[�V����
		MOTION_LANDING,		// ���n���[�V����
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

	float GetRadius(void) const override;			// ���a�̎擾����
	float GetHeight(void) const override;			// �����̎擾����
	float GetSpeed(void) const override;			// ���x�̎擾����

	// �ÓI�����o�֐�
	static CEnemyWolf* Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const float fMoveWidth,		// �ړ���
		const float fMoveDepth,		// �ړ����s
		const float fChaseWidth,	// �ǐՕ�
		const float fChaseDepth		// �ǐՉ��s
	);
	static CEnemyWolf* Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rRot,				// ����
		const std::vector<D3DXVECTOR3> route,	// ���[�g�̔z��
		const float fChaseWidth,				// �ǐՕ�
		const float fChaseDepth					// �ǐՉ��s
	);

private:

	// �I�[�o�[���C�h�֐�
	int  UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override;	// ��Ԃ̍X�V����
	void UpdateMotion(int nMotion, const float fDeltaTime) override;	// ���[�V�����̍X�V����
	void UpdateLanding(D3DXVECTOR3* pPos) override;	// ���n�X�V
	void NavMoitonSet(int* pMotion);				// �i�r�Q�[�V�����ɂ�郂�[�V�����̐ݒ菈��

	// �����o�֐�
	int Original(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime) override;	// ���ꂼ��̓Ǝ�����
	int Warning				// �x������
	(
		D3DXVECTOR3* pPos,		// �ʒu
		D3DXVECTOR3* pRot,		// ����
		const float fDeltaTime,	// �f���^�^�C��
		const float fRotRev		// �����̕␳��
	) override;
	int Stalk				// �ǐՏ���
	(
		D3DXVECTOR3* pPos,		// �ʒu
		D3DXVECTOR3* pRot, 		// ����
		const float fDeltaTime,	// �f���^�^�C��
		const float fRotRev		// �����̕␳��
	) override;
	int Attack(const D3DXVECTOR3& rPos) override;	// �U������
	int BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime, const float fRotRev) override;		// �󔒍U������
	int Upset(void) override;						// ���h����
	int Stance(void) override;						// �\������
	bool BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight) override;			// ���̈ʒu�ɖ߂鏈��

	// �����o�ϐ�
	CEnemyNav* m_pNav;					// �i�r�Q�[�V�����̏��
};

#endif	// _ENEMY_CHASE_H_
