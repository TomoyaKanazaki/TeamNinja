#if 1
//============================================================
//
//	�I�u�W�F�N�g�T�[�N��2D���� [objectCircle2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectCircle2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const POSGRID2 MIN_PART = POSGRID2(1, 2);	// �������̍ŏ��l
}

//************************************************************
//	�q�N���X [CObjectCircle2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectCircle2D::CObjectCircle2D(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_pVtxBuff		(nullptr),		// ���_�o�b�t�@
	m_pIdxBuff		(nullptr),		// �C���f�b�N�X�o�b�t�@
	m_pRenderState	(nullptr),		// �����_�[�X�e�[�g�̏��
	m_part			(GRID2_ZERO),	// ������
	m_nNumVtx		(0),			// �K�v���_��
	m_nNumIdx		(0),			// �K�v�C���f�b�N�X��
	m_nTextureID	(0)				// �e�N�X�`���C���f�b�N�X
{
	// �����o�ϐ����N���A
	memset(&m_meshCircle, 0, sizeof(m_meshCircle));	// ���b�V���T�[�N���̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectCircle2D::~CObjectCircle2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectCircle2D::Init(void)
{
	// �����o�ϐ���������
	m_pVtxBuff		= nullptr;	// ���_�o�b�t�@
	m_pIdxBuff		= nullptr;	// �C���f�b�N�X�o�b�t�@
	m_pRenderState	= nullptr;	// �����_�[�X�e�[�g�̏��
	m_part			= MIN_PART;	// ������
	m_nNumVtx		= 0;		// �K�v���_��
	m_nNumIdx		= 0;		// �K�v�C���f�b�N�X��
	m_nTextureID	= NONE_IDX;	// �e�N�X�`���C���f�b�N�X

	m_meshCircle.pos		= VEC3_ZERO;	// �ʒu
	m_meshCircle.rot		= VEC3_ZERO;	// ����
	m_meshCircle.col		= XCOL_WHITE;	// �F
	m_meshCircle.fRadius	= 0.0f;			// ���a

	// ��������ݒ�
	if (FAILED(SetPattern(MIN_PART)))
	{ // �������̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����_�[�X�e�[�g�̐���
	m_pRenderState = CRenderState::Create();
	if (m_pRenderState == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectCircle2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pVtxBuff);

	// �C���f�b�N�X�o�b�t�@�̔j��
	SAFE_RELEASE(m_pIdxBuff);

	// �����_�[�X�e�[�g�̔j��
	SAFE_REF_RELEASE(m_pRenderState);

	// �I�u�W�F�N�g�T�[�N��2D��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectCircle2D::Update(const float fDeltaTime)
{
	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�`�揈��
//============================================================
void CObjectCircle2D::Draw(CShader *pShader)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �����_�[�X�e�[�g��ݒ�
	m_pRenderState->Set();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(object::FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(m_nTextureID));

	// �O���|���S���̕`��
	pDevice->DrawIndexedPrimitive
	( // ����
		D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,
		0,
		m_nNumVtx - 1,						// �g�p���钸�_��
		0,									// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
		(m_nNumIdx - (m_part.x + 1)) - 3	// �v���~�e�B�u (�|���S��) ��
	);

	// ���~�|���S���̕`��
	pDevice->DrawIndexedPrimitive
	( // ����
		D3DPT_TRIANGLEFAN,	// �v���~�e�B�u�̎��
		0,
		0,
		m_part.x + 1,				// �g�p���钸�_��
		m_nNumIdx - (m_part.x + 2),	// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
		m_part.x					// �v���~�e�B�u (�|���S��) ��
	);

	// �����_�[�X�e�[�g���Đݒ�
	m_pRenderState->Reset();
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectCircle2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_meshCircle.pos = rPos;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectCircle2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_meshCircle.rot = rRot;

	// �����̐��K��
	useful::NormalizeRot(m_meshCircle.rot);

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	��������
//============================================================
CObjectCircle2D *CObjectCircle2D::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const float fRadius			// ���a
)
{
	// �I�u�W�F�N�g�T�[�N��2D�̐���
	CObjectCircle2D *pMeshCircle = new CObjectCircle2D;
	if (pMeshCircle == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I�u�W�F�N�g�T�[�N��2D�̏�����
		if (FAILED(pMeshCircle->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I�u�W�F�N�g�T�[�N��2D�̔j��
			SAFE_DELETE(pMeshCircle);
			return nullptr;
		}

		// �ʒu��ݒ�
		pMeshCircle->SetVec3Position(rPos);

		// ������ݒ�
		pMeshCircle->SetVec3Rotation(rRot);

		// �F��ݒ�
		pMeshCircle->SetColor(rCol);

		// ���a��ݒ�
		pMeshCircle->SetRadius(fRadius);

		// ��������ݒ�
		if (FAILED(pMeshCircle->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �I�u�W�F�N�g�T�[�N��2D�̔j��
			SAFE_DELETE(pMeshCircle);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pMeshCircle;
	}
}

//============================================================
//	�����_�[�X�e�[�g���̎擾����
//============================================================
CRenderState *CObjectCircle2D::GetRenderState(void)
{
	// �C���X�^���X���g�p
	assert(m_pRenderState != nullptr);

	// �����_�[�X�e�[�g�̏���Ԃ�
	return m_pRenderState;
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CObjectCircle2D::BindTexture(const int nTextureID)
{
	if (nTextureID >= NONE_IDX)
	{ // �e�N�X�`���C���f�b�N�X���g�p�\�ȏꍇ

		// �e�N�X�`���C���f�b�N�X����
		m_nTextureID = nTextureID;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�e�N�X�`���������� (�p�X)
//============================================================
void CObjectCircle2D::BindTexture(const char *pTexturePass)
{
	// �|�C���^��錾
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	if (pTexturePass != nullptr)
	{ // ���蓖�Ă�e�N�X�`���p�X������ꍇ

		// �e�N�X�`���C���f�b�N�X��ݒ�
		m_nTextureID = pTexture->Regist(pTexturePass);
	}
	else
	{ // ���蓖�Ă�e�N�X�`���p�X���Ȃ��ꍇ

		// �e�N�X�`���Ȃ��C���f�b�N�X��ݒ�
		m_nTextureID = NONE_IDX;
	}
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectCircle2D::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F��ݒ�
	m_meshCircle.col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	���a�̐ݒ菈��
//============================================================
void CObjectCircle2D::SetRadius(const float fRadius)
{
	// �����̔��a��ݒ�
	m_meshCircle.fRadius = fRadius;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�������̐ݒ菈��
//============================================================
HRESULT CObjectCircle2D::SetPattern(const POSGRID2& rPart)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �������̐ݒ�s��
	assert(rPart.x >= MIN_PART.x);
	assert(rPart.y >= MIN_PART.y);

	// �����̕�������ݒ�
	m_part = rPart;

	// �K�v���_�E�C���f�b�N�X�������߂�
	m_nNumVtx = ((m_part.x + 1) * (m_part.y + 1)) - m_part.x; // �K�v���_��
	m_nNumIdx = ((m_part.x + 1) * (((m_part.y + 1) * 2) - 2) + (m_part.y * 2) - 2) - (m_part.x + 2); // �K�v�C���f�b�N�X��

	// ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pVtxBuff);
	assert(m_pVtxBuff == nullptr);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * m_nNumVtx,	// �K�v���_��
		D3DUSAGE_WRITEONLY,		// �g�p���@
		object::FVF_VERTEX_2D,	// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		// �������̎w��
		&m_pVtxBuff,			// ���_�o�b�t�@�ւ̃|�C���^
		nullptr
	)))
	{ // ���_�o�b�t�@�̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	SAFE_RELEASE(m_pIdxBuff);
	assert(m_pIdxBuff == nullptr);

	// �C���f�b�N�X�o�b�t�@�̐���
	if (FAILED(pDevice->CreateIndexBuffer
	( // ����
		sizeof(WORD) * m_nNumIdx,	// �K�v�C���f�b�N�X��
		D3DUSAGE_WRITEONLY,	// �g�p���@
		D3DFMT_INDEX16,		// �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g
		D3DPOOL_MANAGED,	// �������̎w��
		&m_pIdxBuff,		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		nullptr
	)))
	{ // �C���f�b�N�X�o�b�t�@�̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���_�E�C���f�b�N�X���̐ݒ�
	SetVtx();
	SetIdx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectCircle2D::SetVtx(void)
{
	// �|�C���^��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != nullptr)
	{ // �g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // �c�̕����� +1��J��Ԃ�

			// �J��Ԃ��������߂�
			int nLoop = (m_part.x * (1 - (nCntHeight / m_part.y))) + 1;

			for (int nCntWidth = 0; nCntWidth < nLoop; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ� (�V�ӂ̒��_���͌J��Ԃ��� 1��ɂ���)

				// ���_���W�̕�����ݒ�
				float fRotWidth  = D3DXToRadian(nCntWidth * (360 / (float)m_part.x));	// ���_�ʒu�̐ݒ����
				float fDisHeight = (((m_meshCircle.fRadius / (float)m_part.y) * -(nCntHeight - m_part.y)));	// ���_�ʒu�̐ݒ苗��
				D3DXVECTOR3 vecPos = D3DXVECTOR3
				( // ����
					sinf(fRotWidth + m_meshCircle.rot.z) * fDisHeight,	// x
					cosf(fRotWidth + m_meshCircle.rot.z) * -fDisHeight,	// y
					0.0f	// z
				);

				// ���_���W�̐ݒ�
				pVtx[0].pos = m_meshCircle.pos + vecPos;

				// rhw �̐ݒ�
				pVtx[0].rhw = 1.0f;

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_meshCircle.col;

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2
				( // ����
					0.5f + sinf(fRotWidth) * (0.5f / m_meshCircle.fRadius) * fDisHeight,	// u
					0.5f + cosf(fRotWidth) * (0.5f / m_meshCircle.fRadius) * fDisHeight		// v
				);

				// ���_�f�[�^�̃|�C���^�� 1���i�߂�
				pVtx += 1;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	�C���f�b�N�X���̐ݒ菈��
//============================================================
void CObjectCircle2D::SetIdx(void)
{
	// �|�C���^��錾
	WORD *pIdx;	// �C���f�b�N�X���ւ̃|�C���^

	if (m_pIdxBuff != nullptr)
	{ // �g�p���̏ꍇ

		// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
		m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < m_part.y - 1; nCntHeight++)
		{ // �c�̕����� +1��J��Ԃ�

			for (nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ�

				pIdx[0] = (WORD)((m_part.x + 1) * (nCntHeight + 1) + nCntWidth);
				pIdx[1] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth);

				// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
				pIdx += 2;
			}

			if (nCntHeight != m_part.y - 2)
			{ // ��Ԏ�O�̕����ꏊ�ł͂Ȃ��ꍇ

				pIdx[0] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth - 1);
				pIdx[1] = (WORD)((m_part.x + 1) * (nCntHeight + 2));

				// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
				pIdx += 2;
			}
		}

		// �����̓V�ӂ̒��_���w��
		pIdx[0] = (WORD)(m_nNumVtx - 1);

		// �C���f�b�N�X�f�[�^�̃|�C���^�� 1���i�߂�
		pIdx += 1;

		for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
		{ // ���̕����� +1��J��Ԃ�

			pIdx[0] = (WORD)(((m_part.x + 1) * (m_part.y - 1)) + nCntWidth);

			// �C���f�b�N�X�f�[�^�̃|�C���^�� 1���i�߂�
			pIdx += 1;
		}

		// �C���f�b�N�X�o�b�t�@���A�����b�N����
		m_pIdxBuff->Unlock();
	}
}

#endif