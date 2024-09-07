//============================================================
//
//	�u���[2D���� [blur2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "blur2D.h"
#include "anim2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 5;	// �u���[2D�̗D�揇��
}

//************************************************************
//	�e�N���X [CBlur2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBlur2D::CBlur2D() : CObject(CObject::LABEL_BLUR, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY),
	m_pParent		(nullptr),		// �e�I�u�W�F�N�g
	m_col			(XCOL_WHITE),	// �u���[���f�F
	m_fStartAlpha	(0.0f),			// �u���[�̊J�n�����x
	m_nMaxLength	(0),			// �ێ�����e�I�u�W�F�N�g�̍ő吔
	m_state			(STATE_NONE),	// ���
	m_nCounterState	(0)				// ��ԊǗ��J�E���^�[
{
	// �����o�ϐ����N���A
	m_oldObject.clear();	// �ߋ��I�u�W�F�N�g���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CBlur2D::~CBlur2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBlur2D::Init(void)
{
	// �����o�ϐ���������
	m_oldObject.clear();			// �ߋ��I�u�W�F�N�g���
	m_pParent		= nullptr;		// �e�I�u�W�F�N�g
	m_col			= XCOL_WHITE;	// �u���[���f�F
	m_fStartAlpha	= 0.0f;			// �u���[�̊J�n�����x
	m_nMaxLength	= 0;			// �ێ�����e�I�u�W�F�N�g�̍ő吔
	m_state			= STATE_NONE;	// ���
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CBlur2D::Uninit(void)
{
	for (auto& rVec : m_oldObject)
	{ // �ۑ��I�u�W�F�N�g�����J��Ԃ�

		// �A�j���[�V����2D�̏I��
		SAFE_UNINIT(rVec);
	}

	// �ۑ��I�u�W�F�N�g�����N���A
	m_oldObject.clear();

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CBlur2D::Update(const float fDeltaTime)
{
	// �c���̐���
	if (m_state == STATE_NORMAL)
	{ // �ʏ��Ԃ̏ꍇ

#if 0
		// �I�u�W�F�N�g�ۑ��p�̓��I�z��𐶐�
		std::vector<CObjectModel*> tempBlur2D;
		tempBlur2D.resize((size_t)m_pParent->GetNumParts());	// �p�[�c�����z�񐶐�

		int nCntParts = 0;	// �p�[�c�C���f�b�N�X
		for (auto& rVecParts : tempBlur2D)
		{ // �p�[�c�����J��Ԃ�

			// �p�[�c�̐���
			rVecParts = CObjectModel::Create(VEC3_ZERO, VEC3_ZERO);
			if (rVecParts != nullptr)
			{ // �p�[�c�̐����ɐ��������ꍇ

				CObjectModel *pBlur2DParts = rVecParts;	// �c���p�[�c
				CMultiModel *pOriginParts = m_pParent->GetParts(nCntParts);		// ���_�p�[�c
				D3DXMATRIX mtxParts = pOriginParts->GetMtxWorld();				// �c���������̃}�g���b�N�X

				// ���𐶐����ƈ�v������
				pBlur2DParts->BindModel(pOriginParts->GetModelID());	// ���ꃂ�f��������
				pBlur2DParts->SetMtxWorld(mtxParts);					// ����}�g���b�N�X��ݒ�

				// �ʒu�E�����E�傫�����}�g���b�N�X����擾���ݒ�
				pBlur2DParts->SetVec3Position(useful::GetMatrixPosition(mtxParts));
				pBlur2DParts->SetVec3Rotation(useful::GetMatrixRotation(mtxParts));
				pBlur2DParts->SetVec3Scaling(useful::GetMatrixScaling(mtxParts));

				// ���Z�����ɂ���
				pBlur2DParts->GetRenderState()->SetAlphaBlend(CRenderState::BLEND_ADD);
			}

			// �p�[�c�C���f�b�N�X��i�߂�
			nCntParts++;
		}
#endif

		// �A�j���[�V����2D�ۑ��p�C���X�^���X�𐶐�
		CAnim2D* pTempBlur2D = CAnim2D::Create(1, 1, VEC3_ZERO, VEC3_ZERO);
		if (pTempBlur2D != nullptr)
		{ // �����ɐ��������ꍇ

			// ���𐶐����ƈ�v������
			pTempBlur2D->BindTexture(m_pParent->GetTextureIndex());	// ����e�N�X�`��������

			// �A�j���[�V���������擾���ݒ�
			pTempBlur2D->SetWidthPattern(m_pParent->GetWidthPattern());
			pTempBlur2D->SetHeightPattern(m_pParent->GetHeightPattern());
			pTempBlur2D->SetPattern(m_pParent->GetPattern());

			// �ʒu�E�����E�傫�����擾���ݒ�
			pTempBlur2D->SetVec3Position(m_pParent->GetVec3Position());
			pTempBlur2D->SetVec3Rotation(m_pParent->GetVec3Rotation());
			pTempBlur2D->SetVec3Sizing(m_pParent->GetVec3Sizing());

			// ���Z�����ɂ���
			pTempBlur2D->GetRenderState()->SetAlphaBlend(CRenderState::BLEND_ADD);
		}

		// �z��̍Ō���Ɏc����ǉ�
		m_oldObject.push_back(pTempBlur2D);
	}

	// �c���̐F���f
	{
		int nCntBlur2D = 0;	// �u���[�C���f�b�N�X
		for (auto& rVec : m_oldObject)
		{ // �ۑ��I�u�W�F�N�g�����J��Ԃ�

			// �����x�����ɂȂ�ɂ�ĉ����Ă���
			D3DXCOLOR colSet = m_col;	// �ݒ�F
			colSet.a = (m_fStartAlpha / m_nMaxLength) * nCntBlur2D;

			// �F�𔽉f
			rVec->SetColor(colSet);

			// �u���[�C���f�b�N�X��i�߂�
			nCntBlur2D++;
		}
	}

	// �c���̍폜
	{
		bool bDelete = false;	// �폜��
		switch (m_state)
		{ // ��Ԃ��Ƃ̏���
		case STATE_NONE:
			break;

		case STATE_NORMAL:

			// �폜�������w�� (�c�����ő吔�𒴂����ꍇ�폜)
			bDelete = (int)m_oldObject.size() > m_nMaxLength;
			break;

		case STATE_VANISH:

			// �폜�������w�� (�m��폜)
			bDelete = true;
			break;

		default:
			assert(false);
			break;
		}

		if (bDelete)
		{ // �폜���s���ꍇ

			if (m_oldObject.size() <= 0)
			{ // �z��v�f���Ȃ��ꍇ
			
				// �������Ȃ���Ԃɂ���
				m_state = STATE_NONE;

				// �����𔲂���
				return;
			}

			// �擪�̏I��
			SAFE_UNINIT(m_oldObject.front());

			// �c���̍Ō����z�񂩂�폜
			m_oldObject.erase(m_oldObject.begin());
		}
	}
	
}

//============================================================
//	�`�揈��
//============================================================
void CBlur2D::Draw(CShader *pShader)
{
	// �������Ȃ���Ԃ̏ꍇ������
	if (m_state == STATE_NONE) { return; }

	for (auto& rVec : m_oldObject)
	{ // �ۑ��I�u�W�F�N�g�����J��Ԃ�

		// �p�[�c�̕`��
		rVec->Draw(pShader);
	}
}

//============================================================
//	��������
//============================================================
CBlur2D *CBlur2D::Create
(
	CAnim2D *pParent,			// �e�I�u�W�F�N�g
	const D3DXCOLOR& rCol,		// �u���[���f�F
	const float fStartAlpha,	// �u���[�J�n�����x
	const int nMaxLength		// �ێ��I�u�W�F�N�g�ő吔
)
{
	// �u���[2D�̐���
	CBlur2D *pBlur2D = new CBlur2D;
	if (pBlur2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �u���[2D�̏�����
		if (FAILED(pBlur2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �u���[2D�̔j��
			SAFE_DELETE(pBlur2D);
			return nullptr;
		}

		// �e�I�u�W�F�N�g��ݒ�
		pBlur2D->m_pParent = pParent;

		// �u���[���f�F��ݒ�
		pBlur2D->SetColor(rCol);

		// �u���[�̊J�n�����x��ݒ�
		pBlur2D->SetStartAlpha(fStartAlpha);

		// �ێ��I�u�W�F�N�g�̍ő吔��ݒ�
		pBlur2D->SetMaxLength(nMaxLength);

		// �m�ۂ����A�h���X��Ԃ�
		return pBlur2D;
	}
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CBlur2D::SetState(const EState state)
{
	if (state == m_state && state != STATE_NORMAL)
	{ // �ݒ肷���Ԃ����݂̏�Ԋ��A�ݒ肷���Ԃ��ʏ��Ԃ̏ꍇ

		// �����𔲂���
		return;
	}

	if (m_state == STATE_NONE && state == STATE_VANISH)
	{ // ���łɏ����Ă���̂ɏ��������悤�Ƃ��Ă���ꍇ

		// �����𔲂���
		return;
	}

	// �����̏�Ԃ�ݒ�
	m_state = state;

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����
	case STATE_VANISH:	// �������
	case STATE_NORMAL:	// �ʏ���
		break;

	default:	// ��O���
		assert(false);
		break;
	}
}
