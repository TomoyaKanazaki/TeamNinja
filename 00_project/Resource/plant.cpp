//===========================================
//
//  �A���N���X(plant.cpp)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "plant.h"
#include "manager.h"
#include "camera.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const float	POLYGON_SCALE = 60.0f;		// �|���S���̃T�C�Y
	const int	ALPHA_NUMREF = 180;			// ���e�X�g�̎Q�ƒl
	const D3DXVECTOR3 GAP_SCALE = D3DXVECTOR3(15.0f, 0.0f, 0.0f); // �h��͈�
}

//===========================================
//  �ÓI�����o�ϐ��錾
//===========================================
CListManager<CPlant>* CPlant::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//===========================================
//  �R���X�g���N�^
//===========================================
CPlant::CPlant():
	m_fGapRate(0.0f)
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
	if (FAILED(CObject3D::Init())) { assert(false); return E_FAIL; }

	// ���_�ݒ�
	SetOrigin(ORIGIN_CENTER);

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

	// �h�������ݒ�
	m_fGapRate = (float)(rand() % 628 + 1) * 0.01f;

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
	CObject3D::Uninit();
}

//===========================================
//  �X�V����
//===========================================
void CPlant::Update(const float fDeltaTime)
{
	// �����]��
	Rotation();

	// �h���ݒ�
	Shake(fDeltaTime);

	// �e�N���X�̍X�V
	CObject3D::Update(fDeltaTime);
}

//===========================================
//  �`�揈��
//===========================================
void CPlant::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CObject3D::Draw(pShader);
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
	pPlant->SetVec3Sizing(useful::GetTexAspect(GET_MANAGER->GetTexture()->Regist(sPass)) * POLYGON_SCALE);

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

//==========================================
//  ��]����
//==========================================
void CPlant::Rotation()
{
	// ���g�̍��W���擾
	D3DXVECTOR3 posThis = GetVec3Position();

	// ���_�̍��W���擾
	D3DXVECTOR3 posCamera = GET_CAMERA->GetPositionV();

	// ���g�̍��W�Ǝ��_�̍��W�����ԃx�N�g�����쐬
	D3DXVECTOR3 vecToPos = posThis - posCamera;

	// 2�_�Ԃ����ԃx�N�g���̊p�x���Z�o
	float fRot = atan2f(vecToPos.x, vecToPos.z);

	// ���g�̌������擾
	D3DXVECTOR3 rot = GetVec3Rotation();

	// y�̌������J�����Ɍ�����
	rot.y = fRot;

	// ������ݒ�
	SetVec3Rotation(rot);
}

//==========================================
//  �h��鏈��
//==========================================
void CPlant::Shake(const float fDeltaTime)
{
	// �h�ꎞ�Ԃ̉��Z
	m_fGapRate += fDeltaTime;

	// ���Ԃ̊ۂߍ���
	if (m_fGapRate > D3DX_PI * 2.0f)
	{
		m_fGapRate -= D3DX_PI * 2.0f;
	}

	// �㔼�����ړ�����
	SetGapPosition(0, GAP_SCALE * sinf(m_fGapRate));
	SetGapPosition(1, GAP_SCALE * sinf(m_fGapRate));
}
