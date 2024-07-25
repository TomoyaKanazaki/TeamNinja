//============================================================
//
//	�v���C���[�̕��g�w�b�_�[ [player_clone.h]
//	Author�F��������
//	Adder �F���c�E��
//	Adder �F�������
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
		MOTION_DASH,		// ���s���[�V����
		MOTION_STEALTHWALK,	// �E�ё����[�V����
		MOTION_FALL,		// �������[�V����
		MOTION_LANDING,		// ���n���[�V����
		MOTION_JUMP_IDOL,	// �W�����v��ҋ@���[�V����
		MOTION_JUMP_WALK,	// �W�����v��ړ����[�V����
		MOTION_CATAPULT,	// �W�����v��ł��グ���[�V����
		MOTION_LADDER,		// ��q/���擪���[�V����
		MOTION_BRIDGE,		// �����[�V����
		MOTION_OPEN,		// ���グ���[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	// �s���p�^�[��
	enum EAction
	{
		ACTION_MOVE = 0,		// ���s
		ACTION_FALL_TO_WAIT,	// ���Ƃ����x��
		ACTION_FALL,			// ���Ƃ�������
		ACTION_JUMPTABLE,		// �W�����v��
		ACTION_HEAVYDOOR,		// �d����
		ACTION_STEP,			// ��q
		ACTION_BRIDGE,			// ��
		ACTION_BUTTON,			// �{�^��
		ACTION_VANISH,			// ����
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

	bool HitKnockBack(const int nDamage, const D3DXVECTOR3& rVecKnock);	// �m�b�N�o�b�N�q�b�g
	bool Hit(const int nDamage);				// �q�b�g
	void SetGimmick(CGimmickAction* gimmick);	// �M�~�b�N�̃|�C���^���󂯎��
	void DeleteGimmick();						// �������Ă���M�~�b�N���폜
	void SetField(CField* field);				// �t�B�[���h�̃|�C���^���󂯎��
	void DeleteField(CField* field);			// �������Ă���t�B�[���h���폜
	float GimmickAngle(const EAngle angle);		// �����ϊ�����

	EAction GetAction() const			{ return m_Action; }	// �s�����擾
	CGimmickAction* GetGimmick() const { return m_pGimmick; }	// �����M�~�b�N���擾
	CField* GetField() const { return m_pField; }				// �����t�B�[���h���擾
	void SetAction(EAction action)		{ m_Action = action; }	// �s����ݒ�
	void AddFrags(const char cFrag);							// ������(�t���O)�̒ǉ�
	void SabFrags(const char cFrag);							// ������(�t���O)�̍폜
	bool GetFrags(const char cFrag);							// ������(�t���O)�̎擾

	// �ÓI�����o�֐�
	static CPlayerClone* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);	// ����(���s�^)
	static CPlayerClone* Create(CGimmickAction* gimmick);							// ����(���ڃM�~�b�N)
	static void Delete();															// �S�������� (����ǉ�)
	static void Delete(const int nNum);												// �I���������� (����ǉ�)
	static void Delete(const EAction act);											// �I���������� (����ǉ�)
	static void Delete(CPlayerClone* pClone);										// �I���������� (����ǉ�)
	static CListManager<CPlayerClone>* GetList(void);								// ���X�g�擾
	static float GetRadius();														// ���a�̎擾
	static float GetHeight();														// �g���̎擾

	// �ÓI�����o�֐� (���c�ǉ�)
	static void VanishAll(void);	// �S����

	// �����o�֐� (���c�ǉ�)
	void Vanish(void);	// ����

private:
	// �����o�֐�
	EMotion UpdateMove(const float fDeltaTime);			// �ړ��s�����̍X�V
	EMotion UpdateFallToWait(const float fDeltaTime);	// ���Ƃ����x��
	EMotion UpdateFall(const float fDeltaTime);			// ���Ƃ�������
	EMotion UpdateJumpTable(const float fDeltaTime);	// �W�����v��s�����̍X�V
	EMotion UpdateHeavyDoor(const float fDeltaTime);	// �d�����s�����̍X�V
	EMotion UpdateStep(const float fDeltaTime);			// ��q�s�����̍X�V
	EMotion UpdateBridge(const float fDeltaTime);		// ���s�����̍X�V
	EMotion UpdateButton(const float fDeltaTime);		// �{�^���s�����̍X�V

	void UpdateOldPosition(void);			// �ߋ��ʒu�̍X�V
	void UpdateGravity(void);				// �d�͂̍X�V
	void UpdateRotation(D3DXVECTOR3& rRot);	// �����̍X�V
	void UpdateLanding(D3DXVECTOR3& rPos, EMotion* rCurMotion = nullptr);	// ���n�󋵂̍X�V

	void UpdateMotion(int nMotion, const float fDeltaTime);		// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	bool UpdateFadeOut(const float fAdd);	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	bool UpdateFadeIn(const float fSub);	// �t�F�[�h�C����Ԏ��̍X�V

	bool UpdateActive(const float fDeltaTime); // �A�N�e�B�u��Ԃł̏���

	// �����o�֐� (����ǉ�)
	void CheckGimmick();	// �M�~�b�N�̐ڐG����
	void Approach();		// �ڕW�ʒu�Ɍ���������
	CPlayerClone* Block();	// ���g�o�����Ȃ���̏���
	bool CollisionActor();	// �A�N�^�[�Ƃ̓����蔻��
	bool CollisionWall();	// �ǂƂ̓����蔻��

	// �����o�֐� (���c�ǉ�)
	CPlayerClone* GetGimmickNextClone();	// �M�~�b�N�̎��̕��g�擾

	// �ÓI�����o�ϐ�
	static CListManager<CPlayerClone>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CPlayerClone>::AIterator m_iterator;	// �C�e���[�^�[

	COrbit* m_pOrbit;			// �O�Ղ̏��
	D3DXVECTOR3 m_move;			// �ړ���
	EAction m_Action;			// ���ݍs��
	EAction m_OldAction;		// �ߋ��s��
	float m_fDeleteTimer;		// �������Ń^�C�}�[
	float m_fGimmickTimer;		// �M�~�b�N��t���ԃ^�C�}�[
	CGimmickAction* m_pGimmick;	// �M�~�b�N�̃|�C���^
	std::string m_sFrags;		// �M�~�b�N�t���O�̕�����
	int m_nIdxGimmick;			// �M�~�b�N���̊Ǘ��ԍ�
	float m_fFallStart;			// ���Ƃ����̗�����O�̍���
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
