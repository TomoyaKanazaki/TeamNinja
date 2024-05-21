//<================================================
//�}�b�v���f������(MapModel.cpp)
//
//Author:kazuki watanabe
//<================================================
#include "MapModel.h"
#include "manager.h"
#include "scene.h"

//<**********************************************
//�ÓI�����o�ϐ�
//<**********************************************
int CMapModel::m_nNumAll = 0;

//<**********************************************
//���O�錾
//<**********************************************
namespace
{
	const char* MODEL_NAME = "data\\MODEL\\Tree\\BirchTree_1.x"; //���f���p�X
}

//<==============================================
//�R���X�g���N�^
//<==============================================
CMapModel::CMapModel()
{

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
CMapModel* CMapModel::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const D3DXVECTOR3& rSize)
{
	CMapModel* pMapModel = new CMapModel;

	assert(pMapModel && pMapModel->Init()==S_OK);

	pMapModel->SetVec3Position(rPos);	//�ʒu���
	pMapModel->SetVec3Rotation(rRot);	//�������
	pMapModel->SetVec3Scaling(rSize);	//�������
	pMapModel->BindModel(MODEL_NAME);	//���f�����蓖��

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

	// ���g�̃��x����ݒ�
	//SetLabel(LABEL_MAPMODEL);

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
	//�e�N���X�̍X�V
	CObjectModel::Update(fDeltaTime);
}
//<==============================================
//�`�揈��
//<==============================================
void CMapModel::Draw(CShader* pShader)
{
	CObjectModel::Draw();
}