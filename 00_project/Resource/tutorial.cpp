//==========================================
//
//  �`���[�g���A���N���X(tutorial.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "tutorial.h"
#include "manager.h"
#include "player.h"
#include "collision.h"

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

	const float WIDTH = 500.0f; // �|���S���̉���
	const D3DXVECTOR3 HIT_BOX = D3DXVECTOR3(250.0f, 250.0f, 250.0f); // ���苗��
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
CTutorial::CTutorial() : CObjectBillboard(CObject::LABEL_UI, CObject::SCENE_MAIN, CObject::DIM_3D, 7),
	m_sizeDefault(VEC3_ZERO)
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
	// �e�N���X�̏�����
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

	// ���x����ݒ�
	SetLabel(LABEL_UI);

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
	// ����������
	bool bFar = useful::IsNearPosR(GetVec3Position());
	SetEnableDraw(bFar);
	if (!bFar) { return; }

	// �v���C���[�����̔���
	MeasureDistance();

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

	// �A�X�y�N�g�䂩�珉���T�C�Y��ݒ�
	m_sizeDefault = D3DXVECTOR3
	(
		WIDTH,
		useful::GetTexHeightFromAspect(WIDTH, GET_MANAGER->GetTexture()->Regist(pTexturePass)),
		0.0f
	);
}

//==========================================
//  �v���C���[�Ƃ̋����𑪂鏈��
//==========================================
void CTutorial::MeasureDistance()
{
	// �v���C���[�����擾
	CPlayer* pPlayer = GET_PLAYER;
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();
	D3DXVECTOR3 sizePlayer = D3DXVECTOR3(pPlayer->GetRadius(), pPlayer->GetHeight(), pPlayer->GetRadius());

	// �����蔻��
	if (collision::Box3D
	(
		GetVec3Position(), posPlayer,
		HIT_BOX, HIT_BOX,
		sizePlayer, sizePlayer
	))
	{
		m_bMove = true;
	}
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
	D3DXVECTOR3 pos = rPos;
	pos.y += 100.0f;
	pTutorial->SetVec3Position(pos);

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
	return m_pList;
}

//==========================================
//  �g�k����
//==========================================
void CTutorial::Scaling(const float fDeltaTime)
{
	// �ړ��t���O��off�̏ꍇ�֐��𔲂���
	if (!m_bMove) { return; }
}
