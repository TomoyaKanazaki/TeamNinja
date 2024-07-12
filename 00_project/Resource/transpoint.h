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

	// �ÓI�����o�֐�
	static CTransPoint *Create(const char* pPass, const D3DXVECTOR3& rPos);			// ����
	static CTransPoint* Collision(const D3DXVECTOR3& rPos, const float fRadius);	// �J�ڃ|�C���g�Ƃ̓����蔻��
	static CListManager<CTransPoint>* GetList(void);	// ���X�g�擾
	static HRESULT LoadSetup(const char* pPass);		// �Z�b�g�A�b�v

	// �����o�֐�
	const std::string GetTransMapPass(void) { return m_sTransMapPass; }	// �J�ڐ�}�b�v�p�X�擾

private:
	// �ÓI�����o�ϐ�
	static CListManager<CTransPoint>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CTransPoint>::AIterator m_iterator;	// �C�e���[�^�[
	const std::string m_sTransMapPass;	// �J�ڐ�}�b�v�p�X
};

#endif	// _TRANSPOINT_H_
