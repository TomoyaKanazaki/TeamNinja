//===========================================
//
//  �A���Ǘ��N���X(multi_plant.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "multi_plant.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	// TODO �����Ƃ����A���̃e�N�X�`���p�ӂ���
	const char* TEXTURE_FILE[] = // �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\test.png", // �e�X�g�e�N�X�`��
	};
}

//===========================================
//  �ÓI�x������
//===========================================
static_assert(NUM_ARRAY(TEXTURE_FILE) == CMultiPlant::TYPE_MAX, "ERROR : Type Count Mismatch");

//===========================================
//  �R���X�g���N�^
//===========================================
CMultiPlant::CMultiPlant() : CObject3D(),
m_Type(CMultiPlant::TYPE_MAX),
m_pos(VEC3_ZERO),
m_size(VEC3_ZERO)
{
}

//===========================================
//  �f�X�g���N�^
//===========================================
CMultiPlant::~CMultiPlant()
{
}

//===========================================
//  ����������
//===========================================
HRESULT CMultiPlant::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CObject3D::Init())){ assert(false); return E_FAIL; }

	// ������Ԃ�
	return S_OK;
}

//===========================================
//  �I������
//===========================================
void CMultiPlant::Uninit(void)
{
	// �e�N���X�̏I��
	CObject3D::Uninit();
}

//===========================================
//  �X�V����
//===========================================
void CMultiPlant::Update(const float fDeltaTime)
{
	// �e�N���X�̍X�V
	CObject3D::Update(fDeltaTime);
}

//===========================================
//  �`�揈��
//===========================================
void CMultiPlant::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CObject3D::Draw(pShader);
}

//===========================================
//  ��������
//===========================================
CMultiPlant* CMultiPlant::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const EType type)
{
	// �M�~�b�N�̐���
	CMultiPlant* pPlant = new CMultiPlant;

	// �������̊m�ۂɎ��s�����ꍇ�֐��𔲂���
	if (pPlant == nullptr) { assert(false); return nullptr; }

	// ������
	if (FAILED(pPlant->Init())) { assert(false); SAFE_DELETE(pPlant); return nullptr; }

	// �ʒu��ݒ�
	pPlant->SetVec3Position(rPos);

	// �T�C�Y��ݒ�
	pPlant->SetVec3Sizing(rSize);

	// �^�C�v�̐ݒ�
	pPlant->m_Type = type;

	// �m�ۂ����A�h���X��Ԃ�
	return pPlant;
}
