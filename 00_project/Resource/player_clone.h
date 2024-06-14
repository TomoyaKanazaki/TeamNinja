//============================================================
//
//	�v���C���[�̕��g�w�b�_�[ [player_clone.h]
//	Author�F��������
//	Adder �F���c�E��
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
class CField;	// �n�ʃN���X
class CGimmickAction;	// �A�N�V�����M�~�b�N�N���X
class CField;	// �t�B�[���h�N���X

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
		MODEL_MAX			// ���̗񋓌^�̑���
	};

	// ���[�V������
	enum EMotion
	{
		MOTION_IDOL = 0,	// �ҋ@���[�V����
		MOTION_CAUTIOUS,	// �x�����[�V����
		MOTION_DASH,		// ���s���[�V����
		MOTION_STEALTHWALK,	// �E�ё����[�V����
		MOTION_FALL,		// �������[�V����
		MOTION_LANDING,		// ���n���[�V����
		MOTION_JUMP_IDOL,	// �W�����v�䃂�[�V����
		MOTION_CATAPULT,	// �ł��グ���[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	// �s���p�^�[��
	enum EAction
	{
		ACTION_CHASE = 0,		// �ʏ�̍s��
		ACTION_MOVE,			// ���s
		ACTION_MOVE_TO_WAIT,	// �ҋ@�ʒu�ւ̈ړ�
		ACTION_WAIT,			// �M�~�b�N�ҋ@
		ACTION_FALL_TO_WAIT,	// ���Ƃ����x��
		ACTION_FALL,			// ���Ƃ�������
		ACTION_JUMPTABLE,		// �W�����v��
		ACTION_JUMPOFF,			// ��э~��
		ACTION_HEAVYDOOR,		// �d����
		ACTION_MAX
	};

	// �M�~�b�N�Ή����
	enum EGimmick
	{
		GIMMICK_IGNORE = 0, // �M�~�b�N�𖳎�������
		GIMMICK_REACTION, // �M�~�b�N�ɔ���������
		GIMMICK_ACTION, // �M�~�b�N�ɔ����������
		GIMMICK_MAX
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

	bool HitKnockBack(const int nDamage, const D3DXVECTOR3& rVecKnock);	// �m�b�N�o�b�N�q�b�g
	bool Hit(const int nDamage);				// �q�b�g
	void SetGimmick(CGimmickAction* gimmick);	// �M�~�b�N�̃|�C���^���󂯎��
	void DeleteGimmick() { m_pGimmick = nullptr; } // �������Ă���M�~�b�N���폜

	EAction GetAction() const			{ return m_Action; }	// �s�����擾
	CGimmickAction* GetGimmick() const	{ return m_pGimmick; }	// �����M�~�b�N���擾
	void SetAction(EAction action)		{ m_Action = action; }	// �s����ݒ�
	void AddFrags(const char cFrag);							// ������(�t���O)�̒ǉ�
	void SabFrags(const char cFrag);							// ������(�t���O)�̍폜
	bool GetFrags(const char cFrag);							// ������(�t���O)�̎擾
	EGimmick IsGimmickFrag() const { return m_eGimmick; }			// �M�~�b�N�t���O�̎擾
	void SetGimmickFrag(EGimmick gimmick) { m_eGimmick = gimmick; }	//�M�~�b�N�t���O�̐ݒ�

	// �ÓI�����o�֐�
	static CPlayerClone* Create();													// ����
	static CPlayerClone* Create(const D3DXVECTOR3& move);							// ����(���s�^)
	static CPlayerClone* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);	// ����(���s�^)
	static CPlayerClone* Create(CGimmickAction* gimmick);							// ����(���ڃM�~�b�N)
	static void Delete(const int nNum);												// ��������
	static void Delete(const EAction act = ACTION_CHASE);							// �I���������� (����ǉ�)
	static CListManager<CPlayerClone>* GetList(void);								// ���X�g�擾
	static void CallBack();															// ���g���Ăі߂�

private:
	// �����o�֐�
	EMotion UpdateMove(const float fDeltaTime);			// �ړ��s�����̍X�V
	EMotion UpdateChase(const float fDeltaTime);		// �Ǐ]�s�����̍X�V
	EMotion UpdateMoveToWait(const float fDeltaTime);	// �ҋ@�ʒu�ւ̈ړ����̍X�V
	EMotion UpdateWait(const float fDeltaTime);			// �M�~�b�N�ҋ@
	EMotion UpdateFallToWait(const float fDeltaTime);	// ���Ƃ����x��
	EMotion UpdateFall(const float fDeltaTime);			// ���Ƃ�������
	EMotion UpdateJumpTable(const float fDeltaTime);	// �W�����v��s�����̍X�V

	void UpdateOldPosition(void);			// �ߋ��ʒu�̍X�V
	void UpdateGravity(void);				// �d�͂̍X�V
	void UpdateRotation(D3DXVECTOR3& rRot);	// �����̍X�V
	void UpdateLanding(D3DXVECTOR3& rPos, EMotion* rCurMotion = nullptr);	// ���n�󋵂̍X�V

	void UpdateMotion(int nMotion, const float fDeltaTime);		// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	bool UpdateFadeOut(const float fAdd);	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	bool UpdateFadeIn(const float fSub);	// �t�F�[�h�C����Ԏ��̍X�V

	void UpdateIgnore(); // ���������Ԃł̍X�V
	void UpdateReAction(); // ���������Ԃł̍X�V
	void UpdateAction(); // ����������Ԃł̍X�V

	// �����o�֐� (����ǉ�)
	CPlayerClone::EMotion ChasePrev(D3DXVECTOR3* pPosThis, D3DXVECTOR3* pRotThis);	// �O�ɂ��Ă�������
	CPlayerClone::EMotion Chase	// ���Ă�������
	( // ����
		D3DXVECTOR3* pPosThis,			// ���g�̈ʒu
		D3DXVECTOR3* pRotThis,			// ���g�̌���
		const D3DXVECTOR3& rPosPrev,	// ���Ă�����̈ʒu
		const D3DXVECTOR3& rRotPrev		// ���Ă�����̌���
	);
	void ViewTarget(const D3DXVECTOR3& rPosThis, const D3DXVECTOR3& rPosPrev); // �ڕW�̕�������������
	CPlayerClone* Block();				// ���g�o�����Ȃ���̏���
	D3DXVECTOR3 CalcStartPos() const;	// �����ʒu���Z�o
	D3DXVECTOR3 CalcPrevBack(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot) const;	// ��O�̑Ώۂ̌����Z�o
	bool CollisionActor();				// �A�N�^�[�Ƃ̓����蔻��
	void CheckGimmick();			// �M�~�b�N�Ƃ̓����蔻��

	// �ÓI�����o�ϐ�
	static CListManager<CPlayerClone>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CPlayerClone>::AIterator m_iterator;	// �C�e���[�^�[
	CShadow* m_pShadow;			// �e�̏��
	COrbit* m_pOrbit;			// �O�Ղ̏��
	D3DXVECTOR3 m_move;			// �ړ���
	EAction m_Action;			// �s��
	float m_fDeleteTimer;		// �������Ń^�C�}�[
	float m_fGimmickTimer;		// �M�~�b�N��t���ԃ^�C�}�[
	CGimmickAction* m_pGimmick;	// �M�~�b�N�̃|�C���^
	std::string m_sFrags;		// �M�~�b�N�t���O�̕�����
	int m_nIdxGimmick;			// �M�~�b�N���̊Ǘ��ԍ�
	float m_fFallStart;			// ���Ƃ����̗�����O�̍���
	EGimmick m_eGimmick;		// �M�~�b�N�t���O
	bool m_bFind;				// �����t���O
	D3DXVECTOR3 m_size;			// �T�C�Y
	CField* m_pField;			// �t�B�[���h�t���O

	// �����o�ϐ� (���c�ǉ�)
	CField *m_pCurField;	// ���݂̒n��
	CField *m_pOldField;	// �ߋ��̒n��
	D3DXVECTOR3	m_oldPos;	// �ߋ��ʒu
	D3DXVECTOR3	m_destRot;	// �ڕW����
	bool m_bJump;			// �W�����v��
};

#endif	// _PLAYER_CLONE_H_
