//============================================================
//
//	�J�ڃ|�C���g�w�b�_�[ [transpoint.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TRANSPOINT_H_
#define _TRANSPOINT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectModel.h"
#include "effekseerControl.h"
#include "effekseerManager.h"

//************************************************************
//	�O���錾
//************************************************************
class CBalloon;			// �����o���N���X
class CBalloonManager;	// �����o���}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �J�ڃ|�C���g�N���X
class CTransPoint : public CObjectModel
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_CAMERA,	// �J�������
		STATE_FADE,		// �t�F�[�h���
		STATE_NORMAL,	// �ʏ���
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CTransPoint(const char* pPass);

	// �f�X�g���N�^
	~CTransPoint() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�

	// �ÓI�����o�֐�
	static CTransPoint *Create(const char* pPass, const D3DXVECTOR3& rPos);			// ����
	static CTransPoint *Collision(const D3DXVECTOR3& rPos, const float fRadius);	// �J�ڃ|�C���g�Ƃ̓����蔻��
	static CTransPoint* GetOpenTransPoint(void) { return m_pOpenTransPoint; }		// ������ꂽ�J�ڃ|�C���g�擾

	static CListManager<CTransPoint>* GetList(void);	// ���X�g�擾
	static HRESULT LoadSetup(const char* pPass);		// �Z�b�g�A�b�v
	static HRESULT LoadOpen(const char* pPass, bool* pOpen);		// ����t���O�Ǎ�
	static HRESULT SaveOpen(const char* pPass, const bool bOpen);	// ����t���O�ۑ�
	static HRESULT LoadScreenShot(const char* pPass, std::string* pStrPath);	// �X�N�V���p�X�Ǎ�
	static HRESULT SaveScreenShot(const char* pPass);							// �X�N�V���p�X�ۑ�

	// �����o�֐�
	const std::string GetTransMapPass(void) { return m_sTransMapPass; }	// �J�ڐ�}�b�v�p�X�擾

private:
	// �ÓI�����o�ϐ�
	static CListManager<CTransPoint>* m_pList;	// �I�u�W�F�N�g���X�g
	static CBalloonManager* m_pBalloonManager;	// �����o���}�l�[�W���[���
	static CTransPoint* m_pOpenTransPoint;		// ������ꂽ�J�ڃ|�C���g

	// �����o�֐�
	HRESULT CreateStageTexture(void);	// �X�e�[�W���e�N�X�`���쐬

	// �����o�ϐ�
	CListManager<CTransPoint>::AIterator m_iterator;	// �C�e���[�^�[
	const std::string m_sTransMapPass;		// �J�ڐ�}�b�v�p�X
	std::string m_sScreenShotPass;			// �X�N�V���p�X
	CEffekseer::CEffectData* m_pEffectData;	// �ێ�����G�t�F�N�g���
	CBalloon* m_pBalloon;	// �����o�����
	EState m_state;			// ���
	bool m_bOpen;			// �X�e�[�W����t���O
	float m_fCurTime;		// ���݂̎���
};

#endif	// _TRANSPOINT_H_
