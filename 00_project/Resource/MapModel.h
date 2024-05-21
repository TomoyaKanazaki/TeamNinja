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

	// �����o�֐�
	CMapModel();
	~CMapModel();

	HRESULT Init() override;							//������
	void Uninit() override;								//�I��
	void Update(const float fDeltaTime) override;		//�X�V
	void Draw(CShader* pShader = nullptr) override;		//�`��

	//�ÓI�����o�֐�
	static CMapModel* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot,const D3DXVECTOR3& rSize); //��������

private:

	static int m_nNumAll;

};

#endif