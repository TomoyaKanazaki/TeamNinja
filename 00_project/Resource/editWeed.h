#pragma once
//===========================================
//
//  草エディター(editWeed.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "editorObject.h"
#include "weed.h"

//==========================================
//  前方宣言
//==========================================
class CObjectMeshCylinder;

//===========================================
//  クラス定義
//===========================================
class CEditWeed : public CEditorObject
{
public:

	// オブジェクト情報構造体
	struct SInfo
	{
		float fSize;	// 大きさ
	};

	// メンバ関数
	CEditWeed(CEditStage* pEditor);
	~CEditWeed() override;

	HRESULT Init(void) override; // 初期化
	void Uninit(void) override; // 終了
	void Update(void) override; // 更新
	HRESULT Save(void) override; // 保存
	bool IsSave(void) override; // 保存状況取得
	void SaveInfo(void) override; // 情報保存
	void LoadInfo(void) override; // 情報読込
	void DrawDebugControl(void) override; // 操作表示描画
	void DrawDebugInfo(void) override; // 情報表示描画

private:

	// メンバ関数
	void UpdateSize(void); // 大きさ更新
	void UpdateTime(void); //生成頻度更新
	void UpdateNum(void); // 生成数更新
	void Create(void); // 草生成
	void Release(void); // 草破棄
	void DeleteCollision(const bool bRelase); // 草の削除判定

	// メンバ変数
	CObjectMeshCylinder* m_pCylinder; // オブジェクト情報
	SInfo m_infoCreate; // オブジェクト配置情報
	bool m_bSave; // 保存状況
	int m_nCoolTime; // 生成頻度
	int m_nNum; // 生成数
};
