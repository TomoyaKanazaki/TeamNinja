//<================================================
//マップモデル処理(MapModel.h)
//
//Author:kazuki watanabe
//<================================================
//<************************************************
//二重インクルード防止
//<************************************************
#ifndef _MAP_MODEL_H_	
#define _MAP_MODEL_H_

//<************************************************
//インクルードファイル
//<************************************************
#include "objectModel.h"

//<************************************************
//クラス宣言
//<************************************************
class CMapModel : public CObjectModel
{
public:

	// メンバ関数
	CMapModel();
	~CMapModel();

	HRESULT Init() override;							//初期化
	void Uninit() override;								//終了
	void Update(const float fDeltaTime) override;		//更新
	void Draw(CShader* pShader = nullptr) override;		//描画

	//静的メンバ関数
	static CMapModel* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot,const D3DXVECTOR3& rSize); //生成処理

private:

	static int m_nNumAll;

};

#endif