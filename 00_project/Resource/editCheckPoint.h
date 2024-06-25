#if 1
//============================================================
//
//	エディットチェックポイントヘッダー [editCheckPoint.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EDIT_CHECKPOINT_H_
#define _EDIT_CHECKPOINT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "editorObject.h"
#include "checkpoint.h"

//************************************************************
//	クラス定義
//************************************************************
// エディットチェックポイントクラス
class CEditCheckPoint : public CEditorObject
{
public:
	// コンストラクタ
	CEditCheckPoint(CEditStage* pEditor);

	// デストラクタ
	~CEditCheckPoint() override;

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
	void UpdatePosition(void) override;		// 位置更新

	// メンバ関数
	void CreateCheckPoint(void);		// チェックポイント生成
	void ReleaseCheckPoint(void);		// チェックポイント破棄
	void DeleteCollisionCheckPoint(const bool bRelase);	// チェックポイントの削除判定

	// メンバ変数
	CCheckPoint* m_pPoint;	// ポイント情報
	bool m_bSave;			// 保存状況
};

#endif	// _EDIT_ACTOR_H_
#endif
