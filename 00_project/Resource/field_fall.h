#pragma once
//=========================================
//
//  ���Ƃ����w�b�_�[ (field_fall.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "field.h"

//*****************************************
// �O���錾
//*****************************************
class CObjectMeshCube;		// ���b�V���L���[�u

//=========================================
//  �N���X��`
//=========================================
class CGimmickFall : public CField
{
public:

	// ���
	enum EState
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_OPEN,			// �J�����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �ÓI�����o�ϐ�
	static constexpr int NUM_FLOOR = 2;			// ���̑���

	// �����o�֐�
	CGimmickFall();
	~CGimmickFall() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

	bool IsFall() const override { return m_bFall; } // ��������

	// �I�[�o�[���C�h�֌W
	void SetVec3Position(const D3DXVECTOR3& rPos) override;		// �ʒu�̐ݒ菈��
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;		// �����̐ݒ菈��
	void SetVec2Sizing(const D3DXVECTOR2& rSize) override;		// �T�C�Y�̐ݒ菈��
	void SetType(const EType type) override;					// ��ނ̐ݒ菈��

	// �ÓI�����o�֐�
	static int GetNumFloor(void) { return NUM_FLOOR; }		// ���̑����̎擾����

private:

	// �����o�֐�
	void Hit(CPlayerClone* pClone) override; // �������Ă����ꍇ�̏���
	void Miss(CPlayerClone* pClone) override; // �������Ă��Ȃ��ꍇ�̏���
	void Hit(CPlayer* pPlayer) override; // �������Ă����ꍇ�̏���
	void Miss(CPlayer* pPlayer) override; // �������Ă��Ȃ��ꍇ�̏���

	void Count(); // ����Ă���L�����N�^�[�����̌v�Z����

	// �����ǉ�
	void State(void);		// ��ԏ���
	void Floor(void);		// ���̏���

	// �����o�ϐ�
	CObjectMeshCube* m_apFloor[NUM_FLOOR];	// ���̏��
	EState m_state;		// ���
	bool m_bFall; // �����t���O

};
