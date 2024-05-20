//============================================================
//
// popupUI�w�b�_�[ [popupUI.h]
// Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _POPUPUI_H_
#define _POPUPUI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// CPopUpUI�N���X
class CPopUpUI : public CObject2D
{
public:

	// ���
	enum EState
	{
		STATE_ZOOM = 0,		// �Y�[�����
		STATE_BOUND,		// �o�E���h���
		STATE_DISP,			// �\�����
		STATE_FADE,			// �t�F�[�Y���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CPopUpUI();

	// �f�X�g���N�^
	~CPopUpUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CPopUpUI* Create(const char* texture = "data\\TEXTURE\\alcohol.png");	// ����

private:

	// �����o�֐�
	void State(void);		// ��ԏ���
	void Zoom(void);		// �Y�[������
	void Bound(void);		// �o�E���h����
	void Disp(void);		// �\������
	bool Fade(void);		// �t�F�[�h����

	// �����o�ϐ�
	D3DXVECTOR3 m_sizeMove;	// �T�C�Y�̈ړ���
	EState m_state;			// ���
	int m_nDispCount;		// �\���J�E���g
	float m_fSubAlpha;		// �����x�̌��Z��
	float m_fAlpha;			// �����x
};

#endif	// _EFFECT2D_H_
