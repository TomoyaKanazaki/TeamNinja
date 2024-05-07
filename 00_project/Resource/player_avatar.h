//============================================================
//
//	�v���C���[�̕��g�w�b�_�[ [player_avatar.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_AVATAR_H_
#define _PLAYER_AVATAR_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectChara.h"

//************************************************************
//	�O���錾
//************************************************************
class CShadow;	// �e�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�̕��g�N���X
class CPlayerAvatar : public CObjectChara
{
public:
	// ���f����
	enum EModel
	{
		MODEL_WAIST = 0,	// ��
		MODEL_BODY,			// ��
		MODEL_HEAD,			// ��
		MODEL_ARMUL,		// ����r
		MODEL_ARMUR,		// �E��r
		MODEL_ARMDL,		// �����r
		MODEL_ARMDR,		// �E���r
		MODEL_HANDL,		// ����
		MODEL_HANDR,		// �E��
		MODEL_LEGUL,		// ��������
		MODEL_LEGUR,		// �E������
		MODEL_LEGDL,		// ����
		MODEL_LEGDR,		// �E��
		MODEL_FOOTL,		// ����
		MODEL_FOOTR,		// �E��
		MODEL_SWORDL,		// ����
		MODEL_SWORDR,		// �E��
		MODEL_MAX			// ���̗񋓌^�̑���
	};

	// ���[�V������
	enum EMotion
	{
		MOTION_IDOL = 0,	// �ҋ@���[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CPlayerAvatar();

	// �f�X�g���N�^
	~CPlayerAvatar() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void Draw(CShader* pShader = nullptr) override;		// �`��
	void SetEnableUpdate(const bool bUpdate) override;	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw) override;		// �`��󋵐ݒ�

	// �ÓI�����o�֐�
	static CPlayerAvatar* Create(void);					// ����
	static void Delete(const int nNum);					// ��������
	static CListManager<CPlayerAvatar>* GetList(void);	// ���X�g�擾

private:
	// �����o�֐�
	void LoadSetup(void);			// �Z�b�g�A�b�v
	EMotion UpdateNormal(void);		// �ʏ��Ԏ��̍X�V

	void UpdateControl(void);		// ���쏈��
	void UpdateMotion(int nMotion, const float fDeltaTime);	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	bool UpdateFadeOut(const float fAdd);				// �t�F�[�h�A�E�g��Ԏ��̍X�V
	bool UpdateFadeIn(const float fSub);				// �t�F�[�h�C����Ԏ��̍X�V

	// �ÓI�����o�ϐ�
	static CListManager<CPlayerAvatar>* m_pList;		// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CPlayerAvatar>::AIterator m_iterator;	// �C�e���[�^�[
	CShadow* m_pShadow;			// �e�̏��
};

#endif	// _PLAYER_H_
