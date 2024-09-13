//============================================================
//
//	�_��UI���� [goditemUI.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "goditemUI.h"

#include "anim2D.h"
#include "goditem_mark.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 0;	// �^�C�}�[�̗D�揇��

	const D3DXVECTOR3 POS[CGodItem::TYPE_MAX] =		// �ʒu
	{
		D3DXVECTOR3(SCREEN_CENT.x, 30.0f, 0.0f),	// ��
		D3DXVECTOR3(615.0f, 75.0f, 0.0f),	// ��
		D3DXVECTOR3(665.0f, 75.0f, 0.0f)	// ��
	};
	const D3DXVECTOR3 ROT[CGodItem::TYPE_MAX] =		// ����
	{
		D3DXVECTOR3(0.0f, 0.0f, (D3DX_PI * 2.0f) * 0.3f),	// ��
		D3DXVECTOR3(0.0f, 0.0f, (D3DX_PI * 2.0f) * 0.6f),	// ��
		D3DXVECTOR3(0.0f, 0.0f, (D3DX_PI * 2.0f) * 0.9f)	// ��
	};
	const D3DXVECTOR3 GROUND_SIZE = D3DXVECTOR3(60.0f, 60.0f, 0.0f);	// ���n�̃T�C�Y
	const char* TEXTURE = "data\\TEXTURE\\itemGod000.png";		// �e�N�X�`��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CGodItemUI>* CGodItemUI::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CGodItemUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGodItemUI::CGodItemUI() : CObject(CObject::LABEL_GODITEMUI, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY)
{
	// �����o�ϐ����N���A
	memset(&m_apGround[0], 0, sizeof(m_apGround));	// ���n�̏��
	memset(&m_apMark[0], 0, sizeof(m_apMark));		// �}�[�N�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CGodItemUI::~CGodItemUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGodItemUI::Init(void)
{
	// �����o�ϐ���������
	for (int nCntGround = 0; nCntGround < CGodItem::TYPE_MAX; nCntGround++)
	{
		// ���n�� NULL �̏ꍇ�A���ɐi��
		if (m_apGround[nCntGround] != nullptr) { assert(false); continue; }

		// ���n�𐶐�����
		m_apGround[nCntGround] = CAnim2D::Create
		(
			CGodItem::TYPE_MAX,	// �e�N�X�`���̉��̕�����
			1,					// �e�N�X�`���̏c�̕�����
			POS[nCntGround],	// �ʒu
			GROUND_SIZE,		// �T�C�Y
			ROT[nCntGround],	// ����
			XCOL_BLACK			// �F
		);

		// �e�N�X�`�������蓖�Ă�
		m_apGround[nCntGround]->BindTexture(TEXTURE);

		// �D�揇�ʂ�ݒ�
		m_apGround[nCntGround]->SetPriority(PRIORITY);

		// �J�E���^�[��ݒ肷��
		m_apGround[nCntGround]->SetCounter(0);

		// �p�^�[����ݒ肷��
		m_apGround[nCntGround]->SetPattern(nCntGround);

		// ��~�󋵂�ݒ�
		m_apGround[nCntGround]->SetEnableStop(false);
	}

	// �����o�ϐ���������
	for (int nCntMark = 0; nCntMark < CGodItem::TYPE_MAX; nCntMark++)
	{
		// �}�[�N�� NULL �̏ꍇ�A���ɐi��
		if (m_apMark[nCntMark] != nullptr) { assert(false); continue; }

		// ��������
		m_apMark[nCntMark] = CGodItemMark::Create
		(
			POS[nCntMark],
			ROT[nCntMark],
			nCntMark,
			PRIORITY
		);
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CGodItemUI>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGodItemUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < CGodItem::TYPE_MAX; nCnt++)
	{
		// ���n�Ɩ{�̂�j������
		SAFE_UNINIT(m_apGround[nCnt]);
		SAFE_UNINIT(m_apMark[nCnt]);
	}

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CGodItemUI::Update(const float fDeltaTime)
{
	// ������1�����̏ꍇ�A��~
	if (m_pList->GetNumAll() > 1) { assert(false); }

	for (int nCnt = 0; nCnt < CGodItem::TYPE_MAX; nCnt++)
	{
		if (m_apGround[nCnt] != nullptr)
		{ // ���n�� NULL ����Ȃ��ꍇ

			// �X�V����
			m_apGround[nCnt]->Update(fDeltaTime);
		}

		if (m_apMark[nCnt] != nullptr)
		{ // �{�̂� NULL ����Ȃ��ꍇ

			// �X�V����
			m_apMark[nCnt]->Update(fDeltaTime);
		}
	}
}

//============================================================
//	�`�揈��
//============================================================
void CGodItemUI::Draw(CShader* /*pShader*/)
{
	for (int nCntGround = 0; nCntGround < CGodItem::TYPE_MAX; nCntGround++)
	{
		// ���n�� NULL ����Ȃ��ꍇ�A���ɐi��
		if (m_apGround[nCntGround] == nullptr) { continue; }

		// �`�揈��
		m_apGround[nCntGround]->Draw();
	}

	for (int nCntMark = 0; nCntMark < CGodItem::TYPE_MAX; nCntMark++)
	{
		// �{�̂� NULL ����Ȃ��ꍇ�A���ɐi��
		if (m_apMark[nCntMark] == nullptr) { continue; }

		// �`�揈��
		m_apMark[nCntMark]->Draw();
	}
}

//============================================================
// �擾����
//============================================================
void CGodItemUI::Get(const CGodItem::EType type)
{
	// ���Ɏ擾���Ă����ꍇ�A��~
	if (m_apMark[type]->GetState() != CGodItemMark::STATE_NONE) { assert(false); return; }

	// �g�k��Ԃɂ���
	m_apMark[type]->SetState(CGodItemMark::STATE_SCALING);
}

//============================================================
//	��������
//============================================================
CGodItemUI* CGodItemUI::Create(void)
{
	// �^�C�}�[�̐���
	CGodItemUI* pTimer = new CGodItemUI;

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

		// �m�ۂ����A�h���X��Ԃ�
		return pTimer;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CGodItemUI>* CGodItemUI::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}
