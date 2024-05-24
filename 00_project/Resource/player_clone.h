//============================================================
//
//	�v���C���[�̕��g�w�b�_�[ [player_clone.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_CLONE_H_
#define _PLAYER_CLONE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectChara.h"

//************************************************************
//	�O���錾
//************************************************************
class CShadow;	// �e�N���X
class COrbit;	// �O�ՃN���X
class CGimmick;	//�@�M�~�b�N�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�̕��g�N���X
class CPlayerClone : public CObjectChara
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

	// �s���p�^�[��
	enum EAction
	{
		ACTION_NONE, // �ʏ�̍s��
		ACTION_MOVE, // ���s
		ACTION_MAX
	};

	// �R���X�g���N�^
	CPlayerClone();

	// �f�X�g���N�^
	~CPlayerClone() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void Draw(CShader* pShader = nullptr) override;		// �`��
	void SetEnableUpdate(const bool bUpdate) override;	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw) override;		// �`��󋵐ݒ�

	bool HitKnockBack(const int nDamage, const D3DXVECTOR3& rVecKnock);		// �m�b�N�o�b�N�q�b�g
	bool Hit(const int nDamage);		// �q�b�g
	EAction GetAction() const { return m_Action; } // �s�����擾
	void SetGimmick(CGimmick* gimmick) { m_pGimmick = gimmick; } // �M�~�b�N�̃|�C���^���󂯎��

	// �ÓI�����o�֐�
	static CPlayerClone* Create();							// ����
	static CPlayerClone* Create(const D3DXVECTOR3& move);	// ����(���s�^)
	static void Delete(const int nNum);						// ��������
	static void Delete(const EAction act = ACTION_NONE);	// �S�������� (����ǉ�)
	static CListManager<CPlayerClone>* GetList(void);		// ���X�g�擾

private:
	// �����o�֐�
	EMotion UpdateNormal(void);		// �ʏ��Ԏ��̍X�V
	void UpdateMotion(int nMotion, const float fDeltaTime);	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	bool UpdateFadeOut(const float fAdd);				// �t�F�[�h�A�E�g��Ԏ��̍X�V
	bool UpdateFadeIn(const float fSub);				// �t�F�[�h�C����Ԏ��̍X�V

	// �����o�֐� (����ǉ�)
	void ChasePrev(); // �O�ɂ��Ă�������
	void Chase(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot); // ���Ă�������
	void ViewTarget(const D3DXVECTOR3& rPos); // �ڕW�̕�������������

	// �ÓI�����o�ϐ�
	static CListManager<CPlayerClone>* m_pList;		// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CPlayerClone>::AIterator m_iterator;	// �C�e���[�^�[
	CShadow* m_pShadow;			// �e�̏��
	COrbit* m_pOrbit;			// �O�Ղ̏��
	D3DXVECTOR3 m_move; // �ړ���
	EAction m_Action; // �s��
	float m_fDeleteTimer; // �������Ń^�C�}�[
	float m_fChargeTimer; // ���ߎ��ԃ^�C�}�[
	CGimmick* m_pGimmick; // �M�~�b�N�̃|�C���^

};

#endif	// _PLAYER_H_
