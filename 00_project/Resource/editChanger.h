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

	// オブジェクト情報構造体
	struct SInfo
	{
		D3DXVECTOR3 size;	// 大きさ
		CCameraChanger::EDirection dir;	// 種類
		CCameraChanger::ERotation rot;	// 種類
	};

	// メンバ関数
	CEditChanger(CEditStage* pEditor);
	~CEditChanger() override;

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
	void UpdateRotation(void) override;	// 向き更新

	void UpdateSize(void); // 大きさ更新
	void ChangeDirection(void); // 方向変更
	void ChangeRotation(void); // 角度変更
	void CreateField(void); // フィールド生成
	void ReleaseField(void); // フィールド破棄
	void DeleteCollisionField(const bool bRelase); // フィールドの削除判定

	// メンバ変数
	CCameraChanger* m_pChanger; // オブジェクト情報
	SInfo m_infoCreate; // オブジェクト配置情報
	bool m_bSave; // 保存状況
};
