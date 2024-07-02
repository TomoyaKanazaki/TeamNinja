#pragma once
//===========================================
//
//  カメラ情報変更地点エディター(editChanger.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "editorObject.h"
#include "camera_change.h"

//===========================================
//  クラス定義
//===========================================
class CEditChanger : public CEditorObject
{
public:
	// コンストラクタ
	CEditChanger(CEditStage* pEditor);

	// デストラクタ
	~CEditChanger() override;

	// フィールド情報構造体
	struct SInfo
	{
		CField::EType type;	// 種類
		D3DXVECTOR2 size;	// 大きさ
		POSGRID2 part;		// 分割数
		POSGRID2 texPart;	// テクスチャ分割数
	};

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	HRESULT Save(void) override;	// 保存
	bool IsSave(void) override;		// 保存状況取得
	void SaveInfo(void) override;	// 情報保存
	void LoadInfo(void) override;	// 情報読込
	void DrawDebugControl(void) override;	// 操作表示描画
	void DrawDebugInfo(void) override;		// 情報表示描画

private:
	// オーバーライド関数
	void UpdateRotation(void) override;	// 向き更新

	// メンバ関数
	void UpdateSizing(void);	// 大きさ更新
	void UpdateTexPart(void);	// テクスチャ分割更新
	void ChangeType(void);		// 種類変更
	void CreateField(void);		// フィールド生成
	void ReleaseField(void);	// フィールド破棄
	void DeleteCollisionField(const bool bRelase);	// フィールドの削除判定
	void InitAllColorField(void);					// フィールドの色全初期化

	// メンバ変数
	CEditChanger* m_pChanger; // オブジェクト情報
	SInfo m_infoCreate; // オブジェクト配置情報
	bool m_bSave; // 保存状況
};
