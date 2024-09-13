#pragma once
//===========================================
//
//  フォグ変更クラス(fog_change.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "objectMeshCube.h"

//===========================================
//  クラス定義
//===========================================
class CFogChanger : public CObjectMeshCube
{
public:

	// メンバ関数
	CFogChanger();
	~CFogChanger();

	HRESULT Init() override; // 初期化処理
	void Uninit() override; // 終了処理
	void Update(const float fDeltaTime) override; // 更新処理
	void Draw(CShader* pShader = nullptr) override; // 描画処理

	// 静的メンバ関数
	static CFogChanger* Create(); // 生成処理
	static CListManager<CFogChanger>* GetList(void);	// リスト取得

private:
	
	// メンバ関数
	bool CollChange(); // ヒット判定の取得
	void Move(); // 移動

	// メンバ変数
	CListManager<CFogChanger>::AIterator m_iterator; // イテレーター

	// 静的メンバ変数
	static CListManager<CFogChanger>* m_pList; // オブジェクトリスト
};
