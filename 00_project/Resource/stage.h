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
		std::string sMap;		// �}�b�v���p�X
		std::string sStage;		// �X�e�[�W���p�X
		std::string sEnemyAtc;	// �U���G���p�X
		std::string sCoin;		// �R�C�����p�X
		std::string sGodItem;	// �_����p�X
		std::string sActor;		// �A�N�^�[���p�X
		std::string sGimmick;	// �M�~�b�N���p�X
		std::string sPoint;		// �|�C���g���p�X
		std::string sTrans;		// �J�ڃ|�C���g���p�X
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
	SPass Regist(const char* pMapPass);		// �}�b�v�p�X�o�^
	HRESULT BindStage(const char* pPass);	// �X�e�[�W���� (�p�X)
	HRESULT BindStage(const SPass& rPass);	// �X�e�[�W���� (���)
	std::string GetCurMapStagePass(void) const;			// ���݃}�b�v�̃X�e�[�W�p�X�擾
	std::string GetCurMapGodItemPass(void) const;		// ���݃}�b�v�̐_��p�X�擾
	std::string GetCurMapSaveGodItemPass(void) const;	// ���݃}�b�v�̐_��ۑ��p�X�擾
	void SetInitMapPass(const std::string& rMapPass)	{ m_sInitMapPass = rMapPass; }	// �������}�b�v�p�X�ݒ�
	std::string GetInitMapPass(void) const				{ return m_sInitMapPass; }		// �������}�b�v�p�X�擾
	std::string GetCurMapDirectory(void) const			{ return m_sCurMapFolder; }		// ���݃}�b�v�̃f�B���N�g���擾
	std::string GetOpenMapDirectory(void) const			{ return m_sOpenMapFolder; }	// �N���A������}�b�v�̃f�B���N�g���擾
	void SetLimit(const SLimit& rLimit)	{ m_limit = rLimit; }	// �͈͐ݒ�
	SLimit GetLimit(void) const			{ return m_limit; }		// �͈͎擾

	bool LandLimitPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight);	// �͈͊O�̒��n����
	bool LandFieldPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rOldPos, D3DXVECTOR3& rMove, CField** ppCollField = nullptr);	// �n�ʒ��n
	bool LandFieldPositionTop(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, CField** ppCollField = nullptr);						// ��ԏ�̒n�ʒ��n
	bool CollisionWall(D3DXVECTOR3& rPos, D3DXVECTOR3& rPosOld, const float fRadius, const float fHeight, D3DXVECTOR3& rMove, bool* pJump = nullptr);	// �ǂƂ̓����蔻��
	void LimitPosition(D3DXVECTOR3& rPos, const float fRadius);	// �ʒu�␳
	bool CollisionKillY(const D3DXVECTOR3& rPos);				// �L��Y���W�Ƃ̓����蔻��
	bool IsFieldPositionRange(const D3DXVECTOR3& rPos);			// ���b�V���͈͓̔��擾
	float GetFieldPositionHeight(const D3DXVECTOR3& rPos);		// ���b�V���̒��n�ʒu�擾
	float GetFieldDownPositionHeight(const D3DXVECTOR3& rPos);	// ���b�V���̒��n�ʒu�擾 (���݈ʒu���͉�)

	// �ÓI�����o�֐�
	static CStage *Create(void);			// ����
	static void Release(CStage *&prStage);	// �j��
	static HRESULT LoadPass(const char* pMapPass, SPass* pPassInfo);	// �p�X���Ǎ�

private:
	// �����o�֐�
	HRESULT LoadSetup(const char* pPass);					// �Z�b�g�A�b�v
	HRESULT LoadOpen(const char* pString, FILE *pFile);		// ����}�b�v�f�B���N�g���̓Ǎ�
	HRESULT LoadLimit(const char* pString, FILE *pFile);	// �͈͏��̓Ǎ�
	HRESULT LoadField(const char* pString, FILE *pFile);	// �n�ʏ��̓Ǎ�
	HRESULT LoadWall(const char* pString, FILE *pFile);		// �Ǐ��̓Ǎ�
	HRESULT LoadScenery(const char* pString, FILE *pFile);	// �i�F���̓Ǎ�
	HRESULT LoadSky(const char* pString, FILE *pFile);		// ����̓Ǎ�
	HRESULT LoadLiquid(const char* pString, FILE* pFile);	// �t�̏��̓Ǎ�
	HRESULT LoadChanger(const char* pString, FILE* pFile);	// �J�����ύX�n�_���̓Ǎ�
	HRESULT LoadWeed(const char* pString, FILE* pFile);		// ���̓Ǎ�

	// �����o�ϐ�
	std::map<std::string, SPass> m_mapPass;	// �}�b�v�p�X�A�z�z��
	std::string m_sInitMapPass;		// ���������ɐ�������}�b�v�p�X
	std::string m_sCurMapFolder;	// ���݃}�b�v�̃t�H���_�p�X
	std::string m_sOpenMapFolder;	// �N���A������}�b�v�̃t�H���_�p�X
	SLimit m_limit;	// �͈͏��
};

#endif	// _STAGE_H_
