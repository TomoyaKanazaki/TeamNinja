//============================================================
//
//	�X�e�[�W�w�b�_�[ [stage.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _STAGE_H_
#define _STAGE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CField;	// �n�ʃN���X

//************************************************************
//	�N���X��`
//************************************************************
// �X�e�[�W�N���X
class CStage
{
public:
	// ������
	enum ELimit
	{
		LIMIT_NONE = 0,	// �͈͐����Ȃ�
		LIMIT_BOX,		// ��`�͈�
		LIMIT_CIRCLE,	// �~�͈�
		LIMIT_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CStage();

	// �f�X�g���N�^
	~CStage();

	// �Ǎ��p�X�\����
	struct SPass
	{
		std::string sStage;	// �X�e�[�W���p�X
		std::string sActor;	// �A�N�^�[���p�X
		std::string sPoint;	// �|�C���g���p�X
	};

	// �͈͍\����
	struct SLimit
	{
		ELimit mode;		// �������[�h
		D3DXVECTOR3 center;	// ���S���W
		float fNear;		// �����ʒu (�O)
		float fFar;			// �����ʒu (��)
		float fRight;		// �����ʒu (�E)
		float fLeft;		// �����ʒu (��)
		float fRadius;		// �����ʒu (���a)
		float fField;		// �����ʒu (�n��)
		float fKillY;		// �����ʒu (�폜)
	};

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(const float fDeltaTime);	// �X�V
	void SetLimit(const SLimit& rLimit);	// �͈͐ݒ�
	SLimit GetLimit(void) const;			// �͈͎擾

	bool LandLimitPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight);	// �͈͊O�̒��n����
	bool LandFieldPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rOldPos, D3DXVECTOR3& rMove, CField** ppCollField = nullptr);	// �n�ʒ��n
	bool LandFieldPositionTop(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, CField** ppCollField = nullptr);						// ��ԏ�̒n�ʒ��n
	bool CollisionWall(D3DXVECTOR3& rPos, D3DXVECTOR3& rPosOld, const float fRadius, const float fHeight, D3DXVECTOR3& rMove, bool* pJump = nullptr);	// �ǂƂ̓����蔻��
	void LimitPosition(D3DXVECTOR3& rPos, const float fRadius);	// �ʒu�␳
	bool CollisionKillY(const D3DXVECTOR3& rPos);				// �L��Y���W�Ƃ̓����蔻��
	bool IsFieldPositionRange(const D3DXVECTOR3& rPos);			// ���b�V���͈͓̔��擾
	float GetFieldPositionHeight(const D3DXVECTOR3& rPos);		// ���b�V���̒��n�ʒu�擾

	// �ÓI�����o�֐�
	static CStage *Create(const SPass& rPass);	// ����
	static void Release(CStage *&prStage);		// �j��

private:
	// �ÓI�����o�֐�
	static HRESULT LoadSetup(const char* pPass, CStage *pStage);	// �Z�b�g�A�b�v
	static HRESULT LoadLimit(const char* pString, FILE *pFile, CStage *pStage);		// �͈͏��̓Ǎ�
	static HRESULT LoadField(const char* pString, FILE *pFile, CStage *pStage);		// �n�ʏ��̓Ǎ�
	static HRESULT LoadWall(const char* pString, FILE *pFile, CStage *pStage);		// �Ǐ��̓Ǎ�
	static HRESULT LoadScenery(const char* pString, FILE *pFile, CStage *pStage);	// �i�F���̓Ǎ�
	static HRESULT LoadSky(const char* pString, FILE *pFile, CStage *pStage);		// ����̓Ǎ�
	static HRESULT LoadLiquid(const char* pString, FILE* pFile, CStage* pStage);	// �t�̏��̓Ǎ�
	static HRESULT LoadChanger(const char* pString, FILE* pFile, CStage* pStage);	// �J�����ύX�n�_���̓Ǎ�

	// �����o�ϐ�
	SLimit m_limit;	// �͈͏��
};

#endif	// _STAGE_H_
