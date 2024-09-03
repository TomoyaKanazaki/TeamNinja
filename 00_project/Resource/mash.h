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
	CMash(const D3DXVECTOR3& rPos);
	~CMash();

	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	void SetVec3Scaling(const D3DXVECTOR3& rScale) override;

	// �����ʒu�̎擾����
	D3DXVECTOR3 GetDefaultPos(void) const { return m_posDefault; }

#ifdef _DEBUG

	void SetDefaultPos(const D3DXVECTOR3& rPos);		// �����ʒu�̐ݒ菈��

#endif // _DEBUG


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
	void State(const float fDeltaTime);	// ��ԏ���
	void Close(const float fDeltaTime);	// ������
	void Open(const float fDeltaTime);	// �J�����

	void CollSizeSet(const D3DXVECTOR3& rScale);		// �����蔻��̃T�C�Y�̐ݒ菈��

	// �����o�ϐ�
	D3DXVECTOR3 m_posDefault;	// �����ʒu��ۑ�����ϐ�
	D3DXVECTOR3 m_offsetMove;	// �ړ���̃I�t�Z�b�g��ۑ�����ϐ�
	D3DXVECTOR3 m_move;			// �ړ��ʂ�ۑ�����ϐ�
	D3DXVECTOR3 m_collMax;		// �����蔻��̍ő�l
	D3DXVECTOR3 m_collMin;		// �����蔻��̍ŏ��l
	EState m_state;				// ���

};
