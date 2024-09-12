//============================================================
//
//	���ʃ}�[�NUI�w�b�_�[ [goditem_mark.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GODITEM_MARK_H_
#define _GODITEM_MARK_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "anim2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �_��}�[�N�N���X
class CGodItemMark : public CAnim2D
{
public:

	// ���
	enum EState
	{
		STATE_NONE = 0,	// �������
		STATE_SCALING,	// �g�k���
		STATE_STOP,		// ��~���
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CGodItemMark(const int nPriority);

	// �f�X�g���N�^
	~CGodItemMark();

	// �����o�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	void SetData					// ���̐ݒ菈��
	(
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const int nIdx				// �C���f�b�N�X
	);

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const EState state)	{ m_state = state; }	// ��Ԑݒ菈��
	EState GetState(void) const			{ return m_state; }		// ��Ԏ擾����

	// �ÓI�����o�֐�
	static CGodItemMark* Create		// ����
	(
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const int nIdx,				// �C���f�b�N�X
		const int nPriority			// �D�揇��
	);

private:

	// �����o�֐�
	void UpdateScaling(void);		// �g�k����

	// �����o�ϐ�
	EState m_state;		// ���
	float m_fScaling;	// �g�k��
};

#endif	// _GODITEM_MARK_H_
