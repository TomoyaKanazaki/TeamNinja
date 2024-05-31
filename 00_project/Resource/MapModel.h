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

	//<============================================
	//モデルタイプの宣言
	//<============================================
	enum MODEL_TYPE
	{
		MODEL_TYPE_BIRCHTREE = 0,		//白樺の木
		MODEL_TYPE_BIRCHTREE_MID,		//普通サイズの白樺の木
		MODEL_TYPE_COM_TREE,			//普通の木
		MODEL_TYPE_COM_TREE_MID,		//普通サイズの普通の木
		MODEL_TYPE_WILLOW,				//柳の木
		MODEL_TYPE_WILLOW_MID,			//普通サイズの柳の木
		MODEL_TYPE_ROCK_SMALL,			//小さい岩
		MODEL_TYPE_ROCK_MID,			//普通の岩
		MODEL_TYPE_ROCK_BIG,			//大きい岩
		MODEL_TYPE_MOSS_ROCK_SMALL,		//小さい苔岩
		MODEL_TYPE_MOSS_ROCK_MID,		//普通の苔岩
		MODEL_TYPE_MOSS_ROCK_BIG,		//大きい苔岩
		MODEL_TYPE_WOODLOG,				//原木
		MODEL_TYPE_TREESTUMP,			//木の幹
		MODEL_TYPE_PLANT1,				//4つしかつながっていない草
		MODEL_TYPE_PLANT2,				//多くつながっている草
		MODEL_TYPE_BUSH,				//草むら
		MODEL_TYPE_HOUSE1,				//設置用
		MODEL_TYPE_HOUSE2,				//設置用
		MODEL_TYPE_HOUSE3,				//設置用
		MODEL_TYPE_HOUSE4,				//設置用
		MODEL_TYPE_HOUSE5,				//設置用
		MODEL_TYPE_LANTERN1,
		MODEL_TYPE_MAX
	};

	//メンバ関数
	CMapModel();
	~CMapModel();

	HRESULT Init() override;							//初期化
	void Uninit()override { CObjectModel::Uninit(); }	//終了
	void Update(const float fDeltaTime) override;		//更新
	void Draw(CShader* pShader = nullptr) override;		//描画

	void ChangeModel(void);
	void DebugRotate(void);

	//静的メンバ関数
	static CMapModel* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const int& nModelId,const D3DXVECTOR3& rSize=VEC3_ONE); //生成処理

private:
	
	void Collision(void);

	static int m_nNumAll;									//総数
	static const char* mc_apModelFile[MODEL_TYPE_MAX];		//モデル定数

	int m_nModelId;											//モデル番号

};

#endif