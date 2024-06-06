//<================================================
//�}�b�v���f������(MapModel.cpp)
//
//Author:kazuki watanabe
//<================================================
//<**********************************************
//�C���N���[�h�t�@�C��
//<**********************************************
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
	"data\\MODEL\\Tree\\TreeStump000.x",							//�؂̊�
	"data\\MODEL\\Plant\\Plant000.x",								//4�����Ȃ����Ă��Ȃ���
	"data\\MODEL\\Plant\\Plant001.x",								//�����Ȃ����Ă��鑐
	"data\\MODEL\\Plant\\Bush000.x",								//���ނ�
	"data\\MODEL\\House\\House000.x",								//���ނ�
	"data\\MODEL\\House\\House001.x",								//���ނ�
	"data\\MODEL\\House\\House002.x",								//���ނ�
	"data\\MODEL\\House\\House003.x",								//���ނ�
	"data\\MODEL\\House\\House004.x",								//���ނ�
	"data\\MODEL\\Lantern\\Lantern001.x",								//���ނ�
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
	//�l�̃N���A
	m_nNumAll++;
	m_nModelId = 0;
}
//<==============================================
//�f�X�g���N�^
//<==============================================
CMapModel::~CMapModel()
{
	//���Z
	m_nNumAll--;
}
//<==============================================
//��������
//<==============================================
CMapModel* CMapModel::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const int& nModelId,const D3DXVECTOR3& rSize)
{
	//�C���X�^���X����
	CMapModel* pMapModel = new CMapModel;

	//���g�Ə������`�F�b�N
	assert(pMapModel);

	pMapModel->Init();

	pMapModel->SetVec3Position(rPos);				//�ʒu���
	pMapModel->SetVec3Rotation(rRot);				//�������
	pMapModel->SetVec3Scaling(rSize);				//�T�C�Y���
	pMapModel->BindModel(mc_apModelFile[nModelId]);	//�ԍ��Ń��f�����蓖��
	pMapModel->m_nModelId = nModelId;				//���f���ԍ��ۑ�

	return pMapModel;
}
//<==============================================
//����������
//<==============================================
HRESULT CMapModel::Init()
{
	//�e�N���X�̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//���x���̐ݒ�
	SetLabel(LABEL_MAPMODEL);

	return S_OK;
}
//<==============================================
//�X�V����
//<==============================================
void CMapModel::Update(const float fDeltaTime)
{
	//�f�o�b�O��p�̃��f���ύX����
	ChangeModel();

	DebugRotate();

	//�e�N���X�̍X�V
	CObjectModel::Update(fDeltaTime);
}
//<==============================================
//�`�揈��
//<==============================================
void CMapModel::Draw(CShader* pShader)
{
	//�g�D�[���V�F�[�_�[�����擾
	CToonShader* pToonShader = CToonShader::GetInstance();

	//�G�t�F�N�g�̎g�p���ł���ꍇ
	if (pToonShader->IsEffectOK())
	{

		//�g�D�[���V�F�[�_�[���g�����`�������
		CObjectModel::Draw(pToonShader);
	}
	//�g�p�ł��Ȃ��ꍇ
	else
	{ 
		//�G�t�F�N�g�G���[
		assert(false);

		//�I�u�W�F�N�g���f���̕`��
		CObjectModel::Draw(pShader);
	}
}
//<==============================================
//���f���ύX����
//<==============================================
void CMapModel::ChangeModel(void)
{
#ifdef _DEBUG

	//���f���ԍ���i�߂�
	if (GET_INPUTKEY->IsTrigger(DIK_F9)){m_nModelId += 1;}

	//���f���̍ő�ɒB������A�ŏ��̃��f���ɖ߂�
	if (m_nModelId >= MODEL_TYPE_MAX){m_nModelId = MODEL_TYPE_MAX - MODEL_TYPE_MAX;}

	//���f���̊��蓖�Ă�����
	BindModel(mc_apModelFile[m_nModelId]);

#endif
}
//<==============================================
//�f�o�b�O�p�̉�]����
//<==============================================
void CMapModel::DebugRotate(void)
{
	// ���邭�邵�Ă݂�
	D3DXVECTOR3 rot = GetVec3Rotation();

	//���f���ԍ���i�߂�
	if (GET_INPUTKEY->IsPress(DIK_F8)) { rot.y += 0.1f; }


	SetVec3Rotation(rot);

}
//<==============================================
//�����蔻��(�����������Ă����܂�)
//<==============================================
void CMapModel::Collision(void)
{
	
}