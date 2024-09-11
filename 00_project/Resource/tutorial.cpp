//==========================================
//
//  �`���[�g���A���N���X(tutorial.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "tutorial.h"
#include "manager.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const char* TEXTURE_PASS[] = // �e�N�X�`���t�@�C���p�X
	{
		"data\\TEXTURE\\tutorial000.png",
		"data\\TEXTURE\\tutorial001.png",
		"data\\TEXTURE\\tutorial002.png"
	};

	const float WIDTH = 250.0f; // �|���S���̉���
	const D3DXVECTOR3 HIT_BOX = D3DXVECTOR3(250.0f, 250.0f, 250.0f); // ����͈�
}

//==========================================
//  �ÓI�x������
//==========================================
static_assert(NUM_ARRAY(TEXTURE_PASS) == CTutorial::TYPE_MAX, "ERROR : Type Count Missmatch");

//==========================================
//  �ÓI�ϐ��錾
//==========================================
CListManager<CTutorial>* CTutorial::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//==========================================
//  �R���X�g���N�^
//==========================================
CTutorial::CTutorial()
{
}

//==========================================
//  �f�X�g���N�^
//==========================================
CTutorial::~CTutorial()
{
}

//==========================================
//  ����������
//==========================================
HRESULT CTutorial::Init(void)
{
	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObjectBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CTutorial>::Create();
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

//==========================================
//  �I������
//==========================================
void CTutorial::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �e�N���X�̏I��
	CObjectBillboard::Uninit();
}

//==========================================
//  �X�V����
//==========================================
void CTutorial::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CObjectBillboard::Update(fDeltaTime);
}

//==========================================
//  �`�揈��
//==========================================
void CTutorial::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CObjectBillboard::Draw(pShader);
}

//==========================================
//  �e�N�X�`�����蓖��
//==========================================
void CTutorial::BindTexture(const char* pTexturePass)
{
	// �e�N�X�`�������蓖��
	CObjectBillboard::BindTexture(pTexturePass);

	// ���蓖�Ă��e�N�X�`������A�X�y�N�g����擾
	SetVec3Sizing(useful::GetTexAspect(GET_MANAGER->GetTexture()->Regist(pTexturePass)) * WIDTH);
}

//==========================================
//  ��������
//==========================================
CTutorial* CTutorial::Create(const D3DXVECTOR3& rPos, const EType type)
{
	// ���������m��
	CTutorial* pTutorial = new CTutorial;

	// �������̊m�ۂɎ��s�����ꍇ�֐��𔲂���
	if (pTutorial == nullptr) { assert(false); return nullptr; }

	// ����������
	if (FAILED(pTutorial->Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���g��UI�̔j��
		SAFE_DELETE(pTutorial);
		return nullptr;
	}

	// ���W��ݒ�
	pTutorial->SetVec3Position(rPos);

	// ��ނɍ��킹���e�N�X�`�������蓖��
	pTutorial->BindTexture(TEXTURE_PASS[type]);

	// �m�ۂ����A�h���X��Ԃ�
	return pTutorial;
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CTutorial>* CTutorial::GetList(void)
{
	return nullptr;
}
