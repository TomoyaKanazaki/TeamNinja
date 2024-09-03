//============================================================
//
//	�J�����w�b�_�[ [camera.h]
//	Author�F���c�E��
//	Adder : �������
//	Adder : ��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
// �O���錾
//************************************************************
class CPlayer;	// �v���C���[

//************************************************************
//	�N���X��`
//************************************************************
// �J�����N���X
class CCamera
{
public:
	// �J������
	enum EType
	{
		TYPE_MAIN = 0,	// ���C���J����
		TYPE_MODELUI,	// ���f��UI�\���J����
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,		// �Ȃɂ����Ȃ����
		STATE_CONTROL,		// ������
		STATE_ROTATE,		// ��]���
		STATE_FOLLOW,		// �Ǐ]���
		STATE_AROUND,		// ��荞��
		STATE_TELEPHOTO,	// �]��
		STATE_GODITEM,		// �_��l��
		STATE_RESULT,		// ���U���g
		STATE_SELECT,		// �I��
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CCamera();

	// �f�X�g���N�^
	~CCamera();

	// �J�����h��\����
	struct SSwing
	{
	public:
		// �R���X�g���N�^
		SSwing() {}
		SSwing(const float ShiftLength, const float SubAngle, const float SubLength)
		{
			shiftPos		= VEC3_ZERO;	// �ʒu�����
			fShiftAngle		= 0.0f;			// �ʒu�����炷�p�x
			fShiftLength	= ShiftLength;	// �ʒu�����炷����
			fSubAngle		= SubAngle;		// ���炷�p�x�̌��Z��
			fSubLength		= SubLength;	// ���炷�����̌��Z��
		}

		// �f�X�g���N�^
		~SSwing() {}

		// �����o�ϐ�
		D3DXVECTOR3	shiftPos;		// �ʒu�����
		float		fShiftAngle;	// �ʒu�����炷�p�x
		float		fShiftLength;	// �ʒu�����炷����
		float		fSubAngle;		// ���炷�p�x�̌��Z��
		float		fSubLength;		// ���炷�����̌��Z��
	};

	// �J�����\����
	struct SCamera
	{
		D3DXVECTOR3		posV;			// ���݂̎��_
		D3DXVECTOR3		posR;			// ���݂̒����_
		D3DXVECTOR3		destPosV;		// �ڕW�̎��_
		D3DXVECTOR3		destPosR;		// �ڕW�̒����_
		D3DXVECTOR3		posOldV;		// ���݂̎��_
		D3DXVECTOR3		posOldR;		// ���݂̒����_
		D3DXVECTOR3		vecU;			// ������x�N�g��
		D3DXVECTOR3		rot;			// ���݂̌���
		D3DXVECTOR3		destRot;		// �ڕW�̌���
		float			fDis;			// ���݂̎��_�ƒ����_�̋���
		float			fDestDis;		// �ڕW�̎��_�ƒ����_�̋���
		SSwing			swingInfo;		// �J�����h����
		D3DVIEWPORT9	viewportInfo;	// �r���[�|�[�g���
		D3DXMATRIX		mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
		D3DXMATRIX		mtxView;		// �r���[�}�g���b�N�X
	};

	// �X�^�[�g�J�����\����
	struct SStart
	{
		// ���
		enum EState
		{
			STATE_LAND = 0,		// ���n�҂����
			STATE_ROUND,		// ��荞�ݏ��
			STATE_BACK,			// �߂���
			STATE_MAX			// ���̗񋓌^�̑���
		};

		int nCount;			// �J�E���g
		EState state;		// ���
	};

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(const float fDeltaTime);	// �X�V
	void Reset(void);						// �Đݒ�
	void SetCamera(const EType type);		// �J�����ݒ�
	SCamera GetCamera(const EType type);	// �J�����擾
	void SetState(const EState state);		// �J������Ԑݒ�
	EState GetState(void) const;			// �J������Ԏ擾
	void SetSwing(const EType type, const SSwing swing);	// �J�����h��ݒ�
	void SetEnableUpdate(const bool bUpdate);				// �X�V�󋵐ݒ�

	void SetDestRotate(void);		// �J�����ڕW�ʒu�ݒ� (��])
	void SetDestFollow(void);		// �J�����ڕW�ʒu�ݒ� (�Ǐ])
	void SetDestAround(void);		// �J�����ڕW�ʒu�ݒ� (��荞��)
	void SetDestTelephoto(void);	// �J�����ڕW�ʒu�ݒ� (�]��)
	void SetDestGodItem(void);		// �J�����ڕW�ʒu�ݒ� (�_��l��)
	void SetDestResult(void);		// �J�����ڕW�ʒu�ݒ� (���U���g)
	void SetDestSelect(void);		// �J�����ڕW�ʒu�ݒ� (�I��)
	void SetPositionV(const D3DXVECTOR3& rPos);	// ���_�ݒ�
	void SetPositionR(const D3DXVECTOR3& rPos);	// �����_�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	void SetDistance(const float fDis);			// �����ݒ�
	D3DXVECTOR3 GetPositionV(void) const		{ return m_aCamera[TYPE_MAIN].posV; }		// ���݂̎��_�擾
	D3DXVECTOR3 GetDestPositionV(void) const	{ return m_aCamera[TYPE_MAIN].destPosV; }	// �ڕW�̎��_�擾
	D3DXVECTOR3 GetPositionR(void) const		{ return m_aCamera[TYPE_MAIN].posR; }		// ���݂̒����_�擾
	D3DXVECTOR3 GetDestPositionR(void) const	{ return m_aCamera[TYPE_MAIN].destPosR; }	// �ڕW�̒����_�擾
	D3DXVECTOR3 GetRotation(void) const			{ return m_aCamera[TYPE_MAIN].rot; }		// ���݂̌����擾
	D3DXVECTOR3 GetDestRotation(void) const		{ return m_aCamera[TYPE_MAIN].destRot; }	// �ڕW�̌����擾
	float GetDistance(void) const				{ return m_aCamera[TYPE_MAIN].fDis; }		// ���݂̋����擾
	float GetDestDistance(void) const			{ return m_aCamera[TYPE_MAIN].fDestDis; }	// �ڕW�̋����擾

	// ����ǉ�
	D3DXVECTOR3 CalcPlayerPos(); // �v���C���[��񎟌����W�ɕϊ����鏈��
	D3DXVECTOR3 CalcWorldToScreen(const D3DXVECTOR3& pos); // ���[���h���W���X�N���[�����W�ɕϊ�����
	bool OnScreen(const D3DXVECTOR3& pos); // �X�N���[��������
	bool OnScreen(const D3DXVECTOR3& pos, D3DXVECTOR3& posOut); // �X�N���[�����W��Ԃ��X�N���[��������

	// �����ǉ�
	void StartReset(void);					// �X�^�[�g��Ԃ̃��Z�b�g����
	void StartCamera(void);					// �X�^�[�g���o���̃J����
	void StartLand(CPlayer* pPlayer);		// ���n�҂��J����
	void StartRound(CPlayer* pPlayer);		// ��荞�݃J����
	void EnterGame(CPlayer* pPlayer);		// �Q�[���J�ڏ���

	// �ÓI�����o�֐�
	static CCamera *Create(void);				// ����
	static void Release(CCamera *&prCamera);	// �j��

private:
	// �����o�֐�
	void Rotate(void);		// �J�����̍X�V (��])
	void Follow(void);		// �J�����̍X�V (�Ǐ])
	void Control(void);		// �J�����̍X�V (����)
	void GodItem(void);		// �J�����̍X�V (�_��l��)
	void Result(void);		// �J�����̍X�V (���U���g)
	void Select(void);		// �J�����̍X�V (�I��)
	void Move(void);		// �ʒu�̍X�V (����)
	void Distance(void);	// �����̍X�V (����)
	void Rotation(void);	// �����̍X�V (����)
	void Swing(void);		// �J�����h��̍X�V

	// ����ǉ�
	void Around();										// ��荞��
	void CalcAround(const D3DXVECTOR3& posPlayer);		// ��荞�݂̌v�Z
	void Telephoto();									// �]��
	void CollisionWallV();								// ���_�ƕǂ̓����蔻��
	void CollisionWallR();								// �����_�ƕǂ̓����蔻��

	// �����o�ϐ�
	SCamera	m_aCamera[TYPE_MAX];	// �J�����̏��
	SStart	m_startInfo;	// �X�^�[�g�J�����̏��
	EState	m_state;	// ���
	bool	m_bUpdate;	// �X�V��
	float	m_fFov;		// ����p
	float	m_fDis;	// �J��������
};

#endif	// _CAMERA_H_
