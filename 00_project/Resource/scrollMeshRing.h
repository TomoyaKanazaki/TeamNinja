//============================================================
//
//	�X�N���[�����b�V�������O�w�b�_�[ [scrollMeshRing.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCROLL_MESHRING_H_
#define _SCROLL_MESHRING_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectMeshRing.h"

//************************************************************
//	�N���X��`
//************************************************************
// �X�N���[�����b�V�������O�N���X
class CScrollMeshRing : public CObjectMeshRing
{
public:
	// �R���X�g���N�^
	explicit CScrollMeshRing(const CObject::ELabel label = LABEL_NONE, const EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// �f�X�g���N�^
	~CScrollMeshRing() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;			// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	void SetVec2Sizing(const D3DXVECTOR2& rSize) override;	// �傫���ݒ�
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�

	// �ÓI�����o�֐�
	static CScrollMeshRing *Create	// ����
	( // ����
		const float fMoveU,			// �����W�̈ړ���
		const float fMoveV,			// �c���W�̈ړ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR2& rSize,	// �傫��
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart		// ������
	);

	// �����o�֐�
	void SetTexDir(const ETexDir texDir);			// �e�N�X�`�������ݒ�
	void SetHoleRadius(const float fHoleRadius);	// ���̔��a�ݒ�
	void SetThickness(const float fThickness);		// �����ݒ�
	void SetOuterPlusY(const float fOuterPlusY);	// �O����Y���W���Z�ʐݒ�
	HRESULT SetPattern(const POSGRID2& rPart);		// �������ݒ�
	void SetTexPattern(const POSGRID2& rTexPart);	// �e�N�X�`���������ݒ�
	void SetTexU(const float fTexU);	// �����W�̊J�n�ʒu�ݒ�
	void SetTexV(const float fTexV);	// �c���W�̊J�n�ʒu�ݒ�
	void SetMoveU(const float fMoveU);	// �����W�̈ړ��ʐݒ�
	float GetMoveU(void) const;			// �����W�̈ړ��ʎ擾
	void SetMoveV(const float fMoveV);	// �c���W�̈ړ��ʐݒ�
	float GetMoveV(void) const;			// �c���W�̈ړ��ʎ擾

private:
	// �����o�ϐ�
	float m_fTexU;	// �e�N�X�`�������W�̊J�n�ʒu
	float m_fTexV;	// �e�N�X�`���c���W�̊J�n�ʒu
	float m_fMoveU;	// �e�N�X�`�������W�̈ړ���
	float m_fMoveV;	// �e�N�X�`���c���W�̈ړ���
};

#endif	// _SCROLL_MESHRING_H_