//============================================================
//
//	�^�C�}�[�w�b�_�[ [timer.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TIMER_H_
#define _TIMER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �^�C�}�[�N���X
class CTimer : public CObject
{
public:
	// �v����
	enum EState
	{
		STATE_NONE = 0,	// �����Ȃ�
		STATE_MEASURE,	// �v����
		STATE_END,		// �v���I�� 
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// ���Ԍ`����
	enum ETime
	{
		TIME_MSEC,	// �~���b
		TIME_SEC,	// �b
		TIME_MIN,	// ��
		TIME_MAX	// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CTimer();

	// �f�X�g���N�^
	~CTimer() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CTimer *Create	// ����
	( // ����

	);

	// �����o�֐�
	void Start(void);	// �v���J�n
	void End(void);		// �v���I��
	void EnableStop(const bool bStop);	// �v����~�ݒ�
	void SetLimit(const ETime stateTime, const float fTime);	// �������Ԑݒ�

	EState GetState(void) const	{ return m_state; }		// �v����Ԏ擾
	float GetLimit(void) const	{ return m_fLimit; }	// �������Ԏ擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override { CObject::Release(); }	// �j��

	// �����o�ϐ�
	EState	m_state;	// �v�����
	float	m_fTime;	// �v������
	float	m_fLimit;	// ��������
	bool	m_bStop;	// �v����~��
};

#endif	// _TIMER_H_
