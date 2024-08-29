#pragma once
//==========================================
//
//  �ӂ���(mash.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "actor.h"

//==========================================
//  �N���X��`
//==========================================
class CMash : public CActor
{
public:

	// ���
	enum EState
	{
		STATE_CLOSE = 0,	// �����
		STATE_OPEN,			// �J�����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �����o�֐�
	CMash();
	~CMash();

	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	void SetVec3Position(const D3DXVECTOR3& rPos) override;

private:

	void Collision						// �����蔻�菈��
	(
		D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
		const float fRadius,			// ���a
		const float fHeight,			// ����
		D3DXVECTOR3& rMove,				// �ړ���
		bool& bJump						// �W�����v��
	) override;
	void Collision						// �����蔻�菈��(�����Ԃ��I�[�o�[���C�h)
	(
		D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rPosOld,		// �O��̈ʒu
		const float fRadius,			// ���a
		const float fHeight,			// ����
		D3DXVECTOR3& rMove,				// �ړ���
		bool& bJump,					// �W�����v��
		bool& bHit						// �Փ˔���
	) override;

	// �����o�֐�
	void Move(const float fDeltaTime); // �ړ�

	// �����o�ϐ�
	D3DXVECTOR3 m_posDefault;	// �����ʒu��ۑ�����ϐ�
	D3DXVECTOR3 m_offsetMove;	// �ړ���̃I�t�Z�b�g��ۑ�����ϐ�
	D3DXVECTOR3 m_move;			// �ړ��ʂ�ۑ�����ϐ�
	EState m_state;				// ���

};
