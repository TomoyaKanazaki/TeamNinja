//<================================================
//�}�b�v���f������(MapModel.cpp)
//
//Author:kazuki watanabe
//<================================================
#include "MapModel.h"
#include "manager.h"
#include "scene.h"
#include "collision.h"

//<**********************************************
//�ÓI�����o�ϐ�
//<**********************************************
int CMapModel::m_nNumAll = 0;										//����
const char* CMapModel::mc_apModelFile[MODEL_TYPE_MAX] =				//���f���t�@�C����(���������`)
{
	"data\\MODEL\\Tree\\BirchTree000.x",							//�����̖�
	"data\\MODEL\\Tree\\BirchTree001.x",							//���ʃT�C�Y�̔����̖�
	"data\\MODEL\\Tree\\CommonTree000.x",							//���ʂ̖�
	"data\\MODEL\\Tree\\CommonTree001.x",							//���ʃT�C�Y�̕��ʂ̖�
	"data\\MODEL\\Tree\\Willow000.x",								//���̖�
	"data\\MODEL\\Tree\\Willow001.x",								//���ʃT�C�Y�̖��̖�
	"data\\MODEL\\Rock\\Rock000.x",									//��������
	"data\\MODEL\\Rock\\Rock001.x",									//���ʂ̊�
	"data\\MODEL\\Rock\\Rock002.x",									//�傫����
	"data\\MODEL\\Rock\\Moss-Rock000.x",							//�������ۊ�
	"data\\MODEL\\Rock\\Moss-Rock001.x",							//���ʂ̑ۊ�
	"data\\MODEL\\Rock\\Moss-Rock002.x",							//�傫���ۊ�
	"data\\MODEL\\Tree\\WoodLog000.x",								//����
	"data\\MODEL\\Tree\\TreeStump000.x",							//����
	"data\\MODEL\\Plant\\Plant000.x",								//��

};	

//<**********************************************
//���O�錾(�g����������Ȃ��̂�)
//<**********************************************
namespace
{
	
}

//<==============================================
//�R���X�g���N�^
//<==============================================
CMapModel::CMapModel()
{
	m_nModelId = 0;
}
//<==============================================
//�f�X�g���N�^
//<==============================================
CMapModel::~CMapModel()
{

}
//<==============================================
//��������
//<==============================================
CMapModel* CMapModel::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const D3DXVECTOR3& rSize, const int& nModelId)
{
	CMapModel* pMapModel = new CMapModel;

	//���g�Ə������`�F�b�N
	assert(pMapModel && pMapModel->Init()==S_OK);

	pMapModel->SetVec3Position(rPos);				//�ʒu���
	pMapModel->SetVec3Rotation(rRot);				//�������
	pMapModel->SetVec3Scaling(rSize);				//�T�C�Y���
	pMapModel->BindModel(mc_apModelFile[nModelId]);	//�ԍ��Ń��f�����蓖��
	pMapModel->m_nModelId = nModelId;

	return pMapModel;
}
//<==============================================
//����������
//<==============================================
HRESULT CMapModel::Init()
{
	// �e�N���X�̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//���g�̃��x����ݒ�
	SetLabel(LABEL_MAPMODEL);

	return S_OK;
}
//<==============================================
//�I������
//<==============================================
void CMapModel::Uninit()
{
	CObjectModel::Uninit();
} 
//<==============================================
//�X�V����
//<==============================================
void CMapModel::Update(const float fDeltaTime)
{
	ChangeModel();

	//�e�N���X�̍X�V
	CObjectModel::Update(fDeltaTime);
}
//<==============================================
//�`�揈��
//<==============================================
void CMapModel::Draw(CShader* pShader)
{
	CToonShader* pToonShader = CToonShader::GetInstance();	// �g�D�[���V�F�[�_�[���
	if (pToonShader->IsEffectOK())
	{ // �G�t�F�N�g���g�p�\�ȏꍇ

		// �I�u�W�F�N�g�L�����N�^�[�̕`��
		CObjectModel::Draw(pToonShader);
	}
	else
	{ // �G�t�F�N�g���g�p�s�\�ȏꍇ

		// �G�t�F�N�g�G���[
		assert(false);

		// �I�u�W�F�N�g�L�����N�^�[�̕`��
		CObjectModel::Draw(pShader);
	}
}
//<==============================================
//�`�揈��
//<==============================================
void CMapModel::ChangeModel(void)
{
	if (GET_INPUTKEY->IsTrigger(DIK_F9))
	{
		m_nModelId += 1;
	}
	if (m_nModelId >= MODEL_TYPE_MAX)
	{
		m_nModelId = MODEL_TYPE_BIRCHTREE;
	}
	BindModel(mc_apModelFile[m_nModelId]);
}
//<==============================================
//�����蔻��(�����������Ă����܂�)
//<==============================================
void CMapModel::Collision(void)
{

}