//============================================================
//
// startUI�w�b�_�[ [startUI.h]
// Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _STARTUI_H_
#define _STARTUI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// startUI�N���X
class CStartUI : public CObject2D
{
public:

	// ���
	enum EState
	{
		STATE_ZOOM = 0,		// �Y�[�����
		STATE_BOUND,		// �o�E���h���
		STATE_STOP,			// ��~���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CStartUI(void);

	// �f�X�g���N�^
	~CStartUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CStartUI* Create(void);	// ����

private:

	// �����o�֐�
	void State(void);		// ��ԏ���
	void Zoom(void);		// �Y�[������
	void Bound(void);		// �o�E���h����

	// �����o�ϐ�
	D3DXVECTOR3 m_sizeMove;	// �T�C�Y�̈ړ���
	EState m_state;			// ���
};

#endif	// _EFFECT2D_H_
