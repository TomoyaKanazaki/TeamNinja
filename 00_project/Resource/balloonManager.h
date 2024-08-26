//============================================================
//
//	�����o���}�l�[�W���[�w�b�_�[ [balloonManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BALLOON_MANAGER_H_
#define _BALLOON_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "godItem.h"

//************************************************************
//	�O���錾
//************************************************************
class CTransPoint;		// �J�ڃ|�C���g�N���X
class CObject2D;		// �I�u�W�F�N�g2D�N���X
class CRoll2D;			// ��]�I�u�W�F�N�g2D�N���X
class CScale2D;			// �g�k�I�u�W�F�N�g2D�N���X
class CAnim2D;			// �A�j���[�V����2D�N���X
class CScrollText2D;	// ��������e�L�X�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �����o���}�l�[�W���[�N���X
class CBalloonManager : public CObject
{
public:
	// �萔
	static constexpr int NUM_STAR = 2;	// �藠���̐�

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// ����
		STATE_STAG,		// ���o
		STATE_END,		// �I��
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CBalloonManager(CTransPoint* pParent, const bool bOpen);

	// �f�X�g���N�^
	~CBalloonManager();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CBalloonManager *Create(CTransPoint* pParent, const bool bOpen);	// ����

	// �����o�֐�
	void SetStag(void);	// ���o�J�n�ݒ�
	bool IsNone(void)	{ return (m_state == STATE_NONE); }	// ���o�󋵎擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override { CObject::Release(); }	// �j��

	// �����o�ϐ�
	CAnim2D* m_apGodItem[CGodItem::TYPE_MAX];	// �_��A�C�R��
	CTransPoint* m_pParent;			// �J�ڃ|�C���g
	CObject2D* m_pStage;			// �X�e�[�W���
	CObject2D* m_pFrame;			// �t���[��
	CRoll2D* m_apStar[NUM_STAR];	// �藠��
	CScale2D* m_pCont;				// ������@
	CScrollText2D* m_pShadow;		// �X�e�[�W���̉e
	CScrollText2D* m_pName;			// �X�e�[�W��
	EState m_state;					// ���
	const bool m_bOpen;				// �X�e�[�W����t���O
};

#endif	// _BALLOON_MANAGER_H_
