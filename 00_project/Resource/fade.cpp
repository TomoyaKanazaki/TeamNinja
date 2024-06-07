//============================================================
//
//	�t�F�[�h���� [fade.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "loading.h"
#include "fadeState.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
#ifdef _DEBUG

	const CScene::EMode INIT_SCENE = CScene::MODE_GAME;		// �����V�[��

#else	// NDEBUG

	const CScene::EMode INIT_SCENE = CScene::MODE_TITLE;	// �����V�[��

#endif	// _DEBUG

	const int	PRIORITY = 7;		// �t�F�[�h�̗D�揇��
	const float	LEVEL	 = 5.0f;	// �t�F�[�h�̃��l������
}

//************************************************************
//	�e�N���X [CFade] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFade::CFade() :
	m_pFuncSetMode	(nullptr),		// ���[�h�ݒ�֐��|�C���^
	m_pState		(nullptr),		// ���
	m_modeNext		(INIT_SCENE),	// ���V�[��
	m_fSubIn		(LEVEL),		// �C���̃��l������
	m_fAddOut		(LEVEL)			// �A�E�g�̃��l������
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CFade::~CFade()
{

}

//============================================================
//	����������
//============================================================
HRESULT CFade::Init(void)
{
	// �����o�ϐ���������
	m_pFuncSetMode	= nullptr;		// ���[�h�ݒ�֐��|�C���^
	m_pState		= nullptr;		// ���
	m_modeNext		= INIT_SCENE;	// ���V�[��
	m_fSubIn		= LEVEL;		// �C���̃��l������
	m_fAddOut		= LEVEL;		// �A�E�g�̃��l������

	// �t�F�[�h�C����Ԃɂ���
	ChangeState(new CFadeStateIn);

	// �I�u�W�F�N�g2D�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �ʒu���X�N���[�������ɂ���
	SetVec3Position(SCREEN_CENT);

	// �傫�����X�N���[���T�C�Y�ɂ���
	SetVec3Sizing(SCREEN_SIZE);

	// �F�����ɂ���
	SetColor(XCOL_BLACK);

	// �D�揇�ʂ���ԏ�ɂ���
	SetPriority(PRIORITY);

	// ���x���w��Ȃ��ɂ���
	SetLabel(CObject::LABEL_NONE);	// �����j���E�X�V���~����

	// �V�[���̏�����
	if (FAILED(GET_MANAGER->InitScene(m_modeNext)))
	{ // �������Ɏ��s�����ꍇ

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
void CFade::Uninit(void)
{
	// ��Ԃ̏I��
	SAFE_UNINIT(m_pState);

	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CFade::Update(const float fDeltaTime)
{
	// �t�F�[�h���Ă��Ȃ��ꍇ������
	if (!IsFade()) { return; }

	// ��Ԃ��Ƃ̍X�V
	assert(m_pState != nullptr);
	m_pState->Update(fDeltaTime);

	// �I�u�W�F�N�g2D�̍X�V
	CObject2D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CFade::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw(pShader);
}

//============================================================
//	���V�[���J�ڏ���
//============================================================
void CFade::TransNextMode(void)
{
	if (m_pFuncSetMode != nullptr)
	{ // ���[�h�ݒ�֐����w�肳��Ă���ꍇ

		// ���[�h�̐ݒ�
		m_pFuncSetMode(m_modeNext);
	}
}

//============================================================
//	�t�F�[�h�����̔���擾����
//============================================================
bool CFade::IsFade(void)
{
	// �t�F�[�h�����̔����Ԃ�
	assert(m_pState != nullptr);
	return (typeid(*m_pState) != typeid(CFadeStateNone));
}

//============================================================
//	���l��������
//============================================================
bool CFade::SubAlpha(const float fDeltaTime)
{
	D3DXCOLOR colFade = GetColor();	// �t�F�[�h�F
	bool bComplete = false;	// ������

	// �����ɂ��Ă���
	colFade.a -= m_fSubIn * fDeltaTime;
	if (colFade.a <= 0.0f)
	{ // �����ɂȂ����ꍇ

		// ���l��␳
		colFade.a = 0.0f;

		// �t�F�[�h������ۑ�
		bComplete = true;
	}

	// �F�𔽉f
	SetColor(colFade);

	// �t�F�[�h�����󋵂�Ԃ�
	return bComplete;
}

//============================================================
//	���l��������
//============================================================
bool CFade::AddAlpha(const float fDeltaTime)
{
	D3DXCOLOR colFade = GetColor();	// �t�F�[�h�F
	bool bComplete = false;	// ������

	// �s�����ɂ��Ă���
	colFade.a += m_fAddOut * fDeltaTime;
	if (colFade.a >= 1.0f)
	{ // �s�����ɂȂ����ꍇ

		// ���l��␳
		colFade.a = 1.0f;

		// �t�F�[�h������ۑ�
		bComplete = true;
	}

	// �F�𔽉f
	SetColor(colFade);

	// �t�F�[�h�����󋵂�Ԃ�
	return bComplete;
}

//============================================================
//	�t�F�[�h�̊J�n����
//============================================================
void CFade::SetFade
(
	const float fAddOut,	// �A�E�g�̃��l������
	const float fSubIn,		// �C���̃��l������
	const int nPriority		// �D�揇��
)
{
	// �t�F�[�h���̏ꍇ������
	if (IsFade()) { return; }

	// ���l�����ʂ�ݒ�
	m_fSubIn  = fSubIn;
	m_fAddOut = fAddOut;

	// �D�揇�ʂ�ݒ�
	SetPriority(nPriority);

	// ���[�h�ݒ�֐��|�C���^��������
	m_pFuncSetMode = nullptr;

	// �t�F�[�h�A�E�g��Ԃɂ���
	ChangeState(new CFadeStateOut);
}

//============================================================
//	���V�[���̐ݒ菈�� (�t�F�[�h�̂�)
//============================================================
void CFade::SetModeFade
(
	const CScene::EMode mode,	// ���V�[��
	const float fWaitTime,		// �]�C����
	const float fAddOut,		// �A�E�g�̃��l������
	const float fSubIn			// �C���̃��l������
)
{
	// �t�F�[�h���̏ꍇ������
	if (IsFade()) { return; }

	// �]�C�t���[���I�[�o�[
	assert(fWaitTime >= 0.0f);

	// �����̃��[�h��ݒ�
	m_modeNext = mode;

	// ���l�����ʂ�ݒ�
	m_fSubIn  = fSubIn;
	m_fAddOut = fAddOut;

	// �D�揇�ʂ�ݒ�
	SetPriority(PRIORITY);

	// ���[�h��ʂ����܂Ȃ����[�h�ݒ�֐���ݒ�
	m_pFuncSetMode = std::bind(&CManager::SetMode, GET_MANAGER, std::placeholders::_1);

	if (fWaitTime <= 0.0f)
	{ // �J�E���^�[�����ݒ�̏ꍇ

		// �t�F�[�h�A�E�g��Ԃɂ���
		ChangeState(new CFadeStateOut);
	}
	else
	{ // �J�E���^�[���ݒ肳�ꂽ�ꍇ

		// �t�F�[�h�]�C��Ԃɂ���
		ChangeState(new CFadeStateWait(fWaitTime, new CFadeStateOut));
	}
}

//============================================================
//	���V�[���̐ݒ菈�� (���[�h��ʕt��)
//============================================================
void CFade::SetLoadFade
(
	const CScene::EMode mode,	// ���V�[��
	const float fWaitTime,		// �]�C����
	const float fAddOut,		// �A�E�g�̃��l������
	const float fSubIn			// �C���̃��l������
)
{
	// �t�F�[�h���̏ꍇ������
	if (IsFade()) { return; }

	// �]�C�t���[���I�[�o�[
	assert(fWaitTime >= 0.0f);

	// �����̃��[�h��ݒ�
	m_modeNext = mode;

	// ���l�����ʂ�ݒ�
	m_fSubIn  = fSubIn;
	m_fAddOut = fAddOut;

	// �D�揇�ʂ�ݒ�
	SetPriority(PRIORITY);

	// ���[�h��ʂ����ރ��[�h�ݒ�֐���ݒ�
	m_pFuncSetMode = std::bind(&CManager::SetLoadMode, GET_MANAGER, std::placeholders::_1);

	if (fWaitTime <= 0.0f)
	{ // �J�E���^�[�����ݒ�̏ꍇ

		// �t�F�[�h�A�E�g��Ԃɂ���
		ChangeState(new CFadeStateOut);
	}
	else
	{ // �J�E���^�[���ݒ肳�ꂽ�ꍇ

		// �t�F�[�h�]�C��Ԃɂ���
		ChangeState(new CFadeStateWait(fWaitTime, new CFadeStateOut));
	}
}

//============================================================
//	��������
//============================================================
CFade *CFade::Create(void)
{
	// �t�F�[�h�̐���
	CFade *pFade = new CFade;
	if (pFade == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �t�F�[�h�̏�����
		if (FAILED(pFade->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �t�F�[�h�̔j��
			SAFE_DELETE(pFade);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pFade;
	}
}

//============================================================
//	��Ԃ̕ύX����
//============================================================
HRESULT CFade::ChangeState(CFadeState *pState)
{
	// ��Ԃ̐����Ɏ��s���Ă���ꍇ������
	if (pState == nullptr) { assert(false); return E_FAIL; }

	// ��ԃC���X�^���X���I��
	SAFE_UNINIT(m_pState);

	// ��ԃC���X�^���X��ύX
	assert(m_pState == nullptr);
	m_pState = pState;

	// ��ԃC���X�^���X��������
	if (FAILED(m_pState->Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ��ԂɃR���e�L�X�g��ݒ�
	m_pState->SetContext(this);

	// ������Ԃ�
	return S_OK;
}
