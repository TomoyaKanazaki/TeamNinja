//===========================================
//
//  �A���N���X(plant.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "plant.h"
#include "manager.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const float	POLYGON_WIDTH	= 40.0f;	// �|���S���̉���
	const int	ALPHA_NUMREF	= 180;		// ���e�X�g�̎Q�ƒl
}

//===========================================
//  �ÓI�����o�ϐ��錾
//===========================================
CListManager<CPlant>* CPlant::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//===========================================
//  �R���X�g���N�^
//===========================================
CPlant::CPlant()
{
}

//===========================================
//  �f�X�g���N�^
//===========================================
CPlant::~CPlant()
{
}

//===========================================
//  ����������
//===========================================
HRESULT CPlant::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CObjectBillboard::Init())) { assert(false); return E_FAIL; }

	// ���_�ݒ�
	SetOrigin(ORIGIN_CENTER);
	
	// ��]�ݒ�
	SetRotate(ROTATE_LATERAL);

	// ���x���̕ύX
	SetLabel(LABEL_GIMMICK);

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaTest(true);			// ���e�X�g�̗L�� / �����̐ݒ�
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaNumRef(ALPHA_NUMREF);	// ���e�X�g�̎Q�ƒl�ݒ�

	// ���C�e�B���O��OFF�ɂ���
	pRenderState->SetLighting(false);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CPlant>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//===========================================
//  �I������
//===========================================
void CPlant::Uninit(void)
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

//===========================================
//  �X�V����
//===========================================
void CPlant::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CObjectBillboard::Update(fDeltaTime);
}

//===========================================
//  �`�揈��
//===========================================
void CPlant::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CObjectBillboard::Draw(pShader);
}

//===========================================
//  ��������
//===========================================
CPlant* CPlant::Create(const D3DXVECTOR3& rPos, const char* sPass)
{
	// �M�~�b�N�̐���
	CPlant* pPlant = new CPlant;

	// �������̊m�ۂɎ��s�����ꍇ�֐��𔲂���
	if (pPlant == nullptr) { assert(false); return nullptr; }

	// ������
	if (FAILED(pPlant->Init())) { assert(false); SAFE_DELETE(pPlant); return nullptr; }

	// �e�N�X�`�����蓖��
	pPlant->BindTexture(GET_MANAGER->GetTexture()->Regist(sPass));

	// �T�C�Y�̐ݒ�
	pPlant->SetVec3Sizing(D3DXVECTOR3
	(
		POLYGON_WIDTH,
		useful::GetTexHeightFromAspect(POLYGON_WIDTH, pPlant->GetTextureIndex()),
		0.0f
	));

	// �ʒu��ݒ�
	pPlant->SetVec3Position(rPos + D3DXVECTOR3(0.0f, pPlant->GetVec3Sizing().y * 0.5f, 0.0f));

	// �m�ۂ����A�h���X��Ԃ�
	return pPlant;
}

//===========================================
//  ���X�g�擾
//===========================================
CListManager<CPlant>* CPlant::GetList(void)
{
	return m_pList;
}
