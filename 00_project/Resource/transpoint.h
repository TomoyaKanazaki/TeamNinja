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
	static CTransPoint* Collision(const D3DXVECTOR3& rPos, const float fRadius);	// �J�ڃ|�C���g�Ƃ̓����蔻��
	static CListManager<CTransPoint>* GetList(void);	// ���X�g�擾
	static HRESULT LoadSetup(const char* pPass);		// �Z�b�g�A�b�v

	// �����o�֐�
	HRESULT LoadOpen(void);	// ����t���O�Ǎ�
	HRESULT SaveOpen(void);	// ����t���O�ۑ�
	const std::string GetTransMapPass(void) { return m_sTransMapPass; }	// �J�ڐ�}�b�v�p�X�擾

private:
	// �ÓI�����o�ϐ�
	static CListManager<CTransPoint>* m_pList;	// �I�u�W�F�N�g���X�g
	static CBalloonManager* m_pBalloonManager;	// �����o���}�l�[�W���[���

	// �����o�֐�
	HRESULT CreateStageTexture(void);	// �X�e�[�W���e�N�X�`���쐬

	// �����o�ϐ�
	CListManager<CTransPoint>::AIterator m_iterator;	// �C�e���[�^�[
	const std::string m_sTransMapPass;		// �J�ڐ�}�b�v�p�X
	CEffekseer::CEffectData* m_pEffectData;	// �ێ�����G�t�F�N�g���
	CBalloon* m_pBalloon;	// �����o�����
	bool m_bOpen;			// �X�e�[�W����t���O
};

#endif	// _TRANSPOINT_H_
