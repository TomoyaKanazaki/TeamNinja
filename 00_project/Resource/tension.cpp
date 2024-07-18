//===========================================
//
//  �m�C�̓Q�[�W(tension.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "tension.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const D3DXVECTOR3 TENSION_SIZE = D3DXVECTOR3(75.0f, 75.0f, 0.0f); // �|���S���̑傫��
	const float TENSION_WIDTH = SCREEN_WIDTH * 0.5f; // �m�C�̓Q�[�W�̕�
	const char* TENSION_TEXTURE = "data\\TEXTURE\\flower.png"; // �m�C�̓e�N�X�`��
}

//===========================================
//  �ÓI�����o�ϐ��錾
//===========================================
CListManager<CTension>* CTension::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//===========================================
//  �R���X�g���N�^
//===========================================
CTension::CTension() : CObject2D(CObject::LABEL_UI),
m_bUse(true)
{
}

//===========================================
//  �f�X�g���N�^
//===========================================
CTension::~CTension()
{
}

//===========================================
//  ����������
//===========================================
HRESULT CTension::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���X�g�����݂��Ă��Ȃ��ꍇ
	if (m_pList == nullptr)
	{
		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CTension>::Create();

		// �����Ɏ��s�����ꍇ�x����\�����֐��𔲂���
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// �傫����ݒ�
	SetVec3Sizing(TENSION_SIZE);

	// �ʒu�̐ݒ�
	CalcPosition();

	// �e�N�X�`�����蓖��
	BindTexture(TENSION_TEXTURE);

	// ������Ԃ�
	return S_OK;
}

//===========================================
//  �I������
//===========================================
void CTension::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �e�N���X�̏I��
	CObject2D::Uninit();
}

//===========================================
//  �X�V����
//===========================================
void CTension::Update(const float fDeltaTime)
{
	// ���W�̌v�Z
	CalcPosition();

	// �F�̕ύX
	ChangeColor();

	// �e�N���X�̍X�V����
	CObject2D::Update(fDeltaTime);
}

//===========================================
//  �`�揈��
//===========================================
void CTension::Draw(CShader* pShader)
{
	CObject2D::Draw(pShader);
}

//===========================================
//  ��������
//===========================================
CTension* CTension::Create()
{
	// �C���X�^���X����
	CTension* pTension = new CTension;

	// �������̊m�ۂɎ��s�����ꍇ�x����\����null��Ԃ�
	if (pTension == nullptr) { assert(false); return nullptr; }

	// ����������
	pTension->Init();

	// ����������
	if (FAILED(pTension->Init())) { SAFE_DELETE(pTension); return nullptr; }

	return pTension;
}

//===========================================
//  ���X�g�̎擾
//===========================================
CListManager<CTension>* CTension::GetList()
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//===========================================
//  ���W�̌v�Z����
//===========================================
void CTension::CalcPosition()
{
	// �ׂƂ̕����Z�o
	float fWidth = TENSION_WIDTH / m_pList->GetNumAll();

	// ���g�̃C���f�b�N�X���擾
	int nIdx = m_pList->GetIndex(this);

	// ���g�̃C���f�b�N�X��x���W�����炷
	float fPosX = (TENSION_SIZE.x * 0.5f) + (fWidth * nIdx);

	// ���W��ݒ�
	D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, TENSION_SIZE.y * 0.5f, 0.0f);

	// ���W��K�p
	SetVec3Position(pos);
}

//===========================================
//  �F�̕ύX
//===========================================
void CTension::ChangeColor()
{
	// �g�p�\�̏ꍇ�͕s����,�s�\�̏ꍇ�͔�����
	if (m_bUse)
	{
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
}
