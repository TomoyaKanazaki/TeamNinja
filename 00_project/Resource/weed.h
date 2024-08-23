//============================================================
//
//	���w�b�_�[ [weed.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _WEED_H_
#define _WEED_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���N���X
class CWeed : public CObject3D
{
public:
	// �R���X�g���N�^
	CWeed();

	// �f�X�g���N�^
	~CWeed() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CWeed *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);
	static CListManager<CWeed>* GetList(void);	// ���X�g�擾

	// �����o�֐�
	bool CollisionPlayer(void);					// �v���C���[�Ƃ̓����蔻��
	void UpdateSwing(const float fDeltaTime);	// ���łȂт��X�V

private:

	// �ÓI�����o�ϐ�
	static CListManager<CWeed>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CWeed>::AIterator m_iterator;	// �C�e���[�^�[
	D3DXVECTOR3 m_offset;	// �㒸�_�I�t�Z�b�g
	bool m_bChange;			// ���R�h��Ɠ��݂��̑J�ڃt���O
	float m_fCurLength;		// ���݂̋���
	float m_fDestLength;	// �ڕW�̋���
	float m_fCurAngle;		// ���݂̊p�x
	float m_fDestAngle;		// �ڕW�̊p�x
	float m_fGapRate;		// �h��͈̔�
};

#endif	// _WEED_H_
