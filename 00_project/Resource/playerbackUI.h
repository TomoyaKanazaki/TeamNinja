//============================================================
//
// �v���C���[��AUI�w�b�_�[ [playerbackUI.h]
// Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYERBACKUI_H_
#define _PLAYERBACKUI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// CPlayerBackUI�N���X
class CPlayerBackUI : public CObject2D
{
public:

	// ���
	enum EState
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_ADD,			// ���Z���
		STATE_SUB,			// ���Z���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CPlayerBackUI();

	// �f�X�g���N�^
	~CPlayerBackUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const EState state)	{ m_state = state; }		// ��Ԑݒ菈��

	// �ÓI�����o�֐�
	static CPlayerBackUI* Create(void);	// ����

private:

	// �����o�֐�
	void AddFunc(const float fDeltaTime);	// ���Z��ԏ���
	void SubFunc(const float fDeltaTime);	// ���Z��ԏ���

	// �����o�ϐ�
	EState m_state;		// ���
};

#endif	// _EFFECT2D_H_
