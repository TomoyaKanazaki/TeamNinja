//============================================================
//
//	�I�u�W�F�N�g���b�V���t�B�[���h�w�b�_�[ [objectMeshField.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_MESHFIELD_H_
#define _OBJECT_MESHFIELD_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g���b�V���t�B�[���h�N���X
class CObjectMeshField : public CObject
{
public:
	// �R���X�g���N�^
	explicit CObjectMeshField(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectMeshField() override;

	// ���b�V���t�B�[���h�\����
	struct SMeshField
	{
		D3DXVECTOR3	pos;		// �ʒu
		D3DXVECTOR3	rot;		// ����
		D3DXVECTOR2	size;		// �傫��
		D3DXCOLOR	col;		// �F
		D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;			// �`��
	void BindTexture(const int nTextureID) override;		// �e�N�X�`������ (�C���f�b�N�X)
	void BindTexture(const char *pTexturePass) override;	// �e�N�X�`������ (�p�X)
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾
	void SetVec2Sizing(const D3DXVECTOR2& rSize) override;	// �傫���ݒ�
	D3DXVECTOR2 GetVec2Sizing(void) const override;			// �傫���擾
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;				// �F�擾

	// �ÓI�����o�֐�
	static CObjectMeshField *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR2& rSize,	// �傫��
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart		// ������
	);

	// �����o�֐�
	HRESULT SetPattern(const POSGRID2& rPart);	// �������ݒ�
	POSGRID2 GetPattern(void) const;			// �������擾
	CRenderState *GetRenderState(void);			// �����_�[�X�e�[�g���擾

	void SetGapPosition(const int nID, const D3DXVECTOR3& rPos);		// ���W�̂���ݒ�
	D3DXVECTOR3 GetGapPosition(const int nID);							// ���W�̂���擾
	void NormalizeNormal(void);											// �@���̐��K��
	bool LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove);			// ���b�V�����n
	void SetMeshVertexPosition(const int nID, const D3DXVECTOR3& rPos);	// ���b�V���̒��_�ʒu�ݒ�
	void SetTerrain(const POSGRID2& rPart, D3DXVECTOR3 *pPosGap);		// �n�`�ݒ�

	int GetNumVertex(void) const;							// ���_���擾
	D3DXVECTOR3 GetMeshVertexPosition(const int nID);		// ���b�V���̒��_�ʒu�擾
	bool IsPositionRange(const D3DXVECTOR3& rPos);			// ���b�V���͈͓̔��擾
	float GetPositionHeight(const D3DXVECTOR3& rPos);		// ���b�V���̒��n�ʒu�擾 (��]��l��)
	float GetPositionRotateHeight(const D3DXVECTOR3& rPos);	// ���b�V���̒��n�ʒu�擾 (��]�l��)

protected:
	// �����o�֐�
	void SetVtx(bool bNor);	// ���_���̐ݒ�
	void SetIdx(void);		// �C���f�b�N�X���̐ݒ�
	void SetScrollTex		// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	( // ����
		const float fTexU,	// �e�N�X�`���̉����W�̊J�n�ʒu
		const float fTexV	// �e�N�X�`���̏c���W�̊J�n�ʒu
	);

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�֐�
	void DrawNormal(void);	// �ʏ�`��
	void DrawShader(CShader *pShader);	// �V�F�[�_�[�`��
	D3DXVECTOR3 GetNormalLeft(VERTEX_3D *pVtx);			// �@���̎擾 (��)
	D3DXVECTOR3 GetNormalLeftTop(VERTEX_3D *pVtx);		// �@���̎擾 (����)
	D3DXVECTOR3 GetNormalLeftBottom(VERTEX_3D *pVtx);	// �@���̎擾 (����)
	D3DXVECTOR3 GetNormalRight(VERTEX_3D *pVtx);		// �@���̎擾 (�E)
	D3DXVECTOR3 GetNormalRightTop(VERTEX_3D *pVtx);		// �@���̎擾 (�E��)
	D3DXVECTOR3 GetNormalRightBottom(VERTEX_3D *pVtx);	// �@���̎擾 (�E��)

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	CRenderState *m_pRenderState;		// �����_�[�X�e�[�g�̏��
	D3DXVECTOR3 *m_pPosGapBuff;	// ���W�̂���o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 *m_pNorBuff;	// �@���o�b�t�@�ւ̃|�C���^
	int *m_pNumNorBuff;			// �@���̎g�p���o�b�t�@�ւ̃|�C���^

	SMeshField m_meshField;	// ���b�V���t�B�[���h�̏��
	POSGRID2 m_part;		// ������
	int m_nNumVtx;			// �K�v���_��
	int m_nNumIdx;			// �K�v�C���f�b�N�X��
	int m_nTextureID;		// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECT_MESHFIELD_H_