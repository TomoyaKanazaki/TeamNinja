//============================================================
//
//	�}���`�����w�b�_�[ [multiValue.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MULTIVALUE_H_
#define _MULTIVALUE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "value.h"

//************************************************************
//	�N���X��`
//************************************************************
// �}���`�����N���X
class CMultiValue : public CObject
{
public:
	// �R���X�g���N�^
	CMultiValue();

	// �f�X�g���N�^
	~CMultiValue() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;			// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// �傫���ݒ�
	D3DXVECTOR3 GetVec3Sizing(void) const override;			// �傫���擾
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;				// �F�擾
	void SetPriority(const int nPriority) override;			// �D�揇�ʐݒ�

	// �ÓI�����o�֐�
	static CMultiValue *Create	// ����
	( // ����
		const CValue::EType type,	// �������
		const int nNum,				// ����
		const int nDigit,			// ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize,	// �傫��
		const D3DXVECTOR3& rSpace,	// ���
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE		// �F
	);

	// �����o�֐�
	void AddNum(const int nNum);		// ���l���Z
	void SetNum(const int nNum);		// ���l�ݒ�
	HRESULT SetDigit(const int nDigit);	// �����ݒ�
	void SetMin(const int nMin);		// �ŏ��l�ݒ�
	void SetMax(const int nMax);		// �ő�l�ݒ�
	int GetNum(void) const;				// ���l�擾
	int GetDigit(void) const;			// �����擾
	int GetMin(void) const;				// �ŏ��l�擾
	int GetMax(void) const;				// �ő�l�擾
	void SetSpace(const D3DXVECTOR3& rSpace);	// ��Ԑݒ�
	D3DXVECTOR3 GetSpace(void) const;			// ��Ԏ擾
	void SetType(const CValue::EType type);		// ��ސݒ�

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�֐�
	void SetPositionRelative(void);	// ���Έʒu�ݒ�
	void SetTexNum(void);	// �����̃e�N�X�`�����W�ݒ�

	// �����o�ϐ�
	std::list<CValue*> m_listValue;	// �������X�g
	D3DXVECTOR3 m_pos;		// ���_�ʒu
	D3DXVECTOR3 m_rot;		// ���_����
	D3DXVECTOR3 m_size;		// �傫��
	D3DXVECTOR3 m_space;	// ���
	D3DXCOLOR m_col;		// �F
	int m_nNum;	// ����
	int m_nMin;	// �ŏ��l
	int m_nMax;	// �ő�l
};

#endif	// _MULTIVALUE_H_
