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

	// �p�[�c���\����
	struct SParts
	{
		std::string strPass;	// ���f���p�X
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 rot;		// ����
		int nParentID;			// �e�C���f�b�N�X
	};

	// �p�[�c�\����
	struct SPartsInfo
	{
		SParts *pInfo;	// �p�[�c���
		int nNumParts;	// �p�[�c��
	};

	// �L�����N�^�[���\����
	struct SCharaData
	{
		// �R���X�g���N�^
		SCharaData() { memset(this, 0, sizeof(*this)); }

		// �����o�ϐ�
		CMotion::SInfo infoMotion;	// ���[�V�������
		SPartsInfo infoParts;		// �p�[�c���
	};

	// �����o�֐�
	HRESULT Init(void);	// ���[�V�����}�l�[�W���[������
	void Uninit(void);	// ���[�V�����}�l�[�W���[�I��
	SCharaData Regist(const char *pTextPass);	// �L�����N�^�[���o�^

	// �ÓI�����o�֐�
	static CMotionManager *Create(void);	// ����
	static void Release(CMotionManager *&prMotionManager);	// �j��

private:
	// �ÓI�����o�֐�
	static HRESULT LoadSetup(SCharaData *pInfoChara, const char *pTextPass);	// �Z�b�g�A�b�v

	// �����o�ϐ�
	std::map<std::string, SCharaData> m_mapMotion;	// ���[�V�����A�z�z��
};

#endif	// _MOTION_MANAGER_H_