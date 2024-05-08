//============================================================
//
//	���[�V�����}�l�[�W���[�w�b�_�[ [motionManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MOTION_MANAGER_H_
#define _MOTION_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "motion.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���[�V�����}�l�[�W���[�N���X
class CMotionManager
{
public:
	// �R���X�g���N�^
	CMotionManager();

	// �f�X�g���N�^
	~CMotionManager();

	// �L�[�\����
	struct SKey
	{
		// �R���X�g���N�^
		SKey(const char *pTextPass, const char **ppModelPassArray) :
		pTextPass(pTextPass), ppModelPassArray(ppModelPassArray) {}

		// ��r���Z�q�I�[�o�[���[�h
		bool operator<(const SKey& rKey) const
		{
			// �����܂Ƃ߂Ĕ�r
			return std::tie(pTextPass, ppModelPassArray) < std::tie(rKey.pTextPass, rKey.ppModelPassArray);
		}

		// �����o�ϐ�
		const char *pTextPass;			// �e�L�X�g�p�X
		const char **ppModelPassArray;	// ���f���z��|�C���^
	};

	// �p�[�c���\����
	struct SParts
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
		int nParentID;		// �e�C���f�b�N�X
	};

	// �p�[�c�\����
	struct SPartsInfo
	{
		SParts aInfo[motion::MAX_PARTS];	// �p�[�c���
		int nNumParts;	// �p�[�c��
	};

	// �L�����N�^�[���\����
	struct SCharaData
	{
		CMotion::SInfo infoMotion;	// ���[�V�������
		SPartsInfo infoParts;		// �p�[�c���
	};

	// �����o�֐�
	HRESULT Init(void);	// ���[�V�����}�l�[�W���[������
	void Uninit(void);	// ���[�V�����}�l�[�W���[�I��
	SCharaData Regist	// �L�����N�^�[���o�^
	( // ����
		const char *pTextPass,			// �e�L�X�g�p�X
		const char **ppModelPassArray	// ���f���p�X�z��
	);

	// �ÓI�����o�֐�
	static CMotionManager *Create(void);	// ����
	static void Release(CMotionManager *&prMotionManager);	// �j��

private:
	// �ÓI�����o�֐�
	static void LoadSetup(SCharaData *pInfoChara, const char *pTextPass, const char **ppModelPassArray);	// �Z�b�g�A�b�v

	// �����o�ϐ�
	std::map<SKey, SCharaData> m_mapMotion;	// ���[�V�����A�z�z��
};

#endif	// _MOTION_MANAGER_H_
