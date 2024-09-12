//============================================================
//
//	�q�b�g�X�g�b�v�w�b�_�[ [hitStop.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _HITSTOP_H_
#define _HITSTOP_H_

//************************************************************
//	�N���X��`
//************************************************************
// �q�b�g�X�g�b�v�N���X
class CHitStop
{
public:
	// �R���X�g���N�^
	CHitStop();

	// �f�X�g���N�^
	~CHitStop();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(const float fDeltaTime);	// �X�V
	void Draw(void);						// �`��

	void SetStop(const float fCurTime, std::function<void(void)> funcEnd = nullptr);	// ��~�󋵐ݒ�
	bool IsStop(void) const	{ return m_bStop; }	// ��~�󋵎擾

	// �ÓI�����o�֐�
	static CHitStop *Create(void);				// ����
	static void Release(CHitStop *&prHitStop);	// �j��

private:
	// �����o�ϐ�
	std::function<void(void)> m_funcEndStop;	// ��~�I���֐��|�C���^
	bool m_bStop;		// ��~��
	float m_fCurTime;	// ��~����
};

#endif	// _HITSTOP_H_
