//============================================================
//
//	�I�u�W�F�N�g�������� [objectTornado.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectTornado.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int MAX_OFFSET = 2;	// �I�t�Z�b�g�̐�
}

//************************************************************
//	�q�N���X [CObjectTornado] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectTornado::CObjectTornado() : CObject(LABEL_NONE, DIM_3D, object::DEFAULT_PRIO),
	m_pVtxBuff		(nullptr),	// ���_�o�b�t�@
	m_pRenderState	(nullptr),	// �����_�[�X�e�[�g�̏��
	m_nNumVtx		(0),		// �K�v���_��
	m_nNumAround	(0),		// �Q�̎���
	m_nPattern		(0),		// �Q�̕�����
	m_nTextureID	(0)			// �e�N�X�`���C���f�b�N�X
{
	// �����o�ϐ����N���A
	memset(&m_tornado, 0, sizeof(m_tornado));	// �����̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectTornado::~CObjectTornado()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectTornado::Init(void)
{
	// �����o�ϐ���������
	m_pVtxBuff		= nullptr;	// ���_�o�b�t�@
	m_pRenderState	= nullptr;	// �����_�[�X�e�[�g�̏��
	m_nNumVtx		= 0;		// �K�v���_��
	m_nNumAround	= 0;		// �Q�̎���
	m_nPattern		= 0;		// �Q�̕�����
	m_nTextureID	= NONE_IDX;	// �e�N�X�`���C���f�b�N�X

	// �����̏���������
	m_tornado.pos			= VEC3_ZERO;	// �ʒu
	m_tornado.rot			= VEC3_ZERO;	// ����
	m_tornado.direRot		= VEC3_ZERO;	// ��������
	m_tornado.col			= XCOL_WHITE;	// �F
	m_tornado.pMtxParent	= nullptr;		// �e�̃}�g���b�N�X
	m_tornado.fMoveRot		= 0.0f;			// �����̕ύX��
	m_tornado.fThickness	= 0.0f;			// �|���S���̑���
	m_tornado.fOuterPlusY	= 0.0f;			// �|���S���O����Y���W���Z��
	m_tornado.fSetWidth		= 0.0f;			// �������̉������
	m_tornado.fSetAlpha		= 0.0f;			// �������̓����x
	m_tornado.fAddWidth		= 0.0f;			// ������̉��Z��
	m_tornado.fAddHeight	= 0.0f;			// �c����̉��Z��
	m_tornado.fSubAlpha		= 0.0f;			// �����x�̌��Z��
	m_tornado.fGrowWidth	= 0.0f;			// ������̐�����
	m_tornado.fGrowHeight	= 0.0f;			// �c����̐�����
	m_tornado.fGrowAlpha	= 0.0f;			// �����x�̐�����

	// �Q��ݒ�
	if (FAILED(SetVortex(1, 1)))
	{ // �Q�̐ݒ�Ɏ��s�����ꍇ

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

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaTest(true);			// ���e�X�g�̗L�� / �����̐ݒ�
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// ���e�X�g�̐ݒ�

	// �|���S���̗��ʂ�\����Ԃɂ���
	pRenderState->SetCulling(D3DCULL_NONE);

	// ���C�e�B���O��OFF�ɂ���
	pRenderState->SetLighting(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectTornado::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pVtxBuff);

	// �����_�[�X�e�[�g�̔j��
	SAFE_REF_RELEASE(m_pRenderState);

	// �I�u�W�F�N�g������j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectTornado::Update(void)
{
	// �����̌�����ύX
	m_tornado.rot.y -= m_tornado.fMoveRot;

	// �����𐳋K��
	useful::NormalizeRot(m_tornado.rot.y);

	// �����𐬒�������
	m_tornado.fSetWidth  += m_tornado.fGrowWidth * m_nNumVtx;	// ���_����̉�����ʂ����Z
	m_tornado.fAddWidth  += m_tornado.fGrowWidth;	// �����̉��̍L��������Z
	m_tornado.fAddHeight += m_tornado.fGrowHeight;	// �����̏c�̍L��������Z
	m_tornado.fSetAlpha  -= m_tornado.fGrowAlpha;	// �����̓����x�����Z (�����ɂ��Ă���)

	// ���_���̐ݒ�
	SetVtx();

	if (m_tornado.fSetAlpha <= 0.0f)
	{ // �����������ɂȂ����ꍇ

		// �����I�u�W�F�N�g�̏I��
		Uninit();

		// �����𔲂���
		return;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CObjectTornado::Draw(CShader *pShader)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxOrigin;			// �������̃}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �����_�[�X�e�[�g��ݒ�
	m_pRenderState->Set();

	//--------------------------------------------------------
	//	�������̃}�g���b�N�X�����߂�
	//--------------------------------------------------------
	// �������̃}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxOrigin);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_tornado.direRot.y, m_tornado.direRot.x, m_tornado.direRot.z);
	D3DXMatrixMultiply(&mtxOrigin, &mtxOrigin, &mtxRot);	// ��������

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_tornado.pos.x, m_tornado.pos.y, m_tornado.pos.z);
	D3DXMatrixMultiply(&mtxOrigin, &mtxOrigin, &mtxTrans);	// �����ʒu

	//--------------------------------------------------------
	//	���[���h�}�g���b�N�X�����߂�
	//--------------------------------------------------------
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_tornado.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_tornado.rot.y, m_tornado.rot.x, m_tornado.rot.z);
	D3DXMatrixMultiply(&m_tornado.mtxWorld, &m_tornado.mtxWorld, &mtxRot);	// ��]��

	//--------------------------------------------------------
	//	�}�g���b�N�X���|�����킹��
	//--------------------------------------------------------
	if (m_tornado.pMtxParent != nullptr)
	{ // �e�̃}�g���b�N�X�����݂���ꍇ

		// �e�̃}�g���b�N�X�Ɗ|�����킹��
		D3DXMatrixMultiply(&mtxOrigin, &mtxOrigin, m_tornado.pMtxParent);
	}

	// �������̃}�g���b�N�X�Ɗ|�����킹��
	D3DXMatrixMultiply(&m_tornado.mtxWorld, &m_tornado.mtxWorld, &mtxOrigin);

	//--------------------------------------------------------
	//	������`��
	//--------------------------------------------------------
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_tornado.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(object::FVF_VERTEX_3D);

	if (pShader == nullptr)
	{ // �V�F�[�_�[���g�p����Ă��Ȃ��ꍇ

		// �ʏ�`��
		DrawNormal();
	}
	else
	{ // �V�F�[�_�[���g�p����Ă���ꍇ

		// �V�F�[�_�[�`��
		DrawShader(pShader);
	}

	// �����_�[�X�e�[�g���Đݒ�
	m_pRenderState->Reset();
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectTornado::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_tornado.pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectTornado::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_tornado.pos;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectTornado::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F��ݒ�
	m_tornado.col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObjectTornado::GetColor(void) const
{
	// �F��Ԃ�
	return m_tornado.col;
}

//============================================================
//	��������
//============================================================
CObjectTornado *CObjectTornado::Create
(
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rDireRot,	// ��������
	const D3DXCOLOR& rCol,			// �F
	D3DXMATRIX *pMtxParent,			// �e�̃}�g���b�N�X
	const int   nNumAround,			// �Q�̎���
	const int   nPattern,			// �Q�̕�����
	const float fMoveRot,			// �����̕ύX��
	const float fThickness,			// �|���S���̑���
	const float fOuterPlusY,		// �|���S���O����Y���W���Z��
	const float fSetWidth,			// �������̉������
	const float fSetAlpha,			// �������̓����x
	const float fAddWidth,			// ������̉��Z��
	const float fAddHeight,			// �c����̉��Z��
	const float fSubAlpha,			// �����x�̌��Z��
	const float fGrowWidth,			// ������̐�����
	const float fGrowHeight,		// �c����̐�����
	const float fGrowAlpha			// �����x�̐�����
)
{
	// �I�u�W�F�N�g�����̐���
	CObjectTornado *pTornado = new CObjectTornado;
	if (pTornado == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I�u�W�F�N�g�����̏�����
		if (FAILED(pTornado->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I�u�W�F�N�g�����̔j��
			SAFE_DELETE(pTornado);
			return nullptr;
		}

		// �ʒu��ݒ�
		pTornado->SetVec3Position(rPos);

		// ������ݒ�
		pTornado->SetDirectionRotation(rDireRot);

		// �F��ݒ�
		pTornado->SetColor(rCol);

		// �e�̃}�g���b�N�X��ݒ�
		pTornado->SetMatrixParent(pMtxParent);

		// �|���S���̑�����ݒ�
		pTornado->SetThickness(fThickness);

		// �|���S���O����Y���W���Z�ʂ�ݒ�
		pTornado->SetOuterPlusY(fOuterPlusY);

		// �������̉�����ʂ�ݒ�
		pTornado->SetWidth(fSetWidth);

		// �������̓����x��ݒ�
		pTornado->SetAlpha(fSetAlpha);

		// ������̉��Z�ʂ�ݒ�
		pTornado->SetAddWidth(fAddWidth);

		// �c����̉��Z�ʂ�ݒ�
		pTornado->SetAddHeight(fAddHeight);

		// �����x�̌��Z�ʂ�ݒ�
		pTornado->SetSubAlpha(fSubAlpha);

		// ��������ݒ�
		pTornado->SetGrow(fMoveRot, fGrowWidth, fGrowHeight, fGrowAlpha);

		// �Q��ݒ�
		if (FAILED(pTornado->SetVortex(nNumAround, nPattern)))
		{ // �Q�̐ݒ�Ɏ��s�����ꍇ

			// �I�u�W�F�N�g�����̔j��
			SAFE_DELETE(pTornado);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pTornado;
	}
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectTornado::SetGrow(float fMoveRot, float fGrowWidth, float fGrowHeight, float fGrowAlpha)
{
	// �����̏���ݒ�
	m_tornado.fMoveRot		= fMoveRot;		// �����̕ύX��
	m_tornado.fGrowWidth	= fGrowWidth;	// ������̐�����
	m_tornado.fGrowHeight	= fGrowHeight;	// �c����̐�����
	m_tornado.fGrowAlpha	= fGrowAlpha;	// �����x�̐�����
}

//============================================================
//	�Q�̐ݒ菈��
//============================================================
HRESULT CObjectTornado::SetVortex(const int nNumAround, const int nPattern)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �����̉Q��ݒ�
	m_nNumAround = nNumAround;
	m_nPattern = nPattern;

	// �K�v���_�������߂�
	m_nNumVtx = m_nNumAround * m_nPattern * MAX_OFFSET;

	// ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pVtxBuff);
	assert(m_pVtxBuff == nullptr);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * m_nNumVtx,	// �K�v���_��
		D3DUSAGE_WRITEONLY,		// �g�p���@
		object::FVF_VERTEX_3D,	// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		// �������̎w��
		&m_pVtxBuff,			// ���_�o�b�t�@�ւ̃|�C���^
		nullptr
	)))
	{ // ���_�o�b�t�@�̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���_���̐ݒ�
	SetVtx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�e�}�g���b�N�X�폜����
//============================================================
void CObjectTornado::DeleteMatrixParent(void)
{
	// �e�}�g���b�N�X��nullptr�ɂ���
	m_tornado.pMtxParent = nullptr;
}

//============================================================
//	�e�̃}�g���b�N�X�̐ݒ菈��
//============================================================
void CObjectTornado::SetMatrixParent(D3DXMATRIX *pMtxParent)
{
	// �����̐e�}�g���b�N�X��ݒ�
	m_tornado.pMtxParent = pMtxParent;
}

//============================================================
//	���������̐ݒ菈��
//============================================================
void CObjectTornado::SetDirectionRotation(const D3DXVECTOR3& rRot)
{
	// �����̐���������ݒ�
	m_tornado.direRot = rRot;

	// ���������̐��K��
	useful::NormalizeRot(m_tornado.direRot);
}

//============================================================
//	���������擾����
//============================================================
D3DXVECTOR3 CObjectTornado::GetDirectionRotation(void) const
{
	// ����������Ԃ�
	return m_tornado.direRot;
}

//============================================================
//	�|���S���̑����̐ݒ菈��
//============================================================
void CObjectTornado::SetThickness(const float fThickness)
{
	// �����̃|���S���̑�����ݒ�
	m_tornado.fThickness = fThickness;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�|���S���̑����擾����
//============================================================
float CObjectTornado::GetThickness(void) const
{
	// �|���S���̑�����Ԃ�
	return m_tornado.fThickness;
}

//============================================================
//	�|���S���O����Y���W���Z�ʂ̐ݒ菈��
//============================================================
void CObjectTornado::SetOuterPlusY(const float fOuterPlusY)
{
	// �����̃|���S���O����Y���W���Z�ʂ�ݒ�
	m_tornado.fOuterPlusY = fOuterPlusY;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�|���S���O����Y���W���Z�ʂ̎擾����
//============================================================
float CObjectTornado::GetOuterPlusY(void) const
{
	// �|���S���O����Y���W���Z�ʂ�Ԃ�
	return m_tornado.fOuterPlusY;
}

//============================================================
//	�������̉�����ʂ̐ݒ菈��
//============================================================
void CObjectTornado::SetWidth(const float fSetWidth)
{
	// �����̐������̉�����ʂ�ݒ�
	m_tornado.fSetWidth = fSetWidth;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�������̉�����ʎ擾����
//============================================================
float CObjectTornado::GetWidth(void) const
{
	// �������̉�����ʂ�Ԃ�
	return m_tornado.fSetWidth;
}

//============================================================
//	�������̓����x�̐ݒ菈��
//============================================================
void CObjectTornado::SetAlpha(const float fSetAlpha)
{
	// �����̐������̓����x��ݒ�
	m_tornado.fSetAlpha = fSetAlpha;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�������̓����x�擾����
//============================================================
float CObjectTornado::GetAlpha(void) const
{
	// �������̓����x��Ԃ�
	return m_tornado.fSetAlpha;
}

//============================================================
//	������̉��Z�ʂ̐ݒ菈��
//============================================================
void CObjectTornado::SetAddWidth(const float fAddWidth)
{
	// �����̉�����̉��Z�ʂ�ݒ�
	m_tornado.fAddWidth = fAddWidth;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	������̉��Z�ʎ擾����
//============================================================
float CObjectTornado::GetAddWidth(void) const
{
	// ������̉��Z�ʂ�Ԃ�
	return m_tornado.fAddWidth;
}

//============================================================
//	�c����̉��Z�ʂ̐ݒ菈��
//============================================================
void CObjectTornado::SetAddHeight(const float fAddHeight)
{
	// �����̏c����̉��Z�ʂ�ݒ�
	m_tornado.fAddHeight = fAddHeight;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�c����̉��Z�ʎ擾����
//============================================================
float CObjectTornado::GetAddHeight(void) const
{
	// �c����̉��Z�ʂ�Ԃ�
	return m_tornado.fAddHeight;
}

//============================================================
//	�����x�̌��Z�ʂ̐ݒ菈��
//============================================================
void CObjectTornado::SetSubAlpha(const float fSubAlpha)
{
	// �����̓����x�̌��Z�ʂ�ݒ�
	m_tornado.fSubAlpha = fSubAlpha;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�����x�̌��Z�ʎ擾����
//============================================================
float CObjectTornado::GetSubAlpha(void) const
{
	// �����x�̌��Z�ʂ�Ԃ�
	return m_tornado.fSubAlpha;
}

//============================================================
//	�����_�[�X�e�[�g���̎擾����
//============================================================
CRenderState *CObjectTornado::GetRenderState(void)
{
	// �C���X�^���X���g�p
	assert(m_pRenderState != nullptr);

	// �����_�[�X�e�[�g�̏���Ԃ�
	return m_pRenderState;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectTornado::SetVtx(void)
{
	// �ϐ���錾
	D3DXVECTOR3 vecPos;	// �����̒��_�����x�N�g��
	float fWidth  = m_tornado.fSetWidth;	// ���_�����̉������
	float fHeight = 0.0f;					// ���_�����̏c�����
	float fAlpha  = m_tornado.fSetAlpha;	// ���_�J���[�̓����x

	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != nullptr)
	{ // �����̒��_�o�b�t�@���g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntWidth = 0; nCntWidth < m_nNumAround; nCntWidth++)
		{ // �Q�̎���̑������J��Ԃ�

			for (int nCntHeight = 0; nCntHeight < m_nPattern; nCntHeight++)
			{ // �Q�̕��������J��Ԃ�

				// ���_�����x�N�g�������߂�
				vecPos.x = sinf(nCntHeight * ((D3DX_PI * 2.0f) / m_nPattern));
				vecPos.y = 1.0f;
				vecPos.z = cosf(nCntHeight * ((D3DX_PI * 2.0f) / m_nPattern));

				for (int nCntSet = 0; nCntSet < MAX_OFFSET; nCntSet++)
				{ // �I�t�Z�b�g�̑������J��Ԃ�

					// ���_���W�̐ݒ�
					pVtx[0].pos.x = 0.0f + vecPos.x * (fWidth + (nCntSet * m_tornado.fThickness));		// x
					pVtx[0].pos.y = 0.0f + vecPos.y * (fHeight + (nCntSet * m_tornado.fOuterPlusY));	// y
					pVtx[0].pos.z = 0.0f + vecPos.z * (fWidth + (nCntSet * m_tornado.fThickness));		// z

					// �@���x�N�g���̐ݒ�
					pVtx[0].nor = VEC3_ZERO;

					if (fAlpha < 0.0f)
					{ // �����x���͈͊O�̏ꍇ

						// �����x��␳
						fAlpha = 0.0f;
					}

					// ���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(m_tornado.col.r, m_tornado.col.g, m_tornado.col.b, fAlpha);

					// �e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2
					( // ����
						1.0f,			// u
						1.0f * nCntSet	// v
					);

					// ���_�f�[�^�̃|�C���^��i�߂�
					pVtx += 1;
				}

				// �c�����L����
				fWidth  += m_tornado.fAddWidth;
				fHeight += m_tornado.fAddHeight;

				// ���l�����Z
				fAlpha -= m_tornado.fSubAlpha;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�j������
//============================================================
void CObjectTornado::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	�ʏ�`�揈��
//============================================================
void CObjectTornado::DrawNormal(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);
}

//============================================================
//	�V�F�[�_�[�`�揈��
//============================================================
void CObjectTornado::DrawShader(CShader *pShader)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �`��J�n
	pShader->Begin();
	pShader->BeginPass(0);

	// �}�g���b�N�X����ݒ�
	pShader->SetMatrix(&m_tornado.mtxWorld);

	// ���C�g������ݒ�
	pShader->SetLightDirect(&m_tornado.mtxWorld, 0);

	// �g�U����ݒ�
	pShader->SetOnlyDiffuse(m_tornado.col);

	// �e�N�X�`����ݒ�
	pShader->SetTexture(m_nTextureID);

	// ��ԕύX�̓`�B
	pShader->CommitChanges();

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);

	// �`��I��
	pShader->EndPass();
	pShader->End();
}