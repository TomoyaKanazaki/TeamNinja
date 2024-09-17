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

	const CScene::EMode INIT_SCENE = CScene::MODE_SELECT;	// �����V�[��

#else	// NDEBUG

	const CScene::EMode INIT_SCENE = CScene::MODE_TITLE;	// �����V�[��

#endif	// _DEBUG

	const int PRIORITY = 7;	// �t�F�[�h�̗D�揇��
	const POSGRID2 PART_CIRCLE = POSGRID2(64, 2);	// �؂蔲���^�̕�����
	const D3DXVECTOR3 SCREEN_EDGE[] =	// �l�p�̒��_
	{
		VEC3_ZERO,								// ����
		D3DXVECTOR3(SCREEN_SIZE.x, 0.0f, 0.0f),	// �E��
		D3DXVECTOR3(0.0f, SCREEN_SIZE.y, 0.0f),	// ����
		SCREEN_SIZE,							// �E��
	};
}

//************************************************************
//	�e�N���X [CFade] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFade::CFade() :
	m_pFuncSetMode	(nullptr),		// ���[�h�ݒ�֐��|�C���^
	m_pCrop			(nullptr),		// �؂蔲���^���
	m_pState		(nullptr),		// ���
	m_modeNext		(INIT_SCENE),	// ���V�[��
	m_fInTime		(0.0f),			// �C������
	m_fOutTime		(0.0f)			// �A�E�g����
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
	m_pCrop			= nullptr;		// �؂蔲���^���
	m_pState		= nullptr;		// ���
	m_modeNext		= INIT_SCENE;	// ���V�[��
	m_fInTime		= 0.0f;			// �C������
	m_fOutTime		= 0.0f;			// �A�E�g����

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

	// �؂蔲���^�̐���
	m_pCrop = CObjectCircle2D::Create(SCREEN_CENT, VEC3_ZERO, XCOL_AWHITE, PART_CIRCLE, 0.0f);
	if (m_pCrop == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����X�V�E�����`����~������
	m_pCrop->SetEnableUpdate(false);
	m_pCrop->SetEnableDraw(false);

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
	// �؂蔲���^�̏I��
	SAFE_UNINIT(m_pCrop);

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
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �X�e���V���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	if (typeid(*m_pState) == typeid(CFadeStateIrisIn)
	||  typeid(*m_pState) == typeid(CFadeStateIrisOut))
	{ // �A�C���X�C���E�A�E�g��Ԃ̏ꍇ

		// ��r�Q�ƒl��ݒ肷��
		pDevice->SetRenderState(D3DRS_STENCILREF, 1);

		// �X�e���V���}�X�N���w�肷�� 
		pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

		// �X�e���V����r�֐����w�肷��
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

		// �X�e���V�����ʂɑ΂��Ă̔��f�ݒ�
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Z�e�X�g�E�X�e���V���e�X�g����
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g���s
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g���s�E�X�e���V���e�X�g����

		// �؂蔲���^�̕`��
		m_pCrop->Draw();
	}

	// ��r�Q�ƒl��ݒ肷��
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);

	// �X�e���V���}�X�N���w�肷�� 
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	// �X�e���V����r�֐����w�肷��
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);

	// �X�e���V�����ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);	// Z�e�X�g�E�X�e���V���e�X�g����
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);	// Z�e�X�g�E�X�e���V���e�X�g���s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);	// Z�e�X�g���s�E�X�e���V���e�X�g����

	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw(pShader);

	// �X�e���V���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
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
//	�t�F�[�h�C�������̔���擾����
//============================================================
bool CFade::IsFadeIn(void)
{
	// �t�F�[�h�����̔����Ԃ�
	assert(m_pState != nullptr);
	return (typeid(*m_pState) == typeid(CFadeStateIn) || typeid(*m_pState) == typeid(CFadeStateIrisIn));
}

//============================================================
//	�t�F�[�h�̊J�n����
//============================================================
void CFade::SetFade
(
	const float fInTime,	// �C������
	const float fOutTime,	// �A�E�g����
	const int nPriority,	// �D�揇��
	const D3DXCOLOR& rCol	// �F
)
{
	// �t�F�[�h���̏ꍇ������
	if (IsFade()) { return; }

	// ���l�������Ԃ�ݒ�
	m_fInTime	= fInTime;
	m_fOutTime	= fOutTime;

	// �D�揇�ʂ�ݒ�
	SetPriority(nPriority);

	// �F�����ɂ���
	SetColor(rCol);

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
	const float fInTime,		// �C������
	const float fOutTime		// �A�E�g����
)
{
	// �t�F�[�h���̏ꍇ������
	if (IsFade()) { return; }

	// �]�C�t���[���I�[�o�[
	assert(fWaitTime >= 0.0f);

	// �����̃��[�h��ݒ�
	m_modeNext = mode;

	// ���l�������Ԃ�ݒ�
	m_fInTime	= fInTime;
	m_fOutTime	= fOutTime;

	// �D�揇�ʂ�ݒ�
	SetPriority(PRIORITY);

	// �F�����ɂ���
	SetColor(XCOL_ABLACK);

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
	const float fInTime,		// �C������
	const float fOutTime		// �A�E�g����
)
{
	// �t�F�[�h���̏ꍇ������
	if (IsFade()) { return; }

	// �]�C�t���[���I�[�o�[
	assert(fWaitTime >= 0.0f);

	// �����̃��[�h��ݒ�
	m_modeNext = mode;

	// ���l�������Ԃ�ݒ�
	m_fInTime	= fInTime;
	m_fOutTime	= fOutTime;

	// �D�揇�ʂ�ݒ�
	SetPriority(PRIORITY);

	// �F�����ɂ���
	SetColor(XCOL_ABLACK);

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
//	�A�C���X�t�F�[�h�̊J�n����
//============================================================
void CFade::SetIrisFade
(
	AGetPos pFuncPos,		// �ʒu�擾�֐�
	const float fInTime,	// �C������
	const float fOutTime,	// �A�E�g����
	const int nPriority,	// �D�揇��
	const D3DXCOLOR& rCol	// �F
)
{
	// �t�F�[�h���̏ꍇ������
	if (IsFade()) { return; }

	// ���l�������Ԃ�ݒ�
	m_fInTime	= fInTime;
	m_fOutTime	= fOutTime;

	// �D�揇�ʂ�ݒ�
	SetPriority(nPriority);

	// �F�����ɂ���
	SetColor(rCol);

	// ���[�h�ݒ�֐��|�C���^��������
	m_pFuncSetMode = nullptr;

	// �A�C���X�t�F�[�h�A�E�g��Ԃɂ���
	ChangeState(new CFadeStateIrisOut(pFuncPos));
}

//============================================================
//	���V�[���̐ݒ菈�� (�A�C���X�t�F�[�h�̂�)
//============================================================
void CFade::SetModeIrisFade
(
	const CScene::EMode mode,	// ���V�[��
	AGetPos pFuncPos,			// �ʒu�擾�֐�
	const float fWaitTime,		// �]�C����
	const float fInTime,		// �C������
	const float fOutTime		// �A�E�g����
)
{
	// �t�F�[�h���̏ꍇ������
	if (IsFade()) { return; }

	// �]�C�t���[���I�[�o�[
	assert(fWaitTime >= 0.0f);

	// �����̃��[�h��ݒ�
	m_modeNext = mode;

	// ���l�������Ԃ�ݒ�
	m_fInTime	= fInTime;
	m_fOutTime	= fOutTime;

	// �D�揇�ʂ�ݒ�
	SetPriority(PRIORITY);

	// �F�����ɂ���
	SetColor(XCOL_ABLACK);

	// ���[�h��ʂ����܂Ȃ����[�h�ݒ�֐���ݒ�
	m_pFuncSetMode = std::bind(&CManager::SetMode, GET_MANAGER, std::placeholders::_1);

	if (fWaitTime <= 0.0f)
	{ // �J�E���^�[�����ݒ�̏ꍇ

		// �A�C���X�t�F�[�h�A�E�g��Ԃɂ���
		ChangeState(new CFadeStateIrisOut(pFuncPos));
	}
	else
	{ // �J�E���^�[���ݒ肳�ꂽ�ꍇ

		// �t�F�[�h�]�C��Ԃɂ���
		ChangeState(new CFadeStateWait(fWaitTime, new CFadeStateIrisOut(pFuncPos)));
	}
}

//============================================================
//	���V�[���̐ݒ菈�� (���[�h��ʕt���A�C���X�t�F�[�h)
//============================================================
void CFade::SetLoadIrisFade
(
	const CScene::EMode mode,	// ���V�[��
	AGetPos pFuncPos,			// �ʒu�擾�֐�
	const float fWaitTime,		// �]�C����
	const float fInTime,		// �C������
	const float fOutTime		// �A�E�g����
)
{
	// �t�F�[�h���̏ꍇ������
	if (IsFade()) { return; }

	// �]�C�t���[���I�[�o�[
	assert(fWaitTime >= 0.0f);

	// �����̃��[�h��ݒ�
	m_modeNext = mode;

	// ���l�������Ԃ�ݒ�
	m_fInTime	= fInTime;
	m_fOutTime	= fOutTime;

	// �D�揇�ʂ�ݒ�
	SetPriority(PRIORITY);

	// �F�����ɂ���
	SetColor(XCOL_ABLACK);

	// ���[�h��ʂ����ރ��[�h�ݒ�֐���ݒ�
	m_pFuncSetMode = std::bind(&CManager::SetLoadMode, GET_MANAGER, std::placeholders::_1);

	if (fWaitTime <= 0.0f)
	{ // �J�E���^�[�����ݒ�̏ꍇ

		// �A�C���X�t�F�[�h�A�E�g��Ԃɂ���
		ChangeState(new CFadeStateIrisOut(pFuncPos));
	}
	else
	{ // �J�E���^�[���ݒ肳�ꂽ�ꍇ

		// �t�F�[�h�]�C��Ԃɂ���
		ChangeState(new CFadeStateWait(fWaitTime, new CFadeStateIrisOut(pFuncPos)));
	}
}

//============================================================
//	�؂蔲�����a�̌v�Z����
//============================================================
float CFade::CalcCropRadius(const D3DXVECTOR3& rPos)
{
	float fMaxLength = 0.0f;	// �ő勗��
	for (int i = 0; i < 4; i++)
	{ // �l���_���J��Ԃ�

		// �l�p�܂ł̋��������߂�
		D3DXVECTOR3 length = SCREEN_EDGE[i] - rPos;	// ��_�Ԃ̋���
		float fLen = fabsf(sqrtf(length.x * length.x + length.y * length.y));	// �����̐�Βl
		if (fMaxLength < fLen)
		{ // ��艓���������̏ꍇ

			// �ő勗�����X�V
			fMaxLength = fLen;
		}
	}

	// �ő勗����Ԃ�
	return fMaxLength;
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

	// ��ԂɃR���e�L�X�g��ݒ�
	m_pState->SetContext(this);

	// ��ԃC���X�^���X��������
	if (FAILED(m_pState->Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}
