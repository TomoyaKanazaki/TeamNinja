//============================================================
//
//	���[�V�����w�b�_�[ [motion.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MOTION_H_
#define _MOTION_H_

//************************************************************
//	�萔�錾
//************************************************************
namespace motion
{
	const int MAX_PARTS	= 32;	// �p�[�c�̍ő吔
}

//************************************************************
//	�O���錾
//************************************************************
class CMultiModel;	// �}���`���f���N���X
class CObjectChara;	// �I�u�W�F�N�g�L�����N�^�[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���[�V�����N���X
class CMotion
{
public:
	// �R���X�g���N�^
	CMotion();

	// �f�X�g���N�^
	~CMotion();

	// ����J�E���g�Ǘ��\����
	struct SCollTime
	{
		SCollTime()
		{
			nMin = 0;
			nMax = 0;
		}

		int nMin;	// �U������̊J�n�J�E���g
		int nMax;	// �U������̏I���J�E���g
	};

	// �L�[���\����
	struct SKey
	{
		SKey()
		{
			pos = VEC3_ZERO;
			rot = VEC3_ZERO;
		}

		D3DXVECTOR3 pos;	// ���f���ʒu
		D3DXVECTOR3 rot;	// ���f������
	};

	// �L�[�Ǘ��\����
	struct SKeyInfo
	{
		SKeyInfo()
		{
			vecKey.clear();
			move = VEC3_ZERO;
			nFrame = 0;
		}

		std::vector<SKey> vecKey;	// �L�[�p�[�c���
		D3DXVECTOR3 move;	// �L�[�ړ���
		int nFrame;			// �L�[�Đ��t���[����
	};

	// ���[�V�����Ǘ��\����
	struct SMotionInfo
	{
		SMotionInfo()
		{
			vecKeyInfo.clear();
			nNumKey = 0;
			nWholeFrame = 0;
			nCancelFrame = 0;
			nComboFrame = 0;
			bLoop = false;
			bWeaponDisp = false;
		}

		std::vector<SKeyInfo> vecKeyInfo;	// �L�[���
		SCollTime collLeft;		// ���U������̃J�E���g
		SCollTime collRight;	// �E�U������̃J�E���g
		int  nNumKey;			// �L�[����
		int  nWholeFrame;		// ���[�V�����S�̃t���[����
		int  nCancelFrame;		// �L�����Z���\�t���[��
		int  nComboFrame;		// �R���{�\�t���[��
		bool bLoop;				// ���[�v ON/OFF
		bool bWeaponDisp;		// ����\�� ON/OFF
	};

	// ���[�V�������\����
	struct SInfo
	{
		SInfo()
		{
			vecMotionInfo.clear();
			vecOriginKey.clear();
			nNumType		= 0;
			nType			= 0;
			nKey			= 0;
			nKeyCounter		= 0;
			nWholeCounter	= 0;
			bFinish			= false;
		}

		std::vector<SMotionInfo> vecMotionInfo;	// ���[�V�������
		std::vector<SKey> vecOriginKey;			// �L�[�p�[�c���_���
		int  nNumType;		// ���[�V������ޑ���
		int  nType;			// ���[�V�������
		int  nKey;			// ���[�V�����L�[�ԍ�
		int  nKeyCounter;	// ���[�V�����L�[�J�E���^�[
		int  nWholeCounter;	// ���[�V�����S�̃J�E���^�[
		bool bFinish;		// ���[�V�����I����
	};

	// �u�����h���\����
	struct SBlend
	{
		SBlend()
		{
			vecKey.clear();
			nFrame = 0;
			nWholeCounter = 0;
		}

		std::vector<SKey> vecKey;	// �u�����h�J�n�p�[�c���
		int nFrame;			// �u�����h�Đ��t���[����
		int nWholeCounter;	// �u�����h�S�̃J�E���^�[
	};

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(const float fDeltaTime);		// �X�V
	void BindPartsData(CMultiModel **ppModel);	// �p�[�c���ݒ�
	void SetAllInfo(const SInfo& rInfo);		// ���[�V�������S�ݒ�
	void AddInfo(const SMotionInfo& rInfo);		// ���[�V�������ǉ�
	void SetEnableUpdate(const bool bUpdate);	// �X�V�󋵐ݒ�
	void SetNumParts(const int nNumParts);		// �p�[�c���ݒ�
	void Set(const int nType, const int nBlendFrame = 0);				// �ݒ�
	void SetOriginPosition(const D3DXVECTOR3& rPos, const int nParts);	// ���_�ʒu�̐ݒ�
	void SetOriginRotation(const D3DXVECTOR3& rRot, const int nParts);	// ���_�����̐ݒ�

	int  GetType(void) const;					// ��ގ擾
	int  GetNumType(void) const;				// ��ޑ����擾
	int  GetKey(void) const;					// �L�[�ԍ��擾
	int  GetNumKey(const int nType) const;		// �L�[�����擾
	int  GetKeyCounter(void) const;				// ���[�V�����L�[�J�E���^�[�擾
	int  GetWholeCounter(void) const;			// ���[�V�����S�̃J�E���^�[�擾
	int  GetWholeFrame(const int nType) const;	// ���[�V�����S�̃t���[�����擾
	int  GetCancelFrame(const int nType) const;	// ���[�V�����L�����Z���t���[���擾
	int  GetComboFrame(const int nType) const;	// ���[�V�����R���{�t���[���擾
	bool IsFinish(void) const;					// �I���擾
	bool IsLoop(const int nType) const;			// ���[�v�擾
	bool IsCancel(const int nType) const;		// �L�����Z���擾
	bool IsCombo(const int nType) const;		// �R���{�擾
	bool IsWeaponDisp(const int nType) const;	// ����\���擾
	bool IsLeftWeaponCollision(void);			// ���̍U������t���O�擾
	bool IsRightWeaponCollision(void);			// �E�̍U������t���O�擾
	D3DXVECTOR3 GetOriginPosition(const int nParts) const;	// ���_�ʒu�̎擾
	D3DXVECTOR3 GetOriginRotation(const int nParts) const;	// ���_�����̎擾

	// �ÓI�����o�֐�
	static CMotion *Create(CObjectChara *pChara);	// ����
	static void Release(CMotion *&prMotion);		// �j��

private:
	// �����o�֐�
	void UpdateMove(void);		// �ړ��X�V
	void UpdateMotion(void);	// ���[�V�����X�V
	void UpdateBlend(void);		// �u�����h�X�V

	// �����o�ϐ�
	CMultiModel **m_ppModel;	// ���f�����
	CObjectChara *m_pChara;		// �I�u�W�F�N�g�L�����N�^�[���
	SInfo  m_info;		// ���[�V�������
	SBlend m_blend; 	// �u�����h���
	int  m_nNumParts;	// �p�[�c��
	bool m_bUpdate;		// �X�V��
};

#endif	// _MOTION_H_
