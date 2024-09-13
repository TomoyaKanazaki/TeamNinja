//==========================================
//
//  �M��ui�N���X(drown.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "drown.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const char* TEXTURE_PAS = "data\\TEXTURE\\wave.png"; // �e�N�X�`���p�X
	const float MOVE_SPEED = -360.0f; // �ړ���
}

//==========================================
//  �R���X�g���N�^
//==========================================
CDrown::CDrown() :
	m_bMove(false),
	m_fMove(MOVE_SPEED)
{
}

//==========================================
//  �f�X�g���N�^
//==========================================
CDrown::~CDrown()
{
}

//==========================================
//  ����������
//==========================================
HRESULT CDrown::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`�����蓖��
	BindTexture(TEXTURE_PAS);

	// ���x����ݒ�
	SetLabel(LABEL_UI);

	// �ړ��ʂ�ݒ�
	m_fMove = MOVE_SPEED;

	// ������Ԃ�
	return S_OK;
}

//==========================================
//  �I������
//==========================================
void CDrown::Uninit(void)
{
	// �e�N���X�̏I��
	CObject2D::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CDrown::Update(const float fDeltaTime)
{
	// �ړ�
	Move(fDeltaTime);

	// �e�N���X�̍X�V
	CObject2D::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CDrown::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CObject2D::Draw(pShader);
}

//==========================================
//  �ړ�����
//==========================================
void CDrown::Move(const float fDeltaTime)
{
	// �t���O��off�Ȃ�֐��𔲂���
	if (!m_bMove) { return; }

	// ���W���擾
	D3DXVECTOR3 pos = GetVec3Position();
	
	// �ړ�
	pos.y += m_fMove * fDeltaTime;

	// ���W��␳����
	if (GetVec3Position().y < SCREEN_CENT.y)
	{
		pos = SCREEN_CENT;
		m_fMove *= -1.0f;
		m_bBrind = true;
	}

	// ���W��K�p
	SetVec3Position(pos);
}

//==========================================
//  ��������
//==========================================
CDrown* CDrown::Create(void)
{
	// �C���X�^���X����
	CDrown* pUI = new CDrown;

	// �������̊m�ۂɎ��s�����ꍇ�x����\����null��Ԃ�
	if (pUI == nullptr) { assert(false); return nullptr; }

	// ����������
	if (FAILED(pUI->Init())) { SAFE_DELETE(pUI); return nullptr; }

	// �T�C�Y��ݒ�
	pUI->SetVec3Sizing(SCREEN_SIZE * 1.4f);

	// ���W��ݒ�
	pUI->SetVec3Position(D3DXVECTOR3(SCREEN_CENT.x, SCREEN_CENT.y * 3.0f, 0.0f));

	return pUI;
}
