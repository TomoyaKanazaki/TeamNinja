//============================================================
//
//	�_��}�[�N���� [goditem_mark.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "goditem_mark.h"
#include "godItem.h"
#include "manager.h"

#include "sound.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 STOP_SIZE = D3DXVECTOR3(60.0f, 60.0f, 0.0f);		// ���n�̃T�C�Y
	const D3DXVECTOR3 BODY_SIZE = D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// �{�̂̃T�C�Y
	const char* TEXTURE = "data\\TEXTURE\\itemGod000.png";		// �e�N�X�`��
	const int WIDTH_PATTERN = 3;		// �e�N�X�`���̉�������
	const int HEIGHT_PATTERN = 1;		// �e�N�X�`���̏c������

	const float ADD_ALPHA = 0.025f;		// �����x�̉��Z��
	const float ADD_SIZE = 0.2f;		// �T�C�Y�̉��Z��
}

//************************************************************
//	�q�N���X [CGodItemMark] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGodItemMark::CGodItemMark(const int nPriority) : CAnim2D(CObject::LABEL_NONE, CObject::SCENE_MAIN, CObject::DIM_2D, nPriority),
m_state(STATE_NONE),	// ���
m_fScaling(0.0f)		// �g�k��
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGodItemMark::~CGodItemMark()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGodItemMark::Init(void)
{
	// ���ʂ�Ԃ�
	return CAnim2D::Init();
}

//============================================================
//	�I������
//============================================================
void CGodItemMark::Uninit(void)
{
	// �I������
	CAnim2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGodItemMark::Update(const float fDeltaTime)
{
	switch (m_state)
	{
	case CGodItemMark::STATE_NONE:

		// �����ɂ��Ă���
		SetAlpha(0.0f);

		break;

	case CGodItemMark::STATE_SCALING:

		// �g�k����
		UpdateScaling();

		break;

	case CGodItemMark::STATE_STOP:



		break;

	default:
		assert(false);
		break;
	}

	// �X�V����
	CAnim2D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CGodItemMark::Draw(CShader* /*pShader*/)
{
	// �`�揈��
	CAnim2D::Draw();
}

//============================================================
// ���̐ݒ菈��
//============================================================
void CGodItemMark::SetData
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const int nIdx				// �C���f�b�N�X
)
{
	// ����ݒ�
	SetVec3Position(rPos);				// �ʒu
	SetVec3Rotation(rRot);				// ����
	SetVec3Sizing(BODY_SIZE);			// �傫��
	SetColor(XCOL_AWHITE);				// �F
	SetCounter(0);						// �J�E���^�[��ݒ肷��
	SetPattern(nIdx);					// �p�^�[����ݒ肷��
	SetEnableStop(false);				// ��~��
	SetWidthPattern(WIDTH_PATTERN);		// �e�N�X�`����������
	SetHeightPattern(HEIGHT_PATTERN);	// �e�N�X�`���c������

	// �e�N�X�`���̊��蓖��
	BindTexture(TEXTURE);

	if (CGodItem::IsGet((CGodItem::EType)(nIdx)))
	{ // ����ς݂̏ꍇ

		// ��~��Ԃɂ���
		m_state = STATE_STOP;
		SetAlpha(1.0f);
		SetVec3Sizing(STOP_SIZE);
	}
	else
	{ // ��L�ȊO

		// ������Ԃɂ���
		m_state = STATE_NONE;
	}
}

//============================================================
//	��������
//============================================================
CGodItemMark* CGodItemMark::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const int nIdx,				// �C���f�b�N�X
	const int nPriority			// �D�揇��
)
{
	// �^�C�}�[�̐���
	CGodItemMark* pTimer = new CGodItemMark(nPriority);

	if (pTimer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �^�C�}�[�̏�����
		if (FAILED(pTimer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �^�C�}�[�̔j��
			SAFE_DELETE(pTimer);
			return nullptr;
		}

		// ���̐ݒ菈��
		pTimer->SetData(rPos, rRot, nIdx);

		// �m�ۂ����A�h���X��Ԃ�
		return pTimer;
	}
}

//============================================================
// �g�k����
//============================================================
void CGodItemMark::UpdateScaling(void)
{
	D3DXVECTOR3 size = GetVec3Sizing();

	// �����x��ݒ�
	SetAlpha(GetAlpha() + ADD_ALPHA);

	// �g�k�������Z����
	m_fScaling += ADD_SIZE;

	// ����������
	size.x -= m_fScaling;
	size.y -= m_fScaling;

	if (size.x <= STOP_SIZE.x ||
		size.y <= STOP_SIZE.z)
	{ // �T�C�Y�����ȉ��ɂȂ����ꍇ

		// �T�C�Y��␳����
		size = STOP_SIZE;

		// �����x��␳����
		SetAlpha(1.0f);

		// ��~��Ԃɂ���
		m_state = STATE_STOP;

		// �_��UI�̉���炷
		PLAY_SOUND(CSound::LABEL_SE_GODITEMUI);
	}

	// �T�C�Y�𔽉f
	SetVec3Sizing(size);
}
