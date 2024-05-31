//<================================================
//�}�b�v���f������(MapModel.h)
//
//Author:kazuki watanabe
//<================================================
//<************************************************
//��d�C���N���[�h�h�~
//<************************************************
#ifndef _MAP_MODEL_H_	
#define _MAP_MODEL_H_

//<************************************************
//�C���N���[�h�t�@�C��
//<************************************************
#include "objectModel.h"

//<************************************************
//�N���X�錾
//<************************************************
class CMapModel : public CObjectModel
{
public:

	//<============================================
	//���f���^�C�v�̐錾
	//<============================================
	enum MODEL_TYPE
	{
		MODEL_TYPE_BIRCHTREE = 0,		//�����̖�
		MODEL_TYPE_BIRCHTREE_MID,		//���ʃT�C�Y�̔����̖�
		MODEL_TYPE_COM_TREE,			//���ʂ̖�
		MODEL_TYPE_COM_TREE_MID,		//���ʃT�C�Y�̕��ʂ̖�
		MODEL_TYPE_WILLOW,				//���̖�
		MODEL_TYPE_WILLOW_MID,			//���ʃT�C�Y�̖��̖�
		MODEL_TYPE_ROCK_SMALL,			//��������
		MODEL_TYPE_ROCK_MID,			//���ʂ̊�
		MODEL_TYPE_ROCK_BIG,			//�傫����
		MODEL_TYPE_MOSS_ROCK_SMALL,		//�������ۊ�
		MODEL_TYPE_MOSS_ROCK_MID,		//���ʂ̑ۊ�
		MODEL_TYPE_MOSS_ROCK_BIG,		//�傫���ۊ�
		MODEL_TYPE_WOODLOG,				//����
		MODEL_TYPE_TREESTUMP,			//�؂̊�
		MODEL_TYPE_PLANT1,				//4�����Ȃ����Ă��Ȃ���
		MODEL_TYPE_PLANT2,				//�����Ȃ����Ă��鑐
		MODEL_TYPE_BUSH,				//���ނ�
		MODEL_TYPE_HOUSE1,				//�ݒu�p
		MODEL_TYPE_HOUSE2,				//�ݒu�p
		MODEL_TYPE_HOUSE3,				//�ݒu�p
		MODEL_TYPE_HOUSE4,				//�ݒu�p
		MODEL_TYPE_HOUSE5,				//�ݒu�p
		MODEL_TYPE_LANTERN1,
		MODEL_TYPE_MAX
	};

	//�����o�֐�
	CMapModel();
	~CMapModel();

	HRESULT Init() override;							//������
	void Uninit()override { CObjectModel::Uninit(); }	//�I��
	void Update(const float fDeltaTime) override;		//�X�V
	void Draw(CShader* pShader = nullptr) override;		//�`��

	void ChangeModel(void);
	void DebugRotate(void);

	//�ÓI�����o�֐�
	static CMapModel* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const int& nModelId,const D3DXVECTOR3& rSize=VEC3_ONE); //��������

private:
	
	void Collision(void);

	static int m_nNumAll;									//����
	static const char* mc_apModelFile[MODEL_TYPE_MAX];		//���f���萔

	int m_nModelId;											//���f���ԍ�

};

#endif