//============================================================
//
//	�^�C�g���}�l�[�W���[�w�b�_�[ [titleManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TITLEMANAGER_H_
#define _TITLEMANAGER_H_

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �^�C�g���}�l�[�W���[�N���X
class CTitleManager
{
public:
	// �萔
	static constexpr int NUM_LOGO = 4;	// ������

	// �R���X�g���N�^
	CTitleManager();

	// �f�X�g���N�^
	~CTitleManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(const float fDeltaTime);	// �X�V

	// �ÓI�����o�֐�
	static CTitleManager *Create(void);	// ����
	static void Release(CTitleManager *&prTitleManager);	// �j��

private:
	// �����o�֐�


	// �����o�ϐ�
	CObject2D* m_apLogo[NUM_LOGO];	// �^�C�g�����
};

#endif	// _TITLEMANAGER_H_
