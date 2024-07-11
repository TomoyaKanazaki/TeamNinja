//============================================================
//
//	�J�ڃ|�C���g���� [transpoint.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "transpoint.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char*	MODEL_FILE = "data\\MODEL\\MAP\\arena000.x";	// ���f���t�@�C��
	const int	PRIORITY = 3;	// �J�ڃ|�C���g�̗D�揇��
}

//************************************************************
//	�q�N���X [CTransPoint] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTransPoint::CTransPoint(const char* pPass) : CObjectModel(CObject::LABEL_MAP, CObject::DIM_3D, PRIORITY),
	m_pTransMapPass	(pPass)	// �J�ڐ�}�b�v�p�X
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTransPoint::~CTransPoint()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTransPoint::Init(void)
{
	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �J�ڃ|�C���g���f����o�^�E����
	BindModel(MODEL_FILE);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTransPoint::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTransPoint::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CTransPoint::Draw(CShader *pShader)
{
	CToonShader	*pToonShader = CToonShader::GetInstance();	// �g�D�[���V�F�[�_�[���
	if (pToonShader->IsEffectOK())
	{ // �G�t�F�N�g���g�p�\�ȏꍇ

		// �I�u�W�F�N�g���f���̕`��
		CObjectModel::Draw(pToonShader);
	}
	else
	{ // �G�t�F�N�g���g�p�s�\�ȏꍇ

		// �G�t�F�N�g�G���[
		assert(false);

		// �I�u�W�F�N�g���f���̕`��
		CObjectModel::Draw(pShader);
	}
}

//============================================================
//	��������
//============================================================
CTransPoint *CTransPoint::Create
(
	const char* pPass,			// �J�ڐ�}�b�v�p�X
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot		// ����
)
{
	// �J�ڃ|�C���g�̐���
	CTransPoint *pTransPoint = new CTransPoint(pPass);
	if (pTransPoint == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �J�ڃ|�C���g�̏�����
		if (FAILED(pTransPoint->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �J�ڃ|�C���g�̔j��
			SAFE_DELETE(pTransPoint);
			return nullptr;
		}

		// �ʒu��ݒ�
		pTransPoint->SetVec3Position(rPos);

		// ������ݒ�
		pTransPoint->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pTransPoint;
	}
}
