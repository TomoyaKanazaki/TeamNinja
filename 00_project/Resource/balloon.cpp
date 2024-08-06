//============================================================
//
//	�����o������ [balloon.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "balloon.h"
#include "manager.h"
#include "renderer.h"
#include "sceneSelect.h"
#include "selectManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY	 = 0;		// �����o���̗D�揇��
	const int	ALPHA_NUMREF = 120;		// ���e�X�g�̎Q�ƒl
	const float	REV_SCALE	 = 0.4f;	// �g�嗦�̕␳�W��
	const float	DISP_SCALE	 = 1.4f;	// �\������ۂ̊g�嗦
	const float	UNDISP_SCALE = 0.0f;	// �\�����Ȃ��ۂ̊g�嗦

	namespace balloon
	{
		const D3DXVECTOR3 SIZE	 = D3DXVECTOR3(320.0f, 180.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 OFFSET = D3DXVECTOR3(0.0f, 240.0f, 0.0f);		// �I�t�Z�b�g
	}
}

//************************************************************
//	�q�N���X [CBalloon] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBalloon::CBalloon() : CObjectBillboard(CObject::LABEL_UI),
	m_fScale	 (0.0f),	// �g�嗦
	m_fDestScale (0.0f)		// �ڕW�g�嗦
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CBalloon::~CBalloon()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBalloon::Init(void)
{
	// �����o�ϐ���������
	m_fScale	 = 0.0f;	// �g�嗦
	m_fDestScale = 0.0f;	// �ڕW�g�嗦

	// �I�u�W�F�N�g�r���{�[�h�̏�����
	if (FAILED(CObjectBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �傫����ݒ�
	SetVec3Sizing(balloon::SIZE * m_fScale);

	// �r���{�[�h�V�[���̃����_�[�e�N�X�`��������
	BindTexture(GET_RENDERER->GetRenderTextureIndex(CObject::SCENE_BILLBOARD));

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaTest(true);			// ���e�X�g�̗L�� / �����̐ݒ�
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaNumRef(ALPHA_NUMREF);	// ���e�X�g�̎Q�ƒl�ݒ�

	// ��΂ɕ`�悪��������悤�ɂ���
	pRenderState->SetZFunc(D3DCMP_ALWAYS);

	// ���C�e�B���O��OFF�ɂ���
	pRenderState->SetLighting(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CBalloon::Uninit(void)
{
	// �I�u�W�F�N�g�r���{�[�h�̏I��
	CObjectBillboard::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CBalloon::Update(const float fDeltaTime)
{
	// �g�嗦�̍������v�Z
	float fDiffScale = m_fDestScale - m_fScale;

	// �g�嗦���v�Z
	m_fScale += fDiffScale * REV_SCALE;

	// �傫����ݒ�
	SetVec3Sizing(balloon::SIZE * m_fScale);

	// �I�u�W�F�N�g�r���{�[�h�̍X�V
	CObjectBillboard::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CBalloon::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g�r���{�[�h�̕`��
	CObjectBillboard::Draw(pShader);
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CBalloon::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// ���g�̈ʒu��ݒ�
	CObjectBillboard::SetVec3Position(rPos + balloon::OFFSET);
}

//============================================================
//	��������
//============================================================
CBalloon *CBalloon::Create(const D3DXVECTOR3& rPosParent)
{
	// �����o���̐���
	CBalloon *pBalloon = new CBalloon;
	if (pBalloon == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �����o���̏�����
		if (FAILED(pBalloon->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �����o���̔j��
			SAFE_DELETE(pBalloon);
			return nullptr;
		}

		// �ʒu��ݒ�
		pBalloon->SetVec3Position(rPosParent);

		// �m�ۂ����A�h���X��Ԃ�
		return pBalloon;
	}
}

//============================================================
//	�����o���\���̐ݒ菈��
//============================================================
void CBalloon::SetDisp(const bool bDisp)
{
	if (bDisp)
	{ // �\������ꍇ

		// �ڕW�g�嗦��傫������
		m_fDestScale = DISP_SCALE;
	}
	else
	{ // �\�����Ȃ��ꍇ

		// �ڕW�g�嗦������������
		m_fDestScale = UNDISP_SCALE;
	}
}
