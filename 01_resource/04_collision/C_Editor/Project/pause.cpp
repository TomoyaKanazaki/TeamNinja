//============================================================
//
//	�|�[�Y���� [pause.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "pause.h"
#include "renderer.h"
#include "manager.h"
#include "sceneGame.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\pause000.png",	// �ĊJ�e�N�X�`��
		"data\\TEXTURE\\pause001.png",	// ���g���C�e�N�X�`��
		"data\\TEXTURE\\pause002.png",	// �I���e�N�X�`��
	};

	const int PRIORITY = 6;	// �|�[�Y�\���̗D�揇��

	const D3DXCOLOR BG_COL		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);	// �w�i�J���[
	const D3DXCOLOR CHOICE_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �I�𒆃J���[
	const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// ��I�𒆃J���[

	namespace select
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(640.0f, 210.0f, 0.0f);	// �Z���N�g���j���[�̈ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(440.0f, 110.0f, 0.0f);	// �Z���N�g���j���[�̑傫��
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(0.0f, 150.0f, 0.0f);		// �Z���N�g���j���[�̋�
	}
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CPause::SELECT_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CPause] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPause::CPause() :
	m_pBG			(nullptr),			// �w�i���
	m_nSelect		(SELECT_RESUME),	// ���݂̑I��
	m_nOldSelect	(SELECT_RESUME),	// �O��̑I��
	m_bPause		(false)				// �|�[�Y��
{
	// �����o�ϐ����N���A
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// �I�����
}

//============================================================
//	�f�X�g���N�^
//============================================================
CPause::~CPause()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPause::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// �I�����
	m_pBG		 = nullptr;			// �w�i���
	m_nSelect	 = SELECT_RESUME;	// ���݂̑I��
	m_nOldSelect = SELECT_RESUME;	// �O��̑I��
	m_bPause	 = false;			// �|�[�Y��

	// �w�i���̐���
	m_pBG = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		SCREEN_SIZE,	// �傫��
		VEC3_ZERO,		// ����
		BG_COL			// �F
	);
	if (m_pBG == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pBG->SetPriority(PRIORITY);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // �I�����̍��ڐ����J��Ԃ�

		// �I�����̐���
		m_apSelect[nCntPause] = CObject2D::Create
		( // ����
			select::POS + (select::SPACE * (float)nCntPause),	// �ʒu
			select::SIZE,	// �傫��
			VEC3_ZERO,		// ����
			DEFAULT_COL		// �F
		);
		if (m_apSelect[nCntPause] == nullptr)
		{ // ��g�p���̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apSelect[nCntPause]->SetPriority(PRIORITY);

		// �e�N�X�`����o�^�E����
		m_apSelect[nCntPause]->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[nCntPause]));
	}

	// �`��󋵂̐ݒ�
	SetEnableDraw(m_bPause);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPause::Uninit(void)
{
	// �w�i���̏I��
	SAFE_UNINIT(m_pBG);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // �I�����̍��ڐ����J��Ԃ�

		// �I�����̏I��
		SAFE_UNINIT(m_apSelect[nCntPause]);
	}
}

//============================================================
//	�X�V����
//============================================================
void CPause::Update(void)
{
	if (GET_MANAGER->GetFade()->GetState() == CFade::FADE_NONE)
	{ // �t�F�[�h���Ă��Ȃ��ꍇ

		if (GET_INPUTKEY->IsTrigger(DIK_P)
		||  GET_INPUTPAD->IsTrigger(CInputPad::KEY_START))
		{
			// �|�[�Y�󋵂�؂�ւ�
			m_bPause = !m_bPause;

			// ���݂̑I����������
			m_nSelect = SELECT_RESUME;

			// �`��󋵂̐ݒ�
			SetEnableDraw(m_bPause);

			// �T�E���h�̍Đ�
			GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// ���艹00
		}
	}

	if (m_bPause)
	{ // �|�[�Y���̏ꍇ

		// �I�𑀍�
		Select();

		// �O��̑I��v�f�̐F�����ɐݒ�
		m_apSelect[m_nOldSelect]->SetColor(DEFAULT_COL);

		// ���݂̑I��v�f�̐F�𔒂ɐݒ�
		m_apSelect[m_nSelect]->SetColor(CHOICE_COL);

		// ���݂̑I��v�f����
		m_nOldSelect = m_nSelect;
	}

	// �w�i���̍X�V
	m_pBG->Update();

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // �I�����̍��ڐ����J��Ԃ�

		// �I�����̍X�V
		m_apSelect[nCntPause]->Update();
	}
}

//============================================================
//	�`�揈��
//============================================================
void CPause::Draw(void)
{

}

//============================================================
//	�|�[�Y�󋵎擾����
//============================================================
bool CPause::IsPause(void)
{
	// �|�[�Y�󋵂�Ԃ�
	return m_bPause;
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPause::SetEnableDraw(const bool bDraw)
{
	// �w�i���̕`��󋵂�ݒ�
	m_pBG->SetEnableDraw(bDraw);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // �I�����̍��ڐ����J��Ԃ�

		// �I�����̕`��󋵂�ݒ�
		m_apSelect[nCntPause]->SetEnableDraw(bDraw);
	}
}

//============================================================
//	��������
//============================================================
CPause *CPause::Create(void)
{
	// �|�[�Y�̐���
	CPause *pPause = new CPause;
	if (pPause == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �|�[�Y�̏�����
		if (FAILED(pPause->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �|�[�Y�̔j��
			SAFE_DELETE(pPause);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pPause;
	}
}

//============================================================
//	�j������
//============================================================
void CPause::Release(CPause *&prPause)
{
	// �|�[�Y�̏I��
	assert(prPause != nullptr);
	prPause->Uninit();

	// �������J��
	SAFE_DELETE(prPause);
}

//============================================================
//	�f�o�b�O�\���󋵂̐ݒ菈��
//============================================================
#ifdef _DEBUG

void CPause::SetEnableDebugDisp(const bool bDisp)
{
	// �w�i���̕`��󋵂�ݒ�
	m_pBG->SetEnableDebugDisp(bDisp);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // �I�����̍��ڐ����J��Ԃ�

		// �I�����̕`��󋵂�ݒ�
		m_apSelect[nCntPause]->SetEnableDebugDisp(bDisp);
	}
}

#endif	// _DEBUG

//============================================================
//	�f�o�b�O�\���󋵂̎擾����
//============================================================
#ifdef _DEBUG

bool CPause::IsDebugDisp(void) const
{
	// �f�o�b�O�\���󋵂�Ԃ�
	return m_pBG->IsDebugDisp();
}

#endif	// _DEBUG

//============================================================
//	�I�𑀍쏈��
//============================================================
void CPause::Select(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= GET_INPUTKEY;	// �L�[�{�[�h
	CInputPad		*pPad		= GET_INPUTPAD;	// �p�b�h

	if (GET_MANAGER->GetFade()->GetState() == CFade::FADE_NONE)
	{ // �t�F�[�h���Ă��Ȃ��ꍇ

		if (pKeyboard->IsTrigger(DIK_W)
		||  pKeyboard->IsTrigger(DIK_UP)
		||  pPad->IsTrigger(CInputPad::KEY_UP))
		{ // ��ړ��̑��삪�s��ꂽ�ꍇ

			// ��ɑI�������炷
			m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;

			// �T�E���h�̍Đ�
			GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
		}
		if (pKeyboard->IsTrigger(DIK_S)
		||  pKeyboard->IsTrigger(DIK_DOWN)
		||  pPad->IsTrigger(CInputPad::KEY_DOWN))
		{ // ���ړ��̑��삪�s��ꂽ�ꍇ

			// ���ɑI�������炷
			m_nSelect = (m_nSelect + 1) % SELECT_MAX;

			// �T�E���h�̍Đ�
			GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
		}

		if (pKeyboard->IsTrigger(DIK_RETURN)  || pKeyboard->IsTrigger(DIK_SPACE)
		||  pPad->IsTrigger(CInputPad::KEY_A) || pPad->IsTrigger(CInputPad::KEY_B)
		||  pPad->IsTrigger(CInputPad::KEY_X) || pPad->IsTrigger(CInputPad::KEY_Y))
		{ // ����̑��삪�s��ꂽ�ꍇ

			// �T�E���h�̍Đ�
			GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// ���艹00

			switch (m_nSelect)
			{ // �I�����Ƃ̏���
			case SELECT_RESUME:	// �ĊJ

				// �|�[�Y���I������
				m_bPause = false;

				// �`��󋵂̐ݒ�
				SetEnableDraw(m_bPause);

				// �����𔲂���
				break;

			case SELECT_RETRY:	// ���g���C

				// �V�[���̐ݒ�
				GET_MANAGER->SetScene(CScene::MODE_GAME);	// �Q�[�����

				// �����𔲂���
				break;

			case SELECT_EXIT:	// �I��

				// �V�[���̐ݒ�
				GET_MANAGER->SetScene(CScene::MODE_GAME);	// �Q�[�����

				// �����𔲂���
				break;
			}
		}
	}
}